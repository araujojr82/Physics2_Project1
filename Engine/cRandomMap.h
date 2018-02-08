#ifndef _cRandomMap_HG_
#define _cRandomMap_HG_

#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <chrono>

enum direction
{
	north = 0,
	south = 1,
	west = 2,
	east = 3,
};

struct point
{
	int x;
	int y;
	int z;

	point()
	{
		x = 0;
		y = 0;
		z = 0;
	}
};

struct tile
{
	point pos;
	bool nWall, sWall, lWall, rWall;
};

struct directCheck
{
	direction dir;
	int ratio;
};

class cRandomMap
{
public:

	cRandomMap();
	~cRandomMap();

	int rateNorth;
	int rateSouth;
	int rateWest;
	int rateEast;

	int NUMBER_OF_TILES;
	int NUMBER_OF_LEVELS;

	int mapWidth;
	int mapLength;

	int halfWidth;
	int halfLength;

	point centerPos = point();

	std::vector<point> map;
	std::vector<tile> tileMap;
	std::vector<directCheck> directVector;


	void setRates( direction d );
	direction getDirection( int random );
	int getRandomInt( void );
	bool checkSamePositon( point testPoint );
	bool checkValidPositon( point testPoint );
	point getNewPoint( point curPoint );
	void newMap( void );
	void updateDirectionRates( direction dir );
	void printMapInConsole( int halfWidth, int halfLength );

};

#endif