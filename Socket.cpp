/*===============================================================
*   Copyright (C) 2019 All rights reserved.
*   
*   文件名称：Socket.cpp
*   创 建 者：@liuchen
*   创建日期：2019年11月21日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include"ser.h"
  class Socket
{
  public:
    static  int GetSockfd()
    {
      return sockfd;
    }
    Socket(Socket &)=delete;
    Socket & operator=(const Socket&)=delete;
  private:

    Socket()
    {
      sockfd=socket(AF_INET,SOCK_STREAM,0);
      assert(sockfd!=-1);
      memset(&saddr,0,sizeof(saddr));
      saddr.sin_family = AF_INET;
      saddr.sin_port = htons(8000);
      saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
      int rec=bind(sockfd,(struct sockaddr *)&saddr,sizeof(saddr));
      assert(rec!=-1);
      listen(sockfd,5);
    }
    ~Socket()
    {
      close(sockfd);
    }
    static   int  sockfd;
    struct sockaddr_in saddr;
};
int Socket::sockfd;
