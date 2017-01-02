#ifndef PLAYER_H_
#define PLAYER_H_


#include "inclusion.hpp"
#include "objet.hpp"


class Player
{
	public :
		int x,y;
		FreeflyCamera camera;
		int attaque,defense, pv,gold; //Stats du joueur
		int orientation; //Valeur d'oriention de 0 à 3
		bool isInMovement;//Si vrai, l'animation est en cours, pas d'action possible
		bool isTurning; //Si vrai, l'animation est en cours, pas d'action possible
		float drowning; //Valeur permettant de faire couler le personnage
		bool moveEnnemi = false; //Dès qu'il devient vrai, l'ennemi a le droit a une action

		/**********************************************************************************************************************************
															CONSTRUCTEURS
		**********************************************************************************************************************************/
		Player(int x=0, int y=0, int attaque=1, int defense=0, int pv=20,int gold=0){
			this->x=x;
			this->y=y;
			this->attaque=attaque;
			this->defense=defense;
			this->pv=pv;
			this->gold=gold;
			drowning=0.f;
			orientation=0;
			camera.m_Position = glm::vec3(this->x*2,-5,this->y*2);
			isInMovement=false;
			isTurning=false;
			this->moveEnnemi = false;
		};
		
		Player( const Player& other ){
			this->x=other.x;
			this->y=other.y;
			this->attaque=other.attaque;
			this->defense=other.defense;
			this->pv=other.pv;
			this->gold=other.gold;
			this->orientation=other.orientation;
			this->camera=other.camera;
			this->camera.m_Position = other.camera.m_Position;
			this->isInMovement=other.isInMovement;
			this->isTurning=other.isTurning;
			this->drowning=other.drowning;
			this->moveEnnemi = other.moveEnnemi;
		};
		/**********************************************************************************************************************************
															DESTRUCTEUR
		**********************************************************************************************************************************/
		~Player(){	};
		
		
		/**********************************************************************************************************************************
															ACCESSEURS
		**********************************************************************************************************************************/
		void addLoot(Objet loot){
			if(loot.type==0)
				gold+=loot.valeur;
			if(loot.type==1)
				pv+=loot.valeur;
			if(loot.type==2)
				attaque+=loot.valeur;
			if(loot.type==3)
				defense+=loot.valeur;
		};
		
		Player operator+=(Objet loot){
			addLoot(loot);
			return *this;
		};
		
		void changePosition(int x, int y);
		void Afficher();
		bool isDead(){return pv<=0;}
		void TurnLeft();
		void TurnRight();
		
		/**********************************************************************************************************************************
															AUTRES
		**********************************************************************************************************************************/
		//S'occupe de mettre à jour la position de la caméra du joueur
		void UpdateMove(int vitesseF, int vitesseL, int rotateF);
		//S'occupe de mettre à jour la rotation de la caméra du joueur
		void UpdateRotCam();
		//Fais couler le joueur
		void drown(){drowning+=0.001f;}
		//Restaure les pvs du joueur
		void revive(){pv=20;}

	
};


#endif
