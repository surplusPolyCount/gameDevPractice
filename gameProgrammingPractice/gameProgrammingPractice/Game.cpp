//we use quotes for header files in same directory as source file
#include "Game.h" 
#include "SpriteComponent.h"
#include "BGSpriteComponent.h"
#include "Actor.h"
//we use brackets for standard libraries 
#include <iostream>
#include <SDL_image.h>
#include <vector>


using namespace std; 

const int width = 1280; 
const int height = 800;


Game::Game() {
	mIsRunning = true; 
	mWindow = nullptr; 
}

bool Game::Initialize() {
	//function initializes desired flag (in this case SDL_INIT_VIDEO)
	//and returns an int that is an or operator
	//on whether appropriate binaries have initialized 
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	//load sdl img lib
	IMG_Init(IMG_INIT_PNG);

	//was giving weird errors, and noticed it is not required to init
	//so we just ignoring this shit
	//IMG_Init(IMG_INIT_PNG); 
	
	//(if int != 0, then not initialized) 
	if (sdlResult != 0) {
		//SDL_Log function similar to printf except prints to console in sdl
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError()); 
		return false; 
	}

	//initialize window pointer 
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ Chap.1", //window title
		100, //top left x-coor of window
		100, //top left y-coor of window
		width, //window width 
		height, //window height
		0    //Flags (0 for no flags set) 
	);

	//check if the window was successfully created
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false; 
	}

	//returns nullptr if it fails to initialize the renderer 
	mRenderer = SDL_CreateRenderer(
		mWindow, //Window to create Renderer for 
		-1,      //Specifies which graphic driver to use (Usually -1)
		//choose an accelerated renderer
		0
	);

	//initalize the timer since game init
	mTicksCount = 0; 

	//load game data 
	LoadData();
	return true; 
}

void Game::Shutdown() {
	// Because ~Actor calls RemoveActor, use a different style loop
	while (!mActors.empty()) {
		mActors.erase(mActors.end());
	}

	//destroys window and closes SDL with the two functions
	SDL_DestroyWindow(mWindow); 
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit(); 
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput(); 
		UpdateGame(); 
		GenerateOutput(); 
	}
}

void Game::ProcessInput() {
	//events are a way to record input actions
	//we are creating a queue of events from windows OS
	SDL_Event event;

	//while there are still events in the queue 
	while (SDL_PollEvent(&event)) {

		//switch statement to check what events are active
		switch (event.type) {
			//if SDL_QUIT event is active, red x was hit stop running the game 
			case SDL_QUIT: 
				mIsRunning = false; 
				break; 
		}
	}

	//get state of keyboard

	//grab entire state of keyboard instead of switching through keyboard events 
	//return pointer to an array that contains current state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	//can query specific key by indexing into array w corresponding SDL_SCANCODE
	//value for key, here we are checking for index of escape key
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false; 
	}
}

void Game::UpdateGame() {
	//implemenet frame limiter to 1 frame call per 16 ms (aka 60 fps) 
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	//Delta time is the difference in ticks from last frame
	//(converted to seconds) 
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	//Update tick counts (for next frame) 
	mTicksCount = SDL_GetTicks();

	//Clamp maximum delta time value 
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	//TODO: update objects in game world as function of delta time 
	//we use delta time to account for the varying speeds of processors 
	//(i.e. we move an object a certain number of pixels per frame based 
	//on how many pixels per second we want an object to move) 

	//update each actor
	mUpdatingActors = true;
	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	//move pending actors into actors vectors
	for (auto pending : mPendingActors) {
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	//Add any dead actors to a tmp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor); 
		}
	}

	//Delete dead actors (which removes them from mActors
	for (auto actor : deadActors) {
		while (!mActors.empty()) {
			delete mActors.back(); 
		}
	}
}

void Game::GenerateOutput() {
	/*
	graphics drawing code (here because graphics are an output)
	*/
	//CLEAR THE BACK BUFFER COLOR
	//back buffer is buffer game writes to 
	//specify the buffer draw color 
	SDL_SetRenderDrawColor(
		mRenderer,		//renderer pointer 
		70, 70, 175, 255 //RGBA
	);
	//clear the back buffer to the current draw color 
	SDL_RenderClear(mRenderer);

	//DRAW THE ENTIRE GAME SCENE  
	//Change the draw color to white
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255); 
	
	for(auto spr : mSprites){
		spr->Draw(mRenderer); 
	}
	
	//FINISH DRAWING GAME SCENE 

	//SWAP THE FRONT BUFFER AND THE BACK BUFFER
	SDL_RenderPresent(mRenderer);
}

void Game::AddActor(Actor * actor) {
	//if updating actors need to add to pending
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor) {
	//TODO: FIGURE THIS SHIT OUT
	delete (actor); 
}

void Game::AddSprite(SpriteComponent* sprite) {
	//Find the insertion point in the sorted vector
	//(The first element with a higher draw order than than this one)
	int myDrawOrder = sprite->GetDrawOrder(); 
	auto iter = mSprites.begin(); 

	for (; iter != mSprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->GetDrawOrder()) {
			break; 
		}
	}

	//Inserts element before position of iterator 
	mSprites.insert(iter, sprite);
}

SDL_Texture* Game::LoadTexture(const char* fileName) {
	//Load from file
	SDL_Surface* surf = IMG_Load(fileName); 
	if (!surf) {
		SDL_Log("Failed to load texture file %s", fileName); 
		SDL_Log("IMG_Load: %s\n", IMG_GetError());
		return nullptr; 
	}
	//create texture from surface 
	SDL_Texture* text = SDL_CreateTextureFromSurface(mRenderer, surf); 
	SDL_FreeSurface(surf);
	if (!text) {
		SDL_Log("Failed to convert surface to texture for %s", fileName); 
		return nullptr; 
	}
	return text; 
}


void Game::LoadData() {
	//generate background
	Actor* background = new Actor(this);
	
	//create background sprite
	SpriteComponent* bgFront =  new SpriteComponent(background, 1);

	mbgrnd = LoadTexture("Assets/Chapter2_Sprites/bckgrnd1.png");

	bgFront->SetTexture(mbgrnd);
	
	this->AddSprite(bgFront);
}