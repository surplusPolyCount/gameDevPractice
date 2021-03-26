#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(class Actor* owner, int drawOrder) {
	mDrawOrder = drawOrder;
	mOwner = owner;
}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime); 
	if (mAnimTextures.size() > 0) {
		//Update the current frame based on frame rate
		//and delta time
		mCurrFrame += mAnimFPS * deltaTime;

		//Wrap current frame if needed
		while (mCurrFrame >= mAnimTextures.size()) {
			mCurrFrame -= mAnimTextures.size();
		}

		//Set the current textrues
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures) {
	mAnimTextures = textures; 
	mCurrFrame = 0;
	SetTexture(mAnimTextures[0]);
}

