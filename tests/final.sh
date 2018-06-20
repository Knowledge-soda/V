#!/bin/bash
printf "Testing %s:\n"
for f in $( ls tests/progs | grep \\.v$ ); do
    B="tests/progs/${f%.*}"
    ./run.sh $B
    $B < $B.in > prog.out
    diff -Z prog.out $B.out && printf "Test %s succeeded!\n" $f \
        || printf "Test %s failed!\n" $f
done
