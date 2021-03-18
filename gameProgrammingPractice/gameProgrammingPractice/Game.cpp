//we use quotes for header files in same directory as source file
#include "Game.h" 
//we use brackets for standard libraries 
#include <iostream>
#include <vector>

const int width = 800; 
const int height = 600;
const int thickness = 15;


Game::Game() {
	mIsRunning = true; 
	mWindow = nullptr; 
}

bool Game::Initialize() {
	//function initializes desired flag (in this case SDL_INIT_VIDEO)
	//and returns an int that is an or operator
	//on whether appropriate binaries have initialized 
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

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

	//initialize positions
	mPaddleOnePos = { mPaddleThickness/2.0f, height / 2};
	mPaddleTwoPos = { static_cast<float>(width) - mPaddleThickness/2, height / 2};
	//initalize balls (their velocity and pos) 
	for (int i = 1; i < 4; i++) {
		mBalls.push_back({
			{static_cast<float>(width) / (i+1), static_cast<float>(height) / (i+1)},
			{ 100.0f * i, 100.0f * i}
		}
		);
	}
	return true; 
}

void Game::Shutdown() {
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

	//get user input for paddle direction 
	//first paddle
	mPaddleOneDir = 0; 
	if (state[SDL_SCANCODE_W]) {
		mPaddleOneDir -= 1;
	}
	if (state[SDL_SCANCODE_S]) {
		mPaddleOneDir += 1;
	}
	//second paddle
	mPaddleTwoDir = 0;
	if (state[SDL_SCANCODE_I]) {
		mPaddleTwoDir -= 1;
	}
	if (state[SDL_SCANCODE_K]) {
		mPaddleTwoDir += 1;
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
	//update paddle
	//first paddle
	if (mPaddleOneDir != 0) {
		//move paddle up/down 300px per second 
		mPaddleOnePos.y += mPaddleOneDir * 600.0f * deltaTime;

		//make sure paddle doesnt go off screen 
		//NOTE: source code adds a gap the size of thickness 
		//for more info, see pg. 112 
		if (mPaddleOnePos.y < (mPaddleH / 2.0f)) {
			mPaddleOnePos.y = mPaddleH / 2.0f;
		}
		else if (mPaddleOnePos.y > (height - mPaddleH/2)) {
			mPaddleOnePos.y = static_cast<float>(height) - mPaddleH / 2;
		}
	}
	//second paddle
	if (mPaddleTwoDir != 0) {
		//move paddle up/down 300px per second 
		mPaddleTwoPos.y += mPaddleTwoDir * 600.0f * deltaTime;

		//make sure paddle doesnt go off screen 
		//NOTE: source code adds a gap the size of thickness 
		//for more info, see pg. 112 
		if (mPaddleTwoPos.y < (mPaddleH / 2.0f)) {
			mPaddleTwoPos.y = mPaddleH / 2.0f;
		}
		else if (mPaddleTwoPos.y > (height - mPaddleH/2)) {
			mPaddleTwoPos.y = height - mPaddleH/2;
		}
	}

	//update ball 
	for (int i = 0; i < 3; i++) {
		Ball* b = &(mBalls[i]); 

		(*b).pos.x += (*b).vel.x * deltaTime;
		(*b).pos.y += (*b).vel.y * deltaTime;

		//check for wall collisions
		if (mBalls[i].pos.y <= thickness && mBalls[i].vel.y < 0.0f) {
			mBalls[i].vel.y *= -1;
		}if (mBalls[i].pos.y >= height - thickness && mBalls[i].vel.y > 0.0f) {
			mBalls[i].vel.y *= -1;
		}

		//check if ball hits paddle
		int diffOne = abs(mPaddleOnePos.y - mBalls[i].pos.y);
		int diffTwo = abs(mPaddleTwoPos.y - mBalls[i].pos.y);
		if (diffOne <= mPaddleH / 2.0f &&
			mBalls[i].pos.x >= 0.0f &&
			mBalls[i].pos.x <= mPaddleOnePos.x + mPaddleThickness / 2 &&
			mBalls[i].vel.x < 0.0f) {
			mBalls[i].vel.x *= -1;
		}
		if (diffTwo <= mPaddleH / 2.0f &&
			mBalls[i].pos.x <= width &&
			mBalls[i].pos.x >= mPaddleTwoPos.x - mPaddleThickness / 2 &&
			mBalls[i].vel.x > 0.0f) {
			mBalls[i].vel.x *= -1;
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
	//draw the rectangle 
	SDL_Rect wall{
		0, // top left x
		0, // top left y
		width, //width of rect
		thickness //height of rect
	};
	SDL_RenderFillRect(mRenderer, &wall); 
	//Draw ball 
	SDL_Rect paddleOne{
		//here we are converting (aka casting) floats from Vector2 struct
		//into ints (using the stat_cast<int>)
		static_cast<int>(mPaddleOnePos.x - mPaddleThickness/2),
		//NOTE: by subtracting half the thickness, we are centering the obj
		static_cast<int>(mPaddleOnePos.y - mPaddleH / 2),
		mPaddleThickness, //width
		mPaddleH //height
	};
	SDL_Rect paddleTwo{
		static_cast<int>(mPaddleTwoPos.x - mPaddleThickness/2),
		//NOTE: by subtracting half the thickness, we are centering the obj
		static_cast<int>(mPaddleTwoPos.y - mPaddleH / 2),
		mPaddleThickness,
		mPaddleH
	};

	for (int i = 0; i < 3; i++) {
		SDL_Rect mball{
			static_cast<int>(mBalls[i].pos.x - thickness / 2),
			static_cast<int>(mBalls[i].pos.y - thickness / 2),
			thickness, 
			thickness  
		};
		SDL_RenderFillRect(mRenderer, &mball);
	}
	SDL_RenderFillRect(mRenderer, &paddleOne);
	SDL_RenderFillRect(mRenderer, &paddleTwo);
	//FINISH DRAWING GAME SCENE 

	//SWAP THE FRONT BUFFER AND THE BACK BUFFER
	SDL_RenderPresent(mRenderer);
}