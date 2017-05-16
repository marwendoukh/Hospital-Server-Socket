/*

    C socket server example, handles multiple clients using threads

*/
 
#include<stdio.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include<semaphore.h>
#include<sys/types.h>
#include<string.h>
#define PLACES_MAX 20

int places= PLACES_MAX;
pthread_mutex_t mutex;

//the thread function
void *connection_handler(void *);

int are_equal(char str1[],char str2[])
{
    if (strlen(str1)!= strlen(str2))
        return 0; // They must be different
    for (int i = 0; i < strlen(str1); i++)
    {
        if (str1[i] != str2[i])
            return 0;  // They are different
    }
    return 1;  // They must be the same
}

int main(int argc , char *argv[])
{

    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    //Bind

    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    //Listen
    listen(socket_desc , 3);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)

        {

            perror("could not create thread");

            return 1;

        }
        //Now join the thread , so that we dont terminate before the thread
        pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    } 

    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    return 0;
}

/*

 * This will handle connection for each client

 * */

void *connection_handler(void *socket_desc)

{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message,*msg,*msg2 , client_message[2000];
    //Send some messages to the client
    //message = "Greetings! I am your connection handler\n";
    //write(sock , message , strlen(message));  
    //message = "Now type something and i shall repeat what you type \n";
    //write(sock , message , strlen(message));
    //Receive a message from client
    while( (read_size = recv(sock , &client_message , 2000 , 0)) > 0 )
    {
	if(are_equal(client_message,"venir"))
	{
	pthread_mutex_lock(&mutex);
	if(places==0)
		{
		pthread_mutex_unlock(&mutex);
		write(sock,"pas_de_places", strlen("pas_de_places"));
		}
	else    
		{
		places--;
		pthread_mutex_unlock(&mutex);
		msg="Vous pouvez venir\n";
		write(sock ,msg, strlen(msg));
		}
		
	}
	else if(are_equal(client_message,"quitter"))
	{
	pthread_mutex_lock(&mutex);
	if(places==20)
		{
		pthread_mutex_unlock(&mutex);
		write(sock,"n'abuser pas :)", strlen("n'abuser pas :)"));
		}
	else    
		{
		places++;
		pthread_mutex_unlock(&mutex);
		msg2="Vous pouvez quitter\n";
		write(sock ,msg2, strlen(msg2));
		}	
	}
	else
        {
        write(sock , "rectifier votre requete\n" , strlen("rectifier votre requete\n"));
	}
	fflush(stdout);
	fflush(stdin);
    } 
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)

    {
        perror("recv failed");
    }    

   //Free the socket pointer

    free(socket_desc);
   return 0;
}
