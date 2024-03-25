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
