#!/bin/bash
if [ $# -eq 2 ]
then
    if [[ $1 == -* ]]
    then
        NAME=$2
        OPTS=$1
    else
        NAME=$1
        OPTS=$2
    fi
else
    NAME=$1
    OPTS="-gtatc"
fi

if [[ $OPTS == *g* ]] ; then ./main < $NAME.v > $NAME.asm
fi
if [[ $OPTS == *a* ]] ; then nasm -Fdwarf -felf32 -o $NAME.o $NAME.asm
fi
if [[ $OPTS == *c* ]] ; then gcc -m32 -o $NAME $NAME.o
fi
if [[ $OPTS == *gt* ]] ; then rm $NAME.asm
fi
if [[ $OPTS == *at* ]] ; then rm $NAME.o
fi
