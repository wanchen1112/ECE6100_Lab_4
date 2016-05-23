
######################################################################################
# This scripts runs all three traces
# You will need to uncomment the configurations that you want to run
# the results are stored in the ../results/ folder 
######################################################################################

./sim -mode 1 ../traces/bzip2.mtr.gz > ../results/A.bzip2.res ;
./sim -mode 1 ../traces/lbm.mtr.gz  > ../results/A.lbm.res ; 
./sim -mode 1 ../traces/libq.mtr.gz   > ../results/A.libq.res ; 

########## ---------------  B ---------------- ################


./sim -mode 2  ../traces/bzip2.mtr.gz  > ../results/B.bzip2.res;
./sim -mode 2 ../traces/lbm.mtr.gz     > ../results/B.lbm.res; 
./sim -mode 2 ../traces/libq.mtr.gz    > ../results/B.libq.res; 


########## ---------------  C ---------------- ################


./sim -mode 3  ../traces/bzip2.mtr.gz  > ../results/C.bzip2.res ;
./sim -mode 3  ../traces/lbm.mtr.gz    > ../results/C.lbm.res ;
./sim -mode 3  ../traces/libq.mtr.gz   > ../results/C.libq.res ; 


echo "All Done. Check the .res file in ../results directory";

