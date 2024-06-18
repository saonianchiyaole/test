#include "Scene/GameOverScene.h"
#include "Scene/GameScene.h"
#include "Scene/MenuScene.h"
#include "Weapon/Bow.h"
#include "Weapon/Sword.h"
USING_NS_CC;

Scene* GameOverScene::createScene() {
	return GameOverScene::create();
}

bool GameOverScene::init() {
	if (!Scene::init()) {
		return false;
	}

	GameScene::setInstanceToNullptr();
	WaveManager::setInstanceToNullptr();
	Bow::setInstanceToNullptr();
	Sword::setInstanceToNullptr();

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Add game over background and labels
	//auto background = Sprite::create("HelloWorld.png");
	auto background = Label::createWithTTF(u8"游戏结束", "fonts/fzkt.ttf", 60);
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 80));
	this->addChild(background);

	// Replay button
	auto replayButton = MenuItemImage::create(u8"重新开始.png", u8"重新开始.png", CC_CALLBACK_0(GameOverScene::restartGame, this));
	replayButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));

	// Menu button
	auto menuButton = MenuItemImage::create(u8"回到主菜单.png", u8"回到主菜单.png", CC_CALLBACK_0(GameOverScene::gotoMenu, this));
	menuButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 200));

	auto menu = Menu::create(replayButton, menuButton, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	return true;
}

void GameOverScene::restartGame() {
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, GameScene::createScene())); // Replace with your actual game scene
}

void GameOverScene::gotoMenu() {
	Director::getInstance()->replaceScene(TransitionFade::create(1.0, MenuScene::createScene())); // Replace with your actual menu scene
}
