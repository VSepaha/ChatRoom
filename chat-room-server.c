/* For now the program is iterative, once it is verified that is working, it will be written 
in parallel. For now the file is opened in the beginning and locks will be used when a thread is
writing to the file. */


#include "csapp.h"

#define OUTPUT_FILE "logfile.txt"
#define MAX_USERS 50
#define BUFFER_SIZE 1024

//This will be used to thread the program once it runs iteratively
// void* processRequest(void* t){
// 	//TODO
// }

int main(int argc, char* argv[]){

	/* Check arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        exit(0);
    }

    // Get the port the server will be listening on
    int port = atoi(argv[1]);

	// Socket
	int sockfd = 0;
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	// This allows for constant reuse of the port
	int enable = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
    	perror("setsockopt: ");

	// Bind
	struct sockaddr_in servaddr;
  	bzero(&servaddr,sizeof(servaddr));
  	servaddr.sin_family=AF_INET;
  	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
  	servaddr.sin_port=htons(port);

  	Bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

	// Listen
	Listen(sockfd, LISTENQ);

	for(;;){
		// Accept a connection
		printf("Waiting for a new client connection... \n");
		int connfd;
		connfd = Accept(sockfd, NULL, 0);
		printf("Got a connection from a new client\n");

		// Open the file we will be writing to (a+ means to append instead of w which overwrites)
    	FILE* fp = Fopen(OUTPUT_FILE, "a+");

		// Read from the client connection
		char textBuf[BUFFER_SIZE];
		int readLen;
		while ( (readLen=Read(connfd, textBuf, sizeof(textBuf))) > 0){
			if(readLen > 1){
				printf("Read Length = %d, buffer = %s", readLen, textBuf);
				// Write to the output file
				Fwrite(textBuf, sizeof(char), readLen, fp);
				bzero(&textBuf, sizeof(textBuf));
			} else {
				printf("Finished reading from the client\n");
				break;
			}
		}
		Fclose(fp);
		Close(connfd);
	}


	return 0;
}