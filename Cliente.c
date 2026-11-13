#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TAM_BUFFER 1024

int crear_socket_cliente() {
    int conector = socket(AF_INET, SOCK_STREAM, 0);
    if (conector < 0) {
        exit(1);
    }
        
    return conector;
}

void conectar_servidor(int conector, char *ip_servidor) {
    struct sockaddr_in direccion = {0};

    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = inet_addr(ip_servidor);
    direccion.sin_port = htons(9999);

    if (direccion.sin_addr.s_addr == INADDR_NONE) {
        exit(1);
    }

    if (connect(conector, (struct sockaddr *)&direccion, sizeof(direccion)) < 0) {
        exit(1);
    }
}

void recibir_y_mostrar(int conector) {
    char buffer[TAM_BUFFER];
    ssize_t leidos;

    while ((leidos = read(conector, buffer, TAM_BUFFER-1)) > 0) {
        buffer[leidos] = '\0';
        printf("%s", buffer);
    }

    if (leidos < 0){
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int conector;

    if (argc == 2) {
        conector = crear_socket_cliente();
        conectar_servidor(conector, argv[1]);
        recibir_y_mostrar(conector);

        close(conector);
    }

    return 0;
}

