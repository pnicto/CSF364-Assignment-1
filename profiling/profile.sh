#!/usr/bin/bash
maxExponent=7
files="$@"
rm -r uniform
rm -r polygon
mkdir uniform
mkdir polygon
mkdir uniform/logs
mkdir uniform/results
mkdir polygon/logs
mkdir polygon/results
for f in $files; do
    g++ -pg -O3 "$f".cpp -o "$f"
    echo -n > uniform/"$f".txt
    echo -n > polygon/"$f".txt
    mkdir uniform/logs/"$f"
    mkdir polygon/logs/"$f"
done

#polygon
numPoints=(5 25 50 250 500 2500 5000)
for i in ${numPoints[@]}; do
    for f in $files; do
        echo -n "$i " >> polygon/"$f.txt"
    done
    for j in $(seq 1 10); do
        python3 genPoints.py --polygon $i
        for name in $files; do
            ./"$name"
            x=$(gprof "$name" gmon.out -bq | awk ' NR == 4 ' | awk '{print $(NF-1)}')
            if echo "$x" | grep -Eq '[0-9]+[.][0-9]+'; then
                echo -n "$x " >> polygon/"$name".txt
            fi
            gprof "$name" gmon.out -bp > polygon/logs/"$name"/"$name"_"$i"_"$j".txt
        done
    done
    for f in $files; do
        echo "" >> polygon/"$f".txt
    done
done
./average.sh polygon

#uniform
for i in $(seq 1 $maxExponent) ; do
    for f in $files; do
        echo -n "$((10**i)) " >> uniform/"$f.txt"
    done
    for j in $(seq 1 10); do
        python3 genPoints.py --uniform $((10**i))
        for name in $files; do
            ./"$name"
            x=$(gprof "$name" gmon.out -bq | awk ' NR == 4 ' | awk '{print $(NF-1)}')
            if echo "$x" | grep -Eq '[0-9]+[.][0-9]+'; then
                echo -n "$x " >> uniform/"$name".txt
            fi
            gprof "$name" gmon.out -bp > uniform/logs/"$name"/"$name"_"$((10**i))"_"$j".txt
        done
    done
    for f in $files; do
        echo "" >> uniform/"$f".txt
    done
done
./average.sh uniform
