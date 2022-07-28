#!/usr/bin/env bash


c=0;
while [ -e snapshot_$c ]; do
    c=$[c+1]
done
echo Creating snapshot snapshot_$c

make matexp.exe && (
    rm -f scores.csv
    mkdir -p snapshot_$c;
    cp matexp_solution.hpp config.make snapshot_$c
    #cse142 job run --lab caches2 --force "make autograde";
    #./autograde.py --submission . --results results.json --scores scores.csv

    #  Replace the lines above with these to turn of the regressions
    cse142 job run --lab $(cat short_name) --force "make bench.csv";
    ./autograde.py --submission . --results results.json --scores scores.csv
    if ! grep "The machine you ran was slow" results.json;  then
       pretty-csv scores.csv
       cp bench.csv bench?.csv snapshot_$c
       cp scores.csv snapshot_$c
    fi
)


