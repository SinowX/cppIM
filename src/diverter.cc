#include"diverter.h"


Diverter::Diverter()
{
    initListenFd();
    initDB();
    u_list=new userList();
    while(true)
    {
        run_once();
    }
    
}

Diverter::~Diverter()
{
    close(listen_fd);
}

void Diverter::initListenFd()
{
    struct sockaddr_in server_addr;
    if((listen_fd = socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("Diverter_socket(): %s\n",strerror(errno));
        return;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LISTEN_PORT);
    server_addr.sin_addr.s_addr = inet_addr(LISTEN_ADDR);

    if( bind(listen_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1)
    {
        printf("Diverter_bind(): %s\n",strerror(errno));
        return;
    }

    if(listen(listen_fd,MAX_LISTEN_QUEUE)==-1)
    {
        printf("Diverter_run_listen(): %s\n",strerror(errno));
        return;
    }else{
        printf("Diverter Socket Listening...");
    }

}

void Diverter::initDB()
{
    m_db = new DBMYSQL("tdsqlshard-3l8qqete.sql.tencentcdb.com",
        50,"remoter","Asd123456@","zqf");
}


void Diverter::run_once()
{
    int client_fd;
    client_fd = accept(listen_fd,NULL,NULL);
    uint32_t buffersize = 0;
    read(client_fd,&buffersize,sizeof(uint32_t));
    void *buffer = (void *)malloc(sizeof(buffersize));
    read(client_fd,buffer,buffersize);
    json_node root = str2json(std::string((char*)buffer));
    free(buffer);
    // root 里边含有 type(uint8) username password
    switch(root["type"].asUInt())
    {
        case rqs_type::LOGIN :{
            const char *username, *password;
            username = root["username"].asCString();
            password = root["password"].asCString();
            if(u_list->getUser(username)!=nullptr)
            {
                root.clear();
                root["type"] = rqs_type::LOGIN;
                root["status"] = rpl_status::MULTILOGIN;
                std::string root_string = json2str(root);
                const char *reply = root_string.c_str();
                buffersize = (strlen(reply)+1)*sizeof(char);
                write(client_fd,&buffersize,sizeof(uint32_t));
                write(client_fd,reply,buffersize);
                break;
                //用户已登录，不允许重复登录
                //返回错误信息，并断开
            }
            char *sql = (char *)malloc(100*sizeof(char));
            sprintf(sql,"select password from user where username = '%s'",username);
            
            MYSQL_RES* res = m_db->select(sql);

            if(res == nullptr)
            {
                //用户不存在，可为其注册
                sprintf(sql,"insert into user(username,password)values('%s','%s')",username,password);
                m_db->insert(sql);
                //注册成功
            }else{
                MYSQL_ROW row = mysql_fetch_row(res);
                if(strcmp(row[0],password))
                {
                    //密码错误
                    //返回错误信息，然后断开
                    root.clear();
                    root["type"] = rqs_type::LOGIN;
                    root["status"] = rpl_status::PWDNOTMATCH;
                    std::string root_string = json2str(root);
                    const char *reply = root_string.c_str();
                    buffersize = (strlen(reply)+1)*sizeof(char);
                    write(client_fd,&buffersize,sizeof(uint32_t));
                    write(client_fd,reply,buffersize);
                    close(client_fd);
                    return;
                }
            }
            //开始为其登录
            //创建新线程
            Worker * thread = new Worker(username,m_db,client_fd,u_list);
            thread->start();
            u_list->addUser(username,client_fd);
            
            root.clear();
            root["type"] = rqs_type::LOGIN;
            root["status"] = rpl_status::SUCCESS;
            std::string root_string = json2str(root);
            const char *reply = root_string.c_str();
            buffersize = (strlen(reply)+1)*sizeof(char);
            write(client_fd,&buffersize,sizeof(uint32_t));
            write(client_fd,reply,buffersize);
            close(client_fd);
            //返回登陆成功

        }
        default:{
            //处理错误
            root.clear();
            root["type"] = rqs_type::UNKNOWN;
            root["status"] = rpl_status::BROKEN_PKG;
            std::string root_string = json2str(root);
            const char *reply = root_string.c_str();
            buffersize = (strlen(reply)+1)*sizeof(char);
            write(client_fd,&buffersize,sizeof(uint32_t));
            write(client_fd,reply,buffersize);
            close(client_fd);
            break;
        }
    }
}