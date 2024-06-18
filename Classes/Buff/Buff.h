#ifndef _BUFF_H
#define _BUFF_H

#include "cocos2d.h"
#include <random>

using namespace std;
using namespace cocos2d;

class Buff {
public:
	Buff() {
		//默认按钮
		path = "CloseNormal.png";
	}
	virtual void addBuff() = 0;

	//buff说明
	string tips;
	//buff图标路径
	string path;
};


#endif
