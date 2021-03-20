#pragma once
#include "Actor.h"

class Component {
public: 
	//Constructor 
	// the lower the update order, the earlier the component updates
	Component(class Actor* owner, int updateOrder = 100);
	//Destructor
	virtual ~Component(); 

	//Update this component by delta time 
	virtual void Update(float deltaTime); 
	//Note: use const to not allow function to modify the object from which it was called
	//      here we are making a "const function" that returns and int 
	//      unlike if we were to "const int GetUpdateOrder()"
	int GetUpdateOrder() const { return mUpdateOrder;}

protected: 
	//owning actor
	class Actor* mOwner; 
	//update order of component
	int mUpdateOrder; 
};