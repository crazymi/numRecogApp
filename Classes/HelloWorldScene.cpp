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
	_flagArray = (int*)malloc(sizeof(int)*BOX_X*BOX_Y);
	memset(_flagArray, 0, sizeof(int)*BOX_X*BOX_Y);
	_boxArray.clear();

	// initialize neuron
	_myNeuron = new NeuralNetworkAlgorithm();
	_myNeuron->init_weight();
	_myNeuron->basicLearn();

	// backgound
	CCLOG("background");
	auto background = DrawNode::create();
	background->drawSolidRect(_origin, _winSize, Color4F(0.6, 0.6, 0.6, 1.0));
	this->addChild(background);
	
	//
	CCLOG("answerlabel");
	_answerLabel = Label::createWithSystemFont("", "arial", 34);
	_answerLabel->setPosition(_winSize.width / 2, 15);
	this->addChild(_answerLabel);
	
	// 
	
	CCLOG("editbox");
	_editBox = ui::EditBox::create(Size(60, 60), ui::Scale9Sprite::create("box.png"));
	_editBox->setPosition(Vec2(_winSize.width / 2, _winSize.height / 5));
	_editBox->setMaxLength(1);
	_editBox->setFont("", 50);
	_editBox->setFontColor(Color3B::RED);
	_editBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	_editBox->setInputMode(ui::EditBox::InputMode::NUMERIC);
	_editBox->setDelegate(this);
	this->addChild(_editBox);
	

	CCLOG("boxarray");
	createBoxArray();

	CCLOG("okbutton");
	_okButton = Sprite::create("CloseNormal.png");
	_okButton->setScale(3.0f);
	_okButton->setAnchorPoint(Vec2(1, 0));
	_okButton->setPosition(Vec2(_origin.x + _winSize.width - _okButton->getContentSize().width, _origin.y));
	this->addChild(_okButton);

	CCLOG("resetbutton");
	_resetButton = Sprite::create("CloseSelected.png");
	_resetButton->setScale(3.0f);
	_resetButton->setAnchorPoint(Vec2(0, 0));
	_resetButton->setPosition(Vec2(_origin.x, _origin.y));
	this->addChild(_resetButton);

	CCLOG("listener");
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	//listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	this->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

    return true;
}

void HelloWorld::createBoxArray()
{
	CCLOG("createBoxArray function START");
	for (int j = 0; j < BOX_Y; j++)
	{
		for (int i = 0; i < BOX_X; i++)
		{
			auto box = Sprite::create("box.png");
			box->setAnchorPoint(Vec2(0, 1));
			box->setPosition(Vec2(_origin.x + box->getContentSize().width * i,
				_winSize.height - box->getContentSize().height * j + _origin.y));
			box->setOpacity(255);
			this->addChild(box);

			_boxArray.pushBack(box);
		}
	}
	CCLOG("createBoxArray function END");
}

void HelloWorld::checkTouchBox(Vec2 touchPoint)
{
	CCLOG("checkTouchBox function START");
	Sprite* box;
	std::vector<Sprite*>::const_iterator it = _boxArray.begin();

	for (int i = 0; i < _boxArray.size(); i++)
	{
		box = (Sprite*)(*it);
		if (box->getBoundingBox().containsPoint(touchPoint) && _state != i)
		{
			if (_flagArray[i] == 0)
			{
				box->setOpacity(40);
				//box->setTexture("box2.png");//Director::getInstance()->getTextureCache()->addImage("box2.png"));
				_flagArray[i] = 1;
			}
			else
			{
				box->setOpacity(255);
				//box->setTexture("box.png");//Director::getInstance()->getTextureCache()->addImage("box.png"));
				//box->setTexture(TextureCache::sharedTextureCache()->addImage("box.png"));
				_flagArray[i] = 0;
			}
			break;
		}
		it++;
	}
	CCLOG("checkTouchBox function END");
}

void HelloWorld::resetBoxArray()
{
	CCLOG("resetBoxArray function START");
	std::vector<Sprite*>::const_iterator it = _boxArray.begin();
	Sprite* box;
	for (int i = 0; i < _boxArray.size(); i++)
	{
		box = (Sprite*)(*it);
		if (_flagArray[i] != 0)
		{
			box->setOpacity(255);
			//box->setTexture("box.png"); //Director::getInstance()->getTextureCache()->addImage("box.png"));
			_flagArray[i] = 0;
		}
		it++;
	}
	_answerLabel->setString("");
	CCLOG("resetBoxArray function END");
}

int HelloWorld::okButtonCallBack()
{
	CCLOG("--- okButtonCallBack function START ---");
	int ans =  _myNeuron->getNumber(_flagArray);
	std::ostringstream ost;
	ost << ans;
	std::string s(ost.str());
	_answerLabel->setString(s);
	CCLOG("--- okButtonCallBack function END ---");
	return ans;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event *unused_event)
{
	Vec2 touchPoint = touch->getLocation();
	if (_okButton->getBoundingBox().containsPoint(touchPoint))
	{
		okButtonCallBack();
	}
	else if (_resetButton->getBoundingBox().containsPoint(touchPoint))
	{
		resetBoxArray();
	}
	else
	{
		checkTouchBox(touchPoint);
	}
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

void HelloWorld::editBoxReturn(ui::EditBox* editBox)
{
	CCLOG("--- editBoxReturn ---");
}

void HelloWorld::editBoxEditingDidEnd(ui::EditBox* editBox)
{
	CCLOG("--- editBoxEditingDidEnd ---");
	if (editBox->getText() != "")
	{
		_myNeuron->getWeight(_flagArray, atoi(editBox->getText()));
		_answerLabel->setString("Successfully learn");
	}
}

void HelloWorld::editBoxEditingDidBegin(ui::EditBox* editBox)
{
	CCLOG("--- editBoxEditingDidBegin ---");
}

void HelloWorld::editBoxTextChanged(ui::EditBox* editBox, const std::string& text)
{
	CCLOG("--- editBoxTextChanged ---");
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
