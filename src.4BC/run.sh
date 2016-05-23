
######################################################################################
# This scripts runs all three traces
# You will need to uncomment the configurations that you want to run
# the results are stored in the ../results/ folder 
######################################################################################



########## ---------------  B ---------------- ################


./sim -mode 2 -L2sizeKB 1024 ../traces/bzip2.mtr.gz  > ../results/B.S1MB.bzip2.res 
./sim -mode 2 -L2sizeKB 1024 ../traces/lbm.mtr.gz    > ../results/B.S1MB.lbm.res &
./sim -mode 2 -L2sizeKB 1024 ../traces/libq.mtr.gz    > ../results/B.S1MB.libq.res 


echo "All Done. Check the .res file in ../results directory";

