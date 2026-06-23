#!/bin/bash

set -e

echo "Building NetMonitor..."

cmake -S . -B build

cmake --build build

echo "Build completed."