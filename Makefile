# Makefile для учебника «Алгоритмы и игры на Си».
# Требуется: pandoc, xelatex (MacTeX/TeX Live), компилятор cc (clang/gcc).

BOOK   := Algoritmy-i-igry-na-Si.pdf
TASKS  := Zadachi-s-sobesedovaniy-po-Si.pdf

# Все исходники, влияющие на PDF (для пересборки при изменениях).
CHAPTERS := $(wildcard docs/tutorial/*.md)
SOURCES  := $(wildcard code/*.c)

.PHONY: all pdf book tasks check clean help

## all   — собрать обе книги (полную + задачи) и проверить код
all: check pdf tasks

## pdf / book — собрать полный учебник
pdf book: $(BOOK)

$(BOOK): $(CHAPTERS) build/metadata.yaml build/build-pdf.sh
	./build/build-pdf.sh

## tasks — собрать отдельный мини-PDF только с задачами
tasks: $(TASKS)

$(TASKS): docs/tutorial/18a-interview-guide.md \
          docs/tutorial/19-interview-junior.md \
          docs/tutorial/20-interview-middle.md \
          docs/tutorial/21-interview-senior.md \
          build/metadata-tasks.yaml build/build-tasks-pdf.sh
	./build/build-tasks-pdf.sh

## check — скомпилировать все примеры code/*.c с -Wall -Wextra (без warning)
check:
	./build/check-code.sh

## clean — удалить собранные PDF
clean:
	rm -f $(BOOK) $(TASKS)

## help  — показать список целей
help:
	@grep -E '^## ' $(MAKEFILE_LIST) | sed 's/^## /  /'
