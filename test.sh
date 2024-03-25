#!/usr/bin/bash
exponent=3
xmin=1
xmax=1500
ymin=60
ymax=700
echo -n "" > in.txt
for ((i = 0; i < $((10**exponent)); i++)); do
    ( echo -n "("; shuf -i $xmin-$xmax -n 1 | tr -d '\n'; echo -n ","; shuf -i $ymin-$ymax -n 1 | tr -d '\n'; 
    echo ")" ) >> in.txt
done
hp1=pyout.txt
hp2=jarvis.txt
hp3=kps.txt
flag=1
if ! python3 test.py $hp1 $hp2 ; then
    flag=0
    echo "The Python and Jarvis March hull points are different"
fi
if ! python3 test.py $hp1 $hp3; then
    flag=0
    echo "The Python and Kirkpatrick-Seidel hull points are different"
fi
if ! python3 test.py $hp2 $hp3; then
    flag=0
    echo "The Jarvis March and Kirkpatrick-Seidel hull points are different"
fi

if [ $flag -eq 1 ]; then
    echo "All hull points are the same"
fi

