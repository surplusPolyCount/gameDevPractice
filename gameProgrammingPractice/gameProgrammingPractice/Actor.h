#pragma once
#include <vector>

class Actor {

struct Vector2
{
	float x;
	float y;
};

public: 
	//used to check the state of actor
	enum State
	{
		EActive, 
		EPaused, 
		EDead 
	};

	//constructor/destructor 
	Actor(class Game* game); 
	virtual ~Actor(); 

	//Update function called from Game (not overridable)
	void Update(float deltaTime); 
	//Updates all components attached to the actor 
	void UpdateComponents(float deltaTime); 
	//Any actor-specific update code (overridable) 
	virtual void UpdateActor(float deltaTime); 


	//Getters/setters 
	State GetState() const { return mState; }
	virtual Vector2 GetPosition() const { return mPosition; }
	float GetScale() const { return mScale; }
	float GetRotation() const { return mRotation; }

	//Add/remove components
	void AddComponent(class Component* component); 
	void RemoveComponent(class Component* component); 

private: 
	//Actor's state
	State mState; 
	//Transform
	Vector2 mPosition; //center position of actor
	float mScale;      // Unofrms scalae of actor (1.0f for 100%)
	float mRotation;   //rotation angle (in radians)
	//Components held by this actor 
	std::vector<class Component*> mComponents; 
	class Game* mGame; 
};