#ifndef TILEMAP_H_
#define TILEMAP_H_


#include "inclusion.h"
#include "objet.h"
#include "ennemi.h"
#include "player.h"



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
		
		GLint LocMVPMatrix, LocMVMatrix, LocNormalMatrix;
		glm::mat4 ProjMatrix, MVMatrix, NormalMatrix;
		
		GLint LocTexture;
		GLuint textures[4];
		
		Player player;
		std::vector<Ennemi> ennemis;
		std::vector<Objet> objets;
		
		std::vector<glimac::OBJMesh> ennemyMesh;
		std::vector<glimac::OBJMesh> itemMesh;
		
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
		
		//Affiche la carte
		void AfficherMap();
		//Dessine la carte en 3D 
		void DrawMap(float time);
		//Gère les textures
		void InitTextures(std::vector<std::string>);
		void cleanTextures();
		//Upload les modèles 3D des monstres
		void LoadMeshes();
		
		//Gère la camera
		void BuildMatrix(GLint Id);
		void UpdateMove(int vitesseF, int vitesseL, int rotateF);

		bool CollectObjets();
		void UpdateEnnemis();
	
	
	private : 
		void ChargerMap_donnees(FILE* F);
		void ChargerMap_level(std::ifstream& F);
		void DrawEnnemis(float time);
		void DrawObjets(float time);

	
	
};


#endif
