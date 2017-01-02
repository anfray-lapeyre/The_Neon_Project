#ifndef OBJET_H_
#define OBJET_H_


#include "inclusion.hpp"


class Objet
{
	public :
		int x,y,id;
		int type; //Or, vie, arme
		int valeur; //Montant qu'il donne (vaut le nombre de dégat par coup pour une arme)
		std::string nom; // On stocke le nom de l'objet
		std::string modele; //Et l'adresse de son modèle, même si malheureusement pas utilisé
		
		
		
		/**********************************************************************************************************************************
															CONSTRUCTEURS
		**********************************************************************************************************************************/
		Objet(int id=0,int x=0, int y=0, std::string nom='\0', int type=0, int valeur=0, std::string modele='\0'){
			
			this->id=id;
			this->x=x;
			this->y=y;
			this->nom=nom;
			this->type=type;
			this->valeur=valeur;
			this->modele=modele;
		};
		
		
		Objet( const Objet& other ){
			this->id=other.id;
			this->x=other.x;
			this->y=other.y;
			this->nom=other.nom;
			this->type=other.type;
			this->valeur=other.valeur;
			this->modele=other.modele;
		}
		
		/**********************************************************************************************************************************
															DESTRUCTEUR
		**********************************************************************************************************************************/
		
		~Objet(){	}
		
		
		/**********************************************************************************************************************************
															ACCESSEURS
		**********************************************************************************************************************************/
		
		void addValue(int value){
			this->valeur+=value;
		}
		
		Objet& operator+=(int loot){
			addValue(loot);
		}
		
		Objet& operator-=(int loot){
			addValue(-loot);
		}
		
		/**********************************************************************************************************************************
															AFFICHAGE
		**********************************************************************************************************************************/
		void Afficher(){std::cout<< "Nom : "  <<this->nom << " - Valeur : " <<this->valeur << std::endl;}

	
};


#endif
