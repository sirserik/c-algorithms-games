#!/usr/bin/env bash
# Компилирует все примеры из code/*.c с теми же флагами, что в книге.
# Падает, если хоть один файл не собрался или дал предупреждение.

set -uo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
CODE="$ROOT/code"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

fail=0
shopt -s nullglob
for f in "$CODE"/*.c; do
  name="$(basename "$f" .c)"
  out="$TMP/$name"
  if cc -std=c11 -Wall -Wextra -O2 "$f" -o "$out" 2> "$TMP/$name.log"; then
    if [ -s "$TMP/$name.log" ]; then
      echo "WARN  $name.c"
      cat "$TMP/$name.log"
      fail=1
    else
      echo "OK    $name.c"
    fi
  else
    echo "FAIL  $name.c"
    cat "$TMP/$name.log"
    fail=1
  fi
done

if [ "$fail" -ne 0 ]; then
  echo "--- есть проблемы ---"
  exit 1
fi
echo "--- все примеры собрались чисто ---"
