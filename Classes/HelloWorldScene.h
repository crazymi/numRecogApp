#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#define BOX_X 3
#define BOX_Y 5

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
private:
	Size _winSize = Director::getInstance()->getWinSize();
	int _state;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

	
	Vector<Sprite*> boxArray;

	void createBoxArray();
	void checkTouchBox(Vec2 touchPoint);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
	
	bool onTouchBegan(Touch* touch, Event *unused_event);
	void onTouchMoved(Touch* touch, Event *unused_event);
	void onTouchEnded(Touch* touch, Event *unused_event);
	void onTouchCancelled(Touch* touch, Event *unused_event);    
	
};

#endif // __HELLOWORLD_SCENE_H__
