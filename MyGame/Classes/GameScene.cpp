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
    isTouchCorrect = false;
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
                
                if( j < 6)
                {
                    Drop* tapDrop = dropMap.at(tag);
                    Drop* upDrop = dropMap.at(tag + 1);
                    dropMap.erase(tag);
                    dropMap.insert(std::pair<int, Drop*>(tag, upDrop));
                }
               
                isTouchCorrect = true;
                return true;
            }
        }
    }
    
    return true;
}

void GameScene::onTouchEnded(Touch* pTouch, Event* pEvent)
{
    if(!isTouchCorrect)
    {
        return;
    }
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
                int num = 0;
                Node* upDrop = this->bg->getChildByTag(tag + 1);
                while(upDrop == nullptr)
                {
                    num++;
                    Node* upDrop = this->bg->getChildByTag(tag + num);
                }
                Point loc = getLocation(gameCoordinates);
                upDrop->setPosition(loc);
                upDrop->setTag(tag);
                Drop* insertDrop = dropMap.at(tag + 1);
                dropMap.erase(tag);
                dropMap.insert(std::pair<int, Drop*>(tag, insertDrop));
                x = gameCoordinates.x + 1;
            }
            else if(drop == nullptr && tag % 7 == 0)
            {
                x = gameCoordinates.x + 1;
            }
            
        }
    }
   
    //++++++++++++++++++++++
    std::map<int, Drop*>::iterator it = dropMap.begin();
	while( it != dropMap.end() )
	{
		log("first: %d second: %d",(*it).first, (*it).second->getSprite()->getTag());
		++it;
	}
    
    
    
    
    
    if(x == 0)
    {
        createNewDrop(addTag);
    }
    else
    {
        createNewDrop(addTag * x);
    }
    
    
    checkDeleteDrop();
    deleteChainDrop();
    
    
    
    moveDrop();
    getNullTagList();


    while(removeTagNamuber.size() > 0)
    {
        for(auto &it : removeTagNamuber)
        {
            createNewDrop(it);
        }
        checkDeleteDrop();
        deleteChainDrop();
        moveDrop();
        getNullTagList();
    }
    
    int y = 0;
//    checkDeleteDrop();
//    deleteChainDrop();
}

void GameScene::moveDrop()
{
    int tag=0;
    for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
            GameCoordinates gameCoordinates(i, j);
            tag++;
            Node* drop = this->bg->getChildByTag(tag);
            
            if(drop == nullptr && tag % 7 != 0)
            {
                std::map<int, Drop*>::iterator it = dropMap.begin();
                while( it != dropMap.end() )
                {
                    log("前 first: %d second: %d",(*it).first, (*it).second->getSprite()->getTag());
                    ++it;
                }

//                log("空:%d", tag);
                int num = 1;
                Node* upDrop = this->bg->getChildByTag(tag + num);
                while(upDrop == nullptr)
                {
                    if((tag + num) % 7 == 0)
                    {
                        break;
                    }
                    num++;
                    upDrop = this->bg->getChildByTag(tag + num);
                    
                }
                if(upDrop != nullptr)
                {
                    Point loc = getLocation(gameCoordinates);
                    upDrop->setPosition(loc);
                    upDrop->setTag(tag);
                    Drop* insertDrop = dropMap.at(tag + num);
//                    log("%d", tag);
                    insertDrop->getSprite()->setTag(tag);
                    dropMap.erase(tag);
                    dropMap.insert(std::pair<int, Drop*>(tag, insertDrop));
                    std::map<int, Drop*>::iterator it = dropMap.begin();
                    while( it != dropMap.end() )
                    {
                        log("後 first: %d second: %d",(*it).first, (*it).second->getSprite()->getTag());
                        ++it;
                    }

                }
            }
        }
    }
   
}

void GameScene::getNullTagList()
{
    removeTagNamuber.clear();
    int tag = 0;
    for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
            tag++;
            Node* drop = this->bg->getChildByTag(tag);
            if (drop == nullptr) {
                removeTagNamuber.push_back(tag);
            }

        }
    }
}

void GameScene::updateDropMap()
{
    dropMap.clear();
    int tag = 0;
    for (int i = 0; i < MAX_ROW; i++) {
		for (int j = 0; j < MAX_COL; j++) {
            tag++;
            
        }
    }
}


void GameScene::createNewDrop(int addTag)
{
    int number=CCRANDOM_0_1()*4;
    int num = (addTag - 1);
    if(addTag % 7 == 0)
    {
        Drop* drop = Drop::create(number + 1);
        GameCoordinates gameCoordinates((addTag - 1)/ 7, 6);
        Point loc = getLocation(gameCoordinates);
        drop->setLocation(loc);
        drop->getSprite()->setTag(addTag);
        this->bg->addChild(drop->getSprite());
        dropMap.erase(addTag);
        dropMap.insert(std::pair<int, Drop*>(addTag, drop));
    }
    else
    {
        Drop* drop = Drop::create(number + 1);
        GameCoordinates gameCoordinates(num / 7, (addTag - (7 * (addTag / 7) + 1)));
        Point loc = getLocation(gameCoordinates);
        drop->setLocation(loc);
        drop->getSprite()->setTag(addTag);
        this->bg->addChild(drop->getSprite());
        dropMap.insert(std::pair<int, Drop*>(addTag, drop));
    }
    
    //+++++++++
    std::map<int, Drop*>::iterator it = dropMap.begin();
	while( it != dropMap.end() )
	{
		log(" create first: %d second: %d",(*it).first, (*it).second->getSprite()->getTag());
		++it;
	}
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
//                    log("連続だよ");
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
//    log("x: %d y:%d",targetCoordnates.x ,targetCoordnates.y);
    
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
    //******
    std::map<int, Drop*>::iterator it = dropMap.begin();
	while( it != dropMap.end() )
	{
		log("first: %d second: %d",(*it).first, (*it).second->getSprite()->getTag());
		++it;
	}

    std::vector<int>deleteList;
    deleteList = removeTagNamuber;
    for(auto &it : deleteList)
    {
        std::map<int, Drop*>::iterator it4 = dropMap.begin();
        while( it4 != dropMap.end() )
        {
            log("first: %d second: %d",(*it4).first, (*it4).second->getSprite()->getTag());
            ++it4;
        }
        
        Drop* drop = dropMap.at(it);
        drop->getSprite()->removeFromParentAndCleanup(false);
        
        //+++++++++
        std::map<int, Drop*>::iterator it3 = dropMap.begin();
        while( it3 != dropMap.end() )
        {
            log("%d回 %d second: %d",it,(*it3).first, (*it3).second->getSprite()->getTag());
            ++it3;
        }
        
        
    }
    
    //++++++++
    std::map<int, Drop*>::iterator it2 = dropMap.begin();
	while( it2 != dropMap.end() )
	{
		log("first: %d second: %d",(*it2).first, (*it2).second->getSprite()->getTag());
		++it2;
	}
    
}

