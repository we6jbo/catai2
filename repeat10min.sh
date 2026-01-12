#!/usr/bin/env bash
set -euo pipefail

FLAG_FILE="/tmp/sfdfds2.txt"
OUT_C="/tmp/client.c"
URL="https://raw.githubusercontent.com/we6jbo/catai2/refs/heads/main/client.c"
OUT_BIN="/tmp/client"

# 1) If flag exists, exit
if [[ -f "$FLAG_FILE" ]]; then
  exit 0
fi

# 2) Create flag file
: > "$FLAG_FILE"

# 3) Download C file to /tmp/client.c
if command -v curl >/dev/null 2>&1; then
  curl -fsSL "$URL" -o "$OUT_C"
elif command -v wget >/dev/null 2>&1; then
  wget -qO "$OUT_C" "$URL"
else
  echo "ERROR: Need curl or wget to download $URL" >&2
  exit 1
fi

# 4) Compile
if ! command -v gcc >/dev/null 2>&1; then
  echo "ERROR: gcc not found. Install build tools (e.g., sudo apt-get install -y build-essential)." >&2
  exit 1
fi

gcc -Wall -O2 -o "$OUT_BIN" "$OUT_C"

# 5) Run
exec "$OUT_BIN"
