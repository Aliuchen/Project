#include"ser.h"
int main()
{
  Event ev;
  event_add(ev.Getsock_ev(),nullptr);  //   将sockfd事件处理器添加到事件队列中
  event_base_dispatch(ev.Getbase());  //    函数来执行事件循环
}
