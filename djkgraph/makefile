# Makefile para compilar e executar o código

# Comando de compilação
all: main

main: main.cpp dijkstra.cpp
	g++ -o main main.cpp dijkstra.cpp

# Comando de execução
run: all
	./main

# Limpeza de arquivos temporários
clean:
	rm -f main