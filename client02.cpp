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
    addr.sin_addr.s_addr = inet_addr("192.168.1.1");//25.42.128.91

    cout<<"\nÍàéòè èãðîêà?\n";
    system("pause");

    if(connect (sock, (struct sockaddr *)&addr, sizeof (addr)) < 0) { //çàïðîñ íà ïîäêëþ÷åíèå
        perror ("connect");
        exit (2);
    }


    cout<<"\nÈãðà íà÷àëàñü\n10-Îòäàòü ïåðâûé õîä ïðîòèâíèêó";
    initgraph(&gdriver, &gmode, "");
    Fon();


    while(1){
        if(koord !=10) cout<<"\nÂàø õîä\n";
            else {cout<<"\nÂû óæå îòäàëè ïåðâûé õîä ïðîòèâíèêó!\n";}

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

        if(koord==10) cout<<"\nÆäèòå õîäà ïðîòèâíèêà!\n";

        bytes_read = recv (sock, buf, sizeof (buf), 0);
        //printf ("Ïðèíÿòî%d áàéò\t÷èñëî: %s\n", bytes_read, buf);
        koord=atof(buf);

        if (bytes_read==8){
            cout<<"-Ïîëå çàíÿòî!\n";
            continue;
        }

        if (bytes_read==15){
            bytes_read = recv (sock, buf, sizeof (buf), 0);
            koord=atof(buf);
            nolik(X[koord-1],Y[koord-1]);
            cout<<"\nÂû âûèãðàëè!\n";
            break;
        }

        if (one_server==1){
            one_server=2;
            krestik(X[koord-1],Y[koord-1]);
            continue;
        }
        else {
                one_server=2;
                nolik(X[koord-1],Y[koord-1]); //åñëè êëèåíò õîäèë ïåðâûì
        }

        printf ("\nÆäèòå õîäà ïðîòèâíèêà!\n");
        bytes_read = recv (sock, buf, sizeof (buf), 0);
        if (bytes_read==16){
            recv (sock, buf, sizeof (buf), 0);
            koord=atof(buf);
            krestik(X[koord-1],Y[koord-1]);
            cout<<"\nÂû ïðîèãðàëè!\n";
            break;
        }
        koord=atof(buf);
        krestik(X[koord-1],Y[koord-1]);
    }

    closesocket ( sock );
    system("pause");
    return 0;
}
