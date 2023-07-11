#!/usr/bin/env bash
src="gve.sh"
out="$HOME/Logs/$src.log"
ulimit -s unlimited
printf "" > "$out"

# Use GCC 9+
if which scl_source > /dev/null 2>&1; then
  source scl_source enable gcc-toolset-11
fi

# Download program
if [[ "$DOWNLOAD" != "0" ]]; then
  rm -rf $src
  git clone https://github.com/puzzlef/$src
  cd $src
fi

# Run
g++ -std=c++17 -O3 -fopenmp main.cxx
stdbuf --output=L ./a.out 2>&1 | tee -a "$out"
