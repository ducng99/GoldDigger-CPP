#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "entity.h"

enum class Types
{
	UNKNOWN,
	DIAMOND,
	GOLD,
	HEART,
	MINE,
	DIRT,
	CHEST,
	BGROCK
};

class GameObject : public Entity
{
public:
	GameObject();
	GameObject(Types type, int value);

	Types GetType();
	int GetValue();
	bool IsDiscovered();
	void SetDiscovered(bool discovered);
private:
	Types type;
	int value;
	bool discovered;
};

#endif //__GAMEOBJECT_H__