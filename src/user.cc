#include"user.h"

userList::userList()
{
    head=(userInfo *)malloc(sizeof(userInfo));
    head->next=nullptr;
    printf("UserList Build Success\n");
}

void userList::addUser(const char *name,int client_fd)
{
    userInfo *p=head;
    userInfo *info = (userInfo *)malloc(sizeof(userInfo));
    strcpy(info->name,name);
    info->client_fd=client_fd;
    // info->thread=thread;
    info->next=p->next;
    p->next=info;
}

void userList::dropUser(const char *name)
{
    userInfo *p,*q;
    q=head;
    p=q->next;
    while(p!=nullptr)
    {
        if(!strcmp(p->name,name))
        {
            q->next=p->next;
            free(p);
            break;
        }
    }
}

// if none return nullptr
userInfo *userList::getUser(const char *name)
{
    printf("Geting User Now\n");
    userInfo *p=head->next;
    while(p!=nullptr)
    {
        if(!strcmp(p->name,name))
            return p;
        p=p->next;
    }

    return nullptr;
}
