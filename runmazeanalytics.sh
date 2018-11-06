#!/bin/sh

runtimes=5
width=10
height=10

for i in $*; do
    case ${i} in
        -w*)
            width=$(echo "${i}" | sed 's/^-w\([0-9]\+\)$/\1/')
            ;;
        -h*)
            height=$(echo "${i}" | sed 's/^-h\([0-9]\+\)$/\1/')
            ;;
        *)
            runtimes=${i}
            ;;
    esac
done

for i in $(seq -s' ' ${runtimes}); do
    ./put ${width} ${height}
done | perl maze_analytics.pl
