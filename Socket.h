#ifndef SOCKET_H
#define SOCKET_H

#include <utility>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class Socket
{
    public:
        Socket();
        Socket(int &fd);
        Socket(Socket&&);
        virtual ~Socket();

        Socket accept();
        void cerrar();
        bool enlazar(struct addrinfo *result);
        bool conectar(struct addrinfo* result);
        bool escuchar();
        int enviar(char* buffer, int tam_buffer);
        int recibir(char* buffer, int tamanio_msg);
        int enlazar_y_escuchar(struct addrinfo* result);

    protected:

    private:
    int fd;
    bool conectado;
};

#endif // SOCKET_H
