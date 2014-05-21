//
//  GameCoordinates.h
//  ConquerWar
//
//  Created by Toda Shusetsu on 2/23/14.
//
//

#ifndef ConquerWar_GameCoordinates_h
#define ConquerWar_GameCoordinates_h

#define MAX_LOCATION_NUM 5

struct GameCoordinates {
	int x;
	int y;
	
	GameCoordinates() {
		this->x = 0;
		this->y = 0;
	};
	
	GameCoordinates(int x, int y) {
		this->x = x;
		this->y = y;
	};
	
	void setCoordinates(int x, int y) {
		this->x = x;
		this->y = y;
	};
	
	bool isValidCoord() {
		if (this->x < 0 || this->x >= MAX_LOCATION_NUM || this->y < 0 || this->y >= MAX_LOCATION_NUM) {
			return false;
		}
		return true;
	};
	
	bool operator==(const GameCoordinates& coord) const {
		if (this->x == coord.x && this->y == coord.y) {
			return true;
		}
		return false;
	};
	
	
};

struct GameCoordinatesCompare {
	bool operator() (const GameCoordinates& lhs, const GameCoordinates& rhs) const {
		return lhs.x + lhs.y * 10 < rhs.x + rhs.y * 10;
	}
};
#endif
