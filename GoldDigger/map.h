#ifndef __MAP_H__
#define __MAP_H__

#include <vector>

class GameObject;
class Dirt;
class BackgroundRock;

class Map
{
public:
	Map(int width, int height);
	~Map();
	void InitializeNewMap();
	void GenerateMap();
	GameObject* GenerateBlock();
	GameObject* GetBlock(int x, int y);
	void SetBlock(int x, int y, GameObject* object);

	std::vector<std::vector<Dirt*>>& GetDirtBlocks();
	std::vector<std::vector<BackgroundRock*>>& GetBackgroundBlocks();
private:
	void DestroyMap();
	void InitializeMap();
private:
	int gameWidth;
	int gameHeight;
	std::vector<std::vector<GameObject*>> gameObjects;
	std::vector<std::vector<Dirt*>> dirtObjects;
	std::vector<std::vector<BackgroundRock*>> backgroundRocks;
};

#endif // !__MAP_H__