#!/bin/bash

{ echo "SCALE=F"; echo "SCALE=C"; echo "PERIOD=1"; echo "STOP"; echo "START"; echo "LOG HELLO"; echo "OFF"; } | ./lab4b --log=output >/dev/null
if [ $? -eq 0 ]
then
	echo "Successfully returned with exit code 0"
else
	echo "Error, program returned with non-zero value"
fi

tests=("SCALE=F" "SCALE=C" "PERIOD=1" "STOP" "START" "LOG HELLO" "OFF")
for test in "${tests[@]}";
do
    grep "$test" output > /dev/null
    if [ $? -eq 0 ]
    then
        echo "$test test passed"
    else
        echo "$test test failed"
    fi
done

rm -f output