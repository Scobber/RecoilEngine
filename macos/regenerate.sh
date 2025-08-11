#!/usr/bin/env bash
set -e
cd "$(dirname "$0")/.."
cmake -S . -B build/Debug-Metal -DCMAKE_BUILD_TYPE=Debug "$@"
cmake -S . -B build/Release-Metal -DCMAKE_BUILD_TYPE=Release "$@"
