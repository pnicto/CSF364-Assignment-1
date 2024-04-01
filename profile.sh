#!/usr/bin/bash
maxExponent=7
files="$@"
rm -r profiling/uniform
rm -r profiling/polygon
mkdir profiling/uniform
mkdir profiling/polygon
mkdir profiling/uniform/logs
mkdir profiling/uniform/results
mkdir profiling/polygon/logs
mkdir profiling/polygon/results
for f in $files; do
    g++ -pg -O3 "$f".cpp -o "$f"
    echo -n > profiling/uniform/"$f".txt
    echo -n > profiling/polygon/"$f".txt
    mkdir profiling/uniform/logs/"$f"
    mkdir profiling/polygon/logs/"$f"
done

#polygon
numPoints=(5 25 50 250 500 2500 5000)
for i in ${numPoints[@]}; do
    for f in $files; do
        echo -n "$i " >> profiling/polygon/"$f.txt"
    done
    for j in $(seq 1 10); do
        python3 genPoints.py --polygon $i
        for name in $files; do
            ./"$name"
            x=$(gprof "$name" gmon.out -bq | awk ' NR == 4 ' | awk '{print $(NF-1)}')
            if echo "$x" | grep -Eq '[0-9]+[.][0-9]+'; then
                echo -n "$x " >> profiling/polygon/"$name".txt
            fi
            gprof "$name" gmon.out -bp > profiling/polygon/logs/"$name"/"$name"_"$i"_"$j".txt
        done
    done
    for f in $files; do
        echo "" >> profiling/polygon/"$f".txt
    done
done
(
cd profiling
./average.sh polygon
)

#uniform
for i in $(seq 1 $maxExponent) ; do
    for f in $files; do
        echo -n "$((10**i)) " >> profiling/uniform/"$f.txt"
    done
    for j in $(seq 1 10); do
        python3 genPoints.py --uniform $((10**i))
        for name in $files; do
            ./"$name"
            x=$(gprof "$name" gmon.out -bq | awk ' NR == 4 ' | awk '{print $(NF-1)}')
            if echo "$x" | grep -Eq '[0-9]+[.][0-9]+'; then
                echo -n "$x " >> profiling/uniform/"$name".txt
            fi
            gprof "$name" gmon.out -bp > profiling/uniform/logs/"$name"/"$name"_"$((10**i))"_"$j".txt
        done
    done
    for f in $files; do
        echo "" >> profiling/uniform/"$f".txt
    done
done
(
cd profiling
./average.sh uniform
)

