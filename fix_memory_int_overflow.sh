#!/bin/bash

set -e

if [[ $# -ne 2 ]]; then
    echo "Usage: $0 --enable|--disable <file>"
    exit 1
fi

MODE="$1"
FILE="$2"

case "$MODE" in
    --enable)
        sed -i.bak 's|float=FALSE)|float=FALSE, wrap.const=function(x) paste0("(long int)", x), wrap.var = function(x) paste0("(long int)", x))|g' "$FILE"
        ;;
    --disable)
        sed -i.bak 's|float=FALSE, wrap.const=function(x) paste0("(long int)", x), wrap.var = function(x) paste0("(long int)", x))|float=FALSE)|g' "$FILE"
        ;;
    *)
        echo "Unknown mode: $MODE"
        echo "Use --enable or --disable"
        exit 1
        ;;
esac

echo "Patch applied: $MODE"

