#include"database.h"

DBMYSQL::DBMYSQL(const char*host,int port, const char* user,const char* passwd,const char* dbname)
{
    if(mysql_init(&handle)==NULL)
    {
        printf("DBMYSQL_mysql_init(): %s\n",mysql_error(&handle));
        exit(-1);
    }
    if(mysql_real_connect(&handle,host,user,passwd,dbname,port,NULL,0)==NULL)
    {
        printf("DBMYSQL_mysql_real_connect(): %s\n",mysql_error(&handle));
        exit(-1);
    }
    printf("Database Connect Success\n");
}

DBMYSQL::~DBMYSQL()
{
    mysql_close(&handle);
    printf("Database Close Success\n");
}

int DBMYSQL::insert(const char* sql)
{
    if(mysql_real_query(&handle,sql,strlen(sql))!=0)
    {
        printf("DBMYSQL_mysql_real_query(): %s\n",mysql_error(&handle));
        return -1;
    }
    return 0;
}

MYSQL_RES* DBMYSQL::select(const char* sql)
{
    if(mysql_real_query(&handle,sql,strlen(sql))!=0)
    {
        printf("DBMYSQL_mysql_real_query(): %s\n",mysql_error(&handle));
        return NULL;
    }
    MYSQL_RES* res = mysql_store_result(&handle);
    return res;
    // if (res==NULL)
    // {
    //     printf("DBMYSQL_mysql_real_query(): %s\n",mysql_error(&handle));
    //     return NULL;
    // }
    // MYSQL_ROW row = mysql_fetch_row(res);
    // return row;
}

