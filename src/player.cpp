#include "player.h"


#define CACHE_SIZE 5000

using namespace std;


void Player::Afficher(){
	std::cout<< "Attaque : " <<this->attaque << " - Defense : " <<this->defense << " - PV : " <<this->pv <<" - Gold : " <<this->gold<<  std::endl;
}

void Player::UpdateMove(int vitesseF, int vitesseL, int rotateF)
{	
	
	
	if(!isInMovement&& !isTurning){
		UpdateRotCam();
		if(rotateF==-1)
			TurnLeft();
		else if (rotateF==1)
			TurnRight();
		
		camera.m_Position = glm::vec3(-this->x*2,-5,-this->y*2);
	}
	else{
		if(isInMovement){
			UpdateRotCam();
			switch(orientation){
				case 2:
					if(y+ camera.m_Position.z/2. > VITESSE_DEPLACEMENT*2)
						camera.moveFront(VITESSE_DEPLACEMENT);
					else if(y + camera.m_Position.z/2. < -VITESSE_DEPLACEMENT*2)
						camera.moveFront(-VITESSE_DEPLACEMENT);
					else if(x+ camera.m_Position.x/2. >VITESSE_DEPLACEMENT*2)
						camera.moveLeft(VITESSE_DEPLACEMENT);
					else if(x+ camera.m_Position.x/2. <-VITESSE_DEPLACEMENT*2)
						camera.moveLeft(-VITESSE_DEPLACEMENT);
					else
						isInMovement=false;
				break;
				case 3:
				
					if(y+ camera.m_Position.z/2. > VITESSE_DEPLACEMENT*2)
						camera.moveLeft(-VITESSE_DEPLACEMENT);
					else if(y + camera.m_Position.z/2. < -VITESSE_DEPLACEMENT*2)
						camera.moveLeft(VITESSE_DEPLACEMENT);
					else if(x+ camera.m_Position.x/2. >VITESSE_DEPLACEMENT*2)
						camera.moveFront(VITESSE_DEPLACEMENT);
					else if(x+ camera.m_Position.x/2. <-VITESSE_DEPLACEMENT*2)
						camera.moveFront(-VITESSE_DEPLACEMENT);
					else
						isInMovement=false;

				break;
				case 0:
					if(y+ camera.m_Position.z/2. > VITESSE_DEPLACEMENT*2)
						camera.moveFront(-VITESSE_DEPLACEMENT);
					else if(y + camera.m_Position.z/2. < -VITESSE_DEPLACEMENT*2)
						camera.moveFront(VITESSE_DEPLACEMENT);
					else if(x+ camera.m_Position.x/2. >VITESSE_DEPLACEMENT*2)
						camera.moveLeft(-VITESSE_DEPLACEMENT);
					else if(x+ camera.m_Position.x/2. <-VITESSE_DEPLACEMENT*2)
						camera.moveLeft(VITESSE_DEPLACEMENT);
					else
						isInMovement=false;
				break;
				case 1:
					if(y+ camera.m_Position.z/2. > VITESSE_DEPLACEMENT*2)
						camera.moveLeft(VITESSE_DEPLACEMENT);
					else if(y + camera.m_Position.z/2. < -VITESSE_DEPLACEMENT*2)
						camera.moveLeft(-VITESSE_DEPLACEMENT);
					else if(x+ camera.m_Position.x/2. >VITESSE_DEPLACEMENT*2)
						camera.moveFront(-VITESSE_DEPLACEMENT);
					else if(x+ camera.m_Position.x/2. <-VITESSE_DEPLACEMENT*2)
						camera.moveFront(VITESSE_DEPLACEMENT);
					else
						isInMovement=false;
				break;
				default: 
				break;
			}
		}
		if(isTurning){
			
			if((orientation*90-glm::degrees(camera.m_fPhi) >0.5 && orientation*90-glm::degrees(camera.m_fPhi) <=260) || (orientation*90-glm::degrees(camera.m_fPhi)<-260)&& orientation*90-glm::degrees(camera.m_fPhi)>-360.){
				camera.m_fPhi+=VITESSE_ROTATION;
			}
			else if((orientation*90-glm::degrees(camera.m_fPhi) <-0.5&& orientation*90-glm::degrees(camera.m_fPhi) >-359.5 ) || (orientation*90-glm::degrees(camera.m_fPhi)>260. && orientation*90-glm::degrees(camera.m_fPhi)<360.)){
				camera.m_fPhi-=VITESSE_ROTATION;
			}
			else{
				isTurning=false;
			}
			camera.rotateLeft(0.);
		}
	}
	
	
	
}

void Player::changePosition(int x, int y){
	this->x=x;
	this->y=y;
	camera.m_Position = glm::vec3(-this->x*2,-5,-this->y*2);
}

void Player::TurnLeft(){
	
	orientation = (orientation+3)%4;
	isTurning=true;
}


void Player::TurnRight(){
	orientation = (orientation+1)%4;
	isTurning=true;
	
}

void Player::UpdateRotCam(){
	this->camera.m_fPhi=glm::radians(orientation*90.f);
	camera.rotateLeft(0.);
}