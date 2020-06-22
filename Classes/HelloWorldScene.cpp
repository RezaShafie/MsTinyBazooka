#include "HelloWorldScene.h"
#include "Enemy.h"
#include "Projectile.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"


CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

HelloWorld::HelloWorld()
{}

HelloWorld::~HelloWorld()
{}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }    

	gravity = ccp(0, -5);
	jump = false;
	jumpTimer = 0;

	visibleSize = CCDirector::sharedDirector()->getVisibleSize();    
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	scrollingBgLayer = new ScrollingBgLayer(3.0);
	this->addChild(scrollingBgLayer);

	hero = CCSprite::create("bookGame_tinyBazooka.png");
	hero->setPosition(ccp(visibleSize.width * 0.125, visibleSize.height * 0.5));
	addChild(hero, 5);

   //player animation       
    CCSpriteBatchNode* spritebatch = CCSpriteBatchNode::create("player_anim.png");
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("player_anim.plist");
    
    hero->createWithSpriteFrameName("player_idle_1.png");
    hero->addChild(spritebatch);    
       
	//idle animation
	CCArray* animFrames = CCArray::createWithCapacity(4);    
    char str1[100] = {0};
    for(int i = 1; i <= 4; i++)
    {
        sprintf(str1, "player_idle_%d.png", i);
        CCSpriteFrame* frame = cache->spriteFrameByName( str1 );
        animFrames->addObject(frame);
    }
        
    CCAnimation* idleanimation = CCAnimation::createWithSpriteFrames(animFrames, 0.25f);
    mIdleAction = CCRepeatForever::create(CCAnimate::create(idleanimation)) ;
	mIdleAction->retain();

	//boost animation
	animFrames->removeAllObjects();
	char str2[100] = {0};
    for(int i = 1; i <= 4; i++)
    {
        sprintf(str2, "player_boost_%d.png", i);
        CCSpriteFrame* frame = cache->spriteFrameByName( str2 );
        animFrames->addObject(frame);
    }
        
    CCAnimation* boostanimation = CCAnimation::createWithSpriteFrames(animFrames, 0.25f);
	hero->runAction( CCRepeatForever::create( CCAnimate::create(boostanimation) ) );
	
	mBoostAction = CCRepeatForever::create(CCAnimate::create(boostanimation)) ;
	mBoostAction->retain();
	
	leftButton = CCRectMake(0, 0,visibleSize.width/2, visibleSize.height);
	rightButton = CCRectMake(visibleSize.width/2, 0, visibleSize.width/2, visibleSize.height);

	projectileLayer = new ProjectileLayer(hero);
	this->addChild(projectileLayer);	

	hudLayer = new HUDLayer();
	this->addChild(hudLayer, 15); //keeping at top most layer


	//flameParticle = CCParticleSystemQuad::create("jetBoost.plist");
	//flameParticle->setPosition(ccpAdd(hero->getPosition(), ccp(-hero->getContentSize().width * 0.25, 0)));
	//this->addChild(flameParticle);   


	this->setTouchEnabled(true);

	this->scheduleUpdate();    
	this->schedule(schedule_selector(HelloWorld::spawnEnemy),3.0);

    return true;
}

void HelloWorld::idleAnim()
{
	if (mActionState != kActionStateIdle)
	{
		hero->stopAllActions();
		hero->runAction(mIdleAction);
		mActionState = kActionStateIdle;
	}
}

void HelloWorld::boostAnim()
{
	if (mActionState != kActionStateBoost)
	{
		hero->stopAllActions();
		hero->runAction(mBoostAction);
		mActionState = kActionStateBoost;
	}
}

void HelloWorld::AnimationStates()
{
	//CCLOG("action state");

	switch(mPlayerState)
	{
		case kPLayerStateIdle: 
			this->idleAnim(); break;
		case kPlayerStateBoost: 
			this->boostAnim(); break;
		default: break;	
	}
}


void HelloWorld::update(float dt)
{

	if(!projectileLayer->gameOver)
	{
		this->AnimationStates();

		scrollingBgLayer->update();
		projectileLayer->update();
		hudLayer->updateScore(projectileLayer->score);

		//flameParticle->setPosition(ccpAdd(hero->getPosition(), ccp(-hero->getContentSize().width * 0.25, 0)));

		float maxY = visibleSize.height - hero->getContentSize().height/2;
		float minY = hero->getContentSize().height/2;
 
		float newY = hero->getPosition().y ;
		newY = MIN(MAX(newY, minY), maxY);
		hero->setPosition(ccp(hero->getPosition().x, newY));
	
		if(jump)
		{
			jumpTimer = 10;
			jump = false;

		}

		if(jumpTimer>0)
		{
			//CCLog("boost");

			mPlayerState = kPlayerStateBoost;
			jumpTimer--;
			CCPoint p = hero->getPosition();
			CCPoint mP = ccpAdd(p,ccp(0,7));
			hero->setPosition(mP);
		}
		else
		{
			//CCLog("idle");
			
			mPlayerState = kPLayerStateIdle;
			jumpTimer = 0;
			CCPoint p = hero->getPosition();
			CCPoint pM = ccpAdd(p,gravity);
			hero->setPosition(pM);
		}

	}
	else
	{
		GameOver();
	}

}//update 

void HelloWorld::GameOver()
{
	//CCLog("Game Over!!!!");

		this->unscheduleAllSelectors();

		if(projectileLayer->enemies->count() >0)
		{
			for(int i=0; i< projectileLayer->enemies->count(); i++)
			{
				Enemy* en = (Enemy*)projectileLayer->enemies->objectAtIndex(i);
				en->unscheduleAllSelectors();
			}
		}

	CCMenuItemImage*mainmenuItem = CCMenuItemImage::create("_bookgame_UI_mainmenu.png",
														 "_bookgame_UI_mainmenu.png", this,
														 menu_selector(HelloWorld::mainMenuScene));
    
	mainmenuItem->setPosition(ccp(visibleSize.width/2,
								visibleSize.height * 0.2));
	
	
	CCMenu *mainMenu = CCMenu::create(mainmenuItem, NULL);
	mainMenu->setPosition(CCPointZero);
	this->addChild(mainMenu);



		CCLabelBMFont* gameOverLabel = CCLabelBMFont::create("GAMEOVER", "PixelFont.fnt");
		gameOverLabel->setPosition(ccp(visibleSize.width * 0.5, visibleSize.height * 0.6));
		this->addChild(gameOverLabel, 10);

		int highScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("bazookaGameHighScore");

	if(projectileLayer->score > highScore)
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("bazookaGameHighScore", projectileLayer->score);
        CCUserDefault::sharedUserDefault()->flush();
        
        CCLabelBMFont* newHighScoreLabel = CCLabelBMFont::create("NEW HIGH SCORE", "PixelFont.fnt");
        newHighScoreLabel->setPosition(ccp(visibleSize.width * 0.5, visibleSize.height * 0.5));
        this->addChild(newHighScoreLabel, 10);
        newHighScoreLabel->setScale(0.75);
        
        CCLabelBMFont* GOscoreLabel = CCLabelBMFont::create("0", "PixelFont.fnt");
        GOscoreLabel->setPosition(ccp(visibleSize.width * 0.5, visibleSize.height * 0.4));
        this->addChild(GOscoreLabel, 10);
        GOscoreLabel->setScale(0.75);        
        
        char scoreTxt[100];
        sprintf(scoreTxt, "%d", projectileLayer->score);
        GOscoreLabel->setString(scoreTxt);
        
    }
    else
    {
        CCLabelBMFont* newHighScoreLabel = CCLabelBMFont::create("BETTER LUCK NEXT TIME", "PixelFont.fnt");
        newHighScoreLabel->setPosition(ccp(visibleSize.width * 0.5, visibleSize.height * 0.5));
        this->addChild(newHighScoreLabel, 10);
        newHighScoreLabel->setScale(0.75);    
    } 

}

void HelloWorld::spawnEnemy(float dt)
{
	//CCLog("spawn enemy");

	Enemy* e = Enemy::createEnemy(projectileLayer);
	projectileLayer->addChild(e);
	e->shoot(0.016);

	projectileLayer->enemies->addObject(e);
}

void HelloWorld::fireRocket()
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("fireRocket.wav");
	
	CCPoint p = hero->getPosition();

	p.x = p.x + hero->getContentSize().width/2;
	p.y = p.y - hero->getContentSize().height * 0.05;

	Projectile* rocket = Projectile::createProjectile(p,2);
	projectileLayer->addChild(rocket);
	projectileLayer->playerBullets->addObject(rocket);

	//CCParticleFire: Point particle system. Uses Gravity mode.
	//CCParticleFireworks: Point particle system. Uses Gravity mode.
	//CCParticleSun: Point particle system. Uses Gravity mode.
	//CCParticleGalaxy: Point particle system. Uses Gravity mode.
	//CCParticleFlower: Point particle system. Uses Gravity mode.
	//CCParticleMeteor: Point particle system. Uses Gravity mode.
	//CCParticleSpiral: Point particle system. Uses Gravity mode.
	//CCParticleExplosion: Point particle system. Uses Gravity mode.
	//CCParticleSmoke: Point particle system. Uses Gravity mode.
	//CCParticleSnow: Point particle system. Uses Gravity mode.
	//CCParticleRain: Point particle system. Uses Gravity mode.

	CCParticleSystemQuad* m_emitter = new CCParticleSystemQuad();
	m_emitter = CCParticleExplosion::create();
	m_emitter->setPosition(ccpAdd(hero->getPosition(),
								  ccp(hero->getContentSize().width/2 ,0 )));
	
	m_emitter->setStartColor(ccc4f(1.0, 1.0, 1.0, 1.0));
	m_emitter->setEndColor(ccc4f(0.0, 0.0, 0.0, 0.0));
	
	m_emitter->setTotalParticles(10);
	m_emitter->setLife(0.25);
	m_emitter->setSpeed(2.0);
	m_emitter->setSpeedVar(50.0);

	//** gravity
	//m_emitter->setEmitterMode(kCCParticleModeGravity);
	//m_emitter->setGravity(ccp(0,90));
	
	//** mode radius
	//m_emitter->setEmitterMode(kCCParticleModeRadius);
	//m_emitter->setStartRadius(0);
	//m_emitter->setStartRadiusVar(50);
	//m_emitter->setRotatePerSecond(2);
	//m_emitter->setRotatePerSecondVar(5);

	this->addChild(m_emitter, 0);

	m_emitter->setAutoRemoveOnFinish(true);



}

void HelloWorld::gamePaused()
{
	this->unscheduleUpdate();
	this->unschedule(schedule_selector(HelloWorld::spawnEnemy));

	if(projectileLayer->enemies->count() >0)
	{
		for(int i=0; i< projectileLayer->enemies->count(); i++)
		{
			Enemy* en = (Enemy*)projectileLayer->enemies->objectAtIndex(i);
			en->pauseSchedulerAndActions();
		}
	}
}

void HelloWorld::gameResumed()
{
	this->scheduleUpdate();
	this->schedule(schedule_selector(HelloWorld::spawnEnemy),3.0);

	if(projectileLayer->enemies->count() >0)
	{
		for(int i=0; i< projectileLayer->enemies->count(); i++)
		{
			Enemy* en = (Enemy*)projectileLayer->enemies->objectAtIndex(i);
			en->resumeSchedulerAndActions();
		}
	}	
}

void HelloWorld::mainMenuScene(CCObject* pSender)
{
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pop.wav");
	
	
	CCScene *mScene = MainMenu::scene();
	CCDirector::sharedDirector()->replaceScene(mScene);
}



# pragma - mark Touches
void HelloWorld::ccTouchesBegan(CCSet* pTouches, CCEvent* event)
{
	//CCLog("TouchesBegan");

    CCTouch *touch =(CCTouch*)pTouches->anyObject();

	CCPoint location = touch->getLocationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);

	//CCLog("location: xpos:%f , ypos:%f", location.x, location.y);

	if(!projectileLayer->gameOver)
	{
		if(leftButton.containsPoint(location))
			jump = true;
		if(rightButton.containsPoint(location))
			fireRocket();	
	}
}

void HelloWorld::ccTouchesMoved(CCSet* pTouches, CCEvent* event)
{
    CCTouch *touch =(CCTouch*)pTouches->anyObject();
}

void HelloWorld::ccTouchesEnded(CCSet* pTouches, CCEvent* event)
{
	//CCLog("TouchesEnded");    
}