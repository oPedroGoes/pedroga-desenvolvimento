# Nome do executável
TARGET = ted

# Nome do aluno para o pacote .zip
ALUNO = pedrogoes

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -O0 -g -ggdb

# Diretório de código-fonte e cabeçalhos
SRC_DIR = .
INC_DIR = .

# Bibliotecas adicionais (use -lm para a biblioteca matemática, por exemplo)
LIBS = -lm

# Encontra todos os arquivos .c no diretório de origem
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Gera a lista de arquivos .o a partir dos .c
OBJS = $(SRCS:.c=.o)

# Adiciona o diretório de include às flags
INC_FLAGS = -I$(INC_DIR)

# --- Regras de Compilação ---

# Regra principal: cria o executável (por padrão, é a primeira regra)
all: $(TARGET)

# Regra de compilação do executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

# Regra de compilação dos arquivos de objeto
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# Limpar arquivos intermediários e o executável
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)

# Empacotar para entrega
# ATENÇÃO: A variável PROJ_NAME não estava definida, usei a TARGET
pack:
	@echo "Empacotando o projeto para entrega..."
	@echo $(ALUNO) > LEIA-ME.txt
	@date >> LEIA-ME.txt
	@zip -r $(ALUNO).zip *.c *.h Makefile LEIA-ME.txt

# Falso alvo para evitar conflitos com arquivos
.PHONY: all clean pack