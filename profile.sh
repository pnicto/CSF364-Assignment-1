#!/usr/bin/bash
maxExponent=7
files="$@"
for f in $files; do
    echo -n > profiling/"$f".txt
    g++ -pg -O3 "$f".cpp -o "$f"
    rm -r profiling/logs/"$f"
    mkdir profiling/logs/"$f"
done
for i in $(seq 1 $maxExponent) ; do
    for f in $files; do
        echo -n "$((10**i)) " >> profiling/"$f.txt"
    done
    for j in $(seq 1 10); do
        python3 genPoints.py $((10**i))
        for name in $files; do
            ./"$name"
            x=$(gprof "$name" gmon.out -bq | awk ' NR == 4 ' | awk '{print $(NF-1)}')
            if echo "$x" | grep -Eq '[0-9]+[.][0-9]+'; then
                echo -n "$x " >> profiling/"$name".txt
            fi
            gprof "$name" gmon.out -bp > profiling/logs/"$name"/"$name"_"$((10**i))"_"$j".txt
        done
    done
    for f in $files; do
        echo "" >> profiling/"$f.txt"
    done
done
