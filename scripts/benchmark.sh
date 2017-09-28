#!/bin/bash

# number of executions
nruns=10

# sum of garbling times
gtsum=0

# sum of evaluation times
etsum=0

for i in `seq 1 $nruns`;
do
    $1 > tmp.txt
    let gtsum+=$(grep garbling tmp.txt | sed 's/Circuit garbling time (microseconds) = //')
    let etsum+=$(grep evaluation tmp.txt | sed 's/Circuit evaluation time (microseconds) = //')
done

rm tmp.txt

echo "Average garbling time $(($gtsum/$nruns)) microseconds"
echo "Average evaluation time $(($etsum/$nruns)) microseconds"