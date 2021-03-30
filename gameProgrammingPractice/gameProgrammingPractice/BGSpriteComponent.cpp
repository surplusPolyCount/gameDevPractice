#include "BGSpriteComponent.h"

BGSpriteComponent::BGSpriteComponent(class Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder), mTexWidth(1280), mTexHeight(800), mScrollSpeed(-100.0f), mScreenSize({ 1280, 800 })
{
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
		/*
		int w, h; 
		SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
		SDL_Log("width and height are: %d %d", w, h);
		*/
	}
	SDL_Log("set up bg textures with size: %d", mBGTextures.size());
}

void BGSpriteComponent::Update(float deltaTime){
	//SpriteComponent::Update(deltaTime); 
	for (auto& bg : mBGTextures) {
		//update the x offset
		bg.mOffset.x += mScrollSpeed * deltaTime; 
		//if this is completely off the screen, reset offset to
		//the right side of the last bg texture
		if (bg.mOffset.x < -mScreenSize.x) {
			bg.mOffset.x = (mBGTextures.size() - 1) * mScreenSize.x - 1;
		}
		SDL_Log("UPDATE: current pos x and y: %f %f", bg.mOffset.x, bg.mOffset.y);
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer) {
	for (auto& bg : mBGTextures) {
		SDL_Rect r;
		//Scale the width/height by owner's scale

		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());

		//TODO: make this shit relative to the center of the obj
		//center the rect around the position of the owner (relative to center of object) 
		r.x = static_cast<int>(mOwner->GetPosition().x + bg.mOffset.x);
		r.y = static_cast<int>(mOwner->GetPosition().y);
		//TODO: fix this bottom line of code with the right shiat
		//r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2 + bg.mOffset.y);
			
		//Draw rectangle w texture
		SDL_RenderCopy(
			renderer,   //renderer 
			bg.mTexture,//Texture to draw
			nullptr,    //Source rectangle
			&r          //destination rectangle
		);
		SDL_Log("DRAW: current pos x and y: %f %f", bg.mOffset.x, bg.mOffset.y, mTexWidth);
	}
}