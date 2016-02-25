#include "HelloWorldScene.h"
#include "NeuralNetworkAlgorithm.h"
#include "ui\UIEditBox\UIEditBox.h"

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

	//
	_origin = Director::getInstance()->getVisibleOrigin();
	_winSize = Director::getInstance()->getWinSize();

    // initialize variables
	_state = -1;
	_flag = 1;
	_flagArray = (int*)malloc(sizeof(int)*BOX_X*BOX_Y);
	memset(_flagArray, 0, sizeof(int)*BOX_X*BOX_Y);
	_boxArray.clear();

	// initialize neuron
	_myNeuron = new NeuralNetworkAlgorithm();
	_myNeuron->init_weight();
	_myNeuron->basicLearn();

	// backgound
	auto background = DrawNode::create();
	background->drawSolidRect(_origin, _winSize, Color4F(0.6, 0.6, 0.6, 1.0));
	this->addChild(background);
	
	//
	_answerLabel = Label::createWithSystemFont("", "arial", 34);
	_answerLabel->setPosition(_winSize.width / 2, 25);
	this->addChild(_answerLabel);	

	createBoxArray();

	// 
	_editBox = ui::EditBox::create(Size(60, 60), ui::Scale9Sprite::create("box.png"));
	_editBox->setPosition(Vec2(_winSize.width / 2, _winSize.height / 5));
	_editBox->setMaxLength(1);
	_editBox->setFont("", 50);
	_editBox->setAnchorPoint(Vec2(0, 0));
	_editBox->setFontColor(Color3B::RED);
	_editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	_editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
	this->addChild(_editBox);

	_numButton = Sprite::create("CloseNormal.png");
	_numButton->setScale(3.0f);
	_numButton->setAnchorPoint(Vec2(0, 0));
	_numButton->setPosition(_editBox->getPosition() + Vec2(_editBox->getContentSize().width,0));
	this->addChild(_numButton);
	
	//
	_okButton = Sprite::create("CloseNormal.png");
	_okButton->setScale(3.0f);
	_okButton->setAnchorPoint(Vec2(1, 0));
	_okButton->setPosition(Vec2(_origin.x + _winSize.width - _okButton->getContentSize().width, _origin.y));
	this->addChild(_okButton);

	_resetButton = Sprite::create("CloseSelected.png");
	_resetButton->setScale(3.0f);
	_resetButton->setAnchorPoint(Vec2(0, 0));
	_resetButton->setPosition(Vec2(_origin.x, _origin.y));
	this->addChild(_resetButton);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	this->setKeypadEnabled(true);

    return true;
}

void HelloWorld::createBoxArray()
{
	for (int j = 0; j < BOX_Y; j++)
	{
		for (int i = 0; i < BOX_X; i++)
		{
			auto box = Sprite::create("box.png");
			box->setAnchorPoint(Vec2(0, 1));
			box->setPosition(Vec2(_origin.x + box->getContentSize().width * i,
				_winSize.height - box->getContentSize().height * j + _origin.y));
			box->setOpacity(40);
			this->addChild(box);

			_boxArray.pushBack(box);
		}
	}
}

void HelloWorld::checkTouchBox(Vec2 touchPoint, int _flag)
{
	Sprite* box;
	std::vector<Sprite*>::const_iterator it = _boxArray.begin();

	for (int i = 0; i < _boxArray.size(); i++)
	{
		box = (Sprite*)(*it);
		if (box->getBoundingBox().containsPoint(touchPoint))
		{
			if (_flag == 1 || _flag == 0 && _state != i)
			{
				if (_flagArray[i] == 0)
				{
					box->setOpacity(255);
					_flagArray[i] = 1;
				}
				else
				{
					box->setOpacity(40);
					_flagArray[i] = 0;
				}
				_state = i;
			}
			break;
		}
		it++;
	}
}

void HelloWorld::resetBoxArray()
{
	std::vector<Sprite*>::const_iterator it = _boxArray.begin();
	Sprite* box;
	for (int i = 0; i < _boxArray.size(); i++)
	{
		box = (Sprite*)(*it);
		if (_flagArray[i] != 0)
		{
			box->setOpacity(40);
			_flagArray[i] = 0;
		}
		it++;
	}
	_answerLabel->setString("");
}

int HelloWorld::okButtonCallBack()
{
	int ans =  _myNeuron->getNumber(_flagArray);
	std::ostringstream ost;
	ost << ans;
	std::string s(ost.str());
	_answerLabel->setString(s);
	return ans;
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keycode, Event* unused_event)
{
	if (keycode == EventKeyboard::KeyCode::KEY_BACK)
	{
		Director::getInstance()->end();
	}
}

bool HelloWorld::onTouchBegan(Touch* touch, Event *unused_event)
{
	Vec2 touchPoint = touch->getLocation();

	_flag = 1;

	if (_okButton->getBoundingBox().containsPoint(touchPoint))
	{
		okButtonCallBack();
	}
	else if (_resetButton->getBoundingBox().containsPoint(touchPoint))
	{
		resetBoxArray();
	}
	else if (_numButton->getBoundingBox().containsPoint(touchPoint))
	{
		if (_editBox->getText() != "")
		{
			_myNeuron->getWeight(_flagArray, atoi(_editBox->getText()));
			_answerLabel->setString("Successfully learn");
		}
	}
	else
	{
		checkTouchBox(touchPoint, _flag);
	}
	return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event *unused_event)
{
	Vec2 touchPoint = touch->getLocation();
	_flag = 0;
	checkTouchBox(touchPoint, _flag);
}

void HelloWorld::onTouchEnded(Touch* touch, Event *unused_event)
{
}

void HelloWorld::onTouchCancelled(Touch* touch, Event *unused_event)
{
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
	CCLOG("menuCloseCallback");
    Director::getInstance()->end();

	delete _myNeuron;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
