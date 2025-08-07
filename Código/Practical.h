#ifndef PRACTICAL_H_
#define PRACTICAL_H_

#include <stdbool.h>  //bool, true, false
#include <stdio.h>
#include <sys/socket.h>


void DieWithUserMessage(const char *msg, const char *detail); //função de erro do usuário

void DieWithSystemMessage(const char *msg); //função de erro do sistema

void HandleTCPClient(int clntSocket); //funcção principal, ela trata do http do cliente

enum sizeConstants  //define o tamanho maximo para string e buffer
{
  MAXSTRINGLENGTH = 128,
  BUFSIZE = 1024,
};

#endif 