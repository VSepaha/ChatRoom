#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h>
#include <netinet/in.h>
#include <ctype.h>
#include <sys/time.h>


int  main(int argc, char **argv)
{

int socketfd;

if(argc!=3)
	{
	
	fprintf(stderr,"Please give a  hostname port\n");
	exit(1);
	}


struct addrinfo info;
struct addrinfo * ptr;

memset(&info,0,sizeof(info));

info.ai_family = AF_INET;
info.ai_socktype = SOCK_STREAM;

if(getaddrinfo(argv[1],argv[2],&info,&ptr)!=0)
	{
	perror("getaddrinfo");
	exit(1);
	
	}
if(ptr == NULL)
	{
	fprintf(stderr,"UNABLE TO CONNECT TO HOST!\n");
	exit(1);
	}

socketfd = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);

if(socketfd == -1)
	{
	perror("socket");
	exit(1);
	}
if(connect(socketfd,ptr->ai_addr,ptr->ai_addrlen)==-1)
	{
		close(socketfd);
		perror("connect");
		exit(1);
	
	}

freeaddrinfo(ptr);
int num;
int isize;
char buff[2000];
memset(buff,0,2000);

while(1)
{
	printf("please enter your text and wait for the next prompt!\n");
	isize = read(STDIN_FILENO,buff,sizeof(buff));
	if(isize == -1)
	{
		fprintf(stderr,"an error occured while reading please see below and input your message again");
		perror("read");
		continue;
	}
	num = send(socketfd,buff,isize,0);
	if(num == -1)
	{
		fprintf(stderr,"ERROR SENDING TO CHAT HOST\n");
		perror("send");
	}
	printf("we have sucessfully sent the message to the server!\n");
}






}