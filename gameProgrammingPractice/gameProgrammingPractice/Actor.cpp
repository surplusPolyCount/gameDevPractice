#include "Actor.h"
#include "Component.h"

Actor::Actor(class Game* game) {
	mGame = game; 
	mScale = 1; 
	mPosition = { 0,0 };
	mRotation = 0;
};

void Actor::Update(float deltaTime) {
	UpdateComponents(deltaTime);
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