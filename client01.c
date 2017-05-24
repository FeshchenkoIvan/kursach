#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>

int main ()
{
    char message[255];
    printf("For entering floating point numbers use a dot as the separator character\n");
    printf("Please, enter number for sending message to server\n");
    gets(message);
    char buf [sizeof (message)];

    struct sockaddr_in addr;

    WSADATA wsa;
    SOCKET sock;

    if (WSAStartup (MAKEWORD (2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError ());
        perror("socket");
        exit(1);
    }

    if  ((sock = socket (AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        printf ("Could not create socket : %d" , WSAGetLastError ());
        perror ("socket");
        exit (1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons (3425);
    addr.sin_addr.s_addr = htonl (INADDR_LOOPBACK);

    if(connect (sock, (struct sockaddr *)&addr, sizeof (addr)) < 0) {
        perror ("connect");
        exit (2);
    }

    send (sock, message, sizeof (message), 0);
    int bytes_read = 0;
    printf ("Waiting for a message\n");
    bytes_read = recv (sock, buf, sizeof (buf), 0);
    printf ("Received %d bytes\tSQRT: %s\n", bytes_read, buf);
    closesocket ( sock );

    system("pause");
    return 0;
}
