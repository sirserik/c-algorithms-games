#!/usr/bin/env bash
# Собирает PDF учебника «Алгоритмы и игры на Си» из docs/tutorial/*.md.
# Использование: ./build/build-pdf.sh [output.pdf] [файл1.md файл2.md ...]

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
# Файл метаданных можно переопределить через переменную META
# (CI подставляет вариант со свободными шрифтами).
META="${META:-$ROOT/build/metadata.yaml}"
OUT="${1:-$ROOT/Algoritmy-i-igry-na-Si.pdf}"
shift || true

if [ "$#" -gt 0 ]; then
  FILES=("$@")
else
  FILES=(
    "$ROOT/docs/tutorial/00-intro.md"
    "$ROOT/docs/tutorial/01-setup.md"
    "$ROOT/docs/tutorial/02-basics.md"
    "$ROOT/docs/tutorial/03-arrays-strings-pointers.md"
    "$ROOT/docs/tutorial/04-numbers.md"
    "$ROOT/docs/tutorial/05-euclid-primes.md"
    "$ROOT/docs/tutorial/06-array-algorithms.md"
    "$ROOT/docs/tutorial/07-search.md"
    "$ROOT/docs/tutorial/08-sorting.md"
    "$ROOT/docs/tutorial/09-recursion.md"
    "$ROOT/docs/tutorial/10-strings.md"
    "$ROOT/docs/tutorial/11-data-structures.md"
    "$ROOT/docs/tutorial/12-guess-number.md"
    "$ROOT/docs/tutorial/13-rock-paper-scissors.md"
    "$ROOT/docs/tutorial/14-hangman.md"
    "$ROOT/docs/tutorial/15-tic-tac-toe.md"
    "$ROOT/docs/tutorial/16-minesweeper.md"
    "$ROOT/docs/tutorial/17-snake.md"
    "$ROOT/docs/tutorial/18-next.md"
    "$ROOT/docs/tutorial/18a-interview-guide.md"
    "$ROOT/docs/tutorial/19-interview-junior.md"
    "$ROOT/docs/tutorial/20-interview-middle.md"
    "$ROOT/docs/tutorial/21-interview-senior.md"
  )
fi

EXISTING=()
for f in "${FILES[@]}"; do
  if [ -f "$f" ]; then
    EXISTING+=("$f")
  else
    echo "skip: $f (нет файла)" >&2
  fi
done

echo "Сборка PDF: ${#EXISTING[@]} глав → $OUT"

# Генерим .tex через pandoc, потом гоним xelatex 3 раза подряд —
# чтобы TOC, ссылки и номера страниц сошлись после любого объёма правок.
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

pandoc \
  -s \
  "$META" \
  "${EXISTING[@]}" \
  --pdf-engine=xelatex \
  --top-level-division=chapter \
  --highlight-style=tango \
  --listings=false \
  -o "$TMP/book.tex"

cd "$TMP"
for i in 1 2 3; do
  echo "  xelatex pass $i/3..."
  xelatex -interaction=batchmode -halt-on-error book.tex >/dev/null 2>&1 || {
    xelatex -interaction=nonstopmode book.tex 2>&1 | tail -30
    exit 1
  }
done

mv "$TMP/book.pdf" "$OUT"
cd "$ROOT"

echo "Готово: $OUT ($(du -h "$OUT" | cut -f1))"
