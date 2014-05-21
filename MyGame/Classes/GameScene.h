
#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "GameCoordinates.h"
#include "GameBoardZIndex.h"
#include "Drop.h"
//#include "GameCharacter.h"
//#include "EffectTile.h"
//#include "GameTurn.h"
#include <map>
#include <string>


#define MAX_ROW 7
#define MAX_COL 7
#define OWN_CHAR_LOC -1
#define ENEMY_CHAR_LOC 5

class GameScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
	
private:
	
	struct TempTile {
		int hitPoint;
		int defencePoint;
		GameCoordinates coordinates;
//		BoardStatus status;
	};
	
	struct TrapData {
//		GameCharacter* gameChar;
		cocos2d::Sprite* trapSprite;
	};
	
	
	cocos2d::Sprite* bg;
	cocos2d::Rect tileSize;
    cocos2d::Array* numbers = cocos2d::Array::create();
    std::vector<int>removeTagNamuber;
    
    
//	GameTurn gameTurn;
    bool flagX = true;
    bool flagY = true;
	bool playerTurnFlg = false;
    std::map<int, Drop*> dropMap;
//	std::map<GameCoordinates,Tile*, GameCoordinatesCompare> tileMap;
	std::map<GameCoordinates,TempTile, GameCoordinatesCompare> tempTileMap;
//	std::map<GameCoordinates,EffectTile*, GameCoordinatesCompare> effectTileMap;
//	std::map<GameCoordinates,GameCharacter*,GameCoordinatesCompare> gameCharMap;
	std::map<GameCoordinates,TrapData,GameCoordinatesCompare> gameTrapMap;
//	std::map<Tile*, GameCharacter*> tileOwnerMap;
//	std::vector<GameCharacter*> enemyCharList;
	int turnCount;
	int ownCharCount;
	cocos2d::LabelTTF* resultLabel;
	cocos2d::LabelTTF* turnLabel;
	std::vector<GameCoordinates> ownAffectedArea;
	std::vector<GameCoordinates> opponentAffectedArea;
	std::vector<GameCoordinates> trapTile;
	bool touchActiceFlg;
	cocos2d::DrawNode* skillBg;
//	GameCharacter* skillActivatedChar;
//	GameCharacter* opponent;
	GameCoordinates prevEnemyCoord;
	
	
	cocos2d::Point getLocation(GameCoordinates gameCoordinates);
	GameCoordinates getCoordinates(cocos2d::Point point);
	bool isOnBoard(GameCoordinates coord);
	bool isCharOnBoard(GameCoordinates coord);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
//	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    //******************
    void createNewDrop(int addTag);
    void checkDeleteDrop();
    std::vector<int> checkNearDrop(std::vector<int>list, GameCoordinates targetCord,int tag);
    bool isContain(std::vector<int>list, int num);
    void deleteChainDrop();
    
    
    //******************
	
	bool onResetBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onResetEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void saveTempTileStatus();
	void setTempTileColor();
	void setTileColor();
	void cleanCharacter();
	void cleanTempCharacter();
//	void setSkillScreen(GameCharacter* target);
//	void setTrapSkillAffectedLoc(GameCharacter* target);
	bool isTrapAvtivateLoc(GameCoordinates loc);
//	void calclateTrapDamage(GameCharacter* target);
	
//	bool isTrapable(GameCoordinates loc, GameCharacter* target);
	
//	cocos2d::DrawNode* getHpBarBg(GameCharacter* target);
	
//	GameCharacter* setEnemyMove();
//	GameCharacter* setEnemyMove(const GameCoordinates& coordEx);
//	void setAffectedLoc(GameCharacter* target, GameCoordinates currentCoord, BoardStatus status);
//	bool battle(GameCharacter* me, GameCharacter* opponent);
//	bool battle(GameCharacter* me, Tile* opponent);
	cocos2d::Sequence* battleAnime(GameCoordinates originalCord, GameCoordinates targetCord
                                   );
	void hideAllEffect();
	void showResult();
	std::string genResultStr();
	std::string genTurnStr();
	std::string to_string(int value);
//	bool isEnemyObtainable(GameCharacter* target, Tile* tile);
	void resetStates();
	
};

#endif // __GAME_SCENE_H__