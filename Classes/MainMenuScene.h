#ifndef __wp8Game__MAINMENU_SCENE__
#define __wp8Game__MAINMENU_SCENE__

#include "cocos2d.h"
#include "ScrollingBgLayer.h"

using namespace cocos2d;

class MainMenu : public cocos2d::CCLayer
{
public:
    
	virtual bool init();  

	ScrollingBgLayer* scrollingBgLayer;

	void MoveDownFinished(CCNode* sender);
    void MoveUpFinished(CCNode* sender);

	void optionsScene(CCObject* pSender);
	void playGame(CCObject* pSender);
	static cocos2d::CCScene* scene();
	void update(float dt);
	CREATE_FUNC(MainMenu);
};

#endif 
