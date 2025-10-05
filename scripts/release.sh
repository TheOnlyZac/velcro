#!/usr/bin/env bash

# Accept a version argument (default: 1.0.0)
Version="${1:-1.0.0}"

# Normalize version string by removing any leading 'v'
if [[ "$Version" == v* ]]; then
    Version="${Version:1}"
fi

# Compose package name and paths using version
packageName="velcro-$Version-linux-x86_64"

# Change to the directory above the script
scriptDir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$scriptDir/.." || exit 1

# Make dist directory if it doesn't exist
if [ ! -d "dist" ]; then
    mkdir -p "dist"
fi

# Delete the build directory if it exists
if [ -d "build" ]; then
    rm -rf "build"
fi

# Create the build directory
mkdir -p build

# Run build commands
cd build || exit 1
cmake ..
cmake --build . --config Release
cd ..

# Move all files in ./bin to ./bin/bin/
if [ -d "build/bin" ]; then
    mkdir -p "build/bin/bin"
    mv build/bin/* build/bin/bin/ 2>/dev/null || true
fi

# Copy docs to bin/docs
docsSrc='docs'
docsDst='build/bin/docs'
if [ -d "$docsDst" ]; then
    rm -rf "$docsDst"
fi
if [ -d "$docsSrc" ]; then
    mkdir -p "$(dirname "$docsDst")"
    cp -r "$docsSrc" "$docsDst"
fi

# Copy samples to bin/samples
samplesSrc='samples'
samplesDst='build/bin/samples'
if [ -d "$samplesDst" ]; then
    rm -rf "$samplesDst"
fi
if [ -d "$samplesSrc" ]; then
    mkdir -p "$(dirname "$samplesDst")"
    cp -r "$samplesSrc" "$samplesDst"
fi

# Concatenate all packages/*/README.md into bin/README.md
outReadme='build/bin/README.md'
rm -f "$outReadme"
shopt -s nullglob
readmeFiles=(packages/*/README.md)
for file in "${readmeFiles[@]}"; do
    if [ -f "$file" ]; then
        cat "$file" >> "$outReadme"
        printf "\n\n" >> "$outReadme"
    fi
done
shopt -u nullglob

# Add top-level heading "Velcro v<version>" and increase all other headings by one level
if [ -f "$outReadme" ]; then
    tmpfile="$(mktemp)"
    echo "# Velcro v$Version" > "$tmpfile"
    # Read and transform lines
    while IFS= read -r line; do
        if [[ "$line" =~ ^(#+)(.*) ]]; then
            hashes="${BASH_REMATCH[1]}"
            text="${BASH_REMATCH[2]}"
            newHashes="$hashes#"
            echo "${newHashes}${text}" >> "$tmpfile"
        else
            echo "$line" >> "$tmpfile"
        fi
    done < "$outReadme"
    mv "$tmpfile" "$outReadme"
fi

# Rename build/bin to build/<packageName>
finalDir="build/$packageName"
if [ -d "$finalDir" ]; then
    rm -rf "$finalDir"
fi
if [ -d "build/bin" ]; then
    mv "build/bin" "$finalDir"
fi

# Create a archives of the directory
zipPath="build/${packageName}.zip"
if [ -f "$zipPath" ]; then
    rm -f "$zipPath"
fi
if [ -d "$finalDir" ]; then
    # Make tar.gz by default
    tar -C "build" -czf "${packageName}.tar.gz" "${packageName}"
    mv "${packageName}.tar.gz" "dist/${packageName}.tar.gz"

    # Make zip if zip command is available
    if command -v zip >/dev/null 2>&1; then
        (cd "build" && zip -r "${packageName}.zip" "${packageName}")
        mv "build/${packageName}.zip" "dist/${packageName}.zip"
    fi
fi

exit 0
