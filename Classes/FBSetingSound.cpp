//
//  FBSetingSound.cpp
//  Game
//
//  Created by apple on 15/3/12.
//
//

#include "FBSetingSound.h"
#include <SimpleAudioEngine.h>

USING_NS_CC;

Scene* FBSetingSound::createScene()
{
    auto *scene = Scene::create();
    auto layer = FBSetingSound::create();
    scene->addChild(layer);
    return scene;
}

bool FBSetingSound::init()
{
    if (!Layer::init()) {
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    this->addSpriteBackGround(visibleSize);
    this->addWidgets();
    //添加触摸监听事件
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->onTouchBegan = CC_CALLBACK_2(FBSetingSound::onTouchBegan, this);
    myListener->onTouchMoved = CC_CALLBACK_2(FBSetingSound::onTouchMoved, this);
    myListener->onTouchEnded = CC_CALLBACK_2(FBSetingSound::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(myListener, this);
    return true;
}
//添加一个背景精灵
void FBSetingSound::addSpriteBackGround(Size visibleSize)
{
    //创建一个精灵背景
    auto spriteBackGround = Sprite::create("back_sound.jpg");
    //    spriteBackGround->setTextureRect(Rect(0, 0, 50, 50));//设置图片缩放区域
    auto widthSpriteBackGround = spriteBackGround->getContentSize().width;
    auto heightSpriteBackGround = spriteBackGround->getContentSize().height;
    spriteBackGround->setScaleX(visibleSize.width/widthSpriteBackGround);
    spriteBackGround->setScaleY(visibleSize.height/heightSpriteBackGround);
    spriteBackGround->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    //添加精灵背景到层中
    this->addChild(spriteBackGround, 1);
}
void FBSetingSound::addWidgets()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //增加一个标签
    auto label = Label::createWithSystemFont("设置声音", "arial", 50.0f);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2+100.0f));
    this->addChild(label,2);
    auto sliderLabel = Label::createWithSystemFont("音量大小:", "arial", 35.0f);
    sliderLabel->setPosition(Vec2(visibleSize.width/2.0f, visibleSize.height/2.0f));
    sliderLabel->setColor(Color3B::BLACK);
    this->addChild(sliderLabel,2);
    //增加滑块
    ControlSlider *slider = ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    slider->setAnchorPoint(Vec2(0.5f, 1.0f));
    slider->setMinimumValue(0.0f); // Sets the min value of range
    slider->setMaximumValue(100.0f); // Sets the max value of range
    slider->setValue(CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume());
    slider->setPosition(visibleSize.width / 2.0f, sliderLabel->getPosition().y - sliderLabel->getContentSize().height);
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(FBSetingSound::valueChanged), Control::EventType::VALUE_CHANGED);
    slider->setTag(1);
    
    
    auto restrictLabel = Label::createWithSystemFont("音效大小:", "arial", 35.0f);
    restrictLabel->setColor(Color3B::BLACK);
    restrictLabel->setPosition(Vec2(visibleSize.width/2.0f, slider->getPosition().y - slider->getContentSize().height - 30.0f));
    this->addChild(restrictLabel,2);

    
    ControlSlider *restrictSlider = ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    restrictSlider->setAnchorPoint(Vec2(0.5f, 1.0f));
    restrictSlider->setMinimumValue(0.0f); // Sets the min value of range
    restrictSlider->setMaximumValue(100.0f); // Sets the max value of range
    restrictSlider->setValue(CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume());
    restrictSlider->setPosition(visibleSize.width / 2.0f, restrictLabel->getPosition().y - restrictLabel->getContentSize().height);
    restrictSlider->setTag(2);
    //same with restricted
    restrictSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(FBSetingSound::valueChanged), Control::EventType::VALUE_CHANGED);
    
    this->addChild(slider,2);
    this->addChild(restrictSlider,2);
    
    auto backMenu = MenuItemImage::create("返回主界面.png", "返回主界面.png", CC_CALLBACK_1(FBSetingSound::menuCloseCallback, this));
    backMenu->setPosition(Vec2(origin.x + visibleSize.width - backMenu->getContentSize().width/2 ,
                                origin.y + backMenu->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(backMenu, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);

}
void FBSetingSound::valueChanged(Ref *sender, Control::EventType controlEvent)
{
    ControlSlider* slider = (ControlSlider*)sender;
    if (slider->getTag() == 1)
    {
         CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(slider->getValue());
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(slider->getValue());
    }
}

bool FBSetingSound::onTouchBegan(Touch *touch, Event *unused_event)
{
    log("触摸开始");
    return true;
}
void FBSetingSound::onTouchMoved(Touch *touch, Event *unused_event)
{
    log("触摸移动");
}
void FBSetingSound::onTouchEnded(Touch *touch, Event *unused_event)
{
//    Director::getInstance()->popScene();
    log("触摸结束");
}
void FBSetingSound::onTouchCancelled(Touch *touch, Event *unused_event)
{
    log("触摸取消");
}

void FBSetingSound::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->popScene();
}

