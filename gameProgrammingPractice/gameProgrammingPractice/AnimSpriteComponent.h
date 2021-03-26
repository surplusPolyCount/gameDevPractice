#pragma once
#include "SpriteComponent.h"

class AnimSpriteComponent : public SpriteComponent {
public: 
	AnimSpriteComponent(class Actor* owner, int drawOrder = 10);
	//Update animation every frame (overriden from component)
	void Update(float deltaTime) override; 
	//set the textures used for animation 
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	
	//Set/get the animation FPS 
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }

private: 
	//All textures in the animation 
	std::vector<SDL_Texture*> mAnimTextures; 
	//current frame displayed
	//(uses float so you can also keep track of how long that frame has displayed) 
	float mCurrFrame;
	//Anim frame rate
	float mAnimFPS; 
};