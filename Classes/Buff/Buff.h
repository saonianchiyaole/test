#ifndef _BUFF_H
#define _BUFF_H

#include "cocos2d.h"
#include <random>

using namespace std;
using namespace cocos2d;

class Buff {
public:
	Buff() {
		//Ĭ�ϰ�ť
		path = "CloseNormal.png";
	}
	virtual void addBuff() = 0;

	//buff˵��
	string tips;
	//buffͼ��·��
	string path;
};


#endif
