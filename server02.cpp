#include<stdio.h>
#include<winsock2.h>
#include<stdlib.h>
#include <graphics.h>
#include <iostream>
#include <string.h>
using namespace std;

char oshibka[] = "Îøèáêà!";
char vinner[15] = "\nÂû âûèãðàëè)\n";
char lose[16] = "\nÂû ïðîèãðàëè(\n";
const int X[9]={110,320,530,110,320,530,110,320,530};
const int Y[9]={400,400,400,240,240,240,80,80,80};
int IGRA[9]={0,0,0,0,0,0,0,0,0};
void Fon(){
    setcolor(WHITE);
    for(int i=215;i<220;i++){
    line(i,500,i,0);
    line(i+200,500,i+200,0);
    line(640,i-55,0,i-55);
    line(640,i+100,0,i+100);
    line(0,0,10,10);//óäàëèòü
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
bool vin(){
    for (int i=0;i<3;i++){
    if ((IGRA[0+i*3]==IGRA[1+i*3])&&(IGRA[1+i*3]==IGRA[2+i*3])&&(IGRA[0+i*3]!=0))
        return true;
    if ((IGRA[0+i]==IGRA[3+i])&&(IGRA[3+i]==IGRA[6+i])&&(IGRA[0+i]!=0))
        return true;
    }
    if (((IGRA[4]==IGRA[8])&&(IGRA[4]==IGRA[0]))||((IGRA[4]==IGRA[6])&&(IGRA[4]==IGRA[2])))
        if (IGRA[4]!=0)
        return true;
    return false;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    struct sockaddr_in addr;
    char buf [255];
    char message[255];
    int bytes_read=0;
    int koord = 0;
    int flag;
    int gdriver = DETECT, gmode;

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

    while(1)
    {
        for(int i=0;i<9;i++)
            IGRA[i]=0;
        cout<<"\nÏîèñê èãðîêà\n";
        sock = accept (listener, NULL, NULL); //ñîçäàíèå íîâîãî ñîêåòà ïðè ðàçðåøåíèè ïîäêëþ÷åíèÿ
        if(sock < 0) {
            perror ("accept");
            exit (3);
        }
        printf ("\nÈãðà íà÷àëàñü\n");
        initgraph(&gdriver, &gmode, "server");

        Fon();
        flag=1;//ôëàã äëÿ ïåðâîãî õîäà ñåðâåðà

        while(1){
            bytes_read = recv (sock, buf,sizeof(buf), 0);//ïîëó÷àåò äàííûå è ñîõðàíÿåò â áóôåð
            if (bytes_read < 0){
                cout<<"\nÈãðîê2 ïîêèíóë èãðó\n";
                break;
            }
            //printf ("Ïðèíÿòî%d áàéò\t÷èñëî: %s\n", bytes_read, buf);
            koord = atof(buf);

            if ((koord == 10)&&(flag==1)){ //åñëè 10, òî õîäèò ïåðâûé ñåðâåð
                    flag=0;
                    printf ("\nÂû õîäèòå ïåðâûé(êðåñòèêè)\n");
                    cout<<"Âàø õîä!\n";
                    while(1){
                        cin>>koord;
                        if ((koord <1)||(koord>9)){
                            cout<<"-Íåò òàêîãî ïîëÿ!\n";
                            continue;
                        }
                        //cout<<"\n  \n"<<koord;
                        IGRA[koord-1]=1;
                        krestik(X[koord-1],Y[koord-1]);
                        sprintf(buf, "%i", koord);
                        cout<<"\nÆäèòå õîäà ïðîòèâíèêà!\n";
                        send (sock, buf, sizeof(buf), 0);
                        break;
                    }
                    continue;
            }
            else{ //õîä êëèåíòà
                    flag=0;
                if ((IGRA[koord-1]==0)&&(koord!=10)){
                    IGRA[koord-1]=2;
                    nolik(X[koord-1],Y[koord-1]);
                    sprintf(buf, "%i", koord);
                    if (vin()==true){
                        cout<<lose;
                        send (sock, vinner, sizeof(vinner), 0);
                        send (sock, buf, sizeof(buf), 0);// èëè êàêîé íèáóäü koord?(10) èëè vin ïîòîì äîðèñîâàòü êðóã?
                        break;
                    }
                    send (sock, buf, sizeof(buf), 0);
                }
                else{
                    send (sock, oshibka, sizeof(oshibka), 0);
                    continue;
                }
            }

            cout<<"\nÂàø õîä!\n";
            while(1){
                        cin>>koord;
                        if ((koord <1)||(koord>9)){
                            cout<<"-Íåò òàêîãî ïîëÿ!\n";
                            continue;
                        }
                        if (IGRA[koord-1]==0){
                            IGRA[koord-1]=1;
                            krestik(X[koord-1],Y[koord-1]);
                            sprintf(buf, "%i", koord);
                            if (vin()==true){
                                cout<<vinner;
                                send (sock, lose, sizeof(lose), 0);
                                send (sock, buf, sizeof(buf), 0);
                                flag = 3;
                                break;
                            }
                            cout<<"\nÆäèòå õîäà ïðîòèâíèêà!\n";
                            send (sock, buf, sizeof(buf), 0);
                            break;
                        }
                        else{
                            cout<<"-Ïîëå çàíÿòî!\n";
                            continue;
                        }
                    }
                    if(flag==3) break;
        }
        system("pause");
        closegraph();
        closesocket (sock); // çàêðûòèå ñîêåòà
        //cleardevice();
    }

    return 0;
}
