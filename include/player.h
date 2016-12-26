#ifndef PLAYER_H_
#define PLAYER_H_


#include "inclusion.h"
#include "objet.h"


class Player
{
	public :
		int x,y;
		FreeflyCamera camera;
		int attaque,defense, pv,gold;
		int orientation;
		bool isInMovement;
		bool isTurning;

		
		Player(int x=0, int y=0, int attaque=0, int defense=0, int pv=0,int gold=0){
			this->x=x;
			this->y=y;
			this->attaque=attaque;
			this->defense=defense;
			this->pv=pv;
			this->gold=gold;
			orientation=0;
			camera.m_Position = glm::vec3(this->x*2,-5,this->y*2);
			isInMovement=false;
			isTurning=false;
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
		};
		
		~Player(){	};
		
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
		
		void UpdateMove(int vitesseF, int vitesseL, int rotateF);
		
		void TurnLeft();
		void TurnRight();
		void UpdateRotCam();
	private : 
		
	
};


#endif
