#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "Practical.h"

static const int MAXPENDING = 5;    //número máximo de coneções pendentes

//função de mensagem de erro usuário
void DieWithUserMessage(const char *msg, const char *detail)
{
    fputs(msg, stderr);
    fputs(": ", stderr);
    fputs(detail, stderr);
    fputc('\n', stderr);
    exit(1);
}

//função de mensagem de erro do sistema
void DieWithSystemMessage(const char *msg)
{
    perror(msg);
    exit(1);
}

//função de tratamento da requisição http dos clientes
void HandleTCPClient(int clntSocket)
{
    char buffer[BUFSIZE];   //buffer que recebe os dados da requisição http
    ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE - 1, 0);    //recebe a requisição http
    if (numBytesRcvd < 0)
        DieWithSystemMessage("recv() failed");

    buffer[numBytesRcvd] = '\0';    //termina a string com null para facilitar a leitura

    char method[16], path[256]; //buffers, um armazena o get e o outro o arquivo do cliente
    sscanf(buffer, "%s %s", method, path);  //lê a primeira linha do get

    if (strcmp(method, "GET") != 0)//ignora se não for get, fecha o socket e sai da função
    {

        close(clntSocket);
        return;
    }
    //define o nome do arquivo
    char *filename = path + 1;
    if (strcmp(filename, "") == 0)
        filename = "index.html";

    FILE *file = fopen(filename, "rb"); //abre o arquivo
    if (file != NULL)   //se encontrar o arquivo envia o 200 OK
    {

        fseek(file, 0, SEEK_END);
        long filesize = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *fileBuffer = malloc(filesize);
        fread(fileBuffer, 1, filesize, file);
        fclose(file);

        char header[256];
        int headerLen = snprintf(header, sizeof(header),
                                 "HTTP/1.1 200 OK\r\n"
                                 "Content-Type: text/html\r\n"
                                 "Content-Length: %ld\r\n"
                                 "\r\n",
                                 filesize);
        send(clntSocket, header, headerLen, 0);
        send(clntSocket, fileBuffer, filesize, 0);

        free(fileBuffer);
    }
    else    //caso contrario envia a mensagem de erro
    {

        const char *notFoundMessage =
            "<html><body><h1>404 Not Found</h1><p>Arquivo nao encontrado.</p></body></html>";
        char header[256];
        int headerLen = snprintf(header, sizeof(header),
                                 "HTTP/1.1 404 Not Found\r\n"
                                 "Content-Type: text/html\r\n"
                                 "Content-Length: %ld\r\n"
                                 "\r\n",
                                 strlen(notFoundMessage));
        send(clntSocket, header, headerLen, 0);
        send(clntSocket, notFoundMessage, strlen(notFoundMessage), 0);
    }

    close(clntSocket);
}