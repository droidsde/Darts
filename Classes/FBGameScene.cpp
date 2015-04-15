//
//  FBGameScene.cpp
//  Game
//
//  Created by apple on 15/3/14.
//
//

#include "FBGameScene.h"

USING_NS_CC;

#define MOVE_DIS 2
#define PAUSE_BUTTON_TAG 100 //暂停按钮Tag
#define SEND_BUTTON_TAG  101 //发射按钮Tag
#define BACK_BUTTON_TAG  102 //返回主界面Tag
#define NEXT_BUTTON_TAG  103 //进入下一关Tag
#define LAYER_COLOR_TAG  104 //颜色层Tag
#define RESET_GAME_TAG    105 //重新开始游戏Tag

using namespace std;

Scene* FBGameScene::createScene()
{
    auto *scene = Scene::create();
    auto layer = FBGameScene::create();
    scene->addChild(layer);
    return scene;
}
bool FBGameScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    guanQiaCount = 1;
    this->myScore = 0;
    this->initGames();
    //添加触摸监听事件
    auto myListener = EventListenerTouchOneByOne::create();
    myListener->onTouchBegan = CC_CALLBACK_2(FBGameScene::onTouchBegan, this);
    myListener->onTouchMoved = CC_CALLBACK_2(FBGameScene::onTouchMoved, this);
    myListener->onTouchEnded = CC_CALLBACK_2(FBGameScene::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(myListener, this);
    return true;
}
#pragma mark - 添加一个背景精灵
void FBGameScene::addSpriteBackGround(Size visibleSize)
{
    //创建一个精灵背景
    auto spriteBackGround = Sprite::create("游戏界面背景.png");
    //    spriteBackGround->setTextureRect(Rect(0, 0, 50, 50));//设置图片缩放区域
    auto widthSpriteBackGround = spriteBackGround->getContentSize().width;
    auto heightSpriteBackGround = spriteBackGround->getContentSize().height;
    spriteBackGround->setScaleX(visibleSize.width/widthSpriteBackGround);
    spriteBackGround->setScaleY(visibleSize.height/heightSpriteBackGround);
    spriteBackGround->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    //添加精灵背景到层中
    this->addChild(spriteBackGround, 1);
}
#pragma mark - 添加Widgets
void FBGameScene::addWidgets()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    this->addSpriteBackGround(visibleSize);
    
    srand((int)time(0));
    
    //添加3个标靶
    for (int i = 0; i <3; i++)
    {
        int digit = rand()%10;
        if (digit == 0) {
            digit++;
        }
        this->addAimRand(digit, i);
    }
    
    //添加角色
    hero = Sprite::create("小忍者.png");
    hero->setPosition(Vec2(60.0f,200.0f));
    this->addChild(hero, 2);
    
    //添加暂停按钮
    auto pauseMenu = MenuItemImage::create("pause_normal.png", "pause_selected.png",CC_CALLBACK_1(FBGameScene::menuCloseCallback, this));
    pauseMenu->setScale(0.4);
    pauseMenu->setTag(PAUSE_BUTTON_TAG);
    pauseMenu->setPosition(pauseMenu->getBoundingBox().size.width/2.0f+10.0f, visibleSize.height - pauseMenu->getBoundingBox().size.height/2.0f);
    
    //添加发射按钮
    auto sendItem = MenuItemImage::create(
                                          "射击.png",
                                          "射击.png",
                                          CC_CALLBACK_1(FBGameScene::menuCloseCallback, this));
    sendItem->setTag(SEND_BUTTON_TAG);
    sendItem->setPosition(Vec2(visibleSize.width/2,sendItem->getContentSize().height/2 + 10));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(pauseMenu,sendItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);
    
    //添加算术Label
    this->getAimDigits();
    string  tempStr = StringUtils::format("%d + %d = ?",digitArr[0],digitArr[1]);
    questionLabel = Label::createWithSystemFont(tempStr, "arial", 50);
    questionLabel->setColor(Color3B::BLACK);
    questionLabel->setPosition(Vec2(visibleSize.width - questionLabel->getContentSize().width/2, visibleSize.height - questionLabel->getContentSize().height/2));
    this->addChild(questionLabel,2);
    
    //添加时间计时Label
    countTime = this->getCountTime(guanQiaCount);
    tempStr = StringUtils::format("时间剩余: %d",countTime);
    countLabel = Label::createWithSystemFont(tempStr, "arial", 30);
    countLabel->setColor(Color3B::BLACK);
    countLabel->setPosition(pauseMenu->getPositionX() + pauseMenu->getContentSize().width,visibleSize.height - countLabel->getContentSize().height/2);
    this->addChild(countLabel, 2);
    //添加关卡label
    tempStr = StringUtils::format("关卡: %d",guanQiaCount);
    guanQiaLabel = Label::createWithSystemFont(tempStr, "arial", 30);
    guanQiaLabel->setColor(Color3B::BLACK);
    guanQiaLabel->setPosition(visibleSize.width/2, visibleSize.height - countLabel->getContentSize().height/2);
    this->addChild(guanQiaLabel,2);
    
    //增加游戏得分
    tempStr = StringUtils::format("得分:%d",myScore);
    scoreLabel = Label::createWithSystemFont(tempStr, "arial", 30);
    scoreLabel->setColor(Color3B::BLACK);
    scoreLabel->setPosition(sendItem->getPositionX() + sendItem->getContentSize().width, sendItem->getContentSize().height/2);
    this->addChild(scoreLabel, 2);
}

#pragma mark - 获得每个关卡的时间限制
int FBGameScene::getCountTime(int guanQia)
{
    switch (guanQia) {
        case 1:
        {
            return 10;
        }
            break;
        case 2:
        {
            return 9;
        }
            break;
        case 3:
        {
            return 8;
        }
            break;
        case 4:
        {
            return 7;
        }
            break;
        case 5:
        {
            return 6;
        }
            break;
            
        default:
        {
            return 3;
        }
            break;
    }
}
#pragma mark -初始化游戏
void FBGameScene::initGames()
{
    this->flag01 = false;
    this->flag02 = false;
    this->flag03 = false;
    this->flag = false;
    this->flagPaues = false;
    
    
    this->addWidgets();
    //添加一个计划
    this->schedule(schedule_selector(FBGameScene::updateSchedule), 0.01);
    this->schedule(schedule_selector(FBGameScene::updataCountTime), 1);
}
#pragma mark - 定时器执行方法
void FBGameScene::updateSchedule(float delay)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //目标aim01的移动逻辑
    float tempAim01 = this->aim01->getPositionY() + this->aim01->getContentSize().height/2;
    if (tempAim01 < visibleSize.height && !flag01)
    {
        this->aim01->setPositionY(this->aim01->getPositionY() + MOVE_DIS);
        float temp = this->aim01->getPositionY() + this->aim01->getContentSize().height/2;
        if(temp > visibleSize.height)
        {
            flag01 = true;
        }
    }
    else
    {
        this->aim01->setPositionY(this->aim01->getPositionY() - MOVE_DIS);
        float tempAim01 = this->aim01->getPositionY() - this->aim01->getContentSize().height/2;
        if (tempAim01 < 0.0) {
            flag01 = false;
        }
    }
    //目标aim02的移动逻辑
    float tempAim02 = this->aim02->getPositionY() + this->aim02->getContentSize().height/2;
    if (tempAim02 < visibleSize.height && !flag02)
    {
        this->aim02->setPositionY(this->aim02->getPositionY() + MOVE_DIS);
        float temp = this->aim02->getPositionY() + this->aim02->getContentSize().height/2;
        if(temp > visibleSize.height)
        {
            flag02 = true;
        }
    }
    else
    {
        this->aim02->setPositionY(this->aim02->getPositionY() - MOVE_DIS);
        float temp = this->aim02->getPositionY() - this->aim02->getContentSize().height/2;
        if (temp < 0.0) {
            flag02 = false;
        }
    }
    //目标aim03的移动逻辑
    float tempAim03 = this->aim03->getPositionY() + this->aim03->getContentSize().height/2;
    if (tempAim03 < visibleSize.height && !flag03)
    {
        this->aim03->setPositionY(this->aim03->getPositionY() + MOVE_DIS);
        float temp = this->aim03->getPositionY() + this->aim03->getContentSize().height/2;
        if(temp > visibleSize.height)
        {
            flag03 = true;
        }
    }
    else
    {
        this->aim03->setPositionY(this->aim03->getPositionY() - MOVE_DIS);
        float temp = this->aim03->getPositionY() - this->aim03->getContentSize().height/2;
        if (temp < 0.0) {
            flag03 = false;
        }
    }

    
}
#pragma mark - 计时器计划方法
void FBGameScene::updataCountTime(float delay)
{
    --countTime;
    string tempStr = StringUtils::format("时间剩余: %d",countTime);
    countLabel->setString(tempStr);
   
    if(!countTime)
    {
        log("失败");
        Size visibleSize = Director::getInstance()->getVisibleSize();
        auto defeatLayer = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
        defeatLayer->setTag(LAYER_COLOR_TAG);
        this->addChild(defeatLayer, 3);
        
        auto back = Sprite::create("back_fail.jpg");
        back->setScaleX(visibleSize.width/back->getContentSize().width);
        back->setScaleY(visibleSize.height/back->getContentSize().height);
        back->setPosition(visibleSize.width/2,visibleSize.height/2);
        defeatLayer->addChild(back, 3);
        
        auto tempStr = StringUtils::format("得分:  %d",myScore);
        auto  tempScoreLabel = Label::createWithSystemFont(tempStr, "arial", 70);
        tempScoreLabel->setColor(Color3B::RED);
        tempScoreLabel->setPosition(tempScoreLabel->getContentSize().width/2, visibleSize.height - tempScoreLabel->getContentSize().height/2);
        defeatLayer->addChild(tempScoreLabel, 3);
        
        auto fail = Sprite::create("fail.png");
        fail->setPosition(visibleSize.width/2, visibleSize.height - 80);
        defeatLayer->addChild(fail, 3);
        
        auto backMenu = MenuItemImage::create("返回主界面.png", "返回主界面.png",CC_CALLBACK_1(FBGameScene::menuCloseCallback, this));
        backMenu->setTag(BACK_BUTTON_TAG);
        backMenu->setPosition(visibleSize.width/2- 200,visibleSize.height/2);
        auto resetMenu = MenuItemImage::create("重新开始.png", "重新开始.png",CC_CALLBACK_1(FBGameScene::menuCloseCallback, this));
        resetMenu->setPosition(visibleSize.width/2 + 200,visibleSize.height/2);
        resetMenu->setTag(RESET_GAME_TAG);
        auto menu = Menu::create(backMenu,resetMenu, NULL);
        menu->setPosition(Vec2::ZERO);
        defeatLayer->addChild(menu, 3);
         this->unscheduleAllCallbacks();
        myScore = 0;
    }
}
#pragma mark - 增加标靶
void FBGameScene::addAimRand(const int aimDigit,const int flag)
{
    //获取屏幕大小
    Size visibleSize = Director::getInstance()->getVisibleSize();
    switch (flag) {
        case 0:
            //标靶aim01
        {
            aim01 = Sprite::create("小标靶.png");
            aim01->setScale(2);
            aim01->setPosition(Vec2(visibleSize.width - 70, visibleSize.height/2 + 150));
            aim01->setTag(aimDigit);
            this->addChild(aim01,2);
            auto aimLabel01 = Label::createWithCharMap("digit.png", 38, 50, '0');
            aimLabel01->setString(StringUtils::format("%d",aimDigit));
            aimLabel01->setScale(0.5);
            aimLabel01->setPosition(Vec2(aim01->getContentSize().width/2, aim01->getContentSize().height/2));
            aim01->addChild(aimLabel01,1);

        } break;
        case 1:
            //标靶aim02
        {
            aim02 = Sprite::create("小标靶.png");
            aim02->setScale(2);
            aim02->setPosition(Vec2(visibleSize.width - 200, visibleSize.height/2));
            aim02->setTag(aimDigit);
            this->addChild(aim02,2);
            auto aimLabel02 = Label::createWithCharMap("digit.png", 38, 50, '0');
            aimLabel02->setString(StringUtils::format("%d",aimDigit));
            aimLabel02->setScale(0.5);
            aimLabel02->setPosition(Vec2(aim01->getContentSize().width/2, aim01->getContentSize().height/2));
            aim02->addChild(aimLabel02,1);

        }
            break;
        case 2:
            //标靶aim03
        {
            aim03 = Sprite::create("小标靶.png");
            aim03->setScale(2);
            aim03->setPosition(Vec2(visibleSize.width - 70, visibleSize.height/2 - 150));
            aim03->setTag(aimDigit);
            this->addChild(aim03,2);
            auto aimLabel03 = Label::createWithCharMap("digit.png", 38, 50, '0');
            aimLabel03->setString(StringUtils::format("%d",aimDigit));
            aimLabel03->setScale(0.5);
            aimLabel03->setPosition(Vec2(aim01->getContentSize().width/2, aim01->getContentSize().height/2));
            aim03->addChild(aimLabel03,1);

        }
            break;
        default:
            break;
    }
}
#pragma mark - 获得目标标靶数字
int FBGameScene::getAimDigit()
{
    srand((int)time(0));
    int tag = rand()%3;
    switch (tag) {
        case 0:
            //标靶01的数字
        {
            return aim01->getTag();
        }
            break;
        case 1:
            //标靶01的数字
        {
            return aim02->getTag();
        }
            break;
        case 2:
            //标靶01的数字
        {
            return aim03->getTag();
        }
            break;
        default:
        {
            return aim01->getTag();
        }
            break;
    }
    return aim01->getTag();
}
#pragma mark - 获得目标标靶由？+？=？
vector<int> FBGameScene::getAimDigits()
{
    digitArr.clear();
    srand((int)time(0));
    int tempDigit = this->getAimDigit();
    int tempData = tempDigit - rand() % tempDigit;
    digitArr.push_back(tempData);
    digitArr.push_back(tempDigit - tempData);
    return this->digitArr;
}
#pragma mark - 触摸开始
bool FBGameScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    log("触摸开始");
    return true;
}
#pragma mark - 触摸移动
void FBGameScene::onTouchMoved(Touch *touch, Event *unused_event)
{
    log("触摸移动");
    this->hero->setPositionY(touch->getLocation().y);
}
#pragma mark - 触摸结束
void FBGameScene::onTouchEnded(Touch *touch, Event *unused_event)
{
//    Director::getInstance()->popScene();
    this->hero->setPositionY(touch->getLocation().y);
}
#pragma mark - 触摸取消
void FBGameScene::onTouchCancelled(Touch *touch, Event *unused_event)
{
    log("触摸取消");
}
#pragma mark - 发射按钮回调方法
void FBGameScene::menuCloseCallback(Ref* pSender)
{
    auto tag = ((Node*)pSender)->getTag();
    if (tag == SEND_BUTTON_TAG) {
        if (!flag)
        {
            log("进入发射按钮回调方法");
            flag = true;
            dart = Sprite::create("小飞镖.png");
            dart->setScale(2);
            dart->setPosition(hero->getPosition());
            this->addChild(dart,2);
            this->schedule(schedule_selector(FBGameScene::updataScheduleDart), 0.01);
        }
    }
    else if (tag == PAUSE_BUTTON_TAG)
    {
        //暂停游戏
        if (!flagPaues) {
            Director::getInstance()->pause();
            flagPaues = true;
        }
        else
        {
            Director::getInstance()->resume();
            flagPaues = false;
        }
    }
    else if (tag == BACK_BUTTON_TAG)
    {
        //返回主界面
        Director::getInstance()->popScene();
    }
    else if(tag == NEXT_BUTTON_TAG)
    {
        //进入下一关
        guanQiaCount++;
        this->removeAllChildrenWithCleanup(true);
//        this->getChildByTag(LAYER_COLOR_TAG)->removeFromParent();
        this->initGames();
    }
    else if (tag == RESET_GAME_TAG)
    {
        guanQiaCount = 1;
        this->removeAllChildrenWithCleanup(true);
//        this->getChildByTag(LAYER_COLOR_TAG)->removeFromParent();
        this->initGames();
    }
}
#pragma mark - 飞镖定时移动方法
void FBGameScene::updataScheduleDart(float delay)
{
    dart->setPositionX(dart->getPositionX()+10);
    if(dart->getBoundingBox().intersectsRect(aim01->getBoundingBox()))
    {
        this->isRightAndError(aim01);
    }
    else if (dart->getBoundingBox().intersectsRect(aim02->getBoundingBox()))
    {
        this->isRightAndError(aim02);

    }
    else if (dart->getBoundingBox().intersectsRect(aim03->getBoundingBox()))
    {
        this->isRightAndError(aim03);
    }
    else
    {
        if (dart->getPositionX() > Director::getInstance()->getVisibleSize().width)
        {
            flag = false;
            dart->removeFromParent();
            this->unschedule(schedule_selector(FBGameScene::updataScheduleDart));
        }

    }
}
#pragma mark - 碰撞后检测是否射到正确的标靶上了
void FBGameScene::isRightAndError(cocos2d::Sprite *sprite)
{
    //获取屏幕大小
    this->unschedule(schedule_selector(FBGameScene::updataScheduleDart));
    this->unschedule(schedule_selector(FBGameScene::updateSchedule));
    this->unschedule(schedule_selector(FBGameScene::updataCountTime));
    Size visibleSize = Director::getInstance()->getVisibleSize();
    if (sprite->getTag() == digitArr[0] + digitArr[1])
    {
        //胜利， 返回主界面 进入下一关
        log("胜利");
        this->myScore += 10;
        auto victoryLayer = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
        victoryLayer->setTag(LAYER_COLOR_TAG);
        this->addChild(victoryLayer, 3);
        
        auto back = Sprite::create("back_fail.jpg");
        back->setScaleX(visibleSize.width/back->getContentSize().width);
        back->setScaleY(visibleSize.height/back->getContentSize().height);
        back->setPosition(visibleSize.width/2,visibleSize.height/2);
        victoryLayer->addChild(back, 3);
        
        auto tempStr = StringUtils::format("得分:%d",myScore);
        auto  tempScoreLabel = Label::createWithSystemFont(tempStr, "arial", 70);
        tempScoreLabel->setColor(Color3B::RED);
        tempScoreLabel->setPosition(tempScoreLabel->getContentSize().width/2, visibleSize.height - tempScoreLabel->getContentSize().height/2);
        victoryLayer->addChild(tempScoreLabel, 3);
        
        auto success = Sprite::create("success.png");
        success->setPosition(visibleSize.width/2, visibleSize.height - 80);
        victoryLayer->addChild(success, 3);
        
        auto backMenu = MenuItemImage::create("返回主界面.png", "返回主界面.png",CC_CALLBACK_1(FBGameScene::menuCloseCallback, this));
        backMenu->setTag(BACK_BUTTON_TAG);
        backMenu->setPosition(visibleSize.width/2- 200,visibleSize.height/2);
        auto nextMenu = MenuItemImage::create("下一关.png", "下一关.png",CC_CALLBACK_1(FBGameScene::menuCloseCallback, this));
        nextMenu->setPosition(visibleSize.width/2 + 200,visibleSize.height/2);
        nextMenu->setTag(NEXT_BUTTON_TAG);
        auto menu = Menu::create(backMenu,nextMenu, NULL);
        menu->setPosition(Vec2::ZERO);
        victoryLayer->addChild(menu, 3);

    }
    else
    {
        //失败 返回主界面   重新开始游戏
        log("失败");
        auto defeatLayer = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
        defeatLayer->setTag(LAYER_COLOR_TAG);
        this->addChild(defeatLayer, 3);
        
        auto back = Sprite::create("back_fail.jpg");
        back->setScaleX(visibleSize.width/back->getContentSize().width);
        back->setScaleY(visibleSize.height/back->getContentSize().height);
        back->setPosition(visibleSize.width/2,visibleSize.height/2);
        defeatLayer->addChild(back, 3);
        
        auto tempStr = StringUtils::format("得分:%d",myScore);
        auto  tempScoreLabel = Label::createWithSystemFont(tempStr, "arial", 70);
        tempScoreLabel->setColor(Color3B::RED);
        tempScoreLabel->setPosition(tempScoreLabel->getContentSize().width/2, visibleSize.height - tempScoreLabel->getContentSize().height/2);
        defeatLayer->addChild(tempScoreLabel, 3);
        
        auto fail = Sprite::create("fail.png");
        fail->setPosition(visibleSize.width/2, visibleSize.height - 80);
        defeatLayer->addChild(fail, 3);
        
        auto backMenu = MenuItemImage::create("返回主界面.png", "返回主界面.png",CC_CALLBACK_1(FBGameScene::menuCloseCallback, this));
        backMenu->setTag(BACK_BUTTON_TAG);
        backMenu->setPosition(visibleSize.width/2- 200,visibleSize.height/2);
        auto resetMenu = MenuItemImage::create("重新开始.png", "重新开始.png",CC_CALLBACK_1(FBGameScene::menuCloseCallback, this));
        resetMenu->setPosition(visibleSize.width/2 + 200,visibleSize.height/2);
        resetMenu->setTag(RESET_GAME_TAG);
        auto menu = Menu::create(backMenu,resetMenu, NULL);
        menu->setPosition(Vec2::ZERO);
        defeatLayer->addChild(menu, 3);
        myScore = 0;
    }
}

