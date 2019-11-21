#include<iostream>
#include<string>
#include<vector>
using namespace std;


void Asyn_head_text(string s,string &head, string &text)
{
	int pos1 = 0;
	int pos2 = 0;
	pos2 = s.find("\r\n\r\n", pos1);
	head = s.substr(pos1, pos2-pos1+1);
	pos1 = pos2+2;
	text = s.substr(pos1, s.size()-pos1);
}
