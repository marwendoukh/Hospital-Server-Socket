/*
    C ECHO client example using sockets
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

    //Create socket (TCP socket)
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    // address of the server
    server.sin_addr.s_addr = inet_addr("192.168.43.191");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");

    //keep communicating with server
    while(1)
    {
        // get client msg
        printf("Enter message : ");
        scanf("%s" , message);
        /// check if the client's request has the right length
        if(strlen(message)<=strlen("coming?"))
        {

            //Send request to server
            if( send(sock , message , strlen(message) , 0) < 0)
                {
                    puts("Send failed");
                    return 1;
                  }

            //Receive a reply from the server
            if( recv(sock , server_reply , 2000 , 0) < 0)
                {
                    puts("recv failed");
                    break;
                  }
            // print server's response
            puts(server_reply);

          }
        else
          {
            // the client wrote a wrong request
            puts("You wrote a wrong request !");
          }

    }

    // closing socket
    close(sock);
    return 0;
}
