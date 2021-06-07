# socket-chat

Chat-room application implemented using sockets in C++.

## Grupo

| **Nome**                 | **N°USP**    |
|-----------------------------------|--------------|
| Thiago Sena de Queiroz            |   11218768   |
| Fernando de Souza Lincoln         |   10716960   |
| Dikson Ferreira dos Santos        |   11275147   |
| Victor Kendi Arakaki              |   11219092   |

## Building

Use ```make all``` to build the application. It will geneterate two executable files: ```server``` and ```client```. Run both of them with ```./{filename}```.

## SO's and compiler

The application has been tested in:

* Ubuntu 20.10
* Ubuntu 18.04

The application was built using **GCC 10.2.0**.

## Using the application

### Server

After running the server it doesn't require any interaction, to stop it, just give it any input. If the server is interrupted, it will send a message to all conected client applications to stop.

### Client

Every interaction with the server follow the model: ```command:{text}```. Therre are 4 commands available:

* ```login```: changes user's display name. Ex: ```login:MyAccount```.
* ```room```: changes user's room. Only users on the same room receive messages from the user. Ex: ```room:MyRoom```.
* ```msg```: sends a message to the user's current room. Ex: ```msg:Hello World``` .
* ```exit```: stop the client application.

The application will also stop if the servers stops. Only ```exit``` doesn't accept ```text```.

Observation: because the input reading blocks the thread, if the server sends a stop signal, the client application will need an input to actually stop.