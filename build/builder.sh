#!/bin/bash

# Clean (Remove everything except himself)
if [[ $(echo "$1" | grep -Eq "^.*C.*$"; echo $?) -eq 0 ]]; then
    find ../build/* ! -iname 'builder.sh' -exec rm -rf {} +
fi

# Build Debug
if [[ $(echo "$1" | grep -Eq "^.*D.*$"; echo $?) -eq 0 ]]; then
    if [[ $(echo "$1" | grep -Eq "^.*S.*$"; echo $?) -eq 0 ]]; then
        cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DUSE_SANITIZE=ON && make
    else
        cmake .. -DCMAKE_BUILD_TYPE=DEBUG && make
    fi
fi

# Build Release
if [[ $(echo "$1" | grep -Eq "^.*R.*$"; echo $?) -eq 0 ]]; then
    cmake .. && make
fi

