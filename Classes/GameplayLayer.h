

#ifndef __wp8Game__GameplayLayer__
#define __wp8Game__GameplayLayer__

#include "cocos2d.h"
using namespace cocos2d;


#include "Projectile.h"

class GameplayLayer: public CCLayer
{
public:
    GameplayLayer(CCSprite* _hero);
    ~GameplayLayer();
	void update(); 

	int score;
	bool gameOver;

	CCArray* getEnemiesArray();
	CCArray* getPlayerBulletsArray();
	CCArray* getEnemyBulletsArray();

private:
	CCSprite* hero;	
	CCSize visibleSize;  

	CCArray* playerBullets;
	CCArray* enemies;
	CCArray* enemiesToBeDeleted;
	CCArray* enemyBullets;
	CCArray* enemyBulletsToBeDeleted;     

	bool checkBoxCollision(CCSprite* box1, CCSprite *box2);
};

#endif /* defined(__wp8Game__GameplayLayer__) */
