#!/usr/bin/bash
for f in "$1"/*.txt; do
    python3 averagetimes.py "$f" > "$1"/results/"$(basename "$f")"
done
