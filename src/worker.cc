#include"worker.h"

Worker::Worker(const char* name,DBMYSQL *db, int fd,userList *user_list)
{
    username = name;
    m_db = db;
    client_fd = fd;
    u_list = user_list;
}

void Worker::start()
{
    pthread_create(&thread,NULL,run,this);
}

void *Worker::run(void *args)
{
    uint32_t buffersize = 0;
    void *buffer = (void *)malloc(MAX_BUFFER_SIZE*sizeof(char));
    const char *sender_p1 = (char *)malloc(MAX_USERNAME_LENGTH*sizeof(char));
    const char *recver_p2 = (char *)malloc(MAX_USERNAME_LENGTH*sizeof(char));
    const char *content = (char *)malloc(MAX_MESSAGE_LENGTH*sizeof(char));
    char *sql = (char *)malloc(MAX_SQL_LENGTH*sizeof(char));

    while(true)
    {
        read(client_fd,&buffersize,sizeof(uint32_t));
        read(client_fd,buffer,buffersize);
        json_node root = str2json(std::string((char*)buffer));
        switch(root["type"].asUInt())
        {
            case rqs_type::P2PMSG:{

                sender_p1 = root["sender_p1"].asCString();               
                if(strcmp(sender_p1,username))
                {
                    //发送方与本人不符
                    //返回错误信息 丢弃本次
                    root.clear();
                    root["type"]=rqs_type::P2PMSG;
                    root["status"]=rpl_status::NOPRIVILEGE;
                    std::string root_string = json2str(root);
                    const char *reply = root_string.c_str();
                    buffersize = (strlen(reply)+1)*sizeof(char);
                    write(client_fd,&buffersize,sizeof(uint32_t));
                    write(client_fd,reply,buffersize);
                    break;
                }
                recver_p2 = root["receiver"].asCString();
                content = root["content"].asCString();
                sprintf(sql,"insert into message_log(sender,receiver,content)values"
                "'%s','%s','%s'",sender_p1,recver_p2,content);
                m_db->insert(sql);

                userInfo *info = u_list->getUser(recver_p2);
                if(info!=nullptr)
                {
                    write(info->client_fd,buffer,buffersize);
                    //接收方在线，将消息发送
                }
                
                break;
            }
            case rqs_type::SYNC:{
                sender_p1 = root["person1"].asCString();
                recver_p2 = root["person2"].asCString();
                if(strcmp(sender_p1,username)&&strcmp(recver_p2,username))
                {
                    //p1 p2 都不是本人
                    // 返回错误信息 丢弃本次
                    root.clear();
                    root["type"]=rqs_type::SYNC;
                    root["status"]=rpl_status::NOPRIVILEGE;
                    std::string root_string = json2str(root);
                    const char *reply = root_string.c_str();
                    buffersize = (strlen(reply)+1)*sizeof(char);
                    write(client_fd,&buffersize,sizeof(uint32_t));
                    write(client_fd,reply,buffersize);
                    break;
                }
                
                sprintf(sql,"select sender,receiver,content,timestamp from message_log"
                "where sender='%s' or receiver='%s'",sender_p1,recver_p2);
                MYSQL_RES *res = m_db->select(sql);
                uint32_t row_num = mysql_num_rows(res);
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
                    std::string root_string = json2str(root);
                    const char *reply = root_string.c_str();
                    buffersize = (strlen(reply)+1)*sizeof(char);
                    write(client_fd,&buffersize,sizeof(uint32_t));
                    write(client_fd,reply,buffersize);
                }
                mysql_free_result(res);
                // 逐次返回信息
                break;
            }
            case rqs_type::P2GMSG:{
                break;
                // not finished
            }
            default:{
                root.clear();
                root["type"] = rqs_type::UNKNOWN;
                root["status"] = rpl_status::BROKEN_PKG;
                std::string root_string = json2str(root);
                const char *reply = root_string.c_str();
                buffersize = (strlen(reply)+1)*sizeof(char);
                write(client_fd,&buffersize,sizeof(uint32_t));
                write(client_fd,reply,buffersize);
                break;
            }
        }
    }
    
    
    
    
}


