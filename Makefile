all: client server

server: build/Socket.o build/Address.o build/Server.o server.cpp
	g++ -pthread build/Address.o build/Socket.o build/Server.o server.cpp -o server -Wall

client: build/Socket.o build/Address.o client.cpp
	g++ -pthread build/Address.o build/Socket.o client.cpp -o client -Wall

build/Server.o: src/Server.cpp
	g++ -c src/Server.cpp -o build/Server.o

build/Socket.o: src/Socket.cpp
	g++ -c src/Socket.cpp -o build/Socket.o

build/Address.o: src/Address.cpp
	g++ -c src/Address.cpp -o build/Address.o