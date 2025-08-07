#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

static const int MAXPENDING = 5; //mesma coisa do practical.c

int main(int argc, char *argv[])
{

  //verifica se a porta está correta
  if (argc != 2) 
    DieWithUserMessage("Parameter(s)", "<Server Port>");

  in_port_t servPort = atoi(argv[1]); //converte a porta pra um inteiro

  //cria um socket ipv4
  int servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (servSock < 0)
    DieWithSystemMessage("socket() failed");

  //permite reutilizar o endereço local, mesmo que ele ja esteja em uso
  int yes = 1;
  if (setsockopt(servSock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0)
    DieWithSystemMessage("setsockopt() failed");

  
  struct sockaddr_in servAddr;  //armazena endereço de um socket
  memset(&servAddr, 0, sizeof(servAddr)); //zera a estrutura
  servAddr.sin_family = AF_INET;  //define que o socket usará o ipv4
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); //aceita conexoes de qualquer ip da maquina local
  servAddr.sin_port = htons(servPort);  //define a porta do servidor

 
  if (bind(servSock, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) //associa o socket ao endereço/porta
    DieWithSystemMessage("bind() failed");

  
  if (listen(servSock, MAXPENDING) < 0) //coloca o socket em modo para aceitar conexoes
    DieWithSystemMessage("listen() failed");

  printf("Servidor rodando na porta %d\n", servPort);

  for (;;)  //loop de conexao do servidor
  {                             
    struct sockaddr_in clntAddr; //guarda o endereço do cliente
    socklen_t clntAddrLen = sizeof(clntAddr);

    //aceita a conexao
    int clntSock = accept(servSock, (struct sockaddr *)&clntAddr, &clntAddrLen);
    if (clntSock < 0)
      DieWithSystemMessage("accept() failed");

    //converte o endereço do cliente e imprime o ip e porta
    char clntName[INET_ADDRSTRLEN]; 
    if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
                  sizeof(clntName)) != NULL)
      printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
    else
      puts("Unable to get client address");

    HandleTCPClient(clntSock);  //lê a requisição http, responde com o arquivo solicitado ou erro
  }
  
}