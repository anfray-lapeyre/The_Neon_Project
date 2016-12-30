#ifndef TILEMAP_H_
#define TILEMAP_H_


#include "inclusion.hpp"
#include "objet.hpp"
#include "ennemi.hpp"
#include "player.hpp"



//Structure de la carte -> Contient toutes les informations nécessaires à l'affichage du background
class Map
{
	public :
		int nbtilesX,nbtilesY; //Concerne le tileset
		Uint16** schema; //Tableau contenant les valeurs du niveau
		char nom[200];
		int depart[2]; //Contient orientation de départ
		char adresseNiveau[200];
		GLuint ID;
		int nbObjets,nbEnnemis;
		
		int actual_lvl;
		
		GLint LocMVPMatrix, LocMVMatrix, LocNormalMatrix;
		glm::mat4 ProjMatrix, MVMatrix, NormalMatrix;
		
		GLint LocTexture;
		GLuint textures[4];
		
		Player player;
		std::vector<Ennemi> ennemis;
		std::vector<Objet> objets;
		
		std::vector<glimac::Model> ennemyMesh;
		std::vector<glimac::Model> itemMesh;
		
		Mix_Chunk *music, *musicBeep, *musicLoot;
		
		Map(){
			nbtilesX=0;
			nbtilesY=0;
			nbObjets=0;
			nbEnnemis=0;
			
		};
		Map(GLuint id){
			nbtilesX=0;
			nbtilesY=0;
			nbObjets=0;
			nbEnnemis=0;
			this->BuildMatrix(id);
			ID = id;
			
		};
		
		
		//Destructeur
		~Map();

		//Charge les données dans la structure de carte grâce à l'adresse vers un fichier texte
		void ChargerMap(const char* level);
		//Load the datas in the structure thanks to the adress stored in the first txt file
		void ReloadLevel();
		
		
		
		//Affiche la carte
		void AfficherMap();
		//Dessine la carte en 3D 
		void DrawMap(float time);
		//Draws the menu
		void DrawMenu(float time, int hover);
		//Draws the victory menu
		void DrawVictoire(float time, int hover);
		
		
		
		//Gère les textures
		void InitTextures(std::vector<std::string>);
		void cleanTextures();
		//Upload les modèles 3D des monstres
		void LoadMeshes();
		void DrawMeshes(GLint id, float time,glimac::Model m);
		
		//Gère la camera
		void BuildMatrix(GLint Id);
		void UpdateMove(int vitesseF, int vitesseL, int rotateF);

		bool CollectObjets();
		void UpdateEnnemis();
		
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
		
		
		//Initialize de vbos and vaos
		static void InitShaderDatas(GLuint vao, GLuint vbo);

		
	private : 
		void ChargerMap_donnees(FILE* F);
		void ChargerMap_level(std::ifstream& F);
		void DrawEnnemis(float time);
		void DrawObjets(float time);
		bool AreDoorUnlocked(int x,int y){return (schema[x][y]!=3 || (schema[x][y]==3 && player.gold>=100) ) ;}
		
	
};


#endif
