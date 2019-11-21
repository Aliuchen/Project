/*===============================================================
*   Copyright (C) 2019 All rights reserved.
*   
*   文件名称：ser.h
*   创 建 者：@liuchen
*   创建日期：2019年11月20日
*   描    述：
*
*   更新日志：
*
================================================================*/
#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include<unistd.h>
#include<assert.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<event.h>
#include<event2/buffer.h>
#include<event2/bufferevent.h>
#include<fcntl.h>
#include<list>
#include<iostream>
#include<vector>
#include"sql_control.h"
#include<mysql/mysql.h>
#include<unordered_map>
#define MAX FD_SETSIZE

using std::cout;
using std::endl;
void Asyn_head_text(std::string s,std::string &head, std::string &text);
void LandAnalyze(std::string &src,std::unordered_map<std::string, std::string> &mMap);

std::vector<struct event*>map_arr(MAX,nullptr);

bool getLoginInfo(std::unordered_map<std::string, std::string>& info) {
  MysqlOperator mySQL;
  std::string passwd;
  bool sign=mySQL.Query(passwd,info["name"]);
  if (!sign) {
    mySQL.Insert(info["name"], info["passWd"], info["phoneNum"]);
    cout << "注册成功.." << endl;
    return true;
  }
  cout << "注册失败" << endl;
  return false;
}
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

class Dispse  
{
  public:
    //   accept   事件处理方法
    static void Accept_cb(int fd,short ev,void *arg)
 {
 struct event_base* base=(struct event_base*)arg;
 if(ev & EV_READ)
 {
   struct sockaddr_in caddr;
   socklen_t len=sizeof(caddr);
   int c=accept(fd,(struct sockaddr*)&caddr,&len);
   if(c<0)
   {
     return;
   }
   struct event* ev_c = event_new(base,c,EV_READ | EV_PERSIST,Recv_cb,&caddr);  //  创建 accept  事件的处理器
   event_add(ev_c,NULL);   //    将处理事件添加到事件队列中
   map_arr[c]=ev_c;
 }
 }
    //   recv  事件处理方法
  static void Recv_cb(int fd,short ev,void *arg)
{
  if(ev & EV_READ)
  {
    char buff[1024];
    int n=recv(fd,buff,1024,0);
    if(n<=0)
    {
      event_free(map_arr[fd]);
      map_arr[fd]=nullptr;
      close(fd);
      return;
  }
    std::string head;
    std::string text;
    Asyn_head_text(buff,head,text);
    std::unordered_map<std::string,std::string>map;
    LandAnalyze(text,map);
    getLoginInfo(map);
  }
}
//   将处理事件添加到表中
};
class Event
{
  public:
    Event()
    {
      base=event_base_new();  //  创建event_base 实例   （Reactor实例）
      assert(base!=nullptr);
      sock_ev=event_new(base,Socket::GetSockfd(),EV_READ | EV_PERSIST ,Dispse::Accept_cb,(void *)base);  // 创建通用的事件处理器 ，对套接字的处理
    }
    ~Event()  //    释放系统资源
    {
      event_free(sock_ev);    
      event_base_free(base);
    }
    struct event_base* Getbase()
    {
      return base;
    }
    struct event* Getsock_ev()
    {
      return sock_ev;
    }
  private:
    struct event_base* base;
    struct event* sock_ev;
};
