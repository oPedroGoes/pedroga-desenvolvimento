# Nome do executável (altere 'programa' para o nome desejado)
TARGET = ted

# Nome do aluno (para o alvo 'pack', ajuste conforme necessário)
ALUNO = eduardoabreu

# Compilador
CC = gcc

# Flags de compilação
# -Wall -Wextra: Habilita muitos avisos úteis.
# -Wno-unused-parameter: Suprime avisos sobre parâmetros de função não utilizados (útil durante o desenvolvimento).
# -std=c99: Define o padrão C99.
# -O0: Nível de otimização 0 (bom para depuração). Considere -O2 ou -O3 para versões de lançamento.
# -g -ggdb: Gera informações de depuração para o GDB.
# -fstack-protector-all: Adiciona proteção contra estouro de pilha.
CFLAGS = -std=c99 -fstack-protector-all -Wall -Wextra -Wno-unused-parameter -O0 -g -ggdb

# Diretório de código-fonte (diretório atual, pois o Makefile está em src/)
SRC_DIR = .

# Diretório de cabeçalhos (diretório atual, pois os headers estão em src/)
INC_DIR = .

# Diretório de bibliotecas
# Exemplo: ../lib se você tiver uma pasta 'lib' um nível acima do 'src' (na raiz do projeto).
# Ajuste se a estrutura da sua biblioteca for diferente ou se não for usada.
LIB_DIR = ../lib

# Bibliotecas adicionais a serem linkadas
# Exemplo: -lm para a biblioteca matemática. Adicione outras conforme necessário (ex: -lpthread).
LIBS = -lm

# Lista de arquivos de origem (todos os arquivos .c no diretório atual, ou seja, src/)
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Lista de arquivos de objeto (derivados dos arquivos .c)
OBJS = $(SRCS:.c=.o)

# Flags para incluir diretórios de cabeçalho
INC_FLAGS = -I$(INC_DIR)

# Flags para diretórios de bibliotecas
# Descomente e ajuste se você tiver bibliotecas em LIB_DIR.
# Se LIBS forem apenas bibliotecas do sistema (como -lm), isso pode não ser necessário.
LIB_FLAGS = # -L$(LIB_DIR) # Exemplo: -L../lib

# Regra padrão: compilar o executável (geralmente o primeiro alvo é o padrão)
all: $(TARGET)

# Regra de compilação dos arquivos de objeto (.c para .o)
# '$<' é o nome do primeiro pré-requisito (o arquivo .c)
# '$@' é o nome do alvo (o arquivo .o)
%.o: %.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# Regra de compilação do executável (linka todos os arquivos .o)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIB_FLAGS) $(LIBS) -o $(TARGET)

# Limpar arquivos intermediários (.o) e o executável
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET) core.* vgcore.* # Remove também core dumps

# Alvos falsos para evitar conflitos com nomes de arquivos e para clareza
.PHONY: all clean pack

# Regra para empacotar o projeto para entrega
# Esta regra 'pack' assume que:
# 1. Este Makefile está localizado no diretório 'src'.
# 2. Um arquivo 'LEIA-ME.txt' está localizado no diretório pai de 'src' (a raiz do projeto).
# 3. O arquivo zip '$(ALUNO).zip' será criado no diretório pai.
# 4. Um arquivo '.entrega' será criado temporariamente na raiz do projeto.
pack: $(TARGET)
	# Cria o arquivo .entrega no diretório pai (raiz do projeto)
	@echo "Criando arquivo .entrega em ../.entrega"
	@echo "$(ALUNO)" > ../.entrega
	@date >> ../.entrega
	# Remove o arquivo zip antigo no diretório pai, se existir
	@echo "Removendo zip antigo ../$(ALUNO).zip (se existir)"
	@rm -f ../$(ALUNO).zip
	# Vai para o diretório pai e cria o arquivo zip.
	# '$(notdir $(CURDIR))' resultará no nome do diretório atual (que é 'src').
	@echo "Criando arquivo zip ../$(ALUNO).zip"
	@cd .. && zip -q $(ALUNO).zip \
	    src/*.c \
	    src/*.h \
	    src/Makefile \
	    LEIA-ME.txt \
	    .entrega
	# Limpa o arquivo .entrega da raiz do projeto
	@echo "Removendo arquivo ../.entrega"
	@rm -f ../.entrega
	@echo "Pacote $(ALUNO).zip criado com sucesso em ../"