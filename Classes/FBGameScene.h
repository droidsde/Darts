//
//  FBGameScene.h
//  Game
//
//  Created by apple on 15/3/14.
//
//

#ifndef __Game__FBGameScene__
#define __Game__FBGameScene__

#include "cocos2d.h"

class FBGameScene : public cocos2d::Layer
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
    void menuCloseCallback(cocos2d::Ref* pSender);
/**
 *添加界面框架
 */
    void initGames();
/**
 *添加一个背景精灵
 */
    void addSpriteBackGround(cocos2d::Size visibleSize);
/**
 *创造标靶
 */
    void addAimRand(const int aimDigit,const int flag);
/**
 *添加Widget
 */
    void addWidgets();
/**
 *获得目标标靶数字
 */
    int getAimDigit();
/**
 *获得每个关卡的时间限制
 */
    int getCountTime(int guanQia);
/**
 *碰撞后检测是否射到正确的标靶上了
 */
    void isRightAndError(cocos2d::Sprite *sprite);
/**
 *获取计算数字组成
 */
    std::vector<int> getAimDigits();
/**
 *定时器执行函数
 */
    void updateSchedule(float delay);
    void updataScheduleDart(float delay);
    void updataCountTime(float delay);
/**
 *添加触摸监听事件
 */
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    CREATE_FUNC(FBGameScene);
private:
/**
 *人物角色
 */
    cocos2d::Sprite *hero;
/**
 *问题标签
 */
    cocos2d::Label *questionLabel;
/**
 *记录关卡
 */
    cocos2d::Label *guanQiaLabel;
    int guanQiaCount;
/**
 *计时标签
 */
    cocos2d::Label *countLabel;
    int countTime;
/**
 *标靶精灵
 */
    cocos2d::Sprite *aim01;
    cocos2d::Sprite *aim02;
    cocos2d::Sprite *aim03;
/**
 *飞镖精灵
 */
    cocos2d::Sprite *dart;
/**
 *保存标靶由数字组成的数字
 */
    std::vector<int>digitArr;
/**
 *所得分数
 */
    cocos2d::Label *scoreLabel;
    int myScore;
/**
 *标志
 */
    bool flag01;
    bool flag02;
    bool flag03;
    bool flag;
    bool flagPaues;
};

#endif /* defined(__Game__FBGameScene__) */
