#pragma once
#include <SDL_Image.h>
#include"Component.h"
#include "Math.h"

class SpriteComponent : public Component {
public: 
	//(Lower draw order corresponds with further back)
	SpriteComponent(class Actor* owner, int drawOrder); 
	~SpriteComponent(); 
	virtual void Draw(SDL_Renderer* renderer); 
	virtual void SetTexture(SDL_Texture* texture); 

	int GetDrawOrder() const{ return mDrawOrder; }
	int GetTexHeight() const{ return mTexHeight; }
	int GetTexWidht () const{ return mTexWidth;  }

//protected allows subs classes to access unlike prive which
//only allows the class of origin to access it
protected:
	//Texture to draw 
	SDL_Texture* mTexture; 
	//draw order used for painter's algorithm
	//painters algorithm is just an algorithm that paints from background 
	//to foreground
	int mDrawOrder; 
	// Width/height of texture
	int mTexWidth; 
	int mTexHeight; 
};