#include "Socket.h"

Socket::Socket()
{
    //ctor
}

Socket::Socket(int &fd, bool estado_conectado){
    this->fd=fd;
    this->conectado = estado_conectado;
}

Socket::Socket(Socket&& otro){
    this->fd = otro.fd;
    this->conectado = otro.conectado;
}


Socket Socket::accept(){
    int fd = ::accept(this->fd,NULL,NULL);
    return (std::move(Socket(fd)));
}

void Socket::cerrar(){
    ::shutdown(this->fd, SHUT_RDWR);
    ::close(this->fd);
}

bool Socket::enlazar(struct addrinfo *result){
    int val =1;
    int s;
    struct addrinfo *ptr;
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next){
		this->fd = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (this->fd == -1){
			continue;
		}
		s = setsockopt(this->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
		if (s == -1){
			close(this->fd);
			continue;
		}
		if (bind(this->fd, ptr->ai_addr, ptr->ai_addrlen) == 0){
			break;
		}
		close(this->fd);
	}
	if (ptr == NULL){
        return false;
	}
	return true;
}

bool Socket::conectar(struct addrinfo* result){
    struct addrinfo* ptr;
	for(ptr = result; ptr != NULL; ptr=ptr->ai_next){
		this->fd= socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if(this->fd == -1){
			continue;
		}
		if (connect(this->fd, ptr->ai_addr, ptr->ai_addrlen) != -1){
			break;
		}
		close(this->fd);
	}
	freeaddrinfo(result);
	if (ptr == NULL){
		return false;
	}
	return true;
}

bool Socket::escuchar(){
    int largo_cola=60;
    int escucho = ::listen(this->fd, largo_cola);
    if (escucho == 0){
        return true;
    }
    return false;
}


int Socket::enviar(char* buffer, int tam_buffer){
    int bytes_enviados=0;
	bool socket_todavia_valido = true;
	while(bytes_enviados < tam_buffer && socket_todavia_valido){
		int contador_aux=::send(this->fd, &buffer[bytes_enviados],
							 tam_buffer-bytes_enviados, 0);
		if (contador_aux == 0){
			socket_todavia_valido = false;
		}
		if (contador_aux == -1){
			socket_todavia_valido = false;
		}
		bytes_enviados+=contador_aux;
	}
	if (socket_todavia_valido){
		return bytes_enviados;
	} else {
		return -1;
	}
}

int Socket::recibir(char* buffer, int tamanio_msg){
    int bytes_recibidos = 0;
	bool socket_todavia_valido = true;
	while (bytes_recibidos < tamanio_msg && socket_todavia_valido){
		int s = recv(this->fd, &buffer[bytes_recibidos],
				 tamanio_msg - bytes_recibidos, 0);
		if (s == 0){
			socket_todavia_valido = false;
		}
		if (s == -1){
			socket_todavia_valido = false;
		}
		bytes_recibidos += s;
	}
	if (socket_todavia_valido){
		return bytes_recibidos;
	} else {
		return -1;
	}
}

int Socket::enlazar_y_escuchar(struct addrinfo* result){
    bool funciono = this->enlazar(result);
	if (funciono == false){
		return 1;
	}
	funciono = this->escuchar();
	if (funciono == false){
		return 1;
	}
	return 0;
}



Socket::~Socket()
{
    if(this->conectado){
        this->cerrar();
    }
    //dtor
}
