/* For now the program is iterative, once it is verified that is working, it will be written 
in parallel. For now the file is opened in the beginning and locks will be used when a thread is
writing to the file. */


#include "csapp.h"

#define OUTPUT_FILE "logfile.txt"
#define NUM_THREADS 25
#define BUFFER_SIZE 100

typedef struct thread_data {
	int connfd;
	int threadId;
} thread_data;

pthread_mutex_t file_lock;

//This will be used to thread the program once it runs iteratively
void* processRequest(void* t){

	//Unpack the data
	thread_data* t_data = (thread_data*)t;
	int connfd = t_data->connfd;
	int threadId = t_data->threadId;

	// Read from the client connection
	char textBuf[BUFFER_SIZE];
	int readLen;

	while ( (readLen=Read(connfd, textBuf, sizeof(textBuf))) > 0){
		if(!strstr(textBuf, "/exit")){
			// Open the file we will be writing to (a+ means to append instead of w which overwrites)
			pthread_mutex_lock(&file_lock);
    		FILE* fp = Fopen(OUTPUT_FILE, "a+");
			printf("User %d: %s", threadId+1, textBuf);
			// Write to the output file
			Fwrite(textBuf, sizeof(char), readLen, fp);
			bzero(&textBuf, sizeof(textBuf));
			Fclose(fp);
			pthread_mutex_unlock(&file_lock);
		} else {
			printf("Finished reading from the client %d\n", threadId+1);
			break;
		}
	}
	Close(connfd);
	return 0;
}

int main(int argc, char* argv[]){

	//Declarations
	int port; // Listening port
	int sockfd; // The socket file descriptor
	int enable = 1; // Used for setsockopt
	int threadId = 0; // How to reference elements in the thread array

	pthread_t threads[NUM_THREADS]; // Array that stores the threads
	thread_data t_data[NUM_THREADS]; // Corresponding struct that hold thread data

	pthread_mutex_init(&file_lock, NULL); // Initialize the file lock


	/* Check arguments */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port number>\n", argv[0]);
        exit(0);
    }

    // Get the port the server will be listening on
    port = atoi(argv[1]);

	// Socket
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	// This allows for constant reuse of the port
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

	for(;;threadId++){
		// Accept a connection
		printf("Waiting for a new client connection... \n");
		int connfd;
		connfd = Accept(sockfd, NULL, 0);
		printf("Got a connection from a new client\n");

		if(threadId <= NUM_THREADS){
			t_data[threadId].connfd = connfd;
			t_data[threadId].threadId = threadId;
			Pthread_create(&threads[threadId], NULL, processRequest, (void*)&t_data[threadId]);
		} else {
			printf("Thread Limit Reached\n");
			threadId = 0;
			int i;
			for(i = 0; i < NUM_THREADS; ++i){
				Pthread_join(threads[i], NULL);
			}
		}
	}
	Close(sockfd);


	return 0;
}