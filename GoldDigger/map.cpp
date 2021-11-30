#include <cassert>
#include "map.h"
#include "utilities.h"

#include "GameObject.h"
#include "Chest.h"
#include "Diamond.h"
#include "Dirt.h"
#include "Gold.h"
#include "Heart.h"
#include "Mine.h"
#include "BackgroundRock.h"

Map::Map(int width, int height) :
    gameWidth(5),
    gameHeight(5)
{
	if (width >= 5 && height >= 5)
	{
		this->gameWidth = width;
		this->gameHeight = height;
	}

	InitializeMap();
}

Map::~Map()
{
	DestroyMap();
}

void Map::InitializeMap()
{
	for (int width = 0; width < this->gameWidth; width++)
	{
		std::vector<GameObject*> objects(this->gameHeight);
		gameObjects.push_back(objects);
	}

	for (int width = 0; width < this->gameWidth; width++)
	{
		std::vector<Dirt*> objects;

		for (int height = 0; height < this->gameHeight; height++)
		{
			objects.push_back(new Dirt());
		}

		dirtObjects.push_back(objects);
	}

	for (int width = 0; width < this->gameWidth + 2; width++)
	{
		std::vector<BackgroundRock*> objects;

		for (int height = 0; height < this->gameHeight; height++)
		{
			objects.push_back(new BackgroundRock());
		}

		backgroundRocks.push_back(objects);
	}
}

void Map::DestroyMap()
{
	for each (std::vector<GameObject*> objects in gameObjects)
	{
		for each (GameObject * object in objects)
		{
			delete object;
			object = 0;
		}

		objects.clear();
		objects.shrink_to_fit();
	}

	gameObjects.clear();
	gameObjects.shrink_to_fit();

	for each (std::vector<Dirt*> objects in dirtObjects)
	{
		for each (Dirt * object in objects)
		{
			delete object;
			object = 0;
		}

		objects.clear();
		objects.shrink_to_fit();
	}

	dirtObjects.clear();
	dirtObjects.shrink_to_fit();

	for each (std::vector<BackgroundRock*> objects in backgroundRocks)
	{
		for each (BackgroundRock * object in objects)
		{
			delete object;
			object = 0;
		}

		objects.clear();
		objects.shrink_to_fit();
	}

	backgroundRocks.clear();
	backgroundRocks.shrink_to_fit();
}

void Map::InitializeNewMap()
{
	DestroyMap();
	InitializeMap();
	GenerateMap();
}

// Generate a map, which is a 2-D array of GameObjects
void Map::GenerateMap()
{
    int chestWidth = Utilities::GenRand(0, gameWidth - 1);

    for (size_t width = 0; width < gameObjects.size(); width++)
    {
        for (size_t height = 0; height < gameObjects[width].size(); height++)
        {
            if (width == chestWidth && height == gameObjects[width].size() - 1)
            {
                SetBlock(width, height, new Chest());
            }
            else if (height >= 3)
            {
				SetBlock(width, height, GenerateBlock());
            }
			else
			{
				SetBlock(width, height, new GameObject(Types::UNKNOWN, 0));
			}
        }
    }
}

// Percentage to appear in each block
GameObject* Map::GenerateBlock()
{
    int random = Utilities::GenRand(1, 100);

    if (random < 3)
    {
        return new Diamond();
    }
    else if (random < 10)
    {
        return new Gold();
    }
    else if (random < 13)
    {
        return new Heart();
    }
    else if (random < 20)
    {
        return new Mine();
    }
    else
    {
        return new GameObject(Types::UNKNOWN, 0);
    }
}

GameObject* Map::GetBlock(int x, int y)
{
    return gameObjects[x][y];
}

void Map::SetBlock(int x, int y, GameObject* object)
{
    assert(object);
    gameObjects[x][y] = object;
}

std::vector<std::vector<Dirt*>>& Map::GetDirtBlocks()
{
	return dirtObjects;
}

std::vector<std::vector<BackgroundRock*>>& Map::GetBackgroundBlocks()
{
	return backgroundRocks;
}
