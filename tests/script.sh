#!/bin/bash
printf "Testing %s:\n" $1
for f in $( ls tests/$1 | grep \\.in$ ); do
    ./test $1 < tests/$1/$f > $1.out
    diff -Z $1.out tests/$1/${f%.*}.out && printf "Test '%s' succeeded!\n" $f \
        || printf "Test '%s' failed!\n" $f
done
