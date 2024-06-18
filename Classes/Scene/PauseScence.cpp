#include "Scene/PauseScene.h"
#include "Scene/GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

Scene* PauseScene::createScene() {
	return PauseScene::create();
}

bool PauseScene::init() {
	if (!Scene::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	// Add a dark layer to achieve the darkening effect
	auto darkLayer = LayerColor::create(Color4B(0, 0, 0, 150)); // Create a semi-transparent black layer
	darkLayer->setContentSize(visibleSize); // Set the size to cover the whole screen
	darkLayer->setPosition(Vec2::ZERO); // Set position to bottom-left corner
	this->addChild(darkLayer, -1); // Add the dark layer to the scene with z-order -1


	// Toggle volume button
	auto volumeButton = MenuItemImage::create("CloseNormal.png", "CloseNormal.png", CC_CALLBACK_0(PauseScene::toggleVolume, this));
	volumeButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	// Return to game button
	auto returnButton = MenuItemImage::create(u8"·µ»Ø.png", u8"·µ»Ø.png", CC_CALLBACK_0(PauseScene::returnToGame, this));
	returnButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));

	auto menu = Menu::create(volumeButton, returnButton, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);

	// ¼üÅÌÊÂ¼þ¼àÌýÆ÷
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
		if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
			Director::getInstance()->popScene();
		}
		};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	return true;
}

void PauseScene::toggleVolume() {
	auto audioEngine = CocosDenshion::SimpleAudioEngine::getInstance();

	static bool isVolumeOn = true;

	if (isVolumeOn) {
		// Turn off the volume
		audioEngine->pauseBackgroundMusic();
		// Change button image to mute button
		auto volumeButton = dynamic_cast<MenuItemImage*>(this->getChildByName("volumeButton"));
		if (volumeButton) {
			volumeButton->setNormalImage(Sprite::create("CloseSelected.png"));
			volumeButton->setSelectedImage(Sprite::create("CloseSelected.png"));
		}
		isVolumeOn = false;
	}
	else {
		// Turn on the volume
		audioEngine->resumeBackgroundMusic();
		// Change button image to volume button
		auto volumeButton = dynamic_cast<MenuItemImage*>(this->getChildByName("volumeButton"));
		if (volumeButton) {
			volumeButton->setNormalImage(Sprite::create("CloseNormal.png"));
			volumeButton->setSelectedImage(Sprite::create("CloseNormal.png"));
		}
		isVolumeOn = true;
	}
}


void PauseScene::returnToGame() {
	Director::getInstance()->popScene();
}
