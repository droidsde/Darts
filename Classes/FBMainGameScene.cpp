//
//  FBMainGameScene.cpp
//  Game
//
//  Created by apple on 15/3/12.
//
//

#include "FBMainGameScene.h"
#include "FBSetingSound.h"
#include "FBGameScene.h"
USING_NS_CC;

Scene* FBMainGameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = FBMainGameScene::create();
    scene->addChild(layer);
    return scene;
}

bool FBMainGameScene::init()
{
    //初始化Layer层
    if (!Layer::init())
    {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    log("屏幕大小为:width:%f,height:%f",visibleSize.width,visibleSize.height);
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //添加背景图
    this->addSpriteBackGround(visibleSize);
    //添加菜单
    this->addMainMenus(visibleSize);
    return true;
}
//添加一个背景精灵
void FBMainGameScene::addSpriteBackGround(Size visibleSize)
{
    //创建一个精灵背景
    auto spriteBackGround = Sprite::create("back.jpg");
    //    spriteBackGround->setTextureRect(Rect(0, 0, 50, 50));//设置图片缩放区域
    auto widthSpriteBackGround = spriteBackGround->getContentSize().width;
    auto heightSpriteBackGround = spriteBackGround->getContentSize().height;
    spriteBackGround->setScaleX(visibleSize.width/widthSpriteBackGround);
    spriteBackGround->setScaleY(visibleSize.height/heightSpriteBackGround);
    spriteBackGround->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    //添加精灵背景到层中
    this->addChild(spriteBackGround, 1);
}
//添加主菜单
void FBMainGameScene::addMainMenus(Size visibleSize)
{
    auto startMenu = MenuItemImage::create("start_menu.png","start_menu.png",CC_CALLBACK_1(FBMainGameScene::menuStartCallback, this));
    startMenu->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2 - 50));
    auto systemSetingMenu = MenuItemImage::create("seting_menu.png","seting_menu.png",CC_CALLBACK_1(FBMainGameScene::menuSetingCallback,this));
    systemSetingMenu->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2 - 120));
    auto exitMenu = MenuItemImage::create("exit_menu.png", "exit_menu.png", CC_CALLBACK_1(FBMainGameScene::menuCloseCallback, this));
    exitMenu->setPosition(Vec2(visibleSize.width/2,visibleSize.height/2 - 190));
    auto mainMenu = Menu::create(startMenu,systemSetingMenu,exitMenu, NULL);
    mainMenu->setPosition(Vec2::ZERO);
    this->addChild(mainMenu, 2);
}
void FBMainGameScene::menuStartCallback(Ref* pSender)
{
    //开始菜单
    auto gameScene = FBGameScene::createScene();
    Director::getInstance()->pushScene(gameScene);
}
void FBMainGameScene::menuSetingCallback(Ref* pSender)
{
    //设置菜单
    auto soundScene = FBSetingSound::createScene();
    Director::getInstance()->pushScene(soundScene);
}
void FBMainGameScene::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
