#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "cache.h"


extern uns64 cycle; // You can use this as timestamp for LRU

extern uns64 SWP_CORE0_WAYS;

////////////////////////////////////////////////////////////////////
// ------------- DO NOT MODIFY THE INIT FUNCTION -----------
////////////////////////////////////////////////////////////////////

Cache  *cache_new(uns64 size, uns64 assoc, uns64 linesize, uns64 repl_policy){

   Cache *c = (Cache *) calloc (1, sizeof (Cache));
   c->num_ways = assoc;
   c->repl_policy = repl_policy;

   if(c->num_ways > MAX_WAYS){
     printf("Change MAX_WAYS in cache.h to support %llu ways\n", c->num_ways);
     exit(-1);
   }

   // determine num sets, and init the cache
   c->num_sets = size/(linesize*assoc);
   c->sets  = (Cache_Set *) calloc (c->num_sets, sizeof(Cache_Set));

   return c;
}

////////////////////////////////////////////////////////////////////
// ------------- DO NOT MODIFY THE PRINT STATS FUNCTION -----------
////////////////////////////////////////////////////////////////////

void    cache_print_stats    (Cache *c, char *header){
  double read_mr =0;
  double write_mr =0;

  if(c->stat_read_access){
    read_mr=(double)(c->stat_read_miss)/(double)(c->stat_read_access);
  }

  if(c->stat_write_access){
    write_mr=(double)(c->stat_write_miss)/(double)(c->stat_write_access);
  }

  printf("\n%s_READ_ACCESS    \t\t : %10llu", header, c->stat_read_access);
  printf("\n%s_WRITE_ACCESS   \t\t : %10llu", header, c->stat_write_access);
  printf("\n%s_READ_MISS      \t\t : %10llu", header, c->stat_read_miss);
  printf("\n%s_WRITE_MISS     \t\t : %10llu", header, c->stat_write_miss);
  printf("\n%s_READ_MISSPERC  \t\t : %10.3f", header, 100*read_mr);
  printf("\n%s_WRITE_MISSPERC \t\t : %10.3f", header, 100*write_mr);
  printf("\n%s_DIRTY_EVICTS   \t\t : %10llu", header, c->stat_dirty_evicts);

  printf("\n");
}



////////////////////////////////////////////////////////////////////
// Note: the system provides the cache with the line address
// Return HIT if access hits in the cache, MISS otherwise 
// Also if is_write is TRUE, then mark the resident line as dirty
// Update appropriate stats
////////////////////////////////////////////////////////////////////

Flag cache_access(Cache *c, Addr lineaddr, uns is_write, uns core_id){
  Flag outcome=MISS;
    
    uns set_index;
    uns ii;
    Addr demand_tag;
    
    set_index = lineaddr & (c->num_sets-1);
    demand_tag = lineaddr / c->num_sets;
    
    
    //printf("lineaddr: 0x%x, set_index: 0x%x, demand_tag: 0x%x\n", lineaddr, set_index, demand_tag);
    
    for(ii = 0; ii < c->num_ways; ii++){
        
        if((c->sets[set_index].line[ii].tag == demand_tag) && (c->sets[set_index].line[ii].valid == TRUE)){
            outcome = HIT;
            c->sets[set_index].line[ii].last_access_time = cycle;
            
            if(is_write == 1){
                c->sets[set_index].line[ii].dirty = TRUE;
                c->stat_write_access++;
            }
            else{
                //c->sets[set_index].line[ii].dirty = FALSE;
                c->stat_read_access++;
            }
            break;
        }
    }
  
  return outcome;
}

////////////////////////////////////////////////////////////////////
// Note: the system provides the cache with the line address
// Install the line: determine victim using repl policy (LRU/RAND)
// copy victim into last_evicted_line for tracking writebacks
////////////////////////////////////////////////////////////////////

void cache_install(Cache *c, Addr lineaddr, uns is_write, uns core_id){

    uns victim_line, set_index;
    Addr demand_tag;
  // Your Code Goes Here
 
    set_index = lineaddr & (c->num_sets-1);
    demand_tag = lineaddr / c->num_sets;
    
  // Find victim using cache_find_victim
    
    victim_line = cache_find_victim(c, set_index, core_id);
    
  // Initialize the evicted entry
    
    c->last_evicted_line = c->sets[set_index].line[victim_line];
    if(c->last_evicted_line.dirty == TRUE)
        c->stat_dirty_evicts++;
    
  // Initialize the victim entry
    
    c->sets[set_index].line[victim_line].valid = TRUE;
    c->sets[set_index].line[victim_line].tag = demand_tag;
    c->sets[set_index].line[victim_line].core_id = core_id;
    c->sets[set_index].line[victim_line].last_access_time = cycle;
    
    if(is_write == 1){
        c->sets[set_index].line[victim_line].dirty = TRUE;
        c->stat_write_miss++;
        c->stat_write_access++;

    }
    else{
        c->sets[set_index].line[victim_line].dirty = FALSE;
        c->stat_read_miss++;
        c->stat_read_access++;

    }

}

////////////////////////////////////////////////////////////////////
// You may find it useful to split victim selection from install
////////////////////////////////////////////////////////////////////


uns cache_find_victim(Cache *c, uns set_index, uns core_id){
  uns victim=0;
    
    uns ii, LRU_line = c->num_ways, temp = 0xFFFFFFFF;
  // TODO: Write this using a switch case statement
    
    switch(c->repl_policy){
            
        case 0:
            
            for(ii = 0; ii < c->num_ways; ii++){
                
                if(c->sets[set_index].line[ii].last_access_time < temp){
                    temp = c->sets[set_index].line[ii].last_access_time;
                    LRU_line = ii;
                }
            }

            if(LRU_line != c->num_ways){
                victim = LRU_line;
            }
            else
                printf("error!");
            
            break;
        case 1:
            
            victim = rand()%c->num_ways;
            
            break;
        case 2:
            
            if(core_id == 0){
                for(ii = 0; ii < SWP_CORE0_WAYS; ii++){
                    
                    if(c->sets[set_index].line[ii].last_access_time < temp){
                        temp = c->sets[set_index].line[ii].last_access_time;
                        LRU_line = ii;
                    }
                }
            }
            else{
                
                for(ii = SWP_CORE0_WAYS; ii < c->num_ways; ii++){
                    
                    if(c->sets[set_index].line[ii].last_access_time < temp){
                        temp = c->sets[set_index].line[ii].last_access_time;
                        LRU_line = ii;
                    }
                }
            }
                
            if(LRU_line != c->num_ways){
                victim = LRU_line;
            }
            else
                printf("error!");
            
            
            break;
        default:
            break;
    }
  
  return victim;
}

