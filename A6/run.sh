#!/bin/bash

for(( c=1; c<=1000; c++ ))
do
    ./soda soda.config ${c} 2
    if [ $? -ne 0 ]
    then
        echo "error"
        break
    else
        echo "passed"
    fi
done