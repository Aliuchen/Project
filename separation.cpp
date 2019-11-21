/*===============================================================
*   Copyright (C) 2019 All rights reserved.
*   
*   文件名称：separation.cpp
*   创 建 者：@liuchen
*   创建日期：2019年11月21日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include"ser.h"
void Asyn_head_text(std::string s,std::string &head, std::string &text)
{
	int pos1 = 0;
	int pos2 = 0;
	pos2 = s.find("\r\n\r\n", pos1);
	head = s.substr(pos1, pos2-pos1+1);
	pos1 = pos2+2;
	text = s.substr(pos1, s.size()-pos1);
}

