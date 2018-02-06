#!/bin/bash
# arg1: line number
# arg2: infile
# arg3: replacement lambda

echo $1'
s/[0-9.]\{1,\}/'$3'/p
w
q' | ed $2
