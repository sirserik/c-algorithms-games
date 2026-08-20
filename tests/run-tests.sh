#!/usr/bin/env bash
# Регрессионные тесты для примеров книги «Алгоритмы и игры на Си».
#
# Каждую программу из code/ собираем, запускаем и сверяем вывод с
# эталоном из tests/expected/. Если вывод изменился — тест падает, и
# сразу видно, какой правкой кода мы сломали то, что напечатано в книге.
#
#   ./tests/run-tests.sh            — прогнать тесты
#   ./tests/run-tests.sh --update   — перезаписать эталоны текущим выводом
#
# Программы, зависящие от случайности (игры с rand) или от адресов в
# памяти, сюда не входят или прогоняются с нормализацией — см. ниже.

set -uo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
CODE="$ROOT/code"
IN="$ROOT/tests/inputs"
EXP="$ROOT/tests/expected"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

UPDATE=0
if [ "${1:-}" = "--update" ]; then
  UPDATE=1
fi

# Детерминированные программы: вывод не зависит ни от времени, ни от rand.
# Для тех, кому нужен ввод, рядом лежит tests/inputs/ИМЯ.txt.
#
# Сюда намеренно не попали:
#   05-bench, 07-bench, 08-bench, 09-fib-bench, 11-growth — печатают
#     секунды, числа плавают от машины к машине;
#   09-depth — нарочно роняет программу переполнением стека, да ещё и
#     собирается только с -O0 (на -O2 компилятор разворачивает
#     рекурсию в цикл, и падения нет);
#   02-types — печатает результат знакового переполнения, а это UB:
#     обещать одинаковый ответ на разных компиляторах нельзя;
#   08-cmp-trap — показывает, что делает qsort с нечестным
#     компаратором, а её реализация в каждой libc своя;
#   12-guess, 13-rps, 14-hangman, 16-minesweeper, 17-snake — игры,
#     вывод зависит от rand и от времени.
PROGRAMS=(
  01-hello
  02-input
  02-multiplication
  03-arrays
  03-dynamic
  03-pointers
  03-strings
  04-digits
  04-factorial
  04-fizzbuzz
  04-overflow
  05-gcd
  05-prime
  05-sieve
  06-count-rotate
  06-minmax
  06-reverse
  07-binary
  07-binary-edge
  07-linear
  08-bubble
  08-insertion
  08-qsort
  08-selection
  08-stable
  09-factorial
  09-fib
  09-hanoi
  10-anagram
  10-palindrome
  10-reverse
  10-words
  11-dynarray
  11-list
  11-queue
  11-stack
  13-rps-check
  15-tic-tac-toe
  18-files
  19-01-swap
  19-02-reverse-string
  19-03-palindrome
  19-04-factorial
  19-05-secondmax
  19-06-vowels
  19-07-prime
  19-08-digits
  20-01-mystr
  20-02-reverse-list
  20-03-floyd
  20-04-popcount
  20-05-binsearch
  20-06-merge
  20-07-dedup
  20-08-oddtimes
  21-01-quicksort
  21-02-mergesort
  21-03-generic-stack
  21-04-atoi-itoa
  21-05-funcptr
  21-06-ringbuf
  21-07-hashtable
  21-08-list-intersection
  21-09-lru
  21-10-reverse-int
)

# 03-pointers печатает настоящие адреса — они меняются от запуска к
# запуску. Заменяем любой 0x... на 0xADDR, чтобы сравнение имело смысл.
normalize() {
  # Нулевой указатель через %p: на macOS это 0x0, в glibc — (nil).
  # Стандарт вид не оговаривает, поэтому приводим оба к одному виду,
  # иначе тест зеленеет на маке и краснеет в CI на Ubuntu.
  # Порядок важен: сначала адреса, потом (nil). Наоборот нельзя —
  # в подставленном 0xADDR первое правило увидит хекс-цифры «ADD»
  # и сделает из него 0xADDRR.
  sed -E -e 's/0x[0-9a-fA-F]+/0xADDR/g' -e 's/\(nil\)/0xADDR/g'
}

mkdir -p "$EXP"

pass=0
fail=0
failed_names=()

for name in "${PROGRAMS[@]}"; do
  src="$CODE/$name.c"
  if [ ! -f "$src" ]; then
    echo "НЕТ ФАЙЛА  $name.c"
    fail=$((fail + 1))
    failed_names+=("$name (нет исходника)")
    continue
  fi

  if ! cc -std=c11 -Wall -Wextra -O2 "$src" -o "$TMP/$name" 2>"$TMP/$name.build"; then
    echo "НЕ СОБРАЛСЯ  $name.c"
    cat "$TMP/$name.build"
    fail=$((fail + 1))
    failed_names+=("$name (сборка)")
    continue
  fi

  stdin_file="$IN/$name.txt"
  if [ -f "$stdin_file" ]; then
    "$TMP/$name" < "$stdin_file" 2>&1 | normalize > "$TMP/$name.out"
  else
    "$TMP/$name" < /dev/null 2>&1 | normalize > "$TMP/$name.out"
  fi

  expected="$EXP/$name.txt"
  if [ "$UPDATE" -eq 1 ]; then
    cp "$TMP/$name.out" "$expected"
    echo "ЭТАЛОН     $name"
    pass=$((pass + 1))
    continue
  fi

  if [ ! -f "$expected" ]; then
    echo "НЕТ ЭТАЛОНА  $name (запусти с --update)"
    fail=$((fail + 1))
    failed_names+=("$name (нет эталона)")
    continue
  fi

  if diff -u "$expected" "$TMP/$name.out" > "$TMP/$name.diff"; then
    echo "OK         $name"
    pass=$((pass + 1))
  else
    echo "ПАДЕНИЕ    $name"
    cat "$TMP/$name.diff"
    fail=$((fail + 1))
    failed_names+=("$name (вывод)")
  fi
done

echo
echo "Прошло: $pass, упало: $fail, всего: ${#PROGRAMS[@]}"

if [ "$fail" -ne 0 ]; then
  echo "Не прошли:"
  for n in "${failed_names[@]}"; do
    echo "  - $n"
  done
  exit 1
fi
