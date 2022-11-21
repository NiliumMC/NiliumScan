#!/bin/bash

# Clean (Remove everything except himself)
if [[ $(echo "$1" | grep -Eq "^.*C.*$"; echo $?) -eq 0 ]]; then
    find ../build/* ! -iname 'builder.sh' -printf "%f\n" -exec rm -rf {} +
fi

# Build Debug
if [[ $(echo "$1" | grep -Eq "^.*D.*$"; echo $?) -eq 0 ]]; then
    cmake .. -DISDEBUG=ON && make
fi

# Build Release
if [[ $(echo "$1" | grep -Eq "^.*R.*$"; echo $?) -eq 0 ]]; then
    cmake .. && make
fi

