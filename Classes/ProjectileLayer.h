#ifndef __wp8Game__ProjectileLayer__
#define __wp8Game__ProjectileLayer__

#include "cocos2d.h"
using namespace cocos2d;


#include "Projectile.h"

class ProjectileLayer: public CCLayer
{
public:
	ProjectileLayer(CCSprite* _hero);
    ~ProjectileLayer();

	
	CCSprite* hero;

	int score;
	bool gameOver;

	CCSize visibleSize;
  
	CCArray* playerBullets;

	CCArray* enemies;
	CCArray* enemiesToBeDeleted;

	CCArray* enemyBullets;
	CCArray* enemyBulletsToBeDeleted;

	CCParticleSystemQuad* smokeParticle;
	CCParticleSystemQuad* dustParticle;

    void update();  
	bool checkBoxCollision(CCSprite* box1, CCSprite *box2);

};

#endif /* defined(__wp8Game__ProjectileLayer__) */
