#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include "../include/Dot.h"
#include "../Client.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface(std::string path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

SDL_Renderer* gRenderer = NULL;

// Client Class
Client* client = new Client();

//Jamie
bool ans = false; 
bool playerOne = false;
bool playerTwo = false;
LTexture gTextureLoser;
LTexture gTextureWinner;
bool gameOver = false;


using namespace std;

struct not_digit {
	bool operator()(const char c)
	{
		return c != ' ' && !isdigit(c);
	}
};

std::vector<int> msgToPos(std::string s)
{
	//bool redWin = true;
	not_digit notADigit;
	
	//std::cout << s << std::endl;
	//std::cout << "Here is another line" << std::endl;

	string::iterator end = std::remove_if(s.begin(), s.end(), notADigit);

	string all_numbers(s.begin(), end);
	//std::cout << all_numbers;

	stringstream ss(all_numbers);
	vector<int> vec;
	//std::cout << std::endl;
	//std::cout << "Let's start next line" << std::endl;
	int i;
	for (; ss >> i;)
	{
		vec.push_back(i);
		//cout << i << endl;
	}
	return vec;
}
bool createPlayer(std::string s)
{
	char firstChar = s.at(0);
	if (firstChar == 'W')
	{
		//bool redWin = true;
		not_digit notADigit;

		//std::cout << s << std::endl;
		//std::cout << "Here is another line" << std::endl;

		string::iterator end = std::remove_if(s.begin(), s.end(), notADigit);

		string all_numbers(s.begin(), end);
		//std::cout << all_numbers;

		stringstream ss(all_numbers);
		vector<int> vec;

		int i;
		for (; ss >> i;)
		{
			vec.push_back(i);
			//cout << i << endl;
		}
		ans = vec[0] - 1;

		//if string has Welcome .. Player 1
		//player = true;
		if (vec[0] == 1) {
			playerOne = true;
		}

		if (vec[0] == 2) {
			playerTwo = true;
		}

		return ans;
	}

	return ans;
}

bool init()
{
	//Initialization flag
	bool success = true;
	client->run();

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	
	
	if (!gTextureLoser.loadFromFile("overLoser.png", gRenderer))
	{
		printf("Failed to load dot texture!\n");
	}

	if (!gTextureWinner.loadFromFile("overWin.png", gRenderer))
	{
		printf("Failed to load dot texture!\n");
	}

	

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load PNG surface
	/*gPNGSurface = loadSurface("spritesheet.png");
	if (gPNGSurface == NULL)
	{
		printf("Failed to load PNG image!\n");
		success = false;
	}*/

	return success;
}

void close()
{
	//Free loaded image
	//SDL_FreeSurface(gPNGSurface);
	//gPNGSurface = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}


int main(int argc, char* args[])
{

	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		Dot player(false);
		player.Init(gRenderer);
	
		
		Dot chaser(true);
		chaser.Init(gRenderer);

		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;


			//While application is running
			while (!quit)
			{
				
				string msg = client->receive();

				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					player.handleEvent(e);
					chaser.handleEvent(e);
					

					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
				}


				//Apply the PNG image
				//SDL_BlitSurface(gPNGSurface, &srcRect, gScreenSurface, &destRect);

				//Update the surface
				SDL_UpdateWindowSurface(gWindow);
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//position messages player
				if (playerOne == true) {
					std::string pos = "player: 1 " + player.GetPosAsString();
					client->sendMsg(pos);
				}
				//chaser
				if (playerTwo == true) {
					std::string chpos = "player: 2 " + chaser.GetPosAsString();
					client->sendMsg(chpos);
				}
				
				
				std::cout << msg << std::endl;

				if (msg.length() > 0)
				{

					char firstChar = msg.at(0);
					if (firstChar == 'S')
					{

						if (playerOne == false) {
							player.SetPosition(msgToPos(msg)[2], msgToPos(msg)[3]);
						}
					}
					
					if (firstChar == 'S')
					{
						if (playerTwo == false) {
							chaser.SetPosition(msgToPos(msg)[2], msgToPos(msg)[3]);
						}
					}

					
					if (msg.length() > 0)
					{
						createPlayer(msg);
						if (ans == false) {
							player.Init(gRenderer);
						}
						if (ans == true) {
							chaser.SetChaser();
							chaser.Init(gRenderer);
						}
					}

					
				}				

			
				if (playerOne == true) {
					player.move(SCREEN_HEIGHT, SCREEN_WIDTH);
				}
				

				if (playerTwo == true) {
					chaser.move(SCREEN_HEIGHT, SCREEN_WIDTH);
				}
				
				player.render(gRenderer);
				chaser.render(gRenderer);

				//check collison
				if (player.Checkcollision(chaser.GetCenterX(), chaser.GetCenterY()) == true) {
					gameOver = true;

				}
				//game over
				if (gameOver == true) {
					if (playerOne)
					{
						gTextureWinner.render(0, 0, gRenderer);
					}
					else
					{
						gTextureLoser.render(0, 0, gRenderer);

					}
				}
				
				SDL_RenderPresent(gRenderer);
			}
			
			

		}
	}

	// Free resources and close SDL
	close();
	return 0;

}

