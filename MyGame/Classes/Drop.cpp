//
//  Drop.cpp
//  MyGame
//
//  Created by 小林 千夏 on 5/20/14.
//
//

#include "Drop.h"

using namespace cocos2d;

Drop::Drop(int dropType) {
    
    this->dropType = dropType;
    
    String* fileName = String::createWithFormat("%d.png",dropType);
    this->dropSprite = Sprite::create(fileName->getCString());
    this->dropSprite->setScale(0.5f);
}


Drop* Drop::create(int dropType) {
    Drop *sprite = new Drop(dropType);
    
    //get char info from the ID
//    std::string charImg = cocos2d::StringUtils::format("%04d.png", dropType);
//    
//    if (sprite && sprite->initWithFile(charImg))
//    {
//        sprite->autorelease();
//        return sprite;
//    }
//    CC_SAFE_DELETE(sprite);
    return sprite;
}

int Drop::getDropType()
{
    return this->dropType;
}

Sprite* Drop::getSprite()
{
    return this->dropSprite;
}

void Drop::setLocation(Point loc)
{
    this->dropSprite->setPosition(loc);
    this->point = loc;
}

Point Drop::getLocation()
{
    return this->point;
}

void Drop::setGameCoordinates(int x, int y) {
    this->loc.setCoordinates(x, y);
}

GameCoordinates Drop::getGameCoordinates() {
    return this->loc;
}
