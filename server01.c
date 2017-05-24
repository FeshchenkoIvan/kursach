//ïîäêëþ÷åíèå çàãîëîâî÷íûõ ôàéëîâ
#include<stdio.h>
#include<winsock2.h>
#include<math.h>
#include<stdlib.h>

char oshibka[] = "You are enter wrong number";

int main()
{
    struct sockaddr_in addr;
    char buf [1024];
    int bytes_read;
    double future_koren = 0;

    WSADATA wsa;
    SOCKET sock, listener;

    if (WSAStartup (MAKEWORD(2,2),&wsa) != 0) {
        printf ("Failed. Error Code : %d",WSAGetLastError());
        perror ("socket");
        exit (1);
    }

    if  ((listener = socket (AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET) {
        printf ("Could not create socket : %d" , WSAGetLastError ());
        perror ("socket");
        exit (1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons (3425);
    addr.sin_addr.s_addr = htonl (INADDR_ANY);

    if (bind (listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror ("bind");
        exit (2);
    }

    listen (listener, 1); //ðåæèì ïðîñëóøèâàíèÿ

    while( 1)
    {
        sock = accept (listener, NULL, NULL); //ñîçäàíèå íîâîãî ñîêåòà ïðè ðàçðåøåíèè ïîäêëþ÷åíèÿ
        if(sock < 0) {
            perror ("accept");
            exit (3);
        }

        while (1){
			printf ("\nWaiting for messages from the client\n");
            bytes_read = recv (sock, buf, 1024, 0);//ïîëó÷àåò äàííûå è ñîõðàíÿåò â áóôåð
            if (bytes_read <= 0) break;
            printf ("Received %d bytes\tchislo: %s\n", bytes_read, buf);
            future_koren = atof(buf);

            if (future_koren < 0){
               send (sock, oshibka, sizeof(oshibka), 0);//îòïðàâêà äàííûõ, âîçâðàùàåò êîë-âî îòïðàâëåííûõ áàéò
               break;
            }


            if (future_koren == 0){
               exit(0);
            }

            future_koren = sqrt(future_koren);

            sprintf(buf, "%1.1f", future_koren);
            printf ("\nSending message back to client\n");
            send (sock, buf, bytes_read, 0);
        }

        closesocket (sock); // çàêðûòèå ñîêåòà
    }

    return 0;
}
