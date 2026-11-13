#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TAM_BUFFER 1024

int crear_socket_servidor() {
    int conector;
    struct sockaddr_in direccion = {0};

    conector = socket(AF_INET, SOCK_STREAM, 0);
    if (conector < 0){
        exit(1);
    }

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(9999);

    if (bind(conector, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) {
        exit(1);
    }

    if (listen(conector, 5) < 0) {
        exit(1);
    }

    return conector;
}

void enviar_archivo(int socket_cliente) {
    char buffer[TAM_BUFFER];
    ssize_t leidos;
    int fd = open("Google.html", O_RDONLY);

    if (fd < 0) {
        close(socket_cliente);
        exit(1);
    }

    while ((leidos = read(fd, buffer, TAM_BUFFER)) > 0) {
        write(socket_cliente, buffer, leidos);
    }

    if (leidos < 0) {
        exit(1);
    }

    close(fd);
}

void bucle_servidor(int conector) {
    int socket_cliente;
    pid_t pid;
    struct sockaddr_in dir_cliente;
    socklen_t longitud;

    while(1) {
        longitud = sizeof(dir_cliente);

        socket_cliente = accept(conector, (struct sockaddr *)&dir_cliente, &longitud);
        if (socket_cliente < 0) {
            exit(1);
        }

        pid = fork();

        if (pid == 0) {
            close(conector);
            enviar_archivo(socket_cliente);
            close(socket_cliente);
            exit(0);
        } 
        
        close(socket_cliente);
    } 
}

int main() {
    int conector;

    conector = crear_socket_servidor();
    bucle_servidor(conector);

    close(conector);

    return 0;
}
