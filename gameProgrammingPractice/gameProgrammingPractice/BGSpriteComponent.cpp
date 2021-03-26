#include "BGSpriteComponent.h"

BGSpriteComponent::BGSpriteComponent(class Actor* owner, int drawOrder) {
	mDrawOrder = drawOrder; 
	mOwner = owner; 
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*> & textures) {
	int count = 0; 
	for (auto tex : textures) {
		BGTexture temp; 
		temp.mTexture = tex; 
		//Each texture is screen width in offset
		//this code assumes texture has width of corresponding window
		temp.mOffset.x = count * mScreenSize.x; 
		temp.mOffset.y = 0; 
		mBGTextures.emplace_back(temp); 
		count++;
	}
}

void BGSpriteComponent::Update(float deltaTime){
	SpriteComponent::Update(deltaTime); 
	for (auto& bg : mBGTextures) {
		//update the x offset
		bg.mOffset.x += mScrollSpeed * deltaTime; 
		//if this is completely off the screen, reset offset to
		//the right side of the last bg texture
		if (bg.mOffset.x < -mScreenSize.x) {
			bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
		}
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer) {
	for (auto& bg : mBGTextures) {
		SDL_Rect r;
		//Scale the width/height by owner's scale

		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexWidth * mOwner->GetScale());

		//center the rect around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().x);
		r.y = static_cast<int>(mOwner->GetPosition().y);

		//Draw
		//assumes that the pos of actor corresponds to its pos onscreen 
		//i.e. won't work for a game where the game world is
		//larger than one single screen
		SDL_RenderCopyEx(
			renderer,
			bg.mTexture, //Texture to draw
			nullptr,  //Source rectangle
			&r,       //destination rectangle
			-Math::ToDegrees(mOwner->GetRotation()), // (Convert angle)
			nullptr,  //point of rotation 
			SDL_FLIP_NONE // Flip behavior 
		);
	}
}