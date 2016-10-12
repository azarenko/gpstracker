#include "devices.h"

#define DECL  "DECLARE mycursor CURSOR FOR "
#define FETCH "FETCH ALL in mycursor"
#define CLOSE "CLOSE mycursor"

void *threadFunc(void *arg)
{   
    PGconn *conn = NULL;  
    
        while(1)
        {
            if(db_login(&conn) == 1)
            {
                int client_fd;
                if(fifo_get(&client_fd) == EXIT_FAILURE)
                {
                    usleep(100 * 1000);
                    continue;
                }        
                proto(&client_fd, conn);    
                close(client_fd);
            }
            else
            {
                sleep(1);
                continue;
            }
        }
}

/*
 * 
 */
int db_login(PGconn **conn)
{    
    if (PQstatus(*conn) == CONNECTION_BAD) 
    {
        char *pgoptions=NULL, *pgtty=NULL;
        *conn = PQsetdbLogin(primarypghost, primarypgport, pgoptions, pgtty, primarydbname, primarypglogin, primarypgpwd);
        if (PQstatus(*conn) == CONNECTION_BAD) 
        { 
            if(debug>1)syslog(LOG_ERR,"Connection to database failed %s", PQerrorMessage(*conn));
            PQfinish(*conn);
                        
            *conn = PQsetdbLogin(secondarypghost, secondarypgport, pgoptions, pgtty, secondarydbname, secondarypglogin, secondarypgpwd);
            
            if (PQstatus(*conn) == CONNECTION_BAD) 
            { 
                if(debug>1)syslog(LOG_ERR,"Connection to database failed %s", PQerrorMessage(*conn));
                PQfinish(*conn);
                return 0;
            }
        }        
        return 1;
    }
    else
    {
        return 1;
    }
}

// function for exec SQL without return data
int execsql(PGconn *conn, char *sql, char *report)
{
    if (PQstatus(conn) == CONNECTION_BAD) 
    {
        return (2);
    }
    
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
PGresult *getexecsql(PGconn *conn, char * sql)
{   
    if (PQstatus(conn) == CONNECTION_BAD) 
    {
        return NULL;
    }
    
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
void clearres(PGconn *conn, PGresult *res)
{
    if (PQstatus(conn) == CONNECTION_BAD) 
    {
        return;
    }
    
    PQclear(res);
    res = PQexec(conn, CLOSE);
    if(debug>1)syslog(LOG_ERR,"CLOSE ok");
    PQclear(res);
    res = PQexec(conn, "COMMIT");
    if(debug>1)syslog(LOG_ERR,"COMMIT ok");
    PQclear(res);    
} 
