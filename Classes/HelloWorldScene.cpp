#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    /////////////////////////////

	boxArray.clear();

	createBoxArray();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

    return true;
}

void HelloWorld::createBoxArray()
{
	for (int i = 0; i < BOX_X; i++)
	{
		for (int j = 0; j < BOX_Y; j++)
		{
			auto box = Sprite::create("box.png");
			box->setAnchorPoint(Vec2(0, 0));
			box->setPosition(Vec2(_winSize.width * 1 / 4 + box->getContentSize().width * i,
				_winSize.height * 1 / 4 + box->getContentSize().height * j));
			this->addChild(box);

			boxArray.pushBack(box);
		}
	}

}

void HelloWorld::checkTouchBox(Vec2 touchPoint)
{
	Sprite* box;
	std::vector<Sprite*>::const_iterator it = boxArray.begin();

	for (int i = 0; i < boxArray.size(); i++)
	{
		box = (Sprite*)(*it);
		CCLOG("%f %f", box->getBoundingBox().getMinX(), box->getBoundingBox().getMinY());
		if (box->getBoundingBox().containsPoint(touchPoint))
		{
			if (box->isVisible())
			{
				box->setVisible(false);
			}
			else
			{
				box->setVisible(true);
			}
			
		}
		it++;
	}
}

bool HelloWorld::onTouchBegan(Touch* touch, Event *unused_event)
{
	Vec2 touchPoint = touch->getLocation();
	checkTouchBox(touchPoint);

	return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event *unused_event)
{
	Vec2 touchPoint = touch->getLocation();
	checkTouchBox(touchPoint);
}

void HelloWorld::onTouchEnded(Touch* touch, Event *unused_event)
{
}

void HelloWorld::onTouchCancelled(Touch* touch, Event *unused_event)
{
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
