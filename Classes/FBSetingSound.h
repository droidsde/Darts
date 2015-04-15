//
//  FBSetingSound.h
//  Game
//
//  Created by apple on 15/3/12.
//
//

#ifndef __Game__FBSetingSound__
#define __Game__FBSetingSound__

#include "cocos2d.h"
#include <cocos-ext.h>
USING_NS_CC_EXT;
class FBSetingSound : public cocos2d::Layer
{
public:
/**
 *创建一个场景
 */
    static cocos2d::Scene* createScene();
    
/**
 *初始化init
 */
    virtual bool init();
/**
 *添加界面框架
 */
    void addWidgets();
/**
 *添加一个背景精灵
 */
    void addSpriteBackGround(cocos2d::Size visibleSize);
/**
 *滑块数值变化时调用该方法
 */
    void valueChanged(Ref *sender,Control::EventType controlEvent);
/**
 *按钮回调方法
 */
       void menuCloseCallback(cocos2d::Ref* pSender);
/**
 *添加触摸监听事件
 */
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    CREATE_FUNC(FBSetingSound);
};

#endif /* defined(__Game__FBSetingSound__) */
