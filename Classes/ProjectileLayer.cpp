

#include "ProjectileLayer.h"
#include "HelloWorldScene.h"
#include "ParticleLayer.h"
#include "Enemy.h"
#include "SimpleAudioEngine.h"


ProjectileLayer::ProjectileLayer(CCSprite* _hero)
{
	hero = _hero;
	score = 0;
	gameOver = false;
	
	playerBullets = new CCArray();
	
	enemies = new CCArray();
	enemiesToBeDeleted = new CCArray();

	enemyBullets = new CCArray();
	enemyBulletsToBeDeleted = new CCArray();

	visibleSize = CCDirector::sharedDirector()->getVisibleSize();    

}

ProjectileLayer::~ProjectileLayer(){}

void ProjectileLayer::update()
{

	//enemies
	if(enemies->count() >= 0 )
	{
		for(int i =0; i <enemies->count(); i++)
		{
			Enemy* e = (Enemy*)enemies->objectAtIndex(i);
			e->update();

			if(e->getPositionX() + e->getContentSize().width/2 < 0)
			{
				gameOver = true;
				enemiesToBeDeleted->addObject(e);
			}
		}	
	}

	//enemy bullets
	if(enemyBullets->count()> 0)
	{
		for(int i = 0; i < enemyBullets->count(); i ++)
		{
			Projectile* pr = (Projectile*) enemyBullets->objectAtIndex(i);
			pr->update();

			if(pr->getPositionX()<= 0 )
			{
				enemyBulletsToBeDeleted->addObject(pr);		
			}
		}	
	}

	//player bullets
	if(playerBullets->count() >= 0)
	{
		for(int i=0; i<playerBullets->count(); i++)
		{
			Projectile* p = (Projectile*)playerBullets->objectAtIndex(i);
			p->update();	

			if(p->getPositionX() >= visibleSize.width)
			{
				this->removeChild(p);
				playerBullets->removeObject(p);
			}
		}	
	}


	//** collisions and scoring **
	//player rocket and enemies collision
	if(playerBullets->count() >= 0)
	{
		for(int i=0; i<playerBullets->count(); i++)
		{
			Projectile* p = (Projectile*)playerBullets->objectAtIndex(i);

			if(enemies->count() > 0)
			{
				for(int j = 0; j< enemies->count(); j++)
				{
					Enemy* en = (Enemy*)enemies->objectAtIndex(j);

					//CCLog("player rocket and enemy");
					if(checkBoxCollision(p,en))
					{
						score++;

						CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("enemyKill.wav");
						CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("rocketExplode.wav");

						ParticleLayer* pLayer = new ParticleLayer(en->getPosition());
						this->addChild(pLayer);	

						this->removeChild(p);
						playerBullets->removeObject(p);

						enemiesToBeDeleted->addObject(en);

						//smokeParticle = CCParticleSystemQuad::create("smoke.plist");
						//smokeParticle->setPosition(en->getPosition());
						//this->addChild(smokeParticle);   
						//smokeParticle->setAutoRemoveOnFinish(true);

						
						//dustParticle = CCParticleSystemQuad::create("dusts.plist");
						//dustParticle->setPosition(en->getPosition());
						//this->addChild(dustParticle);   
						//dustParticle->setAutoRemoveOnFinish(true);
						

			

						return;
					}
				}			
			}
		}	
	}

	//enemy bullets and player
	if(enemyBullets->count() > 0)
	{
		for(int i =0; i < enemyBullets->count(); i++)
		{
			Projectile* pr = (Projectile*)enemyBullets->objectAtIndex(i);

			//CCLog("enemy bullet and player");
			if(checkBoxCollision(pr, hero))
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("playerKill.wav");
				
				enemyBulletsToBeDeleted->addObject(pr);
				gameOver = true;

				return;
			}	
		}
	}
	

	//remove objects from screen and arrays
	CCObject* ee = NULL;
	CCARRAY_FOREACH(enemiesToBeDeleted, ee)
    {
		Enemy *target = (Enemy*)(ee);
		enemies->removeObject(target);
        enemiesToBeDeleted->removeObject(target);
        this->removeChild(target, true);
    }

	CCObject* eb = NULL;
	CCARRAY_FOREACH(enemyBulletsToBeDeleted, eb)
    {
		Projectile *target = (Projectile*)(eb);
		enemyBullets->removeObject(target);
        enemyBulletsToBeDeleted->removeObject(target);
        this->removeChild(target, true);
    }
}

bool ProjectileLayer::checkBoxCollision(CCSprite* box1, CCSprite *box2)
{
	CCRect box1Rect = box1->boundingBox();
	CCRect box2Rect = box2->boundingBox();

    if (box1Rect.intersectsRect(box2Rect))
    {
        return true;
    }
    else
    {
        return false;
    }    
}