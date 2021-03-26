#pragma once
#include <SDL.h>
#include <vector>
#include <SDL_image.h>
//using forward declaration here which is 
//used for classes that have not been initialized yet
class Actor;
class SpriteComponent; 
class BGSpriteComponent; 

class Game {

public: 
	Game(); 
	// Initialize the game 
	bool Initialize(); 
	//Runs the game loop until the game is over
	void RunLoop(); 
	//shutdown the game
	void Shutdown(); 

	//functions for handling actor
	void AddActor(Actor * actor); 
	void RemoveActor(Actor * actor); 

	SDL_Texture* LoadTexture(const char* fileName);
	void AddSprite(SpriteComponent* sprite); 

	void LoadData(); 

	//function for getting/loading textures? 

private: 
	//helper functions for the game loop
	void ProcessInput(); 
	void UpdateGame(); 
	void GenerateOutput(); 

	//Window created by SDL 
	SDL_Window* mWindow; 
	//game should continue to run 
	bool mIsRunning; 
	//object to reference before drawing something
	SDL_Renderer* mRenderer;

	//member var that stores number of milliseconds elapsed
	//since SLD_Init function call
	Uint32 mTicksCount;

	std::vector<Actor*> mActors; 
	std::vector<Actor*> mPendingActors; 
	bool mUpdatingActors; 

	std::vector<SpriteComponent*> mSprites; 

	//game specifc 
	SDL_Texture* mbgrnd; 
};