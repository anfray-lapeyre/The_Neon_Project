#ifndef TILEMAP_H_
#define TILEMAP_H_


#include "inclusion.hpp"
#include "objet.hpp"
#include "ennemi.hpp"
#include "player.hpp"


class Ennemi;
//Structure de la carte -> Contient toutes les informations nécessaires à l'affichage du background
class Map
{
	private : 
		/********************************************************************************************************************************************
																SOUS-FONCTIONS
	*************************************************************************************************************************************************/	
		void ChargerMap_donnees(FILE* F);
		void ChargerMap_level(std::ifstream& F);
		void DrawMur(float time, float life);
		void DrawEau(float time, float life);
		void DrawSol(float time, float life);
		void DrawAutre(float time, float life);
		void DrawEnnemis(float time);
		
		void DrawObjets(float time);
		bool AreDoorUnlocked(int x,int y){return (schema[x][y]!=3 || (schema[x][y]==3 && player.gold>=100) ) ;}
		void InitTexturesMenu();
		
		
		
		int nbtilesX,nbtilesY; //Concerne le tileset
		Uint16** schema; //Tableau contenant les valeurs du niveau
		char nom[200]; //Nom du niveau actuel
		int depart[2]; //Contient orientation de départ
		char adresseNiveau[200]; //Adresse du niveau actuel
		char adresseData[200]; //Adresse du fichier txt
		GLuint ID; //ID du programme pour les shaders (utilisé normalement pour assimp...)
		int nbObjets,nbEnnemis; //Nb total d'objets et d'ennemis
		
		int actual_lvl,nbLvls; //Niveau actuel et nombre total de lvl
		
		GLint LocMVPMatrix, LocMVMatrix, LocNormalMatrix; //Uniform value locations
		glm::mat4 ProjMatrix, MVMatrix, NormalMatrix; //Uniform value
		
		GLint LocTexture;//Correspond au sampler2D
		GLuint textures[6];//Les pointeurs vers les textures chargées en mémoire
		GLuint texturesMenu[7]; //Les pointeurs vers les textures chargées en mémoire pour le menu
		
		Player player; //Contient les données du joueur
		std::vector<Ennemi> ennemis;//Contient tout les ennemis du niveau
		std::vector<Objet> objets; //Contient tout les objets du niveau
		
		//std::vector<glimac::Model> ennemyMesh;
		//std::vector<glimac::Model> itemMesh;
		
		Mix_Chunk *music, *musicBeep, *musicLoot; //Pointeurs pour les musiques
		
		int turn;
		
	
	public :
		/********************************************************************************************************************************************
																CONSTRUCTEURS
	*************************************************************************************************************************************************/		
		Map(){
			nbtilesX=0;
			nbtilesY=0;
			nbObjets=0;
			nbEnnemis=0;
			turn = 0;
			actual_lvl=0;
		};
		Map(GLuint id){
			nbtilesX=0;
			nbtilesY=0;
			nbObjets=0;
			nbEnnemis=0;
			this->BuildMatrix(id);
			ID = id;
			turn = 0;
			actual_lvl=0;
			
		};
		
		/********************************************************************************************************************************************
																DESTRUCTEUR
	*************************************************************************************************************************************************/	
		//Destructeur
		~Map();
		
		void cleanTextures();
		
		/********************************************************************************************************************************************
																ACCESSEURS
	*************************************************************************************************************************************************/	
		
		
		void PlayFire(){Mix_PlayChannel(1,musicBeep,1);};
		void PlayLoot(){Mix_PlayChannel(2,musicLoot,0);};
		void PlayMusic(){Mix_PlayChannel(0,music,-1);};
		void StopMusic(){Mix_HaltChannel(-1);};
		void freeMusic(){
			 Mix_FreeChunk(music);
			Mix_FreeChunk(musicBeep);
			Mix_FreeChunk(musicLoot);
			music=NULL;
			musicBeep=NULL;
			musicLoot=NULL;
			Mix_CloseAudio();
		};
	
	
	
		void toNextLevel(){actual_lvl++;}
		bool isLastLevel(){return actual_lvl+1>1;}
		
		bool isFinished(){return schema[player.x][player.y]==5;}
		
		
		int getTurn(){return turn;}
		void addTurn(){turn++;}
		int getNbLvls(){return nbLvls;}
		int getActualLvl(){return actual_lvl;}
		void toNextLvl(){actual_lvl++;}
		
		bool moveEnnemi(){return player.moveEnnemi;}
		void stopMoving(){player.moveEnnemi=false;}
		
		bool isDead(){return player.isDead();}
		
		int getPV(){return player.pv;}
		void hurt(int value){player.pv-=value;}
		
		/********************************************************************************************************************************************
																FONCTIONS AUTRES
	*************************************************************************************************************************************************/	
	/*****************************************************************
							INITIALISATION
	****************************************************************/	
		//Initialize de vbos and vaos
		static void InitShaderDatas(GLuint vao, GLuint vbo);

		
		//Gère les textures
		void InitTextures(std::vector<std::string>);
		
		
		//Charge les données dans la structure de carte grâce à l'adresse vers un fichier texte
		void ChargerMap(const char* level);
		//Load the datas in the structure thanks to the adress stored in the first txt file
		void ReloadLevel();
		
		//Gère la camera
		void BuildMatrix(GLint Id);
		
		//Upload les modèles 3D des monstres (normalement)
		void LoadMeshes();
		
	/*****************************************************************
							UPDATE
	****************************************************************/	

	//Met à jour la position du joueur (et sa rotation)
		void UpdateMove(int vitesseF, int vitesseL, int rotateF);
		//Vérifie qu'un objet ne dois pas être collecté
		bool CollectObjets();
		//Met à jour les ennemis
		void UpdateEnnemis(bool tour);
		//Gere l'attaque du joueur vers les ennemis
		void AttackEnnemis();
		
		
		//Fonction amie (nous permet d'accéder aux valeurs de tous)
		//Permettant d'update les mouvements des ennemis
		friend void Update(bool tour, Map* map, Ennemi& it);



/*****************************************************************
							DRAW
	****************************************************************/	


	
		//Affiche la carte
		void AfficherMap();
		//Dessine la carte en 3D 
		void DrawMap(float time);
		//Draws the menu
		void DrawMenu(float time, int hover);
		//Draws the victory menu
		void DrawVictoire(float time, int hover);
		
		//Draws the defeat menu
		void DrawDefaite(float time, int hover);
		
		
		
		
		void DrawMeshes(GLint id, float time);//,glimac::Model m);
		
		
	
		
	
};


#endif
