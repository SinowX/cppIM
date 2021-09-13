#include"diverter.h"
#include<iostream>


Diverter::Diverter()
{
    initListenFd();
    initDB();
    u_list=new userList();
    while(true)
    {
        printf("Diverter Runing Loop\n");
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
        printf("Diverter Socket Listening...\n");
    }

}

void Diverter::initDB()
{
    m_db = new DBMYSQL("localhost",
        3306,"remoter","asd123456","zqf");
    
}


void Diverter::run_once()
{
    int client_fd;
    client_fd = accept(listen_fd,NULL,NULL);

    // int iMode=0;
    // ioctl(client_fd,FIONBIO,&iMode);    
    // set client fd blocking mode
    // struct timeval t;    
    // t.tv_sec = 0;
    // t.tv_usec = 0;
    // setsockopt(
    //     client_fd,     // Socket descriptor
    //     SOL_SOCKET, // To manipulate options at the sockets API level
    //     SO_RCVTIMEO,// Specify the receiving or sending timeouts 
    //     (const void *)(&t), // option values
    //     sizeof(t) 
    // );   
    
    printf("Diverter::run_once accept() client_fd = %d\n",client_fd);
    uint32_t buffersize = 0;
    printf("Diverter::run_once buffersize init buffersize = %d\n",buffersize);
    read(client_fd,&buffersize,sizeof(uint32_t));
    printf("Diverter::run_once read() buffersize = %d\n",buffersize);
    void *buffer = (void *)malloc(buffersize*sizeof(char));
    printf("Diverter::run_once *buffer init @ %p\n",buffer);
    uint tmp_num = read(client_fd,buffer,buffersize);
    printf("Diverter::run_once read() *buffer %uB",tmp_num);
    json_node root = str2json(std::string((char*)buffer));
    std::cout<<"Diverter::run_once str2json(buffer) "<<std::endl<<root<<std::endl;
    free(buffer);
    // root 里边含有 type(uint8) username password
    switch(root["type"].asUInt())
    {
        case rqs_type::LOGIN :{
            printf("Diverter::run_once switch to be LOGIN\n");
            char *username =(char*)malloc(MAX_USERNAME_LENGTH*sizeof(char));
            char *password =(char*)malloc(MAX_PWD_LENGTH*sizeof(char));
            strcpy(username,root["username"].asCString());
            strcpy(password,root["password"].asCString());
            // username = root["username"].asCString();
            // password = root["password"].asCString();
            // root[""].asCString() 这个地址好像会被修改，暂不明，还是先用 malloc 复制到 堆上吧

            printf("Diverter::run_once switch username=%s, password=%s\n",username,password);
            if(u_list->getUser(username)!=nullptr)
            {
                printf("Diverter::run_once switch user multi-login\n");
                root.clear();
                printf("Diverter::run_once switch root.clear() \n");
                std::cout<<root<<std::endl;
                root["type"] = rqs_type::LOGIN;
                root["status"] = rpl_status::MULTILOGIN;
                printf("Diverter::run_once switch root assignment\n");
                std::cout<<root<<std::endl;
                std::string root_string = json2str(root);
                printf("Diverter::run_once switch get (string)root_string\n");
                std::cout<<root_string<<std::endl;
                const char *reply = root_string.c_str();
                printf("Diverter::run_once switch reply = root_string.cstr()\n");
                printf("%s\n",reply);
                buffersize = (strlen(reply))*sizeof(char);
                printf("Diverter::run_once switch strlen(reply) = %d\n",buffersize);
                int tmp_write_cnt = write(client_fd,&buffersize,sizeof(uint32_t));
                printf("Diverter::run_once switch write(buffersize) = %d\n",tmp_write_cnt);
                tmp_write_cnt =  write(client_fd,reply,buffersize);
                printf("Diverter::run_once switch write(reply) = %d\n",tmp_write_cnt);
                break;
                //用户已登录，不允许重复登录
                //返回错误信息，并断开
            }
            char *sql = (char *)malloc(100*sizeof(char));
            sprintf(sql,"select password from user where username = '%s'",username);
            printf("Diverter::run_once switch init sql = \n%s\n",sql);
            MYSQL_RES* res = m_db->select(sql);
            printf("Diverter::run_once switch select(sql) *res @ %p\n",res);
            int row_num = mysql_num_rows(res);
            printf("Diverter::run_once switch select(sql) *res row_num = %d\n",row_num);
            if(row_num==0)
            {
                mysql_free_result(res);
                //用户不存在，可为其注册
                printf("Diverter::run_once switch user not exist, try sigin\n");
                sprintf(sql,"insert into user(username,password)values('%s','%s')",username,password);
                int tmp_status = m_db->insert(sql);
                printf("Diverter::run_once switch insert code %d\n",tmp_status);
                //注册成功
            }else{
                printf("Diverter::run_once switch user exist, try login\n");
                MYSQL_ROW row = mysql_fetch_row(res);
                mysql_free_result(res);
                if(strcmp(row[0],password))
                {
                    //密码错误
                    //返回错误信息，然后断开

                    printf("Diverter::run_once switch user exist, login failed\n");
                    root.clear();
                    printf("Diverter::run_once switch user exist, login failed root.clear()\n");
                    std::cout<<root<<std::endl;
                    root["type"] = rqs_type::LOGIN;
                    root["status"] = rpl_status::PWDNOTMATCH;
                    printf("Diverter::run_once switch user exist, login failed root assigned\n");
                    std::cout<<root<<std::endl;
                    std::string root_string = json2str(root);
                    printf("Diverter::run_once switch user exist, login failed get (string)root_string\n");
                    std::cout<<root_string<<std::endl;
                    const char *reply = root_string.c_str();
                    printf("Diverter::run_once switch user exist, login failed reply = root_string.cstr()\n");
                    printf("%s\n",reply);
                    buffersize = (strlen(reply))*sizeof(char);
                    printf("Diverter::run_once switch user exist, login failed strlen(reply) = %d\n",buffersize);
                    int tmp_write_cnt = write(client_fd,&buffersize,sizeof(uint32_t));
                    printf("Diverter::run_once switch user exist, login failed write(buffersize) = %d\n",tmp_write_cnt);
                    tmp_write_cnt = write(client_fd,reply,buffersize);
                    printf("Diverter::run_once switch user exist, login failed write(reply) = %d\n",tmp_write_cnt);
                    close(client_fd);
                    return;
                }
            }
            //开始为其登录
            //创建新线程
            printf("Diverter::run_once switch login success\n");
            Worker * thread = new Worker(username,m_db,client_fd,u_list);
            thread->start();
            printf("Diverter::run_once switch login success, thread start\n");
            u_list->addUser(username,client_fd);
            printf("Diverter::run_once switch login success, add User to User List\n");
            root.clear();
            printf("Diverter::run_once switch login success root.clear()\n");
            root["type"] = rqs_type::LOGIN;
            root["status"] = rpl_status::SUCCESS;
            printf("Diverter::run_once switch login success root asigned\n");
            std::cout<<root<<std::endl;
            std::string root_string = json2str(root);
            const char *reply = root_string.c_str();
            printf("Diverter::run_once switch login success reply = root_string.c_str()\n");
            printf("%s\n",reply);
            buffersize = (strlen(reply))*sizeof(char);
            printf("Diverter::run_once switch login success strlen(reply) = %d\n",buffersize);
            int tmp_write_cnt = write(client_fd,&buffersize,sizeof(uint32_t));
            printf("Diverter::run_once switch login success write(buffersize) = %d\n",tmp_write_cnt);
            tmp_write_cnt = write(client_fd,reply,buffersize);
            printf("Diverter::run_once switch login success write(reply) = %d\n",tmp_write_cnt);
            //返回登陆成功
            break;
        }
        default:{
            //处理错误

            printf("Fatal Error: Diverter::run_once switch unexpected");
            root.clear();
            root["type"] = rqs_type::UNKNOWN;
            root["status"] = rpl_status::BROKEN_PKG;
            std::string root_string = json2str(root);
            const char *reply = root_string.c_str();
            buffersize = (strlen(reply))*sizeof(char);
            write(client_fd,&buffersize,sizeof(uint32_t));
            write(client_fd,reply,buffersize);
            close(client_fd);
            break;
        }
    }
}