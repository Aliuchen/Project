/*===============================================================
*   Copyright (C) 2019 All rights reserved.
*   
*   文件名称：Event.cpp
*   创 建 者：@liuchen
*   创建日期：2019年11月21日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include"ser.h"
#include"Socket.cpp"
#include"Dispse.cpp"
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
