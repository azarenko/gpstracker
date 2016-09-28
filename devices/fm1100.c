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

#define DECL  "DECLARE mycursor CURSOR FOR "
#define FETCH "FETCH ALL in mycursor"
#define CLOSE "CLOSE mycursor"
#define MAXLENQUERY     2048
#define REPORTLEN       256
#define POCKETMAXLEN    1500
#define MAXCHIELDS      4096
#define FD_COPY(f, t)   (void)(*(t) = *(f))
#define INITPACKETLEN 17

static PGconn *conn;


static const unsigned short crc16tab[] = /* CRC lookup table polynomial 0xA001 */
{
0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

#define UPDC16(ch, crc) (crc16tab[((crc) ^ (ch)) & 0xff] ^ ((crc) >> 8))

/*
 * 
 */
unsigned short CRC16 ( unsigned char * puchMsg, unsigned short usDataLen )
{
 unsigned short crc16=0;
 int i;
 for (i=0; i< usDataLen; i++) {
   crc16 = UPDC16( (unsigned short) puchMsg[i], crc16);
 }
 return crc16;
}

/*
 * 
 */
int db_login(void)
{
 char *pgoptions=NULL, *pgtty=NULL;
 conn = PQsetdbLogin(primarypghost, primarypgport, pgoptions, pgtty, primarydbname, primarypglogin, primarypgpwd);
 if (PQstatus(conn) == CONNECTION_BAD) { 
    if(debug>1)syslog(LOG_ERR,"Connection to database failed %s", PQerrorMessage(conn));
    PQfinish(conn);
    return 0;
 }
 return 1;
}

/*
 * 
 */
void db_logout(void)
{
 PQfinish(conn);
}

// function for exec SQL without return data
int execsql(char *sql, char *report)
{
 PGresult   *res;

 res = PQexec(conn, "BEGIN");
 if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
    if(debug)syslog(LOG_ERR,"BEGIN command failed");
    strncpy(report, PQerrorMessage(conn),99);
    PQclear(res);
    return (1);
 }
 if(debug>1)syslog(LOG_ERR,"BEGIN command ok");

 PQclear(res);
 res = PQexec(conn, sql);
 if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
    if(debug)syslog(LOG_ERR,"PQexec command failed");
    strncpy(report, PQerrorMessage(conn),99);
    PQclear(res);
    return (2);
 }
 if(debug>1)syslog(LOG_ERR,"PQexec command ok");

 PQclear(res);
 res = PQexec(conn, "COMMIT");
 if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) { 
    if(debug)syslog(LOG_ERR,"COMMIT command failed");
    strncpy(report, PQerrorMessage(conn),99);
    PQclear(res);
    return (3);
 }
 PQclear(res);
 if(debug>1)syslog(LOG_ERR,"COMMIT command ok");
 return (0); 
}

// function for exec SQL with return data
PGresult *getexecsql(char * sql)
{ 
 char       *sqlfull;
 int        sqllen;
 PGresult   *res;

 res = PQexec(conn, "BEGIN");
 if (!res || PQresultStatus(res) != PGRES_COMMAND_OK){ 
        if(debug)syslog(LOG_ERR,"BEGIN failed");
        PQclear(res);
        return res;
 }
 PQclear(res);
 if(debug>1)syslog(LOG_ERR,"BEGIN ok");

 sqllen=strlen(DECL);              sqllen+=strlen(sql);
 sqlfull=(char*)malloc(sqllen+1);  memset(sqlfull, 0x00, sqllen+1);
 strcpy(sqlfull, DECL);            strncat(sqlfull, sql, strlen(sql)); 
 if(debug>3)syslog(LOG_ERR,"%s",sqlfull);
 res = PQexec(conn, sqlfull);
 if (!res || PQresultStatus(res) != PGRES_COMMAND_OK){
     if(debug)syslog(LOG_ERR,"DECLARE failed");
     PQclear(res);
     return res;
 }
 PQclear(res);
 free(sqlfull);
 if(debug>1)syslog(LOG_ERR,"DECLARE ok");

 res = PQexec(conn, FETCH);
 if (!res || PQresultStatus(res) != PGRES_TUPLES_OK){
     if(debug)syslog(LOG_ERR,"FETCH failed");
     PQclear(res);
     return res;
 }
 if(debug>1)syslog(LOG_ERR,"FETCH ok");
 return res;
}

/*----------------------------------------------------------------------------------*/
void clearres(PGresult *res)
{
 PQclear(res);
 res = PQexec(conn, CLOSE);
 if(debug>1)syslog(LOG_ERR,"CLOSE ok");
 PQclear(res);
 res = PQexec(conn, "COMMIT");
 if(debug>1)syslog(LOG_ERR,"COMMIT ok");
 PQclear(res);
} 

void proto(const int* client_fd)
{   
   unsigned char buf[POCKETMAXLEN+1];
   int j, buflen, waitbuflen=0, readbuflen, childpid;
   char query[MAXLENQUERY], *ip, cmdtext[256];;
   int ret, num;
   PGresult *res;
   int iskill, ifexit, isfirst = 0, offset=0;
   char id[sizeof(long)+1];
   char report[REPORTLEN];
   int  dstport;
   unsigned char allbuf[POCKETMAXLEN+1];
   unsigned int recvcrc, crc;
   unsigned int currnumber=0, numberofdata=0;

   int multibytename;
    long multibyteval;

    long time;
    int lon,lat;
    double lt, ltfract, ltint;
    double ln, lnfract, lnint;
    char dlt, dln;
    int alt;
    int angle;
    unsigned char sat;
    int speed;
    char skipcount=0;
    
    int a1=0, a2=0;
    int batlvl = 0;
   
    struct sockaddr_in client_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);
    getpeername(*client_fd, (struct sockaddr *)&client_addr, &addr_size);
    
   dstport = htons(client_addr.sin_port);
   ip = inet_ntoa(client_addr.sin_addr);
   
readstart:
   childpid=getpid();
   
   bzero(buf,POCKETMAXLEN+1);
   
   //buflen = read(*client_fd,buf,POCKETMAXLEN);  
   if(waitbuflen > 0)
   {
        buflen = readfromsock(*client_fd, waitbuflen - readbuflen, buf, 1000);
   }
   else
   {
        buflen = readfromsock(*client_fd, INITPACKETLEN, buf, 1000);  
   }

   if(debug)syslog(LOG_WARNING, "read pocket length=%d childpid=%d",buflen, childpid);

   if(debug>2 && buflen){
       if(debug)syslog(LOG_WARNING, "read %s",buf);
   }

   if(buflen == -1){
      if(debug)syslog(LOG_WARNING, "read pocket length=%d goto killme childpid=%d",buflen,  childpid);
      return;
   }

   for(j=0;j<=buflen;j++){
      if(debug>2)syslog(LOG_WARNING,"%.3d=%.2x",j,buf[j]);
   }
   
   if(isfirst==0){
      isfirst = 1;
      if(debug)syslog(LOG_WARNING, "read first auth pocket");
      goto parcefirst;
   }

 goto parcenext;

 parcefirst:
 
 waitbuflen=0;

 if(db_login() != 1 ){
      if(debug)syslog(LOG_ERR,"can't login to database may be psql stopped");
      return;
 }
   if(debug>1)syslog(LOG_ERR,"login to database");

   bzero(query,MAXLENQUERY);
   ret = sprintf(query,"SELECT id FROM tbl_devices WHERE (\"imei\"='0%s');", buf+2);

   res = getexecsql(query);
   if(res)
   {
      if (PQgetisnull(res,0,0))
      {
         sprintf(id,"0");
         ifexit=1;
         if(debug>1)syslog(LOG_ERR,"getexec sql id not found ");
        } 
         else 
         {
            ret = sprintf(id,"%s",PQgetvalue(res, 0, 0));
            ifexit=0;
            if(debug>1)syslog(LOG_ERR,"getexec sql found id=%s",id);
         }
         clearres(res);
   }

   bzero(query,MAXLENQUERY);

   ret = sprintf(query,"INSERT INTO log_login (imei, iccid, \"Assigned\", \"when\", ip, port) VALUES('0%s','11111','t',now(),'%s','%d');", buf+2, ip, dstport);

   bzero(report,REPORTLEN);
   ret = execsql(query,report);
   if(ret){
      if(debug)syslog(LOG_WARNING,"can't insert log record errno %d(%s)", ret, report);
      db_logout();
      return;
   }
   if(debug>1)syslog(LOG_ERR,"insert log record errno %d(%s)", ret, report);

   db_logout();
   if(debug>1)syslog(LOG_ERR,"logout from database");

   syslog(LOG_WARNING,"authpkt imei=0%s id=%s fromip=%s:%d",
                               buf+2,   id,   ip,  dstport);
   if (!ifexit){
      bzero(cmdtext,256);
      cmdtext[0]=1;
      ret=write(*client_fd, cmdtext, 1);
      if(ret != 1){
         if(debug>1)syslog(LOG_WARNING,"can't send commands need send %d real send %d", ret, 1);
      }
      if(debug>1)syslog(LOG_ERR,"send accept to device");
      goto readstart;
   }

   // simple say about unknown device
   if(debug)syslog(LOG_WARNING,"unknown authpkt imei=%s id=%s fromip=%s:%d",
                                                buf+2,   id,   ip,  dstport);
   bzero(cmdtext,256);
   cmdtext[0]=0;
   ret=write(*client_fd, cmdtext, 1);
   if(ret != 1){
     if(debug>1)syslog(LOG_WARNING,"can't send noaccept need send %d real send %d", ret, 1);
   }
   if(debug>1)syslog(LOG_ERR,"send no accept to device");
   return;

parcenext:
// read DISCONNECT
   if(waitbuflen==0)
   {
      waitbuflen = buf[4];
      waitbuflen = waitbuflen<<8;
      waitbuflen = waitbuflen + buf[5];
      waitbuflen = waitbuflen<<8;
      waitbuflen = waitbuflen + buf[6];
      waitbuflen = waitbuflen<<8;
      waitbuflen = waitbuflen + buf[7];

      waitbuflen = waitbuflen + 12; // first 4 zero + 4 data length + 4 crc.

      readbuflen = buflen;

      bzero(allbuf,POCKETMAXLEN+1);
      memcpy(allbuf, buf, buflen);
      if(debug>1)syslog(LOG_ERR,"Set waitbuflen=%d read=%d", waitbuflen, readbuflen);

   }
   else
   {

      memcpy(allbuf+readbuflen, buf, buflen);
      readbuflen += buflen;
      if(debug>1)syslog(LOG_ERR,"waitbuflen=%d readbuflen=%d",waitbuflen, readbuflen);

   }
   if(waitbuflen > readbuflen ){
      if(debug>1)syslog(LOG_ERR,"wait next portion %d", waitbuflen - readbuflen);
      goto readstart;
   }
   if(debug>1)syslog(LOG_ERR,"read all data %d", waitbuflen);

   recvcrc = allbuf[waitbuflen-4];
   recvcrc = recvcrc << 8;
   recvcrc = recvcrc+allbuf[waitbuflen-3];
   recvcrc = recvcrc << 8;
   recvcrc = recvcrc+allbuf[waitbuflen-2];
   recvcrc = recvcrc << 8;
   recvcrc = recvcrc+allbuf[waitbuflen-1];

   crc = CRC16(allbuf+8, waitbuflen-(4+8) );
   if( crc != recvcrc ){
      if(debug>1)syslog(LOG_ERR,"bad crc=%d recvcrc=%d goto killme", crc, recvcrc);
      return;
   }
   if(debug>1)syslog(LOG_ERR,"good crc=%d recvcrc=%d", crc, recvcrc);

   offset = 0 ;

   // Four zeros 
   offset++;
   offset++;
   offset++;
   offset++;

   // Data length (Integer)
   offset++;
   offset++;
   offset++;
   offset++;

   if(allbuf[offset] != 0x08){
      if(debug>1)syslog(LOG_ERR,"bad start pkt head wait 08 aktual %.2x", allbuf[offset]);
   }
   offset++;

   currnumber = 0;
   numberofdata=allbuf[offset]; offset++;
   if(debug) syslog(LOG_WARNING,"parce numberofdata %d",numberofdata);

nextframe:
   time = allbuf[offset]; offset++;
   time = time << 8;
   time = time+allbuf[offset]; offset++;
   time = time << 8;
   time = time+allbuf[offset]; offset++;
   time = time << 8;
   time = time+allbuf[offset]; offset++;
   time = time << 8;
   time = time+allbuf[offset]; offset++;
   time = time << 8;
   time = time+allbuf[offset]; offset++;
   time = time << 8;
   time = time+allbuf[offset]; offset++;
   time = time << 8;
   time = time+allbuf[offset]; offset++;
   time = time / 1000;
   time = time - 2*60*60;
   if(debug) syslog(LOG_WARNING,"parce date %lu",time);

   // skip priority
   offset++;

   lat = allbuf[offset]; offset++;
   lat = lat << 8;
   lat = lat+allbuf[offset]; offset++;
   lat = lat << 8;
   lat = lat+allbuf[offset]; offset++;
   lat = lat << 8;
   lat = lat+allbuf[offset]; offset++;
    lt = lat;
    lt = lt / 10000000 ;
   ltfract = modf (lt , &ltint);
   lt = ltint*100 + ltfract*60;
   if(lt < 0 ){
      lt = lt * (-1);
      dlt='S';
   }else{
      dlt='N';
   }
   if(debug) syslog(LOG_WARNING,"parce latitude %c%f", dlt,lt);

   lon = allbuf[offset]; offset++;
   lon = lon << 8;
   lon = lon+allbuf[offset]; offset++;
   lon = lon << 8;
   lon = lon+allbuf[offset]; offset++;
   lon = lon << 8;
   lon = lon+allbuf[offset]; offset++;
    ln = lon;
    ln = ln / 10000000 ;
   lnfract = modf (ln , &lnint);
   ln = lnint*100 + lnfract*60;
   if( ln < 0 ){
      ln = ln * (-1);
      dln='W';
   }else{
      dln='E';
   }
   if(debug) syslog(LOG_WARNING,"parce longitude %c%f", dln,ln);

   alt = allbuf[offset]; offset++;
   alt = alt << 8;
   alt = alt+allbuf[offset]; offset++;
   if(debug) syslog(LOG_WARNING,"parce high %d",alt);

   angle = allbuf[offset]; offset++;
   angle = angle << 8;
   angle = angle+allbuf[offset]; offset++;
   if(debug) syslog(LOG_WARNING,"parce azimuth %d",angle);

   sat = allbuf[offset]; offset++;
   if(debug) syslog(LOG_WARNING,"parce satelites %d",sat);

   speed = allbuf[offset]; offset++;
   speed = speed << 8;
   speed = speed+allbuf[offset]; offset++;
   if(debug) syslog(LOG_WARNING,"parce speed %d",speed);

//   IO element ID of Event generated (in this case when 00 – data generated not on event)
   if(debug) syslog(LOG_WARNING,"parce Event generated  %d",allbuf[offset]);
   offset++;

//   IO element ID of Event generated (in this case when 00 – data generated not on event)
   if(debug) syslog(LOG_WARNING,"count io elements in data  %d",allbuf[offset]);
   offset++;

// count 1 byte param
   skipcount = allbuf[offset]; offset++;
   if(debug) syslog(LOG_WARNING,"parce 1 byte count %d",skipcount);
   for(j=0; j< skipcount; j++){
      // skip paramname
      if(debug) syslog(LOG_WARNING,"parce 1 byte paramname %d",allbuf[offset]);
      offset++;
      // skip value
      if(debug) syslog(LOG_WARNING,"parce 1 byte paramval %d",allbuf[offset]);
      offset++;
   }

// count 2 byte param
   skipcount = allbuf[offset]; offset++;
   if(debug) syslog(LOG_WARNING,"parce 2 byte count %d",skipcount);
   for(j=0; j< skipcount; j++){
      // skip paramname
      if(debug) syslog(LOG_WARNING,"parce 2 byte paramname %d",allbuf[offset]);
      multibytename = allbuf[offset];
      offset++;
      // skip value
      if(debug) syslog(LOG_WARNING,"parce 1 byte paramval %d",allbuf[offset]);
      multibyteval = allbuf[offset]; multibyteval = multibyteval << 8;
      offset++;
      if(debug) syslog(LOG_WARNING,"parce 2 byte paramval %d",allbuf[offset]);
      multibyteval = multibyteval + allbuf[offset];
      offset++;
      if(debug) syslog(LOG_WARNING,"parce param=%d paramval %ld", multibytename, multibyteval);
      // analog input 1
      if(multibytename == 9){
        a1 = multibyteval;
      // analog input 2
      }else if(multibytename == 10){
        a2 = multibyteval;
      // power voltage
      }else if(multibytename == 66){
        batlvl = multibyteval/1000;
      }
   }

// count 4 byte param
   skipcount = allbuf[offset]; offset++;
   if(debug) syslog(LOG_WARNING,"parce 4 byte count %d",skipcount);
   for(j=0; j< skipcount; j++){
      // skip paramname
      if(debug) syslog(LOG_WARNING,"parce 4 byte paramname %d",allbuf[offset]);
      multibytename = allbuf[offset];
      offset++;
      // skip value
      if(debug) syslog(LOG_WARNING,"parce 1 byte paramval %d",allbuf[offset]);
      multibyteval = allbuf[offset]; multibyteval = multibyteval << 8;
      offset++;
      if(debug) syslog(LOG_WARNING,"parce 2 byte paramval %d",allbuf[offset]);
      multibyteval = multibyteval + allbuf[offset]; multibyteval = multibyteval << 8;
      offset++;
      if(debug) syslog(LOG_WARNING,"parce 3 byte paramval %d",allbuf[offset]);
      multibyteval = multibyteval + allbuf[offset]; multibyteval = multibyteval << 8;
      offset++;
      if(debug) syslog(LOG_WARNING,"parce 4 byte paramval %d",allbuf[offset]);
      multibyteval = multibyteval + allbuf[offset];
      offset++;
      if(debug) syslog(LOG_WARNING,"parce param=%d paramval %ld", multibytename, multibyteval);
   }

// count 8 byte param
   skipcount = allbuf[offset]; offset++;
   if(debug) syslog(LOG_WARNING,"parce 8 byte count %d",skipcount);
   for(j=0; j< skipcount; j++){
      // skip paramname
      if(debug) syslog(LOG_WARNING,"parce 8 byte paramname %d",allbuf[offset]);
      multibytename = allbuf[offset];
      offset++;
      // skip value
      if(debug) syslog(LOG_WARNING,"parce 1 byte paramval %d",allbuf[offset]);
      multibyteval = allbuf[offset]; multibyteval = multibyteval << 8;
      offset++;
      if(debug) syslog(LOG_WARNING,"parce 2 byte paramval %d",allbuf[offset]);
      multibyteval = multibyteval + allbuf[offset]; multibyteval = multibyteval << 8;
      offset++;
      if(debug) syslog(LOG_WARNING,"parce 3 byte paramval %d",allbuf[offset]);
      multibyteval = multibyteval + allbuf[offset]; multibyteval = multibyteval << 8;
      offset++;
      if(debug) syslog(LOG_WARNING,"parce 4 byte paramval %d",allbuf[offset]);
      multibyteval = multibyteval + allbuf[offset]; multibyteval = multibyteval << 8;
      offset++;
      if(debug) syslog(LOG_WARNING,"parce 5 byte paramval %d",allbuf[offset]);
      multibyteval = multibyteval + allbuf[offset]; multibyteval = multibyteval << 8;
      offset++;
      if(debug) syslog(LOG_WARNING,"parce 6 byte paramval %d",allbuf[offset]);
      multibyteval = multibyteval + allbuf[offset]; multibyteval = multibyteval << 8;
      offset++;
      if(debug) syslog(LOG_WARNING,"parce 7 byte paramval %d",allbuf[offset]);
      multibyteval = multibyteval + allbuf[offset]; multibyteval = multibyteval << 8;
      offset++;
      if(debug) syslog(LOG_WARNING,"parce 8 byte paramval %d",allbuf[offset]);
      multibyteval = multibyteval + allbuf[offset];
      offset++;
      if(debug) syslog(LOG_WARNING,"parce param=%d paramval %ld", multibytename, multibyteval);
   }


   bzero(query,MAXLENQUERY);
   ret = sprintf(query,
    "INSERT INTO track (local_port,s_date,port,  ip,  id,                        w_date,   y,   x,        speed,satelites,x_direct,y_direct, a1,   a2, d1, d2, d3, d4,  a3,  a4,batlevel)"\
                     " VALUES('%d', now(),'%d','%s','%s',to_timestamp('%lu')::timestamp,'%f','%f',trunc('%f',0),'%d',         '%c',    '%c','%d','%d','f','f','f','f','%d','%d',    '%d');",
                        port,dstport,ip,id,time,lt,ln,speed/1.852, sat, dln, dlt,a1,a2, alt, angle,batlvl);

   if(debug)syslog(LOG_ERR,"query: %s",query);

   if(db_login() != 1 ){
      if(debug)syslog(LOG_ERR,"can't login to database");
      return;
   }
   if(debug>1)syslog(LOG_ERR,"login to database");

   bzero(report,REPORTLEN);
   ret = execsql(query,report);
   if(ret){
      if(debug)syslog(LOG_WARNING,"can't insert track record errno %d(%s)",ret,report);
      if(debug>1)syslog(LOG_WARNING,"%s",query);
      db_logout();
      return;
   }

   db_logout();
   if(debug>1)syslog(LOG_ERR,"logout from database");

   currnumber++;
   if(currnumber < numberofdata){
      if(debug>1)syslog(LOG_ERR,"parce next frame %d of %d ", currnumber, numberofdata);
      goto nextframe;
   }

   bzero(cmdtext,256);
   cmdtext[3]=numberofdata;
   ret=write(*client_fd, cmdtext, 4);
   if(ret != 4){
      if(debug>1)syslog(LOG_WARNING,"can't send acknowledges data reception  need send 4 real send %d", ret);
   }
   if(debug>1)syslog(LOG_ERR,"send acknowledges data reception to device");

   waitbuflen=0;
}
