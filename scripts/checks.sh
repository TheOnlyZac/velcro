#!/bin/bash

set -e
pushd "$(dirname "$0")/.." > /dev/null

mkdir -p build
cd build

# Check debug builds
echo Checking debug build...
cmake .. -DCMAKE_BUILD_TYPE=Debug > /dev/null 2>&1
cmake --build . > /dev/null 2>&1

for infile in ../samples/*.vel; do
    outfile="./$(basename "${infile%.vel}").out"
    echo "Checking $infile (debug)..."
    ./bin/velcroc "$infile" "$outfile" > /dev/null
    ./bin/velcrop "$outfile" "$outfile.pnach" 00000000 > /dev/null
    ./bin/velcrod "$outfile" "$outfile_decompiled.vel" > /dev/null
done

# Check release builds
echo Checking release build...
cmake .. -DCMAKE_BUILD_TYPE=Release > /dev/null 2>&1
cmake --build . > /dev/null 2>&1

for infile in ../samples/*.vel; do
    # Skip sampler.vel
    if [[ "$(basename "$infile")" == "sampler.vel" ]]; then
        continue
    fi
    outfile="./$(basename "${infile%.vel}").out"
    echo "Checking $infile (release)..."
    ./bin/velcroc "$infile" "$outfile" > /dev/null
    ./bin/velcrop "$outfile" "$outfile.pnach" 00000000 > /dev/null
    ./bin/velcrod "$outfile" "$outfile_decompiled.vel" > /dev/null
done

echo "All checks passed!"
popd > /dev/null
