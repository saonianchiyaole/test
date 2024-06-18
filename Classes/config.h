#ifndef _CONFIG_H
#define _CONFIG_H

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <random>
#include <unordered_set>
#include "Manager/TimeManager.h"

namespace fs = std::filesystem;

using namespace std;
using namespace cocos2d;
using namespace cocostudio;
using namespace cocos2d::ui;
using namespace CocosDenshion;

#pragma region 调试模块
//控制台输出调试，调用方法: _lg(这里填需要输出的内容即可)
inline void _lg(string s) {
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	printf((s + "\n").c_str());
}
inline void _lg(int n) {
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	printf((std::to_string(n) + "\n").c_str());
}
inline void _lg(unsigned n) {
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	printf((std::to_string(n) + "\n").c_str());
}
inline void _lg(int m, int n) {
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	printf((std::to_string(m) + " " + std::to_string(n) + "\n").c_str());
}
inline void _lg(float n) {
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	printf((std::to_string(n) + "\n").c_str());
}
inline void _lg(float m, float n) {
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
	printf((std::to_string(m) + " " + std::to_string(n) + "\n").c_str());
}
inline void _lg(Vec2 v) {
	_lg(v.x, v.y);
}
inline void _lg(Size sz) {
	_lg(sz.width, sz.height);
}
#pragma endregion

//帧率
const unsigned FPS = 60u;
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size bigResolutionSize = cocos2d::Size(1500, 900);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

//大屏/小屏
//static cocos2d::Size designResolutionSize = largeResolutionSize;
static cocos2d::Size designResolutionSize = bigResolutionSize;

//路径
	//动画
const string PATH_OF_ARMATURE_OF_PLAYER = "PlayerAnimation/Hero.ExportJson";
const string PATH_OF_ARMATURE_OF_ENEMY = "EnemyAnimation/tauren.ExportJson";
//血条
const string PATH_OF_BLOOD_BAR_EMPTY = "bloodBarEmpty.png";
const string PATH_OF_BLOOD_BAR = "bloodBar.png";
//经验条
const string PATH_OF_EXP_BAR = "expBar.png";
const string PATH_OF_EXP_BAR_EMPTY = "bloodBarEmpty.png";

//PlayerѪ���;������Ŵ����
const float PLAYER_BAR_ENLARGE_RATE = 2.5f;



//LAYER (当每种物体有决定好的LAYER时，直接用这里的即可)
const int ATTACK_LAYER = 5;
const int ENEMY_LAYER = 5;
const int GAME_SCENE_TIMER_LAYER = 10;
const int GAME_SCENE_WAVE_NUM_LAYER = 10;
const int GAME_SCENE_EXP_BAR_LAYER = 9;

//TAG
const int PLAYER_MOVE_TAG = 110;
//const int TAG_OF_BUFF_PLAYER = 111;
const int GAME_SCENE_TIMER_TAG = 113;
//const int TAG_OF_DESCRIPTION_OF_BUFF_PLAYER = 112;
const int GAME_SCENE_WAVE_NUM_TAG = 114;

const int BLOOD_BAR_TAG = 115;
const int BLOOD_BAR_EMPTY_TAG = 116;

const int GAME_SCENE_EXP_BAR_TAG = 117;
const int GAME_SCENE_EXP_BAR_EMPTY_TAG = 118;

//选项1的TAG
const int TAG_OF_BUFF_PLAYER = 111;
//选项1描述的TAG
const int TAG_OF_DESCRIPTION_OF_BUFF_PLAYER = 112;
//选项2的TAG
const int TAG_OF_BUFF_WEAPON = 121;
//选项2描述的TAG
const int TAG_OF_DESCRIPTION_OF_BUFF_WEAPON = 122;

//一个工具方法，它返回文件夹folderPath下的一个随机文件名，主要用于播放随机音效
inline string getRandomFile(const std::string& folderPath) {
	std::vector<std::string> files;
	// 遍历文件夹，并将文件名存储到向量中
	for (const auto& entry : fs::directory_iterator(folderPath)) {
		if (fs::is_regular_file(entry.path())) {
			files.push_back(entry.path().string());
		}
	}

	// 检查文件夹是否为空
	if (files.empty()) {
		std::cerr << "Error: Folder is empty." << std::endl;
		return "";
	}

	// 生成随机数引擎
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, files.size() - 1);

	// 从文件列表中随机选择一个文件
	int randomIndex = dis(gen);
	return files[randomIndex];
}



#include <string>
#include <locale>
#include <codecvt>

inline std::string to_utf8(const std::wstring& wstr) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
	return converter.to_bytes(wstr);
}


#endif