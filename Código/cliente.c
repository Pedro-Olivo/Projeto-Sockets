#include <stdio.h>  //biblioteca padrão de entrada e saída 
#include <stdlib.h> //exit e atoi
#include <string.h> //manipula strings
#include <unistd.h> //close 
#include <sys/types.h>  //tipos de dados dos sockets
#include <sys/socket.h> //funções do socket
#include <netinet/in.h> //ipv4
#include <arpa/inet.h>  //função que converte os endereços ips
#include "Practical.h"  

#define BUFSIZE 1024  //define o tamanho do buffer

int main(int argc, char *argv[])
{
  //verifica se o número de argumentos está correto
  if (argc < 3 || argc > 4)
    DieWithUserMessage("Parameter(s)", "<Server Address> <Resource Path> [<Server Port>]");

  char *servIP = argv[1]; //ip do server
  char *echoString = argv[2]; //salva endereço/porta/arquivo
  in_port_t servPort = (argc == 4) ? atoi(argv[3]) : 8080;  //porta padrão

  //em caso de erro exibe a mensagem
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0)
    DieWithSystemMessage("socket() failed");

  //armazena ip e porta
  struct sockaddr_in servAddr;
  memset(&servAddr, 0, sizeof(servAddr)); //define os bits como 0 para evitar lixo de memória
  servAddr.sin_family = AF_INET;  //define o tipo de endereço

  //converte o ip para binário 
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if (rtnVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (rtnVal < 0)
    DieWithSystemMessage("inet_pton() failed");

  servAddr.sin_port = htons(servPort);  //define a porta do servidor

  //conecta o socket ao endereço do servidor
  if (connect(sock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("connect() failed");

  //monta e envia a requisição http
  char httpRequest[BUFSIZE];
  snprintf(httpRequest, sizeof(httpRequest),
           "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
           echoString, servIP);

  //envia a requisição para o servidor e verifica se todos os dados foram enviados corretamente
  size_t httpRequestLen = strlen(httpRequest);
  ssize_t numBytes = send(sock, httpRequest, httpRequestLen, 0);
  if (numBytes < 0)
    DieWithSystemMessage("send() failed");
  else if (numBytes != httpRequestLen)
    DieWithUserMessage("send()", "sent unexpected number of bytes");

  //recebe e imprime a resposta
  fputs("Received:\n", stdout);
  char buffer[BUFSIZE];

  //lê e imprime os dados do servidor
  while ((numBytes = recv(sock, buffer, BUFSIZE - 1, 0)) > 0)
  {
    buffer[numBytes] = '\0';
    fputs(buffer, stdout);
  }

  if (numBytes < 0)
    DieWithSystemMessage("recv() failed");

  fputc('\n', stdout);  //quebra de linha
  close(sock);  //fecha o socket
  exit(0);  //encerra o programa com sucesso
}
