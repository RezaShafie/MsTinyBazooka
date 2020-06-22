#include "MainMenuScene.h"
#include "HelloWorldScene.h"
#include "OptionsMenuScene.h"
#include <spine/spine-cocos2dx.h>
#include "SimpleAudioEngine.h"


CCScene* MainMenu::scene()
{
    CCScene *scene = CCScene::create();
	MainMenu *layer = MainMenu::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init()
{
    
	//check if background music needs to be played
	bool isPaused = CCUserDefault::sharedUserDefault()->getBoolForKey("tinyBazooka_kSoundPausedKey");

	if(isPaused == true)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();	
	}
	
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	scrollingBgLayer = new ScrollingBgLayer(3.0);
	this->addChild(scrollingBgLayer);


	CCLabelBMFont *nameLabel = CCLabelBMFont::create("Ms.tinyBazooka","PixelFont.fnt");
	//nameLabel->setColor(ccYELLOW);
	nameLabel->setPosition(visibleSize.width/2, visibleSize.height * 0.8);
	this->addChild(nameLabel);


	//actions
	CCFiniteTimeAction* actionMove =CCMoveTo::create( 1, CCPoint(nameLabel->getPosition().x,nameLabel->getPosition().y + 20));
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( nameLabel, callfuncN_selector(MainMenu::MoveUpFinished));
    CCFiniteTimeAction *easeInOut = CCEaseSineInOut::create((CCActionInterval*)actionMove);
    nameLabel->runAction(CCSequence::create(easeInOut, actionMoveDone, NULL));

	extension::CCSkeletonAnimation* skeletonNode = extension::CCSkeletonAnimation::createWithFile("player.json", "player.atlas", 1.0f);
	skeletonNode->addAnimation("runCycle",true,0,0);
	skeletonNode->setPosition(ccp(visibleSize.width * .125 , visibleSize.height * 0.2 - skeletonNode->getContentSize().height/2));
	addChild(skeletonNode);

	
	CCMenuItemImage *pPlayItem = CCMenuItemImage::create("_bookgame_UI_play.png",
														 "_bookgame_UI_play.png",
														  this,
														  menu_selector(MainMenu::playGame));
	pPlayItem->setPosition(ccp(visibleSize.width/2,  visibleSize.height * 0.5));
	
	
	CCMenuItemImage *pOptionsItem = CCMenuItemImage::create("_bookgame_UI_options.png",
															"_bookgame_UI_options.png",
															this,
															menu_selector(MainMenu::optionsScene));
	pOptionsItem->setPosition(ccp(visibleSize.width * 0.75, visibleSize.height * 0.5 ));



    // create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pOptionsItem,pPlayItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 10);


	 CCLabelBMFont* newHighScoreLabel = CCLabelBMFont::create("CURRENT HIGH SCORE", "PixelFont.fnt");
     newHighScoreLabel->setPosition(ccp(visibleSize.width * 0.5, visibleSize.height * 0.15));
     this->addChild(newHighScoreLabel, 10);
     newHighScoreLabel->setScale(0.5);
        

	CCLabelBMFont* highScoreLabel = CCLabelBMFont::create("0", "PixelFont.fnt");
    highScoreLabel->setPosition(ccp(visibleSize.width * 0.5, visibleSize.height * 0.1));
    this->addChild(highScoreLabel, 10);
    highScoreLabel->setScale(0.5);        
        
	int highScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("bazookaGameHighScore");
    char scoreTxt[100];
    sprintf(scoreTxt, "%d", highScore);
    highScoreLabel->setString(scoreTxt);


	this->scheduleUpdate();

	return true;

}


void MainMenu::update(float dt)
{
	scrollingBgLayer->update();
}


void MainMenu:: playGame(CCObject* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pop.wav");
	
	CCScene *mScene = HelloWorld::scene();
    CCDirector::sharedDirector()->replaceScene(mScene);
}


void MainMenu::optionsScene(CCObject* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pop.wav");
	
	CCScene *mScene = OptionsMenu::scene();
    CCDirector::sharedDirector()->replaceScene(mScene);
}

  

void MainMenu::MoveDownFinished(CCNode* sender)
{
    //CCLOG("move down fin");
    CCSprite *sprite = (CCSprite *)sender;
    CCFiniteTimeAction* actionMove =CCMoveTo::create( 1.0, CCPoint(sprite->getPosition().x, sprite->getPosition().y + 20.0) );
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( sprite, callfuncN_selector(MainMenu::MoveUpFinished));
    CCFiniteTimeAction *easeInOut = CCEaseSineInOut::create((CCActionInterval*)actionMove);
    sprite->runAction(CCSequence::create(easeInOut, actionMoveDone, NULL) );
}
void MainMenu::MoveUpFinished(CCNode* sender)
{
    //CCLOG("move up fin");
    CCSprite *sprite = (CCSprite *)sender;
    CCFiniteTimeAction* actionMove =CCMoveTo::create( 1.0, CCPoint(sprite->getPosition().x, sprite->getPosition().y - 20.0) );
	CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( sprite, callfuncN_selector(MainMenu::MoveDownFinished));
    CCFiniteTimeAction *easeInOut = CCEaseSineInOut::create((CCActionInterval*)actionMove);
    sprite->runAction(CCSequence::create(easeInOut, actionMoveDone, NULL) );
}