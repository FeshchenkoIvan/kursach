#include<stdio.h>
#include<winsock2.h>
#include<stdlib.h>
#include <graphics.h>
#include <iostream>
#include <string.h>
#include <time.h>
#define CLOCKS_PER_SEC 1000

using namespace std;

char oshibka[] = "������!";
char vinner[15] = "\n�� ��������)\n";
char lose[16] = "\n�� ���������(\n";
char losetime[33] = "\n��������� �������� �� �������!\n";
const int X[9]={110,320,530,110,320,530,110,320,530};
const int Y[9]={400,400,400,240,240,240,80,80,80};
int IGRA[9]={0,0,0,0,0,0,0,0,0};
void Fon(){
    setcolor(RED);
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
    int koord = 0,flag,tm;
    double tm_begin,tm_end;
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


    listen (listener, 1); //����� �������������



    while(1)
    {
        for(int i=0;i<9;i++)
            IGRA[i]=0;
        cout<<"\n������� ����� �� ���(���)=";
        cin>>tm;
        cout<<"\n����� ������\n";
        //tm_begin=clock();
        sock = accept (listener, NULL, NULL); //�������� ������ ������ ��� ���������� �����������
        if(sock < 0) {
            perror ("accept");
            exit (3);
        }
        printf ("\n���� ��������\n");
        initgraph(&gdriver, &gmode, "server");
        Fon();
        flag=1;//���� ��� ������� ���� �������
        sprintf(buf,"%i", tm);
        send (sock, buf, sizeof(buf), 0);

        while(1){
            tm_begin=clock();
            bytes_read = recv (sock, buf,sizeof(buf), 0);//�������� ������ � ��������� � �����
            tm_end=clock();
            if (bytes_read < 0){
                cout<<"\n�����2 ������� ����\n";
                break;
            }

            koord = atof(buf);
            if ((koord == 10)&&(flag==1)){ //���� 10, �� ����� ������ ������
                if (((tm_end-tm_begin)/CLOCKS_PER_SEC)>tm){
                    cout<<"\n�� ��������,��������� �������� �� �������\n";
                    send (sock, lose, sizeof(lose), 0);
                    break;
                }
                flag=0;
                printf ("\n�� ������ ������(��������)\n");
                cout<<"��� ���!\n";
                tm_begin=clock();
                while(1){
                    cin>>koord;
                    tm_end=clock();
                    if (((tm_end-tm_begin)/CLOCKS_PER_SEC)>tm){
                        cout<<"\n�� ��������� �� �������\n";
                        send (sock, losetime, sizeof(losetime), 0);
                        flag=3;
                        break;
                    }
                    if ((koord <1)||(koord>9)){
                        cout<<"-��� ������ ����!\n";
                        continue;
                    }
                    IGRA[koord-1]=1;
                    krestik(X[koord-1],Y[koord-1]);
                    sprintf(buf, "%i", koord);
                    cout<<"\n����� ���� ����������!\n";
                    send (sock, buf, sizeof(buf), 0);
                    break;
                }
                if (flag==3)
                    break;
                continue;
            }
            else{ //��� �������
                if (((tm_end-tm_begin)/CLOCKS_PER_SEC)>tm){
                    cout<<"\n�� ��������,��������� �������� �� �������\n";
                    send (sock, lose, sizeof(lose), 0);
                    break;
                }
                    flag=0;
                if ((IGRA[koord-1]==0)&&(koord!=10)){
                    IGRA[koord-1]=2;
                    nolik(X[koord-1],Y[koord-1]);
                    sprintf(buf, "%i", koord);
                    if (vin()==true){
                        cout<<lose;
                        send (sock, vinner, sizeof(vinner), 0);
                        send (sock, buf, sizeof(buf), 0);// ��� ����� ������ koord?(10) ��� vin ����� ���������� ����?
                        break;
                    }
                    send (sock, buf, sizeof(buf), 0);
                }
                else{
                    send (sock, oshibka, sizeof(oshibka), 0);
                    continue;
                }
            }

            cout<<"\n��� ���!\n";
            tm_begin=clock();
            while(1){
                        cin>>koord;
                        tm_end=clock();
                        if (((tm_end-tm_begin)/CLOCKS_PER_SEC)>tm){
                            cout<<"\n�� ��������� �� �������\n";
                            send (sock, vinner, sizeof(vinner), 0);
                            flag = 3;
                            break;

                        }
                        if ((koord <1)||(koord>9)){
                            cout<<"-��� ������ ����!\n";
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
                            cout<<"\n����� ���� ����������!\n";
                            send (sock, buf, sizeof(buf), 0);
                            break;
                        }
                        else{
                            cout<<"-���� ������!\n";
                            continue;
                        }
                    }
                    if(flag==3) break;
        }
        system("pause");
        closegraph();
        closesocket (sock); // �������� ������
        //cleardevice();
    }

    return 0;
}
