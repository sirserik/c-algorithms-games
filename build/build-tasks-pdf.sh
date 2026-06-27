#!/usr/bin/env bash
# Собирает ОТДЕЛЬНЫЙ мини-PDF только с задачами с собеседований
# (гид + три приложения Junior/Middle/Senior).

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${1:-$ROOT/Zadachi-s-sobesedovaniy-po-Si.pdf}"

FILES=(
  "$ROOT/docs/tutorial/18a-interview-guide.md"
  "$ROOT/docs/tutorial/19-interview-junior.md"
  "$ROOT/docs/tutorial/20-interview-middle.md"
  "$ROOT/docs/tutorial/21-interview-senior.md"
)

echo "Сборка мини-PDF задач: ${#FILES[@]} раздела → $OUT"

TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

pandoc \
  -s \
  "$ROOT/build/metadata-tasks.yaml" \
  "${FILES[@]}" \
  --pdf-engine=xelatex \
  --top-level-division=chapter \
  --highlight-style=tango \
  --listings=false \
  -o "$TMP/tasks.tex"

cd "$TMP"
for i in 1 2 3; do
  echo "  xelatex pass $i/3..."
  xelatex -interaction=batchmode -halt-on-error tasks.tex >/dev/null 2>&1 || {
    xelatex -interaction=nonstopmode tasks.tex 2>&1 | tail -30
    exit 1
  }
done

mv "$TMP/tasks.pdf" "$OUT"
cd "$ROOT"
echo "Готово: $OUT ($(du -h "$OUT" | cut -f1))"
