CC = gcc
BISON = bison -d
FLEX = flex
CFLAGS = -Wall -Wno-unused-function -g

SRC_DIR = src
LEX_FILE = $(SRC_DIR)/calcLexico.l
YACC_FILE = $(SRC_DIR)/calcSintaxis.y

# Archivos fuente principales, agregamos calcSemantico.c
SRCS = $(SRC_DIR)/ast.c \
       $(SRC_DIR)/main.c \
       $(SRC_DIR)/interprete.c \
       $(SRC_DIR)/tabla_simbolos.c \
       $(SRC_DIR)/calcSemantico.c

# Archivos generados automáticamente + fuentes
OBJS = $(SRC_DIR)/calcSintaxis.tab.c $(SRC_DIR)/lex.yy.c $(SRCS)

TARGET = $(SRC_DIR)/parser

# Compila todo
all: $(TARGET)

# Link final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lfl

# Genera parser con Bison
$(SRC_DIR)/calcSintaxis.tab.c: $(YACC_FILE)
	$(BISON) -o $@ $<

# Genera analizador léxico con Flex
$(SRC_DIR)/lex.yy.c: $(LEX_FILE)
	$(FLEX) -o $@ $<

# Ejecuta un test individual
run: $(TARGET)
	@./$(TARGET) $(TEST) || true

# Ejecuta todos los tests automáticamente
TESTS = $(wildcard $(SRC_DIR)/test*.c)
tests: $(TARGET)
	@for t in $(TESTS); do \
	    echo ">> Ejecutando $$t"; \
	    ./$(TARGET) $$t; \
	    echo ""; \
	done

# Ayuda con los comandos disponibles
help:
	@echo "Comandos disponibles:"
	@echo "  make            -> compila el parser"
	@echo "  make run TEST=x -> ejecuta un test (ej: TEST=src/test1.c)"
	@echo "  make tests      -> ejecuta todos los tests y muestra la salida en terminal"
	@echo "  make clean      -> limpia archivos generados"

# Limpia todos los archivos generados
clean:
	rm -f $(SRC_DIR)/parser \
	      $(SRC_DIR)/calcSintaxis.tab.c \
	      $(SRC_DIR)/calcSintaxis.tab.h \
	      $(SRC_DIR)/lex.yy.c
