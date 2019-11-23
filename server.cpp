#include "ser.h"

static void Asyn_head_text(std::string s,std::string &head, std::string &text)
{
	int pos1 = 0;
	int pos2 = 0;
	pos2 = s.find("\r\n\r\n", pos1);
	head = s.substr(pos1, pos2-pos1+1);
	pos1 = pos2+2;
	text = s.substr(pos1+2, s.size()-pos1);
}
static void LandAnalyze(std::string &src, std::unordered_map<std::string, std::string> &mMap){
  //以\r\n分割
  
  int pos1=0;
  int pos2=0;
	const std::string temp = "\r\n";
  std::vector<std::string>vec;
  for(;;){
    pos2=src.find(temp,pos1);
    if (pos2 == -1)
    {
      break;
    }
    vec.push_back(src.substr(pos1,pos2-pos1));
    pos1=pos2+2;
  }

  //以：分割
  std::vector<std::string> vec2;
  int num1=0;
  int num2=0;
  const std::string temp2=":";
  int len=vec.size();   
  for(int j=0;j<len;j++){
    num2 = 0;
    num1 = 0;
    num2=vec[j].find(temp2,num1);
    if(num2==-1){
      break;
    }
    vec2.push_back(vec[j].substr(num1,num2-num1));
    num1 = num2+1;
    vec2.push_back(vec[j].substr(num1, vec[j].size()-num1));
  }
  int len1=vec2.size();
  for(int i=0;i<len1;i+=2){
    mMap[vec2[i]] = vec2[i+1];
  }
}

static bool getLoginInfo(std::unordered_map<std::string, std::string>& info) {
  MysqlOperator mySQL;
  std::string passwd;
  //bool sign=mySQL.Query(passwd,info["name"]);
  //if (!sign) {
    mySQL.Insert(info["name"], info["passWd"], info["phoneNum"]);
    cout << "注册成功.." << endl;
    return true;
  //}
 //= cout << "注册失败" << endl;
 //= return false;
}

std::vector<struct event*>map_arr(MAX,nullptr);

Socket Socket::sockfd;

int Socket::GetSockfd()
{
  return sockfd.fd;
}
Socket::~Socket()
{
  close(sockfd.fd);
}
Socket::Socket()
{
  sockfd.fd=socket(AF_INET,SOCK_STREAM,0);
  assert(sockfd.fd!=-1);
  memset(&saddr,0,sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(8000);
  saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
  int rec=bind(sockfd.fd,(struct sockaddr *)&saddr,sizeof(saddr));
  assert(rec!=-1);
  listen(sockfd.fd,5);
}



//   accept   事件处理方法
void Dispse::Accept_cb(int fd,short ev,void *arg)
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
   // struct event* ev_c = event_new(base,c,EV_READ | EV_PERSIST,Recv_cb,&caddr);  //  创建 accept  事件的处理器
    //event_add(ev_c,NULL);   //    将处理事件添加到事件队列中
    //map_arr[c]=ev_c;
    struct bufferevent *bf= bufferevent_socket_new(base,c,BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bf,read_cd,write_cd,error_cd,NULL);
    bufferevent_enable(bf,EV_READ|EV_WRITE);
  }
}
//   recv  事件处理方法
 void Dispse::read_cd(bufferevent *bf,void *ctx)
{
  struct evbuffer *input=bufferevent_get_input(bf);
  size_t input_len=evbuffer_get_length(input);
  const char *_line = (const char*)evbuffer_pullup(input,input_len);
  cout<<_line<<endl;
  bufferevent_write(bf,"ok",3);
    std::string head;
    std::string text;
    Asyn_head_text(_line,head,text);
    cout<<"_______"<<endl;
    cout<<head<<endl;
    cout<<"_________________"<<endl;
    cout<<text<<endl;
    cout<<"________________"<<endl;
    std::unordered_map<std::string,std::string>map;
    LandAnalyze(text,map);
    cout<<"**********************"<<endl;

    getLoginInfo(map);
}
 void Dispse::write_cd(bufferevent *bf,void *ctx)
{
   cout<<"ok"<<endl;
}
 void Dispse::error_cd(struct bufferevent *bev, short error, void *ctx)
{
    if (error & BEV_EVENT_EOF)
    {
        /* connection has been closed, do any clean up here */
        printf("connection closed\n");
    }
    else if (error & BEV_EVENT_ERROR)
    {
        /* check errno to see what error occurred */
        printf("some other error\n");
    }
    else if (error & BEV_EVENT_TIMEOUT)
    {
        /* must be a timeout event handle, handle it */
        printf("Timed out\n");
    }
    bufferevent_free(bev);
}
/*void Dispse::Recv_cb(int fd,short ev,void *arg)
{
  if(ev & EV_READ)
  {
    char buff[1024];
    int n=recv(fd,buff,1023,0);
    if(n<=0)
    {
      event_free(map_arr[fd]);
      map_arr[fd]=nullptr;
      close(fd);
      return;
    }
    cout<<buff<<endl;
    std::string head;
    std::string text;
    Asyn_head_text(buff,head,text);
    cout<<"_______"<<endl;
    cout<<head<<endl;
    cout<<"_________________"<<endl;
    cout<<text<<endl;
    cout<<"________________"<<endl;
    std::unordered_map<std::string,std::string>map;
    LandAnalyze(text,map);
    cout<<"**********************"<<endl;

    getLoginInfo(map);

  }
}*/

//   将处理事件添加到表中
Event::Event()
{
  base=event_base_new();  //  创建event_base 实例   （Reactor实例）
  assert(base!=nullptr);
  sock_ev=event_new(base,Socket::GetSockfd(),EV_READ | EV_PERSIST ,Dispse::Accept_cb,(void *)base);  // 创建通用的事件处理器 ，对套接字的处理
}
Event::~Event()  //    释放系统资源
{
  event_free(sock_ev);    
  event_base_free(base);
}
struct event_base* Event:: Getbase()
{
  return base;
}
struct event* Event::Getsock_ev()
{
  return sock_ev;
}
