#!/usr/bin/env bash
set -e
cd "$(dirname "$0")/.."
CONFIG="$CONFIGURATION"
ACTION="${ACTION:-build}"
if [ "$ACTION" = "clean" ]; then
  cmake --build "build/$CONFIG" --target clean
else
  cmake --build "build/$CONFIG"
fi
