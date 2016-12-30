#ifndef ENNEMI_H_
#define ENNEMI_H_


#include "inclusion.hpp"
#include "objet.hpp"


class Ennemi
{
	public :
		int x,y,id;
		int attaque,defense, pv;
		std::vector<Objet> loot;
		std::string nom;
		std::string modele;
		glimac::Model modele3D;
		bool isAlerted;
		
		Ennemi(int id=0 ,int x=0, int y=0, std::string nom='\0', int attaque=0, int defense=0, int pv=0,int valeur=0, std::string modele='\0'){
			this->id=id;
			this->x=x;
			this->y=y;
			this->nom=nom;
			this->attaque=attaque;
			this->defense=defense;
			this->pv=pv;
			this->modele = modele;
			this->loot.push_back(Objet(0,0, 0,"monster", 0, valeur,"loot"));
			isAlerted=false;
			
		};
		
		Ennemi( const Ennemi& other ){
			this->id=other.id;
			this->x=other.x;
			this->y=other.y;
			this->nom=other.nom;
			this->attaque=other.attaque;
			this->defense=other.defense;
			this->pv=other.pv;
			this->modele=other.modele;
			this->isAlerted=other.isAlerted;
			for(int i=0;i<other.loot.size();i++){
				this->loot.push_back(other.loot[i]);
			}
		};
		
		~Ennemi(){	};
		
		void addLoot(Objet loot){
			this->loot.push_back(loot);
		};
		
		Ennemi& operator+=(Objet loot){
			addLoot(loot);
		};
		
		void Afficher();
	private : 
		
	
};


#endif
