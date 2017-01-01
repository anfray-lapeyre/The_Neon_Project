/*
* 					  Anfray Lapeyre 
*			Pong Project (wich is actually a Breakout)
*					 	21/12/2016
*				       C++ - IMAC2
*				  anfray.lapeyre@gmai.com
*/
#include "game_manager.hpp"

using namespace std;
using namespace glimac;


/******************************************************************************************************
									FUNCTIONS
	******************************************************************************************************/		


//Initialize GL, GLEW and SDL, then compiles and uses the shaders and Initialize the map
void GameManager::Init(string addr)throw(int){
	
	/*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
	srand (time(NULL));
    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        throw EXIT_FAILURE;
    }
	 
	if(Mix_OpenAudio(MIX_MAX_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,MIX_DEFAULT_CHUNKSIZE) == -1) //Initialisation de l'API Mixer
	{
		std::cout << "%s", Mix_GetError();
	}

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

	FilePath applicationPath(addr);
	//We hardcode the shaders for the moment
	program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", applicationPath.dirPath() + "shaders/tex3D.fs.glsl");
	program.use();
	ID=program.getGLId();
	glLineWidth(2.f);
	 //We load the data in the constructor, it is useless to do it in a separate call
	Load();
	  
}

void GameManager::Load(){
	
	//We first build the links between c++ and OpenGL
	
	//There we set uniform variables
	 currentMap.BuildMatrix(ID);    ///////////////////////////////////A FAIRE
	 
	 //Then we load the map and its datas
	 currentMap.ChargerMap("assets/levels/lvl.txt");

	//Then we generate the Vertex Buffer Object
	 glGenBuffers(1,&vbo);
	 //.... the Vertex Array Object
	 glGenVertexArrays(1,&vao);
	 //And init the data format that we will send
	 Map::InitShaderDatas(vao,vbo); ///////////////////////////////////A FAIRE
	 
	 //activer le test de profondeur du GPU
     glEnable(GL_DEPTH_TEST);
}
	
	
/**************************************
			Events handling
	***************************************/	
	
//Function that handles all events
void GameManager::handleEvent(){
	// Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                loop = false; // Leave the loop after this iteration
            }
			else if (e.type == SDL_MOUSEMOTION){
				if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)){
					//test.player.camera.rotateLeft(-e.motion.xrel*0.2);
					//test.player.camera.rotateUp(-e.motion.yrel*0.2);
                    
				}

			}

            else if(e.type == SDL_MOUSEBUTTONDOWN)
            {
                if(SDL_BUTTON(SDL_BUTTON_LEFT))
                    // Mix_PlayChannel(1,musicBeep,1);//////////////////////////////////// A FAIRE
					currentMap.PlayFire();
					currentMap.AttackEnnemis();
            }

			else if (e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym){

				default:
					break;
				}

			}
			else if (e.type == SDL_KEYUP){
				Uint8* keyState = SDL_GetKeyState(nullptr);
				switch(e.key.keysym.sym){
				case SDLK_w:
					if(!keyState[SDLK_s])
						vitesseF = -1;
					break;
				case SDLK_s:
					if(!keyState[SDLK_w])
						vitesseF = 1;
					break;
				case SDLK_a:
					if(!keyState[SDLK_d])
						vitesseL = 1;
					break;
				case SDLK_d:
					if(!keyState[SDLK_a])
						vitesseL = -1;
					break;
				case SDLK_q:
					if(!keyState[SDLK_e])
					rotateL = 1;
					break;
				case SDLK_e:
					if(!keyState[SDLK_q])
					rotateL = -1;
						
					break;	
				case SDLK_ESCAPE:
					loop=false;
					currentMap.StopMusic();
					break;
				default:
					break;
				}
			}
        }
}

//Function that handles all events
void GameManager::handleEventMenu(){
	SDL_Event e;
    while(windowManager.pollEvent(e)) {
        if(e.type == SDL_QUIT) {
            loop=false; // Leave the loop after this iteration
        }
		else if(e.type == SDL_MOUSEMOTION){
			if(e.motion.x > 90 && e.motion.x < 400 && e.motion.y>300 && e.motion.y<550){
				
				lives=1;
			}
			else if(e.motion.x > 400 && e.motion.x < 710 && e.motion.y>300 && e.motion.y<550){
				lives =2;
			}else{
				lives=0;
			}
        }
		else if(e.type == SDL_MOUSEBUTTONDOWN ){
				if(lives==1){
					state=GAME_PLAY;
					cout << "PLAY" << endl;
					currentMap.ReloadLevel();
					lives=3;
				}
				else if(lives == 2)
					loop=false;

		}
		else if(e.type == SDL_KEYUP) {
			Uint8* keyState = SDL_GetKeyState(nullptr);
			switch(e.key.keysym.sym){
				case SDLK_ESCAPE:
					loop=false;
					break;
				default:
					break;
			}
        }
		
    }
}



/**************************************
			Update datas
	***************************************/
		
//Updates the game's data
void GameManager::Update(){
		if(currentMap.CollectObjets())
        {
            currentMap.PlayLoot();
        }
		currentMap.UpdateMove(vitesseF,vitesseL,rotateL);
		
		rotateL=0;
		vitesseL=0;
		vitesseF=0;

		currentMap.turn++;
		currentMap.UpdateEnnemis(currentMap.player.moveEnnemi || windowManager.getTime()/2.f > currentMap.turn);
		currentMap.player.moveEnnemi=false;
		
		cout << currentMap.player.pv << endl;
		
		if(currentMap.player.isDead()){
			if(lives>0){
				
				lives--;
				currentMap.ReloadLevel();
				
			}
			else{
				this->state=GAME_LOSE; // Game_Win mais les vies sont négatives, donc l'écran affiche "perdu"
			}
		}
		
		if(currentMap.isFinished()){
			currentMap.actual_lvl++;
			if(currentMap.actual_lvl >= currentMap.nbLvls)
				this->state=GAME_WIN;
			else
				currentMap.ReloadLevel();
		}

}

//Updates the game's data
void GameManager::UpdateMenu(){
	
}

//Updates the game's data
void GameManager::UpdateVictoire(){
	
}


/**************************************
			Drawing objects
	***************************************/
	
//Draws every item of the game
void GameManager::Draw(){
	/*********************************
    * HERE SHOULD COME THE RENDERING CODE
    *********************************/

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	program.use();
	glClearColor(0.03529411764705882352941176470588f,0.0039215686274509803921568627451f,0.08627450980392156862745098039216f,1.f);
	
    glBindVertexArray(vao);
	
	currentMap.DrawMap(windowManager.getTime());
	

		
	//We unbind any texture, just in case
	glBindTexture(GL_TEXTURE_2D,0);
    glBindVertexArray(0);
		
    // Update the display
    windowManager.swapBuffers();
}


//Draws every item of the game
void GameManager::DrawMenu(){
	/*********************************
    * HERE SHOULD COME THE RENDERING CODE
    *********************************/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao);

	currentMap.DrawMenu(windowManager.getTime(),lives);
		
	//We unbind any texture, just in case
	glBindTexture(GL_TEXTURE_2D,0);
    glBindVertexArray(0);
		
    // Update the display
    windowManager.swapBuffers();
}

void GameManager::DrawVictoire(){
	/*********************************
    * HERE SHOULD COME THE RENDERING CODE
    *********************************/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao);
	currentMap.DrawVictoire(windowManager.getTime(),lives);
	//We unbind any texture, just in case
	glBindTexture(GL_TEXTURE_2D,0);
    glBindVertexArray(0);
		
    // Update the display
    windowManager.swapBuffers();
}

void GameManager::DrawDefaite(){
	/*********************************
    * HERE SHOULD COME THE RENDERING CODE
    *********************************/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vao);
	currentMap.DrawDefaite(windowManager.getTime(),lives);
	//We unbind any texture, just in case
	glBindTexture(GL_TEXTURE_2D,0);
    glBindVertexArray(0);
		
    // Update the display
    windowManager.swapBuffers();
}
		
//Cleans memory
void GameManager::Clean(){
	currentMap.cleanTextures();
	currentMap.freeMusic();
}