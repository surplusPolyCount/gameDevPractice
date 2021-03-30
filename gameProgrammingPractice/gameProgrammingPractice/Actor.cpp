#include "Actor.h"
#include "Component.h"
#include "Game.h"

Actor::Actor(class Game* game)
	:mState(EActive),
	mGame(game),
	mScale(1),
	mRotation(0),
	mPosition({ 0,0 })
{
	mGame->AddActor(this);
};

void Actor::Update(float deltaTime) {
	if (mState == EActive) {
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
	}
}

void Actor::UpdateComponents(float deltaTime) {
	for (Component * comp : mComponents) {
		comp->Update(deltaTime);
	}
}

void Actor::UpdateActor(float deltaTime) {

}

void Actor::AddComponent(class Component* component) {
	mComponents.insert(mComponents.end(), component); 
}

void Actor::RemoveComponent(class Component* component) {
	mComponents.erase(find(mComponents.begin(), mComponents.end(), component));
}

Actor::~Actor() {

}