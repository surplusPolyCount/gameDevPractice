#include "SpriteComponent.h"
#include "Math.h"
#include "Game.h"

SpriteComponent::SpriteComponent(class Actor* owner, int drawOrder) 
	:Component(owner)
		, mTexture(nullptr)
		, mDrawOrder(drawOrder)
		, mTexWidth(0)
		, mTexHeight(0)
{
		mOwner->GetGame()->AddSprite(this);
}

void SpriteComponent::SetTexture(SDL_Texture* texture) {
	mTexture = texture; 
	// Get width/height of texture 
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight); 
}

void SpriteComponent::Draw(SDL_Renderer* renderer) {
	if (mTexture) {
		SDL_Rect r; 
		//Scale the width/height by owner's scale

		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());

		//center the rect around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().x);
		r.y = static_cast<int>(mOwner->GetPosition().y);

		//Draw
		//assumes that the pos of actor corresponds to its pos onscreen 
		//i.e. won't work for a game where the game world is
		//larger than one single screen
		SDL_RenderCopyEx(
			renderer,
			mTexture, //Texture to draw
			nullptr,  //Source rectangle
			&r,       //destination rectangle
			-Math::ToDegrees(mOwner->GetRotation()), // (Convert angle)
			nullptr,  //point of rotation 
			SDL_FLIP_NONE // Flip behavior 
			); 
	}
	else {
	}
}
