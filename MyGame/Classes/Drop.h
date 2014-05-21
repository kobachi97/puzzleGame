//
//  Drop.h
//  MyGame
//
//  Created by 小林 千夏 on 5/20/14.
//
//

#ifndef __MyGame__Drop__
#define __MyGame__Drop__

#include "cocos2d.h"
#include "GameBoardZIndex.h"
#include "GameCoordinates.h"
//#include "Location.h"

class Drop : public cocos2d::Sprite {
public:
    Drop();
//    GameCharacter(int charId);
    Drop(int dropType);
    static Drop* create(int dropType);
//    virtual ~Drop();
    GameCoordinates getGameCoordinates();
    void setGameCoordinates(int x, int y);
    int getDropType();
//    std::vector<GameCoordinates> getEffectiveLocation(GameCoordinates currentLocation);
//    void damaged(int attackPoint);
//    int getHitPoint();
//    int getDeffencePoint();
//    int getAttackPoint();
//    bool isDead();
//    int getMaxUserTimes();
//    void setCharType(GameCharacterType charType);
//    bool isOpponent(GameCharacter* me);
//    bool isSelf(GameCharacter* me);
//    bool isOpponent();
//    bool isSelf();
//    void setHpBar();
//    void updateHpBar();
//    std::string getName();
//    Skill getSkill();
//    void useSkill();
//    bool skillAvailable();
    cocos2d::Sprite* getSprite();
    void setLocation(cocos2d::Point loc);
    cocos2d::Point getLocation();
    int gatDropType();
    
private:
    GameCoordinates loc;
//    Skill skill;
//    GameCharacterType charType;
    int dropType;
    cocos2d::Sprite* dropSprite;
    cocos2d::Point point;
//    std::string name;
//    int hitPoint;
//    int maxHitPoiint;
//    int attackPoint;
//    int deffencePoint;
//    int maxUseTimes;
//    int skillId;
//    int locationId;
//    int skillCount;
//    Location location;
//    cocos2d::DrawNode* hpBarBg;
    
//    void setBasicInfo(int charId);
//    void setSkill(int skillId);
    void setLocation(int locationId);
    
    
};

#endif /* defined(__MyGame__Drop__) */
