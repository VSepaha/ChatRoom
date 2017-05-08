
//this portion of the code was writen by brian chu

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

if(argc!=3) //requires user hostname and port hence 3 args
	{
	
	fprintf(stderr,"Please give a  hostname port\n");
	exit(1);
	}


struct addrinfo info; 
struct addrinfo * ptr;

memset(&info,0,sizeof(info));

info.ai_family = AF_INET;
info.ai_socktype = SOCK_STREAM;

if(getaddrinfo(argv[1],argv[2],&info,&ptr)!=0) //get our information of our server connection
	{
	perror("getaddrinfo");
	exit(1);
	
	}
if(ptr == NULL) //if its null its unable to connect
	{
	fprintf(stderr,"UNABLE TO CONNECT TO HOST!\n");
	exit(1);
	}

socketfd = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol); //make a socket

if(socketfd == -1) //opps socket issues
	{
	perror("socket");
	exit(1);
	}
if(connect(socketfd,ptr->ai_addr,ptr->ai_addrlen)==-1) //connect to the server
	{
		close(socketfd);
		perror("connect");
		exit(1);
	
	}

freeaddrinfo(ptr); //clean up
int num;
int isize;
char buff[2000];
memset(buff,0,2000);
int nsize;
char namebuff[2000];
char bigbuff[5000];
memset(bigbuff,0,5000);
memset(namebuff,0,2000);
while(1)
{
	
	printf("Enter a username: \n");
	nsize = read(STDIN_FILENO,namebuff,sizeof(namebuff)); //check the input size
	if(nsize <= 0)
	{
		printf("we have a read error for your name please reenter it\n"); //continue looping on error
		continue;
	}
	printf("Username: %s\n",namebuff); 
	break;//breakin and leavin
	
	
}
strtok(namebuff,"\n");//remove newline
while(1)
{
	printf("Enter Prompt:\n");//promtping
	memset(buff,0,sizeof(buff));
	memset(bigbuff,0,sizeof(bigbuff));
	isize = read(STDIN_FILENO,buff,sizeof(buff)); //promtping
	if(isize == -1) //error test
	{
		fprintf(stderr,"an error occured while reading please see below and input your message again");
		perror("read");
		continue;
	}
	
	strcat(bigbuff,namebuff);//add name
	strcat(bigbuff,": "); //add token
	strcat(bigbuff,buff);//add message
	strcat(bigbuff,"\n");
	num = send(socketfd,bigbuff,strlen(bigbuff),0);//sending 
	//num = send(socketfd,buff,isize,0);
	if(num == -1)//error 
	{
		fprintf(stderr,"ERROR SENDING TO CHAT HOST\n");
		perror("send");
	}
	printf("sent\n");
	if(strstr(bigbuff,"exit/")!=NULL) //exit code
	{
		printf("detected exit now leaving\n");
		break;
	}
}
}