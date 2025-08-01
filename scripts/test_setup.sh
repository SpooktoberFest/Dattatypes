#!/bin/bash

# Define and go to source directory
SOURCE_DIR=$(dirname "$(realpath "$0")")/../
cd "$SOURCE_DIR" || {
    echo "Failed to change directory to $SOURCE_DIR";
    exit 1;
}

# Create test dirs
mkdir -p bin
mkdir -p build

# Build tests
cd build
cmake -S "$SOURCE_DIR" -B .
make

# Run tests
cd ..
for file in bin/*; do
    if [[ -f "$file" && -x "$file" ]]; then
        "$file"
    fi
done

echo "::FINISHED::"
