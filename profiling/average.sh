#!/usr/bin/bash
for f in *.txt; do
    python3 averagetimes.py "$f" > results/"$f"
done
