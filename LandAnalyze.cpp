#include"ser.h"
void LandAnalyze(string &src, unordered_map<string, string> &mMap){
  //以\r\n分割
  
  int pos1=0;
  int pos2=0;
	const string temp = "\r\n";
  vector<string>vec;
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
  vector<string> vec2;
  int num1=0;
  int num2=0;
  const string temp2=":";
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

