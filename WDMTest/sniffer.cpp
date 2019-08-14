#include "mainw.h"
//#include <client.h>
//#include <server.h>
#include "ui_mainw.h"

#define NET_SERVER_2000
//#define ALPHA


#include <stdio.h>
#include <time.h>
#include <chrono>

#include <string>
//#include <iostream>

using namespace std;

#include "RawSock.h"
#include "Socket.h"

CRITICAL_SECTION critsect;

#define CONSOLE_WIDTH 140

static string sCurrPacket = "";
static bool	  bNewPacket = true;

unsigned __stdcall Connection(void* a)
{
    Socket* s = (Socket*) a;
    if (a == NULL)
        return SOCKET_ERROR;
    while (1)
    {
        if (bNewPacket)
        {
            EnterCriticalSection(&critsect);

            s->SendLine(sCurrPacket+(char)13+(char)10);
            bNewPacket = false;

            LeaveCriticalSection(&critsect);
        }
    }

    delete s;
    return 0;
}

unsigned __stdcall AcceptConnections(void* a)
{
    SocketServer in(2000, 20);

    while (1)
    {
        Socket* s = in.Accept();

        unsigned ret;
        _beginthreadex(0, 0, Connection, (void*)s, 0, &ret);
    }
}

long getTimeDiff(clock_t t1, clock_t t2) {
    long elapsed;
    elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000;
    return elapsed;
}

void Worker::doWork(const QString &parameter) {
    /* ... here is the expensive or blocking operation ... */
    QString result;
    FILE*		 f;
    DWORD		 packet_res = 0;
    u_long		 watch_host = 0;
    DWORD		 packets_count = 0;
    u_long       uploadTotalLength = 0;
    u_long       downloadTotalLength = 0;
    u_long       uploadCurrentLength = 0;
    u_long       downloadCurrentLength = 0;
    bool         timetrue    = false;
    clock_t      time1, time2;
    u_long       timeDiff;
    u_long       uploadSpeed;
    u_long       downloadSpeed;


#ifdef NET_SERVER_2000
    InitializeCriticalSection(&critsect);
#endif


    RS_Init();

    f = fopen(RS_Hostname, "wt");

    printf("%s\n", nethost2str(RS_SocketAddress.sin_addr.s_addr));

    RS_SetPromMode(1);

#ifndef ALPHA
    watch_host = inet_addr("127.0.0.1");
#endif
    RS_InitStat();

    forever
    {

        if(abort)
        {

            if (timetrue)
            {
                timetrue = false;
                uploadTotalLength = 0;
                downloadTotalLength = 0;
            }


        }
        else
        {

        if (timetrue == false)
        {
            timetrue = true;
            time1 = clock();

        }

        IPHeader* hdr = RS_Sniff();

        if (hdr)
        {

            printf("adresse ip %s\n", nethost2str(RS_SocketAddress.sin_addr.s_addr));

            char *packet_str;
            packets_count++;
            time(&rawtime);
            timeinfo = localtime (&rawtime);
            packet_str = RS_IPHeaderToStr(hdr);
//            if (f)
//                fprintf(f, "%.2d:%.2d:%.2d>%s\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, packet_str);
            result = QString();
            result.append(packet_str);
            emit resultReady(result);
            time2=clock();
            timeDiff = getTimeDiff(time1,time2);
            printf(" time : %i\n", timeDiff);
            if (timeDiff != 0)
            {
                if (strcmp(nethost2str(hdr->src),nethost2str(RS_SocketAddress.sin_addr.s_addr)))      // check if upload or download packet (src or dest)
                {
                    downloadCurrentLength = ntohs(hdr->length);
                    downloadTotalLength+=downloadCurrentLength;
                    downloadSpeed = downloadTotalLength/timeDiff;
                    newDownloadSpeed( QString::number(downloadSpeed));
                    newDownloadSize( QString::number(downloadTotalLength));
                }
                else
                {

                    uploadCurrentLength = ntohs(hdr->length);
                    uploadTotalLength+=uploadCurrentLength;
                    uploadSpeed = uploadTotalLength/timeDiff;
                    newUploadSpeed( QString::number(uploadSpeed));
                    newUploadSize( QString::number(uploadTotalLength));
                }

            }

            printf("%.2d:%.2d:%.2d>", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

            //for(int i = 0; i < CONSOLE_WIDTH - strlen(packet_str)-9; i++) printf(" ");
            //printf("\n");

            sCurrPacket = packet_str;
            bNewPacket = true;

            free((void*)packet_str);
            free((void*)hdr);

            if (packet_res > 0)
                Sleep(packet_res);
        }
        }
    }


}


