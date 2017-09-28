#!/bin/bash

# adds two decimal numbers together

cd ../build
./run -f ../input/adder_32bit.txt -i $(bash ../scripts/dec2bin.sh $1 32)$(bash ../scripts/dec2bin.sh $2 32)