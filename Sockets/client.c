#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include <sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include <netdb.h>
#include <time.h>

int opDetr(char* clist, char* str);

int main(int argc, char const *argv[]) {
  //Usage: ./client [umass email] [port] [server]
  int clientfd;
  char const* email = argv[1];
  int port;
  port = atoi(argv[2]);
  char const* hostname = argv[3];

  struct hostent* hp;
  struct sockaddr_in serveraddr;

  if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Error making socket.\n");
    return 0;

  }

  if ((hp = gethostbyname(hostname)) == NULL) {
    printf("Error porting hostname!");
    return 0;

  }

  bzero((char *)&serveraddr, sizeof(serveraddr)); //clearing the current struct

  serveraddr.sin_family = AF_INET;
  bcopy((char *)hp->h_addr_list[0], (char *)&serveraddr.sin_addr.s_addr, hp->h_length);
  serveraddr.sin_port = htons(port);

  printf("Client is trying to connect to %s:%d.\n", inet_ntoa(serveraddr.sin_addr), port);

  if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
    printf("Error connecting!\n");
    return 0;

  }

  char greeting[50];
  snprintf(greeting, sizeof(greeting), "cs230 HELLO %s\n",email);

  int count = send(clientfd, greeting, strlen(greeting), 0);

  char msg[100]; //holder of the response

  int randnum = (rand() % (2000 - 300 + 1)) + 300; //random number between 300 & 1800
  char* ops = "+-*/";

  //cs230 STATUS NUM OP NUM\n
  for (int i = 0; i < randnum; i++) {
    char tmp[25];

    bzero(msg, sizeof(msg)); //clear msg
    bzero(tmp, sizeof(tmp)); //clear tmp

    count = recv(clientfd, msg, sizeof(msg), 0);
    printf("%d. ",i+1);
    printf("%s",msg);

    strncpy(tmp, msg + 13, strlen(msg) - 13); //copy equation to tmp

    int solution = opDetr(ops,tmp);

    bzero(tmp, sizeof(tmp)); //zero out temp again (clear)
    snprintf(tmp, sizeof(tmp), "cs230 %d\n", solution); //pack the message

    printf("%s",tmp);

    count = send(clientfd, tmp, strlen(tmp), 0); //send the message

    continue; //just put this here to make sure we aren't wasting time waiting around for something

  }

}

//returns the completed equation
int opDetr(char* clist, char* str) {
  int ret = strcspn(str,clist);
  char c = str[ret]; //operator

  char intc[15]; //temp storing variable

  strncpy(intc, str, ret); //making the first number (everything before the operator)
  int num1 = atoi(intc);

  strncpy(intc, (str + ret + 2), strlen(str)); //making the second number (everything after operator)
  int num2 = atoi(intc);

  switch(c) {
    case '+':
      return num1 + num2;

    case '-':
      return num1 - num2;

    case '*':
      return num1 * num2;

    case '/':
      return num1 / num2;

  }

}
