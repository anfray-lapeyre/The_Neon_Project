#ifndef ENNEMI_H_
#define ENNEMI_H_


#include "inclusion.hpp"
#include "objet.hpp"
#include "tile_map.hpp"

class Map;
class Ennemi
{
	public :
		int x,y,id;
		float realX,realY; //Reelle position, dont le but est de rejoindre x et y
		int attaque,defense, pv; //Stats du monstre
		std::vector<Objet> loot; //Ce que le monstre contient. Pour l'instant on ne lui permet que d'en tenir un
		std::string nom; //On stocke le nom du monstre
		std::string modele; //Et l'adresse de son modèle 3D, même si malheureusement on l'utilise pas
		//glimac::Model modele3D;
		bool isAlerted; //Si vrai, l'ennemi a détecté le joueur et le pourchassera
		
		
		/**********************************************************************************************************************************
															CONSTRUCTEURS
		**********************************************************************************************************************************/
		Ennemi(int id=0 ,int x=0, int y=0, std::string nom='\0', int attaque=0, int defense=0, int pv=0,int valeur=0, std::string modele='\0') : realX(x), realY(y){
			this->id=id;
			this->x=x;
			this->y=y;
			this->nom=nom;
			this->attaque=attaque;
			this->defense=defense;
			this->pv=pv;
			// this->modele = modele;
			this->loot.push_back(Objet(0,0, 0,"monster", 0, valeur,"loot"));
			isAlerted=false;
			
		};
		
		Ennemi( const Ennemi& other ){
			this->id=other.id;
			this->x=other.x;
			this->y=other.y;
			this->realX=other.realX;
			this->realY=other.realY;
			this->nom=other.nom;
			this->attaque=other.attaque;
			this->defense=other.defense;
			this->pv=other.pv;
			// this->modele=other.modele;
			this->isAlerted=other.isAlerted;
			for(int i=0;i<other.loot.size();i++){
				this->loot.push_back(other.loot[i]);
			}
		};
		
		/**********************************************************************************************************************************
															DESTRUCTEUR
		**********************************************************************************************************************************/
		
		~Ennemi(){	};
		
		/**********************************************************************************************************************************
															ACCESSEURS
		**********************************************************************************************************************************/
		void addLoot(Objet loot){
			this->loot.push_back(loot);
		};
		
		Ennemi& operator+=(Objet loot){
			addLoot(loot);
		};
		
		/**********************************************************************************************************************************
															AUTRES
		**********************************************************************************************************************************/
		void Afficher(){std::cout<< "Nom : "  <<this->nom << " - Attaque : " <<this->attaque << " - Defense : " <<this->defense << " - PV : " <<this->pv <<  std::endl;}
		
		//Fonction amie (nous permet d'accéder aux valeurs de tous)
		//Permettant d'update les mouvements des ennemis
		friend void Update(bool tour, Map* map, Ennemi & it);
	private : 
		
	
};

//Fonction amie (nous permet d'accéder aux valeurs de tous)
//Permettant d'update les mouvements des ennemis
void Update(bool tour,Map* map, Ennemi & it);

#endif
