#!/bin/bash

size=$1

# circuit takes two inputs and returns one output
# number of gates in the circuit is size
echo "0 0 0 0 1 1 0 1 0 $size"

# first input of each gate is the first input to the circuit
for i in `seq 1 $size`;
do
    echo -n "0 "
done
echo

# second input of each gate is the second input to the circuit
for i in `seq 1 $size`;
do
    echo -n "1 "
done
echo

# all gates are of type AND
for i in `seq 1 $size`;
do
    echo -n "8 "
done
echo

# dummy output
echo 2