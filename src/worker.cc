#include"worker.h"
#include<iostream>


Worker::Worker(const char* name,DBMYSQL *db, int fd,userList *user_list)
// username(name),m_db(db),client_fd(fd),u_list(user_list)
{
    username = name;
    printf("Worker Constructor get username = %s\n",username);
    // printf("Worker Constructor get username @ %p\n",username);
    m_db = db;
    client_fd = fd;
    u_list = user_list;
}

void Worker::start()
{
    // printf("Worker start try print this addr %p\n",this);
    printf("Worker start try print this->username %s\n",this->username);
    // printf("Worker start try print this->username addr %p\n",this->username);
    pthread_create(&thread,NULL,run,this);
}

void *Worker::run(void *args)
{
    Worker *th = (Worker*)(args);
    // printf("Worker start try print args addr %p\n",args);
    // printf("Worker start try print th->username %s\n",th->username);
    // printf("Worker start try print th->username addr %p\n",th->username);
    printf("Worker for %s start running\n",th->username);
    uint32_t buffersize = 0;
    void *buffer = (void *)malloc(MAX_BUFFER_SIZE*sizeof(char));
    const char *sender_p1 = (char *)malloc(MAX_USERNAME_LENGTH*sizeof(char));
    const char *recver_p2 = (char *)malloc(MAX_USERNAME_LENGTH*sizeof(char));
    const char *content = (char *)malloc(MAX_MESSAGE_LENGTH*sizeof(char));
    char *sql = (char *)malloc(MAX_SQL_LENGTH*sizeof(char));

    while(true)
    {
        printf("Worker for %s running loop\n",th->username);
        int tmp_read_cnt=0;
        // while(tmp_read_cnt==0)
        // {
            tmp_read_cnt = read(th->client_fd,&buffersize,sizeof(uint32_t));
            if(tmp_read_cnt<=0)
            {
                printf("Worker for %s Closed\n",th->username);
                th->u_list->dropUser(th->username);
                return(void*)nullptr;
            }
                
        //     sleep(1);
        // }
        printf("Worker for %s read(buffersize) = %d\n",th->username,tmp_read_cnt);
        // if(tmp_read_cnt == -1)
        //     printf("Worker for %s read(buffersize) error: %s\n",th->username,strerror(errno));
        tmp_read_cnt = read(th->client_fd,buffer,buffersize);
        if(tmp_read_cnt<=0)
        {
            printf("Worker for %s Closed\n",th->username);
            th->u_list->dropUser(th->username);
            return(void*)nullptr;
        }
        printf("Worker for %s read(buffer) = %d\n",th->username,tmp_read_cnt);
        json_node root = str2json(std::string((char*)buffer));
        printf("Worker for %s str2json() get root\n",th->username);
        std::cout<<root<<std::endl;
        switch(root["type"].asUInt())
        {
            case rqs_type::P2PMSG:{
                printf("Worker for %s switch P2PMSG\n",th->username);
                sender_p1 = root["sender"].asCString();
                if(strcmp(sender_p1,th->username))
                {
                    printf("Fatal Error: Worker for %s switch P2PMSG sender is not right\n",th->username);
                    //发送方与本人不符
                    //返回错误信息 丢弃本次
                    // root.clear();
                    // root["type"]=rqs_type::P2PMSG;
                    // root["status"]=rpl_status::NOPRIVILEGE;
                    // std::string root_string = json2str(root);
                    // const char *reply = root_string.c_str();
                    // buffersize = (strlen(reply))*sizeof(char);
                    // write(th->client_fd,&buffersize,sizeof(uint32_t));
                    // write(th->client_fd,reply,buffersize);
                    break;
                }
                recver_p2 = root["receiver"].asCString();
                content = root["content"].asCString();
                sprintf(sql,"insert into message_log(sender,receiver,content)values"
                "('%s','%s','%s')",sender_p1,recver_p2,content);
                th->m_db->insert(sql);
                printf("Worker for %s switch P2PMSG get sql = \n%s\n",th->username,sql);
                userInfo *info = th->u_list->getUser(recver_p2);
                if(info!=nullptr)
                {
                    buffersize = strlen((char *)buffer)*sizeof(char);
                    int tmp_write_cnt = write(info->client_fd,&buffersize,sizeof(uint32_t));
                    printf("Worker for %s switch P2PMSG write(msg) = %d\n",th->username,tmp_write_cnt);
                    tmp_write_cnt = write(info->client_fd,buffer,buffersize);
                    //接收方在线，将消息发送
                }
                // root.clear();
                // root["type"]=rqs_type::P2PMSG;
                // root["status"]=rpl_status::SUCCESS;
                // std::string root_string = json2str(root);
                // const char *reply = root_string.c_str();
                // buffersize = (strlen(reply))*sizeof(char);
                // write(th->client_fd,&buffersize,sizeof(uint32_t));
                // write(th->client_fd,reply,buffersize);
                break;
            }
            case rqs_type::SYNC:{
                printf("Worker for %s switch SYNC\n",th->username);
                sender_p1 = root["person1"].asCString();
                recver_p2 = root["person2"].asCString();
                if(strcmp(sender_p1,th->username)&&strcmp(recver_p2,th->username))
                {
                    //p1 p2 都不是本人
                    // 返回错误信息 丢弃本次
                    printf("Worker for %s switch SYNC p1 && p2 not match user\n",th->username);
                    root.clear();
                    root["type"]=rqs_type::SYNC;
                    root["status"]=rpl_status::NOPRIVILEGE;
                    std::string root_string = json2str(root);
                    const char *reply = root_string.c_str();
                    buffersize = (strlen(reply))*sizeof(char);
                    write(th->client_fd,&buffersize,sizeof(uint32_t));
                    write(th->client_fd,reply,buffersize);
                    break;
                }
                
                sprintf(sql,"select sender,receiver,content,timestamp from message_log "
                "where sender='%s' and receiver='%s' or sender='%s' and receiver='%s'",sender_p1,recver_p2,recver_p2,sender_p1);
                printf("Worker for %s switch SYNC get sql = %s\n",th->username,sql);
                MYSQL_RES *res = th->m_db->select(sql);
                uint32_t row_num = mysql_num_rows(res);
                printf("Worker for %s switch SYNC get history row = %d\n",th->username,row_num);
                MYSQL_ROW row;
                for(int i=0;i<row_num;i++)
                {
                    row = mysql_fetch_row(res);
                    root.clear();
                    root["type"] = rqs_type::SYNC;
                    root["status"] = rpl_status::SUCCESS;
                    root["sender"] = row[0];
                    root["receiver"] = row[1];
                    root["content"] = row[2];
                    root["timestamp"] = row[3];
                    printf("Worker for %s switch SYNC root =\n",th->username);
                    std::cout<<root<<std::endl;
                    std::string root_string = json2str(root);
                    const char *reply = root_string.c_str();
                    printf("Worker for %s switch SYNC reply =\n%s\n",th->username,reply);
                    buffersize = (strlen(reply))*sizeof(char);
                    printf("Worker for %s switch SYNC strlen(reply) = %d\n",th->username,buffersize);
                    int tmp_write_cnt = write(th->client_fd,&buffersize,sizeof(uint32_t));
                    printf("Worker for %s switch SYNC write(buffersize) = %d\n",th->username,tmp_write_cnt);
                    write(th->client_fd,reply,buffersize);
                    printf("Worker for %s switch SYNC write(reply) = %d\n",th->username,tmp_write_cnt);
                    // sleep(1);
                }
                mysql_free_result(res);
                printf("Worker for %s switch SYNC free(res)\n",th->username);
                // 逐次返回信息
                break;
            }
            case rqs_type::P2GMSG:{
                printf("Fatal Error: Worker for %s switch P2GMSG\n",th->username);
                exit(-1);
                break;
                // not finished
            }
            default:{
                printf("Fatal Error: Worker for %s switch unexpected\n",th->username);
                exit(-1);
                root.clear();
                root["type"] = rqs_type::UNKNOWN;
                root["status"] = rpl_status::BROKEN_PKG;
                std::string root_string = json2str(root);
                const char *reply = root_string.c_str();
                buffersize = (strlen(reply))*sizeof(char);
                write(th->client_fd,&buffersize,sizeof(uint32_t));
                write(th->client_fd,reply,buffersize);
                break;
            }
        }
    }
    
    
    
    
}


