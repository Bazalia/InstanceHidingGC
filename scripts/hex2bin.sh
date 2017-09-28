#!/bin/bash

export BC_LINE_LENGTH=0
printf "%0$2s\n" $(echo "obase=2; ibase=16; $(echo $1 | tr [:lower:] [:upper:])" | bc) | tr " " "0"