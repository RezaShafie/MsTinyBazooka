#ifndef __wp8Game__HELLOWORLD_SCENE_H__
#define __wp8Game__HELLOWORLD_SCENE_H__

#include "cocos2d.h"
using namespace cocos2d;

#include "ProjectileLayer.h"
#include "HUDLayer.h"
#include "ScrollingBgLayer.h"
#include <spine/spine-cocos2dx.h>

typedef enum ActionState 
{
	kActionStateNone = 0,
	kActionStateIdle ,
	kActionStateBoost
};

typedef enum PlayerState 
{
	kPlayerStateNone = 0,
	kPLayerStateIdle,
	kPlayerStateBoost
};

class HelloWorld : public cocos2d::CCLayer
{
public:

	HelloWorld();
	~HelloWorld();

	//variables

	

	CCSize visibleSize;
	CCSprite* hero;

	CCRect leftButton, rightButton;
	int jumpTimer;
	bool jump;
	CCPoint gravity;

	ActionState mActionState; 
	PlayerState mPlayerState;
	
	CCAction* mIdleAction;
	CCAction* mBoostAction;

	ProjectileLayer* projectileLayer;
	HUDLayer* hudLayer;
	ScrollingBgLayer* scrollingBgLayer;

	CCParticleSystemQuad* flameParticle;

	//functions
	virtual void update(float dt);
	void spawnEnemy(float dt);
	void fireRocket();
	void GameOver();
	void gamePaused();
	void gameResumed();
	void mainMenuScene(CCObject* pSender);

	void idleAnim();
	void boostAnim();
	void AnimationStates();

	virtual void ccTouchesBegan(CCSet* pTouches, CCEvent* event);
	virtual void ccTouchesMoved(CCSet* pTouches, CCEvent* event);
	virtual void ccTouchesEnded(CCSet* pTouches, CCEvent* event);

    virtual bool init();  
    static cocos2d::CCScene* scene();
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
