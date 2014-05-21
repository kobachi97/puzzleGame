#include "GameScene.h"

using namespace cocos2d;

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	//    srand(time(NULL));
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init() {
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	
	this->turnCount = 10;
	this->touchActiceFlg = false;
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	//    cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();
	
	this->ownCharCount = 0;
	
	//set up touch event
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	
	listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
//	listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	listener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	//set reset button
//	auto resetListener = cocos2d::EventListenerTouchOneByOne::create();
//	resetListener->onTouchBegan = CC_CALLBACK_2(GameScene::onResetBegan,this);
//	resetListener->onTouchEnded = CC_CALLBACK_2(GameScene::onResetEnded,this);
//	resetListener->setSwallowTouches(true);
//
//	cocos2d::Sprite* resetBtn = cocos2d::Sprite::create("CloseSelected.png");
//	resetBtn->setPosition(cocos2d::Point(visibleSize.width * 0.95, visibleSize.height * 0.95));
//	dispatcher->addEventListenerWithSceneGraphPriority(resetListener, resetBtn);
//	this->addChild(resetBtn,GameZOrder::BOARD_STATUS);
//	
//	//get background for the current quest
//	cocos2d::Sprite* questBg = cocos2d::Sprite::create("questBg.jpg");
//	questBg->setPosition(cocos2d::Point(visibleSize.width/2, visibleSize.height/2));
//	this->addChild(questBg, GameZOrder::BG);

    //背景の設定
	this->bg = Sprite::create("bg.png");
    this->bg->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    this->bg->setTag(0);

	this->addChild(this->bg);
	
	//dropの設置
    
    for (int i = 1; i <= 4; i++)
        numbers->addObject(CCInteger::create(i));
    int tagNum = 0;
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
            int index = rand() % numbers->count();
            int number = ((CCInteger*)numbers->objectAtIndex(index))->getValue();
			GameCoordinates gameCoordinates(i, j);
            Drop* drop = Drop::create(number);

			Point loc = getLocation(gameCoordinates);
            drop->setLocation(loc);
          
            tagNum++;
            drop->getSprite()->setTag(tagNum);
            
            
            drop->setScale(0.5f);
            this->bg->addChild(drop->getSprite());
            dropMap.insert(std::pair<int, Drop*>(tagNum, drop));

			if (i==0 && j==0) {
				Size mapSize = this->bg->getContentSize();
				this->tileSize = Rect(0, 0, mapSize.width/MAX_COL, mapSize.height/MAX_ROW);
			}
		}
	}
	
	
	touchActiceFlg = true;
	
	return true;
}


Point GameScene::getLocation(GameCoordinates gameCoordinates) {
	Size mapSize = this->bg->getContentSize();
	float xWidth = mapSize.width / MAX_COL;
	float yWidth = mapSize.height / MAX_ROW;
    
//    log("coordX: %d coordY %d locX: %f locY: %f",gameCoordinates.x,gameCoordinates.y,xWidth,yWidth);
	
	if (gameCoordinates.y >= 0 && gameCoordinates.y < MAX_ROW) {
		return Point(xWidth/2 + gameCoordinates.x * xWidth, yWidth/2 + gameCoordinates.y * yWidth);
	} else if (gameCoordinates.y == -1) {
		return Point(xWidth/2 + gameCoordinates.x * xWidth, -yWidth);
	} else if (gameCoordinates.y == 5) {
		return Point(xWidth/2 + gameCoordinates.x * xWidth, yWidth + mapSize.height);
	}
	
	return cocos2d::Point(0,0);
}

bool GameScene::onTouchBegan(Touch* pTouch, Event* pEvent)
{
    int tag = 0;
	for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
            tag++;
            
            auto target = static_cast<Sprite*>(pEvent->getCurrentTarget()->getChildByTag(0));

            Node* drop = this->bg->getChildByTag(tag);
            Point locationInNode = target->convertToNodeSpace(pTouch->getLocation());
            Rect rect = drop->boundingBox();
            
            if(rect.containsPoint(locationInNode))
            {
                drop->removeFromParentAndCleanup(true);
                dropMap.erase(tag);
                return true;
            }
        }
    }
    
    return true;
}

void GameScene::onTouchEnded(Touch* pTouch, Event* pEvent)
{
    int addTag = 7;
    int tag = 0;
    int x = 0;
    for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
            GameCoordinates gameCoordinates(i, j);
            tag++;
            Node* drop = this->bg->getChildByTag(tag);
            
            if(drop == nullptr && tag % 7 != 0)
            {
                Node* drop = this->bg->getChildByTag(tag + 1);
                Point loc = getLocation(gameCoordinates);
                drop->setPosition(loc);
                drop->setTag(tag);
                x = gameCoordinates.x + 1;
            }
        }
    }
    createNewDrop(addTag * x);
//    checkDeleteDrop();
//    deleteChainDrop();
//
//    for(auto &it : removeTagNamuber)
//    {
//        createNewDrop(it);
//    }
//    checkDeleteDrop();
//    deleteChainDrop();
}

void GameScene::createNewDrop(int addTag)
{
    int number=CCRANDOM_0_1()*4;
    GameCoordinates gameCoordinates(addTag - (6 * ((addTag - 1) / 7)), (addTag - 1) / 7);
    
    Drop* drop = Drop::create(number + 1);
    
    
    Point loc = getLocation(gameCoordinates);
    drop->setLocation(loc);
    drop->getSprite()->setTag(addTag);
    this->bg->addChild(drop->getSprite());
    dropMap.insert(std::pair<int, Drop*>(addTag, drop));
   
}


bool GameScene::isContain(std::vector<int>list, int num)
{
    
    bool isContain = false;
    
    for (auto &it : list)
    {
        if(it == num)
        {
            isContain = true;
            return isContain;
        }
    }
    return isContain;
}

void GameScene::checkDeleteDrop()
{
    removeTagNamuber.clear();

    int tag = 0;
    for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
            GameCoordinates gameCoordinates(i, j);
            tag++;
            if(!isContain(removeTagNamuber, tag))
            {
                std::vector<int>tmp;
                tmp.push_back(tag);
                tmp = checkNearDrop(tmp, gameCoordinates,tag);
                if(tmp.size() >= 4)
                {
                    for (auto &it : tmp)
                    {
                        if(!isContain(removeTagNamuber, it))
                        {
                            removeTagNamuber.push_back(it);                        }
                    }
                    log("連続だよ");
                }
            }
        }
    }
}

std::vector<int> GameScene::checkNearDrop(std::vector<int> removeTagNumber,GameCoordinates targetCoordnates,int tag)
{
    std::vector<int>removeTagList;
    removeTagList = removeTagNumber;
    
    if(!isContain(removeTagList, tag))
    {
        removeTagList.push_back(tag);

    }
    
    GameCoordinates gameCoordinates(targetCoordnates.x + 1, targetCoordnates.y);
    log("x: %d y:%d",targetCoordnates.x ,targetCoordnates.y);
    
    //右方向のドロップチェック
    if(targetCoordnates.x < 6 && flagX)
    {
        gameCoordinates.setCoordinates(targetCoordnates.x + 1, targetCoordnates.y);
        Drop* baseDrop = dropMap.find(tag)->second;
        Drop* rightDrop = dropMap.find(tag + 7)->second;
        if(baseDrop->getDropType() == rightDrop->getDropType())
        {
            if(!isContain(removeTagList, tag + 7))
            {

                removeTagList.push_back(tag + 7);
                removeTagList = checkNearDrop(removeTagList, gameCoordinates, tag + 7);
            }
        }
    }
    
    //左方向のドロップチェック
    if(targetCoordnates.x > 0 && flagX)
    {
        gameCoordinates.setCoordinates(targetCoordnates.x - 1, targetCoordnates.y);
        Drop* baseDrop = dropMap.find(tag)->second;
        Drop* leftDrop = dropMap.find(tag - 7)->second;
        if(baseDrop->getDropType() == leftDrop->getDropType())
        {
            if(!isContain(removeTagList, tag - 7))
            {

                removeTagList.push_back(tag - 7);

                
                removeTagList = checkNearDrop(removeTagList, gameCoordinates, tag - 7);
            }
        }
    }
    
    //上方向のドロップチェック
    if(targetCoordnates.y < 6 && flagY)
    {
        gameCoordinates.setCoordinates(targetCoordnates.x , targetCoordnates.y + 1);
        Drop* baseDrop = dropMap.find(tag)->second;
        Drop* upDrop = dropMap.find(tag + 1)->second;
        if(baseDrop->getDropType() == upDrop->getDropType())
        {
            if(!isContain(removeTagList, tag + 1))
            {
                removeTagList.push_back(tag + 1);
                removeTagList = checkNearDrop(removeTagList, gameCoordinates, tag + 1);
            }
        }
    }
    
    //下方向のドロップチェック
    if(targetCoordnates.y > 0 && flagY)
    {
        gameCoordinates.setCoordinates(targetCoordnates.x , targetCoordnates.y - 1);
        Drop* baseDrop = dropMap.find(tag)->second;
        Drop* underDrop = dropMap.find(tag - 1)->second;
        if(baseDrop->getDropType() == underDrop->getDropType())
        {
            if(!isContain(removeTagList, tag - 1))
            {
                removeTagList.push_back(tag - 1);
                removeTagList = checkNearDrop(removeTagList, gameCoordinates, tag - 1);
            }

        }
    }
    
    return removeTagList;
}

void GameScene::deleteChainDrop()
{
    std::vector<int>deleteList;
    deleteList = removeTagNamuber;
    for(auto &it : deleteList)
    {
        Drop* drop = dropMap.find(it)->second;
        drop->getSprite()->removeFromParentAndCleanup(true);
        dropMap.erase(it);
    }
}
//
//for (int i = 0; i < MAX_ROW; i++) {
//    for (int j = 0; j < MAX_COL; j++) {
//        int index = rand() % numbers->count();
//        int number = ((CCInteger*)numbers->objectAtIndex(index))->getValue();
//        GameCoordinates gameCoordinates(i, j);
//        Drop* drop = Drop::create(number);
//        
//        Point loc = getLocation(gameCoordinates);
//        Size winSize = Director::sharedDirector()->getWinSize();
//        Size cardSize = drop->getContentSize();
//        drop->setPosition(loc);
//        tagNum++;
//        drop->setTag(tagNum);
//        
//        Size size = drop->getContentSize();
//        log("座標： %d %d locX: %f locY: %f",i,j,loc.x,loc.y);
//        
//        drop->setScale(0.5f);
//        this->addChild(drop);
//        
//        if (i==0 && j==0) {
//            Size mapSize = this->bg->getContentSize();
//            this->tileSize = Rect(0, 0, mapSize.width/MAX_COL, mapSize.height/MAX_ROW);
//        }
//    }
//}
//

