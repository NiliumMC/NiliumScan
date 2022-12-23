#!/usr/bin/env bash

for correct_path in $(find * -type f -name '*.c' -or -name '*.h'); do
    if [ "$correct_path" != "$(awk 'BEGIN{} {if(NR == 2){print substr($0,5)}}' $correct_path)" ]; then
        echo -e "[\x1B[31mFAIL\x1B[0m] $correct_path"
    fi
done

