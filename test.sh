#!/usr/bin/bash

checkPoints(){
    hp1=pyout.txt
    hp2=jarvisout.txt
    hp3=kpsout.txt
    flag=1
    if ! python3 compare.py $hp1 $hp2 ; then
        flag=0
        echo "The Python and Jarvis March hull points are different"
    fi
    if ! python3 compare.py $hp1 $hp3; then
        flag=0
        echo "The Python and Kirkpatrick-Seidel hull points are different"
    fi
    if ! python3 compare.py $hp2 $hp3; then
        flag=0
        echo "The Jarvis March and Kirkpatrick-Seidel hull points are different"
    fi

    if [ $flag -eq 1 ]; then
        echo "All hull points are the same"
    fi
}

numPoints=100000
for i in $(seq 1 1) ; do
    python3 genPoints.py $numPoints
    checkPoints
done
