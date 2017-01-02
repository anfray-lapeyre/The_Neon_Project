#include "ennemi.hpp"


#define CACHE_SIZE 5000

using namespace std;
/**********************************************************************************************************************************
															AUTRES
		**********************************************************************************************************************************/

//Fonction amie (nous permet d'accéder aux valeurs de tous)
//Permettant d'update les mouvements des ennemis
void Update(bool tour, Map* map, Ennemi & it){
	float distanceVect[2];	
	int direction[2];
	float distanceNorme;
	if(abs(it.realX - it.x) > VITESSE_DEPLACEMENT*2 || abs(it.realY - it.y) > VITESSE_DEPLACEMENT*2){
			if(abs(it.realX - it.x) > VITESSE_DEPLACEMENT*2){
				it.realX -= VITESSE_DEPLACEMENT/4.*abs(it.x+1)/(it.x+1);
			}
			if(abs(it.realY - it.y) > VITESSE_DEPLACEMENT*2){
				it.realY -= VITESSE_DEPLACEMENT/4.*abs(it.y+1)/(it.y+1);
			}
		}
		else if(tour){
			//calcul vecteur distance, norme distance, direction vecteur
			it.realX=it.x;
			it.realY=it.y;
			distanceVect[0] = abs(it.x - map->player.x);
			distanceVect[1]= abs(it.y - map->player.y);
			
			if((it.x - map->player.x) != 0)
			{
				direction[0] = -(it.x - map->player.x)/distanceVect[0]; //pour voir la direction du vecteur ennemi-player afin que l'ennemi parte pas dans l'autre sens
				direction[1] = -(it.y - map->player.y)/distanceVect[1];
			}
			
			else
			{
				direction[0] = 0;
				direction[1] = 0;
			}

			//cout << direction[0] << "," << direction[1] << endl;

			distanceNorme = std::sqrt(distanceVect[0]*distanceVect[0] + distanceVect[1] * distanceVect[1]);
			
			if(it.isAlerted)
			{
				//cout << distanceVect[0] + distanceVect[1] << endl;
				if((distanceNorme<=6) && (distanceNorme > 0.5))
				{
					if(distanceVect[0] + distanceVect[1] == 1){
						map->hurt(it.attaque);
					}

					else
					{
						if(distanceVect[0] >= distanceVect[1])
						{
							if(map->schema[it.x+direction[0]][it.y] != 0)
							{
								it.x += direction[0];

							}

							else
							{
								it.y += direction[1];
							}
						}

						else if(distanceVect[0] < distanceVect[1])
						{
							if(map->schema[it.x][it.y+direction[1]] != 0)
							{
								it.y += direction[1];
							}

							else
							{
								it.x += direction[0];
							}
						}
					}
				//cout << it.x << "," << it.y << endl;
				}

				else 
				{
					it.isAlerted = false;
				}

			}
			else
			{
				if(distanceNorme<=3.5)
				{
					it.isAlerted=true;
				}
			}
		}
}