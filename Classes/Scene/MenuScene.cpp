#include "Scene/MenuScene.h"
#include "Scene/GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

Scene* MenuScene::createScene() {
	return MenuScene::create();
}

bool MenuScene::init() {
	if (!Scene::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//auto background = Sprite::create("HelloWorld.png");
	auto background = Label::createWithTTF(u8"弓箭手大作战", "fonts/fzkt.ttf", 60);
	background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + 80));
	this->addChild(background);

	//auto startButton = MenuItemImage::create("CloseNormal.png", "CloseNormal.png", CC_CALLBACK_1(MenuScene::startGameCallback, this));
	auto startButton = MenuItemImage::create(u8"开始.png", u8"开始.png", CC_CALLBACK_1(MenuScene::startGameCallback, this));
	startButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	//auto exitButton = MenuItemImage::create("CloseSelected.png", "CloseSelected.png", CC_CALLBACK_1(MenuScene::exitGameCallback, this));
	auto exitButton = MenuItemImage::create(u8"退出.png", u8"退出.png", CC_CALLBACK_1(MenuScene::exitGameCallback, this));
	exitButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));

	auto menu = Menu::create(startButton, exitButton, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void MenuScene::startGameCallback(Ref* pSender) {
	Director::getInstance()->replaceScene(GameScene::createScene());
}

void MenuScene::exitGameCallback(Ref* pSender) {
	Director::getInstance()->end();
}
