# Makefile para compilar o projeto

# Compilador
CC = gcc

# Flags de compilação:
# -Wall: Habilita a maioria dos avisos (warnings)
# -Wextra: Habilita avisos extras
# -g: Gera informações de depuração (para usar com gdb)
# -Isrc: Diz ao compilador para procurar arquivos de cabeçalho (.h) no diretório 'src'
CFLAGS = -Wall -Wextra -g -Isrc

# Flags do linker (se precisar de bibliotecas como a de matemática -lm, adicione aqui)
LDFLAGS =

# Diretórios
SRCDIR = src
OBJDIR = obj
BINDIR = bin

# Arquivos fonte .c (automático se todos os .c em SRCDIR forem parte do executável principal)
# Se tiver outros .c que não fazem parte do executável principal, liste-os explicitamente.
# Para este caso, vamos listar explicitamente, já que são poucos.
SOURCES = $(SRCDIR)/main.c $(SRCDIR)/leituraTerminal.c

# Arquivos objeto .o (serão colocados no diretório OBJDIR)
# Substitui a extensão .c por .o e adiciona o prefixo do diretório de objetos.
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

# Nome do executável final (será colocado no diretório BINDIR)
EXECUTABLE_NAME = main
EXECUTABLE = $(BINDIR)/$(EXECUTABLE_NAME)

# ----- Regras -----

# Regra padrão (o que acontece quando você digita 'make')
all: $(EXECUTABLE)

# Regra para criar o executável final
# Depende de todos os arquivos objeto e do diretório binário existir.
$(EXECUTABLE): $(OBJECTS) | $(BINDIR)
	@echo "Ligando os arquivos objeto para criar o executável..."
	$(CC) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)
	@echo "Executável $(EXECUTABLE) criado com sucesso!"

# Regra genérica para compilar arquivos .c para .o
# Depende do arquivo .c correspondente e do arquivo .h (leituraTerminal.h)
# Também depende do diretório de objetos existir (order-only prerequisite).
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/leituraTerminal.h | $(OBJDIR)
	@echo "Compilando $< -> $@"
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para criar o diretório de binários, se não existir
$(BINDIR):
	@echo "Criando diretório $(BINDIR)..."
	@mkdir -p $(BINDIR)

# Regra para criar o diretório de objetos, se não existir
$(OBJDIR):
	@echo "Criando diretório $(OBJDIR)..."
	@mkdir -p $(OBJDIR)

# Regra para limpar os arquivos gerados (objetos e executável)
clean:
	@echo "Limpando arquivos gerados..."
	@rm -f $(OBJECTS)
	@rm -f $(EXECUTABLE)
	@echo "Removendo diretório de objetos $(OBJDIR)..."
	@rm -rf $(OBJDIR)
	# Você pode descomentar a linha abaixo se quiser remover o diretório bin também,
	# mas geralmente o diretório bin é mantido.
	# @rm -rf $(BINDIR)
	@echo "Limpeza concluída."

# Phony targets: alvos que não são nomes de arquivos.
.PHONY: all clean