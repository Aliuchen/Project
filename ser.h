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
#include<mysql/mysql.h>
#include<unordered_map>
#include"sql_control.h"
#define MAX FD_SETSIZE

using std::cout;
using std::endl;
class Socket
{
  public:
    static  int GetSockfd();
    Socket(Socket &)=delete;
    Socket & operator=(const Socket&)=delete;
    ~Socket();
  private:

    Socket();
    static   Socket  sockfd;
    static   int fd;
    struct sockaddr_in saddr;
};
class Dispse  
{
  public:
    static void Accept_cb(int fd,short ev,void *arg);
    static void Recv_cb(int fd,short ev,void *arg);
};
class Event
{
  public:
    Event();
    ~Event();  //    释放系统资源
    struct event_base* Getbase();
    struct event* Getsock_ev();
  private:
    struct event_base* base;
    struct event* sock_ev;
};
