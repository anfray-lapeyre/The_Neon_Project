#include "inclusion.hpp"
#include "tile_map.hpp"
#include "game_manager.hpp"


using namespace glimac;

using namespace std;

//We define function pointers to call in serie our loop
typedef void (GameManager::*ptrLoopFct)();


int main(int argc, char** argv) {
	try{
		//We create the game that will make all the loops, giving the adress to load a file from the current directory
		GameManager game(argv[0]);
		static const int N = 3;
		
		//We create function pointers for the different states that the game will be in
		ptrLoopFct loopFunc[N][N];
		//For the main loop, for every level
		loopFunc[0][0]= &GameManager::handleEvent;loopFunc[0][1]= &GameManager::Update;loopFunc[0][2]=  &GameManager::Draw;
		//For the menu
		loopFunc[1][0]= &GameManager::handleEventMenu;loopFunc[1][1]= &GameManager::UpdateMenu;loopFunc[1][2]= &GameManager::DrawMenu;
		//For the victory screen
		loopFunc[2][0]= &GameManager::handleEventMenu;loopFunc[2][1]= &GameManager::UpdateVictoire;loopFunc[2][2]= &GameManager::DrawVictoire;
		// Application loop:
		while(game.isLooping()) {
			//We alternate events handling, updating datas, and drawing
			for (int i=0; i<N; i++) {
				//Since the state is an enum, it is in fact an int, that we can use to get the right loop functions
				(game.*loopFunc[game.getState()][i])();
			}

		}
		//After the loop we clean the datas, like textures
		game.Clean();
	}//In case we catch an exception
	catch(int e){
		cerr << "Error catched : "<<e << endl;
		return EXIT_ERROR;
	}
    return EXIT_SUCCESS;

}



int main2(int argc, char** argv) {
	
	
	
	// //Initialize SDL and open a window
    // SDLWindowManager windowManager(800, 600, "The Neon Project");
    // SDL_WM_SetCaption("The Neon Project", NULL); //initialize sdl mixer

    // //Initialize glew for OpenGL3+ support
    // GLenum glewInitError = glewInit();
    // if(GLEW_OK != glewInitError) {
        // std::cerr << glewGetErrorString(glewInitError) << std::endl;
        // return EXIT_FAILURE;
    // }

    // if(Mix_OpenAudio(MIX_MAX_FREQUENCY,MIX_DEFAULT_FORMAT,MIX_DEFAULT_CHANNELS,MIX_DEFAULT_CHUNKSIZE) == -1) //Initialisation de l'API Mixer
    // {
        // std::cout << "%s", Mix_GetError();
    // }

    // std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    // std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    // FilePath applicationPath(argv[0]);
    // Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", 
                                // applicationPath.dirPath() + "shaders/tex3D.fs.glsl");
    // program.use();


	// glLineWidth(2.f);

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

	 Cube cube;
    //Initialisation musique
    // Mix_Chunk *music, *musicBeep, *musicLoot;

    // music = Mix_LoadWAV("assets/music/CaravanPalace-LoneDigger.wav");
    // musicBeep = Mix_LoadWAV("assets/music/pew.wav");
    // musicLoot = Mix_LoadWAV("assets/music/laser.wav");

    // Mix_AllocateChannels(3);
    
    
    // Mix_VolumeChunk(music,MIX_MAX_VOLUME/2.);
    // Mix_VolumeChunk(musicBeep,MIX_MAX_VOLUME/2.);   
    // Mix_VolumeChunk(musicLoot,MIX_MAX_VOLUME/2.);

    // Mix_PlayChannel(0,music,-1);

    //Fin initialisation musique


     // GLuint vbo, vao;

	 // Map test;
	 // test.BuildMatrix(program.getGLId());
	 // test.ChargerMap("assets/levels/lvl.txt");
     

	 
	 
     // glGenBuffers(1,&vbo);
     // glBindBuffer(GL_ARRAY_BUFFER,vbo);

     // glBufferData(GL_ARRAY_BUFFER, cube.getVertexCount()*sizeof(ShapeVertex), cube.getDataPointer(), GL_STATIC_DRAW);

     // glBindBuffer(GL_ARRAY_BUFFER,0);
	 
	 

     // glGenVertexArrays(1,&vao);
     // glBindVertexArray(vao);


     // glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
     // glEnableVertexAttribArray(VERTEX_ATTRIB_NORMALS);
     // glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE);

     // glBindBuffer(GL_ARRAY_BUFFER,vbo);

     // glVertexAttribPointer(VERTEX_ATTRIB_POSITION,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,position)));
     // glVertexAttribPointer(VERTEX_ATTRIB_NORMALS,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,normal)));
     // glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE,2,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,texCoords)));

     // glBindBuffer(GL_ARRAY_BUFFER,0);
     // glBindVertexArray(0);
	 //Program program2 = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl", 
      //                          applicationPath.dirPath() + "shaders/normals.fs.glsl");
    //program2.use();
	// Model ourModel("assets/models/graou.blend");

    //activer le test de profondeur du GPU
      glEnable(GL_DEPTH_TEST);
	
	// int vitesseF = 0, vitesseL = 0;
	// int rotateL = 0;
    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        // SDL_Event e;
        // while(windowManager.pollEvent(e)) {
            // if(e.type == SDL_QUIT) {
                // done = true; // Leave the loop after this iteration
            // }
			// else if (e.type == SDL_MOUSEMOTION){
				// if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)){
					// test.player.camera.rotateLeft(-e.motion.xrel*0.2);
					// test.player.camera.rotateUp(-e.motion.yrel*0.2);
                    
				// }

			// }

            // else if(e.type == SDL_MOUSEBUTTONDOWN)
            // {
                // if(SDL_BUTTON(SDL_BUTTON_LEFT))
                    // Mix_PlayChannel(1,musicBeep,1);
            // }

			// else if (e.type == SDL_KEYDOWN){
				// switch(e.key.keysym.sym){

				// default:
					// break;
				// }

			// }
			// else if (e.type == SDL_KEYUP){
				// Uint8* keyState = SDL_GetKeyState(nullptr);
				// switch(e.key.keysym.sym){
				// case SDLK_w:
					// if(!keyState[SDLK_s])
						// vitesseF = -1;
					// break;
				// case SDLK_s:
					// if(!keyState[SDLK_w])
						// vitesseF = 1;
					// break;
				// case SDLK_a:
					// if(!keyState[SDLK_d])
						// vitesseL = 1;
					// break;
				// case SDLK_d:
					// if(!keyState[SDLK_a])
						// vitesseL = -1;
					// break;
				// case SDLK_q:
					// if(!keyState[SDLK_e])
					// rotateL = 1;
					// break;
				// case SDLK_e:
					// if(!keyState[SDLK_q])
					// rotateL = -1;
					// break;	
				// case SDLK_ESCAPE:
					// done=true;
					// Mix_HaltChannel(-1);
					// break;
				// default:
					// break;
				// }
			// }
        // }
		
        // if(test.CollectObjets())
        // {
            // Mix_PlayChannel(2,musicLoot,0);
        // }
        // test.UpdateEnnemis();
		// test.UpdateMove(vitesseF,vitesseL,rotateL);
		
		// rotateL=0;
		// vitesseL=0;
		// vitesseF=0;

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Change la couleur de fond en un violet foncé qu'on utilise
		// program.use();
		// glClearColor(0.03529411764705882352941176470588f,0.0039215686274509803921568627451f,0.08627450980392156862745098039216f,1.f);
        // glBindVertexArray(vao);
		// test.DrawMap(windowManager.getTime());
		
		// glBindTexture(GL_TEXTURE_2D,0);
        // glBindVertexArray(0);
		//program2.use();
		//test.DrawMeshes(program.getGLId(),windowManager.getTime(),ourModel);
        // Update the display
        // windowManager.swapBuffers();
    }

    // test.cleanTextures();
    // Mix_FreeChunk(music);
    // Mix_FreeChunk(musicBeep);
    // Mix_FreeChunk(musicLoot);
	// music=NULL;
	// musicBeep=NULL;
	// musicLoot=NULL;
    // Mix_CloseAudio();
    return EXIT_SUCCESS;

}

