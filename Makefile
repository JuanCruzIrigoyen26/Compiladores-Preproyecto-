CC = gcc
BISON = bison -d
FLEX = flex
CFLAGS = -Wall -g

SRC_DIR = src
LEX_FILE = $(SRC_DIR)/calcLexico.l
YACC_FILE = $(SRC_DIR)/calcSintaxis.y
SRCS = $(SRC_DIR)/ast.c $(SRC_DIR)/main.c $(SRC_DIR)/interprete.c $(SRC_DIR)/tabla_simbolos.c
OBJS = $(SRC_DIR)/calcSintaxis.tab.c $(SRC_DIR)/lex.yy.c $(SRCS)
TARGET = $(SRC_DIR)/parser

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -lfl

$(SRC_DIR)/calcSintaxis.tab.c: $(YACC_FILE)
	$(BISON) -o $@ $<

$(SRC_DIR)/lex.yy.c: $(LEX_FILE)
	$(FLEX) -o $@ $<

run: $(TARGET)
	./$(TARGET) $(TEST)

TESTS = $(wildcard $(SRC_DIR)/test*.c)

tests: $(TARGET)
	@for t in $(TESTS); do \
	    echo ">> Ejecutando $$t"; \
	    ./$(TARGET) $$t; \
	    echo ""; \
	done

help:
	@echo "Comandos disponibles:"
	@echo "  make            -> compila el parser"
	@echo "  make run TEST=x -> ejecuta un test (ej: TEST=src/test1.c)"
	@echo "  make tests      -> ejecuta todos los tests y muestra la salida en terminal"
	@echo "  make clean      -> limpia archivos generados"

clean:
	rm -f $(SRC_DIR)/parser $(SRC_DIR)/calcSintaxis.tab.c $(SRC_DIR)/calcSintaxis.tab.h $(SRC_DIR)/lex.yy.c
