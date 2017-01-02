/*
* 					  Anfray Lapeyre 
*			Pong Project (wich is actually a Breakout)
*					 	21/12/2016
*				       C++ - IMAC2
*				  anfray.lapeyre@gmai.com
*/
#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_


#include "inclusion.hpp"
#include "tile_map.hpp"


// Different possible states
enum GameState {
    GAME_PLAY,
    GAME_MENU,
    GAME_WIN,
	GAME_LOSE
};

class GameManager{
	private : 
	//The object that will handle everything for SDL
	glimac::SDLWindowManager windowManager;
	//ID is the Program ID
	GLuint vao, vbo, ID;
	glimac::Program program;
	//When false, the game stops
	bool loop;
	//Life counter when in GAME_PLAY state
	//But also work as the value that enables us to know wich rectangle is hovered
	int lives;
	//Enables us to have a fluid movement
	int keyPressed;
	
	//Current level used when in GAME_PLAY state
	Map currentMap;
	
	int vitesseF, vitesseL, rotateL;
	
	// Game state
    GameState  state;	
	public : 
	
	/******************************************************************************************************
									CONSTRUCTORS
	******************************************************************************************************/								
		GameManager() throw(int) : windowManager(WIDTH, HEIGHT, "The Neon Project"), loop(true), state(GAME_MENU), keyPressed(0), vitesseF(0), vitesseL(0), rotateL(0), program(0){
			try{
				// Initialize SDL and open a window
				Init("");
				}
			catch(int e){
				throw e;
			}
		}
		GameManager(std::string addrs) throw(int) : windowManager(WIDTH, HEIGHT, "The Neon Project"), loop(true), state(GAME_MENU), keyPressed(0), vitesseF(0), vitesseL(0), rotateL(0),program(0){
			try{
				// Initialize SDL and open a window
				Init(addrs);
				}
			catch(int e){
				throw e;
			}
		}

		GameManager(std::string addrs,GLuint x, GLuint y, const char * name) throw(int) : windowManager(x, y, name), loop(true), state(GAME_MENU), keyPressed(0), vitesseF(0), vitesseL(0), rotateL(0),program(0){
			try{
				// Initialize SDL and open a window
				Init(addrs);
				}
			catch(int e){
				throw e;
			}
		}
		
		/******************************************************************************************************
									ACCESSORS
	******************************************************************************************************/		
		//Set the program ID
		void setID(GLuint id){ID=id;}
		//Gives the program ID
		GLuint getID(){return ID;}
		//Check if the program is looping
		bool isLooping(){return loop;}
		//Gets the state of the game
		int getState(){return state;}
		
		/******************************************************************************************************
									DESTRUCTOR
	******************************************************************************************************/		
		//This is normally useless
		~GameManager(){}
		
		/******************************************************************************************************
									FUNCTIONS
	******************************************************************************************************/		
		
		//Initialize GL, GLEW and SDL, then compiles and uses the shaders and Initialize the map
		void Init(std::string)throw(int);
		
		//Load the map and everything
		void Load();
	
	/**************************************
			Events handling
	***************************************/
	
	
		//Function that handles all events
		void handleEvent();
		//Function that handles all events for the menu
		void handleEventMenu();
		
		
	/**************************************
			Update datas
	***************************************/
		//Updates the game's data
		void Update();
		//Updates the menu's data
		void UpdateMenu();
		
		void UpdateVictoire();
		
		
	/**************************************
			Drawing objects
	***************************************/
		//Draws every item of the game
		void Draw();
		//Draws every item of the menu
		void DrawMenu();
		//Draws every item of the victory screen
		void DrawVictoire();
		//Draws every item of the defeat screen
		void DrawDefaite();
		
		//Cleans memory
		void Clean();
		
		
};


#endif
