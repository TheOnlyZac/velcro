#! /bin/bash

set -e
pushd "$(dirname "$0")/.."

echo "Running compiler tests..."
./build/bin/velcroc ./samples/earthquake.cro /tmp/earthquake.s2m > /dev/null
./build/bin/velcroc ./samples/timewarp.cro /tmp/timewarp.s2m > /dev/null
./build/bin/velcroc ./samples/sampler.cro /tmp/sampler.s2m > /dev/null

echo "3/3 files compiled."

diff ./samples/earthquake_expected.s2m /tmp/earthquake.s2m
# diff ./samples/timewarp_expected.s2m /tmp/timewarp.s2m
# diff ./samples/sampler_expected.s2m /tmp/sampler.s2m

echo "1/1 files matched expected."

popd
