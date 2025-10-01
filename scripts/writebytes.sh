#!/usr/bin/env bash
#
# write_bytes.sh
# Usage: ./write_bytes.sh [output_file]
# Defaults to data.bin if no output file is given.

set -euo pipefail

OUTFILE=${1:-data.bin}

# Hex dump (big-endian 32-bit words). xxd -r -p ignores whitespace/newlines.
xxd -r -p <<'EOF' >"$OUTFILE"
00000000	00000001	00000000	00000000
FFFFFFFF	00000000	00000000	00000000
00000000	00000000	00000000	00000000
00000000	00000000	00000000	00000000
00000000	00000000	00000000	00000000

# Insert script bytes here

00000001	00000001	00000000	00000000
00000000	00000000	00000000	00000000
00000000	00000000	00000000	00000000
00000000	00000000	00000000	00000000
00000000	00000000	00000000	00000000
EOF

echo "Wrote binary data to $OUTFILE"
