#include "cRandomMap.h"

cRandomMap::cRandomMap()
{
	setRates( north );

	cRandomMap::NUMBER_OF_TILES = 0;
	cRandomMap::NUMBER_OF_LEVELS = 1;

	cRandomMap::mapWidth = 100;
	cRandomMap::mapLength = 100;
}

cRandomMap::~cRandomMap()
{

}

void cRandomMap::updateDirectionRates( direction invalidDir )
{
	int ratioToBeDivided = 0;
	std::vector<directCheck> tempVector;

	if( directVector.size() > 1 )
	{
		for( int i = 0; i != directVector.size(); i++ )
		{
			if( directVector[i].dir == invalidDir )
				ratioToBeDivided = directVector[i].ratio;
			else
				tempVector.push_back( directVector[i] );
		}
		ratioToBeDivided = ratioToBeDivided / tempVector.size();

		directVector.clear();

		for( int i = 0; i != tempVector.size(); i++ )
		{
			tempVector[i].ratio += ratioToBeDivided;
			directVector.push_back( tempVector[i] );
		}
	}

	return;
}

void cRandomMap::setRates( direction d )
{
	directCheck theDirection;

	directVector.clear();

	switch( d )
	{
	case north:
		theDirection.dir = north;
		theDirection.ratio = 80;
		directVector.push_back( theDirection );

		theDirection.dir = west;
		theDirection.ratio = 10;
		directVector.push_back( theDirection );

		theDirection.dir = east;
		theDirection.ratio = 10;
		directVector.push_back( theDirection );
		break;

	case south:
		theDirection.dir = south;
		theDirection.ratio = 80;
		directVector.push_back( theDirection );

		theDirection.dir = west;
		theDirection.ratio = 10;
		directVector.push_back( theDirection );

		theDirection.dir = east;
		theDirection.ratio = 10;
		directVector.push_back( theDirection );
		break;

	case west:
		theDirection.dir = west;
		theDirection.ratio = 80;
		directVector.push_back( theDirection );

		theDirection.dir = south;
		theDirection.ratio = 10;
		directVector.push_back( theDirection );

		theDirection.dir = north;
		theDirection.ratio = 10;
		directVector.push_back( theDirection );
		break;

	case east:
		theDirection.dir = east;
		theDirection.ratio = 80;
		directVector.push_back( theDirection );

		theDirection.dir = south;
		theDirection.ratio = 10;
		directVector.push_back( theDirection );

		theDirection.dir = north;
		theDirection.ratio = 10;
		directVector.push_back( theDirection );
		break;
	}
	return;
}

direction cRandomMap::getDirection( int random )
{
	direction newDirection;
	int endNorth = 0;
	int endSouth = 0;
	int endWest = 0;
	int endEast = 0;
	int highestDirRatio = 0;
	direction highestDir = north;

	// Get each direction Ratio for random check
	for( int i = 0; i != directVector.size(); i++ )
	{
		switch( directVector[i].dir )
		{
		case north:
			endNorth = directVector[i].ratio;
			break;

		case south:
			endSouth = directVector[i].ratio;
			break;

		case west:
			endWest = directVector[i].ratio;
			break;

		case east:
			endEast = directVector[i].ratio;
			break;
		}

		// Update the highest ratio direction
		if( directVector[i].ratio > highestDirRatio )
		{
			highestDirRatio = directVector[i].ratio;
			highestDir = directVector[i].dir;
		}
	}

	// Set end number for each direction
	endSouth = endNorth + endSouth;
	endWest = endSouth + endWest;
	endEast = endWest + endEast;

	// Check where the random number is 
	if( random >= 1 && random <= endNorth )
		newDirection = north;
	else if( random > endNorth && random <= endSouth )
		newDirection = south;
	else if( random > endSouth && random <= endWest )
		newDirection = west;
	else if( random > endWest && random <= endEast )
		newDirection = east;
	else
	{	// In case something fails with the numbers...
		// Return the highest probability direction
		return highestDir;
	}

	// Returns the random chosed direction
	return newDirection;
}

int cRandomMap::getRandomInt( void )
{
	int output;
	int min = 1;
	int max = 100;

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::default_random_engine generator( seed );
	std::uniform_real_distribution<float> distribution( min, max );

	output = 0;

	output = distribution( generator );
	return output;
}

bool cRandomMap::checkSamePositon( point testPoint )
{
	if( testPoint.x < -halfWidth || testPoint.x > halfWidth ||
		testPoint.y < -halfLength || testPoint.y > halfLength )
	{
		return false;
	}

	for( int i = 0; i != map.size(); i++ )
	{
		if( map[i].x == testPoint.x && 
			map[i].y == testPoint.y &&
			map[i].z == testPoint.z )
			return false;
	}
	return true;
}

bool cRandomMap::checkValidPositon( point testPoint )
{
	if( testPoint.x < -halfWidth || testPoint.x > halfWidth ||
		testPoint.y < -halfLength || testPoint.y > halfLength )
	{
		return false;
	}
	else
	{
		return true;
	}
}

point cRandomMap::getNewPoint( point curPoint )
{
	direction newDirection;
	point p; // = curPoint;
	bool valPosition = false;

	for( int i = 0; i !=3; i++ )
	{
		p = curPoint;
		newDirection = getDirection( getRandomInt() );
		switch( newDirection )
		{
		case north:
			p.y++;
			break;
		case south:
			p.y--;
			break;
		case west:
			p.x--;
			break;
		case east:
			p.x++;
			break;
		}
			
		valPosition = checkSamePositon( p );
			
		if( !valPosition )
			updateDirectionRates( newDirection );
		else
			setRates( newDirection );
	}
	if( !valPosition )
	{
		setRates( newDirection );
		while( !valPosition )
		{
			p = curPoint;
			newDirection = getDirection( getRandomInt() );
			switch( newDirection )
			{
			case north:
				p.y++;
				break;
			case south:
				p.y--;
				break;
			case west:
				p.x--;
				break;
			case east:
				p.x++;
				break;
			}
			valPosition = checkValidPositon( p );
		}
	}
	
	return p;
}

void cRandomMap::newMap( void )
{
	halfWidth = mapWidth / 2;
	halfLength = mapLength / 2;

	int newX, newY;
	point curPoint = centerPos;
	map.push_back( curPoint );

	for( int levels = 0; levels != NUMBER_OF_LEVELS; levels++ )
	{

		for( int i = 0; i != NUMBER_OF_TILES; i++ )
		{
			curPoint = getNewPoint( curPoint );
			curPoint.z = levels;

			if( checkSamePositon( curPoint ) )
			{
				map.push_back( curPoint );
			}
		}
	}

	for( int i = 0; i != map.size(); i++ )
	{
		tile newTile;
		newTile.pos = map[i];
		newTile.nWall = false;
		newTile.sWall = false;
		newTile.lWall = false;
		newTile.rWall = false;

		// Test for North adjacent tile
		point testPoint = map[i];
		testPoint.y++;
		if( checkSamePositon( testPoint ) || testPoint.y > halfLength )
			newTile.nWall = true;
		// Test for South adjacent tile
		testPoint = map[i];
		testPoint.y--;
		if( checkSamePositon( testPoint ) || testPoint.y < -halfLength )
			newTile.sWall = true;
		// Test for West adjacent tile
		testPoint = map[i];
		testPoint.x--;
		if( checkSamePositon( testPoint ) || testPoint.x < -halfWidth )
			newTile.lWall = true;
		// Test for East adjacent tile
		testPoint = map[i];
		testPoint.x++;
		if( checkSamePositon( testPoint ) || testPoint.y > halfWidth )
			newTile.rWall = true;

		tileMap.push_back( newTile );
	}
	tile newTile;
	newTile.nWall = true;

	// printMapInConsole( halfWidth, halfLength );

}

void cRandomMap::printMapInConsole( int halfWidth, int halfLength )
{
	point curPoint = centerPos;

	// Print Map in 2d in the console
	for( int x = -halfWidth; x != halfWidth + 1; x++ )
	{
		for( int y = -halfLength; y != halfLength + 1; y++ )
		{
			curPoint.x = x;
			curPoint.y = y;
			if( this->checkSamePositon( curPoint ) )
				std::cout << "X";
			else
				std::cout << " ";

		}
		std::cout << std::endl;
	}
}
