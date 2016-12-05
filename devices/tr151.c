#include <math.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <syslog.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <libpq-fe.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include "../sockutils.h"
#include "../devices.h"

#define MAXLENQUERY     2048
#define REPORTLEN       256
#define POCKETMAXLEN    1500
#define MAXCHIELDS      4096
#define FD_COPY(f, t)   (void)(*(t) = *(f))
#define INITPACKETLEN 17
#define SERIALIZESENSORLEN 2048

void proto(const int* client_fd, PGconn *conn)
{   
   unsigned char buf[POCKETMAXLEN+1];
   int j, buflen, waitbuflen=0, readbuflen, childpid;
   char query[MAXLENQUERY], *ip, cmdtext[256];
   int ret, num;
   PGresult *res;
   int iskill, ifexit, isfirst = 0, offset=0, iswaitpacket = 0;
   char id[sizeof(long)+1];
   char report[REPORTLEN];
   int  dstport;
   unsigned char allbuf[POCKETMAXLEN+1]; 

    char parr[100][100];
    int e,n;
   
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    getpeername(*client_fd, (struct sockaddr *)&client_addr, &addr_size);
    
   dstport = htons(client_addr.sin_port);
   ip = inet_ntoa(client_addr.sin_addr);

   childpid=getpid();   

readstart:   

   bzero(buf,POCKETMAXLEN+1);

   buflen = readfromsock(*client_fd, INITPACKETLEN, buf, betweentimeout * 1000);  

   if(debug)syslog(LOG_WARNING, "read pocket length=%d childpid=%d",buflen, childpid);

   if(debug>2 && buflen){
       if(debug)syslog(LOG_WARNING, "read %s",buf);
   }

   if(!buflen){
      if(debug)syslog(LOG_WARNING, "read pocket length=%d goto killme childpid=%d",buflen,  childpid);
      goto killme;
   }

//    $357248013731314,9,3,111212,155028,E03503.6611,N4826.6176,134.9,1.93,234.89,6,1.43!

   if (buf[0]   != 0x24 ) { // G
      if(debug)syslog(LOG_ERR,"Bad flag start pocket go to exit");
      goto killme;
   }
   goto startparce;

startparce:

 bzero (parr,100*100);
 e=0; n=0;
 for(j=0;j<=buflen;j++){
    if(buf[j]==0x2c){
       e++;
       n=0;
    } else {
       parr[e][n++]=buf[j];
    }
 }

 for(j=0;j<=e;j++){
    syslog(LOG_ERR,"parr num=%d value=%s",j,parr[j]);
 }


   bzero(query,MAXLENQUERY);
   ret = sprintf(query,"SELECT id FROM devices WHERE (\"TelitImei\"='0%s');", parr[0]+1);

   res = getexecsql(query);
   if(res){
      if (PQgetisnull(res,0,0)){
         sprintf(id,"0");
         ifexit=1;
         if(debug>1)syslog(LOG_ERR,"getexec sql id not found ");
         } else {
            ret = sprintf(id,"%s",PQgetvalue(res, 0, 0));
            ifexit=0;
         if(debug>1)syslog(LOG_ERR,"getexec sql found id=%s",id);
         }
         clearres(res);
   }

   bzero(query,MAXLENQUERY);

   ret = sprintf(query,"INSERT INTO log_login (imei, iccid, \"Assigned\", \"when\", ip, port) VALUES('0%s','11111','t',now(),'%s','%d');",
                                          parr[0]+1, ip, dstport);

   bzero(report,REPORTLEN);
   ret = execsql(query,report);
   if(ret){
      if(debug)syslog(LOG_WARNING,"can't insert log record errno %d(%s)", ret, report);
      db_logout();
      goto killme;
   }
   if(debug>1)syslog(LOG_ERR,"insert log record errno %d(%s)", ret, report);

   bzero(argv[0], argv0size);
   snprintf(argv[0], argv0size, "/usr/local/sbin/151-%s", id);
   if(debug>1)syslog(LOG_ERR,"set new process name %s",argv[0]);

   syslog(LOG_WARNING,"authpkt imei=0%s id=%s fromip=%s:%d",
                               parr[0]+1,   id,   ip,  dstport);
   if (!ifexit){
      goto insertcoordinates;
   }

   // simple say about unknown device
   if(debug)syslog(LOG_WARNING,"unknown authpkt imei=%s id=%s fromip=%s:%d",
                                                parr[1],   id,   ip,  dstport);
   goto killme;

insertcoordinates:
//   if(debug)syslog(LOG_ERR,"insert coordinates");

/*
Dec 11 17:59:02 localhost tr151-main: parr num=0 value=$357248013731314
Dec 11 17:59:02 localhost tr151-main: parr num=1 value=9
Dec 11 17:59:02 localhost tr151-main: parr num=2 value=3
Dec 11 17:59:02 localhost tr151-main: parr num=3 value=111212
Dec 11 17:59:02 localhost tr151-main: parr num=4 value=155859
Dec 11 17:59:02 localhost tr151-main: parr num=5 value=E03503.6692
Dec 11 17:59:02 localhost tr151-main: parr num=6 value=N4826.6198
Dec 11 17:59:02 localhost tr151-main: parr num=7 value=129.8
Dec 11 17:59:02 localhost tr151-main: parr num=8 value=0.08
Dec 11 17:59:02 localhost tr151-main: parr num=9 value=35.34
Dec 11 17:59:02 localhost tr151-main: parr num=10 value=6
Dec 11 17:59:02 localhost tr151-main: parr num=11 value=1.48!
*/


   bzero(query,MAXLENQUERY);
   ret = sprintf(query,
    "INSERT INTO track (local_port,s_date,port,ip,id,w_date,y,x,speed,satelites,x_direct,y_direct,a1,a2,d1,d2,d3,d4)"\
                     " VALUES('%s',now(),'%d','%s','%s','20%c%c-%c%c-%c%c %c%c:%c%c:%c%c','%s','%s',trunc('%s',0),'%s','%c','%c','0','0','f','f','f','f');",
                        port,dstport,ip,id,
                        parr[3][4],parr[3][5],parr[3][2],parr[3][3],parr[3][0],parr[3][1],
                        parr[4][0],parr[4][1],parr[4][2],parr[4][3],parr[4][4],parr[4][5],
                        parr[5]+1,parr[6]+1,parr[8],parr[10],parr[5][0],parr[6][0]);


   if(debug)syslog(LOG_ERR,"query: %s",query);

   if(db_login() != 1 ){
      if(debug)syslog(LOG_ERR,"can't login to database");
      goto killme;
   }
   if(debug>1)syslog(LOG_ERR,"login to database");

   bzero(report,REPORTLEN);
   ret = execsql(query,report);
   if(ret){
      if(debug)syslog(LOG_WARNING,"can't insert track record errno %d(%s)",ret,report);
      if(debug>1)syslog(LOG_WARNING,"%s",query);
      db_logout();
      goto killme;
   }

   db_logout();
   if(debug>1)syslog(LOG_ERR,"logout from database");

   bzero(cmdtext,256);
   ret = sprintf(cmdtext,"$OK!");
   ret=write(1, cmdtext, strlen(cmdtext));
   if(ret != strlen(cmdtext)){
      if(debug>1)syslog(LOG_WARNING,"can't send commands need send %d real send %lu", ret, strlen(cmdtext));
   }
   if(debug>1)syslog(LOG_ERR,"send $OK! to device");
}

