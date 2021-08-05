#ifndef DATABASE_H
#define DATABASE_H

#include<stdio.h>
#include<string.h>

#include<mysql/mysql.h>



class DBMYSQL
{
public:
    DBMYSQL(const char*host,int port ,const char* user,const char* passwd,const char* dbname);
    ~DBMYSQL();

    //if failed return -1 else return 0
    int insert(const char* sql);
    
    //if none return nullptr !!!!
    MYSQL_RES* select(const char* sql);

private:
    MYSQL handle;

};

#endif