#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include <graphics.h>
#include <iostream>
#include <string.h>
using namespace std;

void Fon(){
    setcolor(WHITE);
    for(int i=215;i<220;i++){
    line(i,500,i,0);
    line(i+200,500,i+200,0);
    line(640,i-55,0,i-55);
    line(640,i+100,0,i+100);
    }
}
void nolik(int x, int y){
    for (int i=55;i<60;i++)
    circle(x,y,i);
}
void krestik(int x,int y){
    for (int i=0;i<5;i++){
        line(x-60+i,y-70,x+60-i,y+70);
        line(x+60-i,y-70,x-60+i,y+70);
    }

}
const int X[9]={110,320,530,110,320,530,110,320,530};
const int Y[9]={400,400,400,240,240,240,80,80,80};

int main ()
{
    setlocale(LC_ALL, "Russian");
    //char message[255];
    //gets(message);
    char buf [255];
    int koord = 0;
    struct sockaddr_in addr;
    int gdriver = DETECT, gmode;
    int one_server=0;
    int bytes_read = 0;
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

    cout<<"\nНайти игрока?\n";
    system("pause");

    if(connect (sock, (struct sockaddr *)&addr, sizeof (addr)) < 0) { //запрос на подключение
        perror ("connect");
        exit (2);
    }

    bytes_read = recv (sock, buf, sizeof (buf), 0);
    cout<<"\nВремя на ход="<<atof(buf)<<" секунд\n";

    cout<<"\nИгра началась\n10-Отдать первый ход противнику";
    initgraph(&gdriver, &gmode, "");
    Fon();

    while(1){
        if(koord !=10) cout<<"\nВаш ход\n";
            else {cout<<"\nВы уже отдали первый ход противнику!\n";}

        cin>>koord;
        if ((koord <1)||(koord>10)){
            continue;
        }
        else{
            if ((koord==10)&&(one_server!=0))
            continue;
            if ((one_server==0)&&(koord==10))
                one_server=1;
        }
        sprintf(buf, "%i", koord);
        send (sock, buf, sizeof (buf), 0);

        if(koord==10) cout<<"\nЖдите хода противника!\n";

        bytes_read = recv (sock, buf, sizeof (buf), 0);

        if (bytes_read==16){
            cout<<"\nВы проиграли!\n";
            break;
        }

        if (bytes_read==33){
            cout<<buf;
            break;
        }

        koord=atof(buf);

        if (bytes_read==8){
            cout<<"-Поле занято!\n";
            continue;
        }

        if (bytes_read==15){
            bytes_read = recv (sock, buf, sizeof (buf), 0);
            koord=atof(buf);
            nolik(X[koord-1],Y[koord-1]);
            cout<<"\nВы выиграли!\n";
            break;
        }

        if (one_server==1){
            one_server=2;
            krestik(X[koord-1],Y[koord-1]);
            continue;
        }
        else {
                one_server=2;
                nolik(X[koord-1],Y[koord-1]); //если клиент ходил первым
        }

        printf ("\nЖдите хода противника!\n");
        bytes_read = recv (sock, buf, sizeof (buf), 0);
        if (bytes_read==16){
            recv (sock, buf, sizeof (buf), 0);
            koord=atof(buf);
            krestik(X[koord-1],Y[koord-1]);
            cout<<"\nВы проиграли!\n";
            break;
        }
        if (bytes_read==15){
            cout<<"\nВы выиграли!\n";
            break;
        }
        koord=atof(buf);
        krestik(X[koord-1],Y[koord-1]);
    }

    closesocket ( sock );
    system("pause");
    return 0;
}
