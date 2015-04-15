//
//  FBMainGameScene.h
//  Game
//
//  Created by apple on 15/3/12.
//
//

#ifndef __Game__FBMainGameScene__
#define __Game__FBMainGameScene__

#include "cocos2d.h"

class FBMainGameScene : public cocos2d::Layer
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
 *菜单的回调方法
 */
    void menuCloseCallback(cocos2d::Ref* pSender);//退出菜单回调函数
    void menuStartCallback(cocos2d::Ref* pSender);//开始菜单回调函数
    void menuSetingCallback(cocos2d::Ref* pSender);//设置菜单回调函数
    CREATE_FUNC(FBMainGameScene);
private:
/**
 * 添加一个背景精灵
 */
    void addSpriteBackGround(cocos2d::Size visibleSize);
/**
 * 添加主菜单
 */
    void addMainMenus(cocos2d::Size visibleSize);
};


#endif /* defined(__Game__FBMainGameScene__) */
