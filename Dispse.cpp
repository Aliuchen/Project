/*===============================================================
*   Copyright (C) 2019 All rights reserved.
*   
*   文件名称：Dispse.cpp
*   创 建 者：@liuchen
*   创建日期：2019年11月21日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include"ser.h"
std::vector<struct event*>map_arr(MAX,nullptr);
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
  }
}
//   将处理事件添加到表中
};
