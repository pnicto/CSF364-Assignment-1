#!/usr/bin/env bash

rm -rf docs

# build the project
make PLATFORM=PLATFORM_WEB
# generate docs
# doxygen

mkdir -p docs
# mv docs/html/* docs/
mv build/* docs/
mv docs/convex_hull.html docs/index.html
