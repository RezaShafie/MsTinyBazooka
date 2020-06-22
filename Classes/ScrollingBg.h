
#ifndef __wp8Game__ScrollingBg__
#define __wp8Game__ScrollingBg__

#include <iostream>
#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

class ScrollingBg: public CCNode
{
public:
    
    static ScrollingBg* create(string name, float _speed, float _yPos);
    bool initScrollingBg(string _name, float _speed, float _yPos);
    
    CCSprite* gameBg1, *gameBg2;
    
    float speed;
    string name;
    
    CCSize winSize;
    
    void update();
    
};

#endif /* defined(__endlessFlyer__ScrollingBg__) */
