#include "ennemi.h"


#define CACHE_SIZE 5000

using namespace std;


void Ennemi::Afficher(){
	std::cout<< "Nom : "  <<this->nom << " - Attaque : " <<this->attaque << " - Defense : " <<this->defense << " - PV : " <<this->pv <<  std::endl;
}

