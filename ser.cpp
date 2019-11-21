#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<assert.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<event.h>
#include<fcntl.h>
#include<iostream>
#include<vector>
#define MAX 100
using namespace std;
std::vector<struct event*>map_arr(MAX,nullptr); //  将事件与fd  绑定
std::vector<long int>addr(MAX,0);       //   将IP与 文件描述符绑定
//   事件处理方法
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
   addr[c]=caddr.sin_addr.s_addr;  //  将IP  与  文件描述符  绑定
   struct event* ev_c = event_new(base,c,EV_READ | EV_PERSIST,Recv_cb,&caddr);  //  创建 accept  事件的处理器
   event_add(ev_c,NULL);   //    将处理事件添加到事件队列中
   Map_arr_add(c,ev_c);
 }
 }
    //   recv  事件处理方法
  static void Recv_cb(int fd,short ev,void *arg)
{
  struct sockaddr_in * caddr = (struct sockaddr_in*)arg;
  if(ev & EV_READ)
  {
    char buff[1024]={0};
    int n=recv(fd,buff,1023,0);
    if(n<=0)
    {
      event_free(map_arr[fd]);
      map_arr[fd]=nullptr;
      close(fd);
      printf("one client over id=%s  port=%d",inet_ntoa(caddr->sin_addr),ntohs(caddr->sin_port));
      return;
    }
    long int index=inet_addr(buff);
    bool sign=1;
    int i=0;
    for(;i<MAX;i++)
    {
      if(addr[i]==index)
      {
        send(i,"yes",2,0);
        sign=0;
        break;
      }
    }
    if(sign)
    {
      send(fd,"not online",10,0);
    }
    else
    {
      send(fd,"ok",2,0);
    }
  }
}
//   将处理事件添加到表中
  static void Map_arr_add(int fd,struct event* ev) 
{
  if(fd<0||fd>MAX)
  {
    return;
  }
  map_arr[fd]=ev;
}
};
//   socket   创建
class Socket
{
  public:
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
    static  int GetSockfd()
    {
      return sockfd;
    }
    Socket(Socket &)=delete;
    Socket & operator=(const Socket&)=delete;
  private:
    static   int  sockfd;
    struct sockaddr_in saddr;
};
int Socket::sockfd;

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
int main()
{
  Event ev;
  event_add(ev.Getsock_ev(),nullptr);  //   将sockfd事件处理器添加到事件队列中
  event_base_dispatch(ev.Getbase());  //    函数来执行事件循环
}
