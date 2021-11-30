#include "GameObject.h"

GameObject::GameObject() : 
	type(Types::UNKNOWN),
	value(0),
	discovered(false)
{

}

GameObject::GameObject(Types type, int value) :
	type(Types::UNKNOWN),
	value(0),
	discovered(false)
{
	this->type = type;
	this->value = value;
	this->discovered = false;
}

Types GameObject::GetType()
{
	return this->type;
}

int GameObject::GetValue()
{
	return this->value;
}

bool GameObject::IsDiscovered()
{
	return this->discovered;
}

void GameObject::SetDiscovered(bool discovered)
{
	this->discovered = discovered;
}