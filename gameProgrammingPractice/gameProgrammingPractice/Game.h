#pragma once
#include <SDL.h>
#include <vector>

class Game {

struct Vector2
{
	float x;
	float y;
};

struct Ball {
	Vector2 pos; 
	Vector2 vel; 
};

public: 
	Game(); 
	// Initialize the game 
	bool Initialize(); 
	//Runs the game loop until the game is over
	void RunLoop(); 
	//shutdown the game
	void Shutdown(); 

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

	//mem var for direction paddle should move based on user inpt
	int mPaddleOneDir;
	int mPaddleTwoDir; 
	//member variables for positios of paddle and ball
	Vector2 mPaddleOnePos;
	Vector2 mPaddleTwoPos; 
	int mPaddleThickness = 15;
	int mPaddleH = mPaddleThickness * 6;
	std::vector<Ball> mBalls;
};