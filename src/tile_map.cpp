#include "tile_map.hpp"


#define CACHE_SIZE 5000


using namespace std;
using namespace glm;
using namespace glimac;

/*
*Charge les données sur le tileset et les stocke dans props
*Cette fonction est appelée en première
*/
void Map::ChargerMap_donnees(FILE* F) 
{
	int numtile,i,j;
	char buf[CACHE_SIZE];  // un buffer, petite mémoire cache
	char buf2[CACHE_SIZE];  // un buffer, petite mémoire cache
	fscanf(F,"#%s",buf); // #Titre
	strcpy(this->nom,buf);
	fscanf(F,"%s",buf); // Adresse du niveau
	strcpy(this->adresseNiveau,buf);
	fscanf(F,"%d",&numtile); // Adresse du niveau
	player.orientation=numtile;
	//player.camera.rotateLeft(-numtile*90);
	
	vector<string> addrs;//Adresses texture
	fscanf(F,"%s",buf); 
	addrs.push_back(buf);
	fscanf(F,"%s",buf); 
	addrs.push_back(buf);
	fscanf(F,"%s",buf); 
	addrs.push_back(buf);
	fscanf(F,"%s",buf); 
	addrs.push_back(buf);
	fscanf(F,"%s",buf); 
	addrs.push_back(buf);
	InitTextures(addrs);
	
	
	fscanf(F,"%d",&this->nbObjets);
	int infos[7];
	
	for(int i=0;i<nbObjets;i++){

		fscanf(F,"%d:%d:%d:%s %d:%d:%s",&infos[0],&infos[1],&infos[2],buf,&infos[3],&infos[4],buf2);
		this->objets.push_back(Objet(infos[0],infos[1],infos[2],buf,infos[3],infos[4],buf2));
	}
	fscanf(F,"%d",&this->nbEnnemis);
	for(int i=0;i<nbEnnemis;i++){
		fscanf(F,"%d:%d:%d:%s %d:%d:%d:%d:%s",&infos[0],&infos[1],&infos[2],buf,&infos[3],&infos[4],&infos[5],&infos[6],buf2);
		this->ennemis.push_back(Ennemi(infos[0],infos[1],infos[2],buf,infos[3],infos[4],infos[5],infos[6],buf2));
	}
	LoadMeshes();
	// Chargement de la musique
	 music = Mix_LoadWAV("assets/music/CaravanPalace-LoneDigger.wav");
    musicBeep = Mix_LoadWAV("assets/music/pew.wav");
    musicLoot = Mix_LoadWAV("assets/music/laser.wav");

    Mix_AllocateChannels(3);
    
    
    Mix_VolumeChunk(music,MIX_MAX_VOLUME/2.);
    Mix_VolumeChunk(musicBeep,MIX_MAX_VOLUME/2.);   
    Mix_VolumeChunk(musicLoot,MIX_MAX_VOLUME/2.);

    Mix_PlayChannel(0,music,-1);
}

/*
*Charge les données concernant la composition du niveau
*Cette fonction est appelée en second
*/
void Map::ChargerMap_level(ifstream& F)
{
	int i,j;
	char buf[CACHE_SIZE];  // un buffer, petite mémoire cache
	F >> buf; // #P3
	int valmax;
	F >> this->nbtilesX >> this->nbtilesY >> valmax;
	
	F.get();
	
	this->schema = (Uint16**)malloc(this->nbtilesX*sizeof(Uint16*)); //Des Uint16 suffisent largement
	
	for(i=0;i<this->nbtilesX;i++)
		this->schema[i] = (Uint16*)malloc(this->nbtilesY*sizeof(Uint16));
	for(j=0;j<this->nbtilesY;j++)
	{
		for(i=0;i<nbtilesX;i++)
		{
			int tmp1,tmp2,tmp3;
			F >> tmp1 >>tmp2 >>tmp3;
			{
				if(tmp1==0 && tmp2 == 0 && tmp3 ==0)//Noir = mur
					this->schema[i][j] = 0;
				else if(tmp1==valmax && tmp2 == valmax && tmp3 ==valmax) //Blanc = sol
					this->schema[i][j] = 1;	
				else if(tmp1==0 && tmp2 == 0 && tmp3 ==valmax) //Bleu = eau
					this->schema[i][j] = 2;
				else if(tmp1==valmax && tmp2 == 0 && tmp3 ==0) //Rouge = entree
					{
					this->schema[i][j] = 4;
					this->player.changePosition(i,j);
					}
				else if(tmp1==0 && tmp2 == valmax && tmp3 ==0) //Vert = sortie
					this->schema[i][j] = 5;
				else // Marron = porte
					this->schema[i][j] = 3;
			}
			
		}
	}
}

/*
*Charge les données dans la structure de carte grâce à l'adresse vers un fichier texte
*/
void Map::ChargerMap(const char* level)
{

	
	FILE* F;
	F = fopen(level,"r");
	if (!F)
	{

		printf("fichier %s introuvable !! \n",level);
		SDL_Quit();
		system("pause");
		exit(-1);
	}
	ChargerMap_donnees(F);

	fclose(F);
	
	ifstream F2;
	F2.open(this->adresseNiveau, std::ios::binary);

	ChargerMap_level(F2);
	
	F2.clear();
    F2.close();
	
	
	
}


/*
*Affiche la carte uniquement dans les parties couvertes par la caméra
*/
void Map::AfficherMap(){
	cout << this->nom <<endl;
	cout << this->adresseNiveau <<endl;
	cout << "Nb objets : " <<this->nbObjets <<endl;
	for (auto &objet : this->objets) // access by reference to avoid copying
    {  
        objet.Afficher();
    }
	
	cout << "Nb monstres : " <<this->nbEnnemis <<endl;

	for (auto &ennemi : this->ennemis) // access by reference to avoid copying
    {  
        ennemi.Afficher();
    }
		
	for(int i=0;i<this->nbtilesY; i++){
		for(int j=0;j<this->nbtilesX; j++){
			cout<< schema[i][j];
		
		}
		cout << endl;
		
	}
}

/*
*Nettoie la mémoire de tous les pointeurs de la carte
*/
Map::~Map()
{

	free(this->schema);
}


//Dessine la carte en 3D 
void Map::DrawMap(float time){
	ProjMatrix = glm::perspective(glm::radians(70.f+sin(time)*sin(time)), 800.f/600.f,0.1f,100.f );
	Cube cube;
	//mat4 MVMatrix;
	glBindTexture(GL_TEXTURE_2D,textures[0]);
	glUniform1i(LocTexture,0); 
	for(int i=0;i<this->nbtilesY; i++){ //MUR
		for(int j=0;j<this->nbtilesX; j++){
			if(schema[i][j]==0){
				
				 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -5+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
				 
				  MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -7+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
			}
		}
	}
	
	glBindTexture(GL_TEXTURE_2D,textures[1]);
	for(int i=0;i<this->nbtilesY; i++){ //SOL
		for(int j=0;j<this->nbtilesX; j++){
			
			if(schema[i][j]==1 ||schema[i][j]==3){
				 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -7+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
			}
			if(schema[i][j]==2){
				MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -9+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
			}
		}
	}
	glBindTexture(GL_TEXTURE_2D,textures[4]);
	for(int i=0;i<this->nbtilesY; i++){ //SOL
		for(int j=0;j<this->nbtilesX; j++){
			
			if(schema[i][j]==5){
				 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -7+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
			}

		}
	}
	
	
	glBindTexture(GL_TEXTURE_2D,textures[2]);
	for(int i=0;i<this->nbtilesY; i++){ //EAU
		for(int j=0;j<this->nbtilesX; j++){
			
			if(schema[i][j]==2){
				 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -7+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_LINES,0,cube.getVertexCount());
			}
		}
	}
	
	
	
	glBindTexture(GL_TEXTURE_2D,textures[3]);
	for(int i=0;i<this->nbtilesY; i++){  //PORTE ?
		for(int j=0;j<this->nbtilesX; j++){
			
			if(schema[i][j]==3){
				 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -5+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning-(player.gold/100)*2, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
			}
		}
	}
	DrawEnnemis(time);
	DrawObjets(time);
	
}



//Draws the menu
void DrawMenu(float time, int hover){
	
}
//Draws the victory menu
void DrawVictoire(float time, int hover){
	
}


void Map::LoadMeshes()
{
	//Assimp étant le fruit de l'union entre un démon et un étron, cette partie n'est pas implémentée
	
	//On s'occupe d'abord des objets
	
	
	// cout << ennemis[0].modele << endl;
	// ennemis[0].modele3D=Model(ennemis[0].modele);
	//Puis des ennemis
	// for(auto& ennemi:ennemis){
		// ennemi.modele3D=Model(ennemi.modele);
	// }
}


void Map::DrawMeshes(GLint id, float time, Model m){
	 // MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0,0,-5*sin(time))); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				// MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 // glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 // glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 // glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 // m.Draw();
}

void Map::DrawEnnemis(float time)
{
	Cube cube;
	//mat4 MVMatrix;
	glBindTexture(GL_TEXTURE_2D,textures[0]);
	glUniform1i(LocTexture,0); 
	for(auto& ennemi : ennemis){
		//On s'occupe d'abord des objets
		MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*ennemi.x-sin(time*10)/50., -5.25-sin(time*10)/50.+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning, -2*ennemi.y+sin(time*13)/50.)); // Translation
		MVMatrix = glm::scale(MVMatrix,glm::vec3(0.5,0.7+sin(time*10)/100.,0.2)); // Translation
		// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
		MVMatrix = player.camera.getViewMatrix()*MVMatrix;
		glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
		glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
		glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
		// ennemi.modele3D.Draw();
		glDrawArrays(GL_LINES,0,cube.getVertexCount());
	}
}

void Map::UpdateEnnemis()
{

	float distanceVect[2];	
	int direction[2];
	float distanceNorme;

	for(vector<Ennemi>::iterator it = ennemis.begin() ; it!=ennemis.end();it++)
	{
		//calcul vecteur distance, norme distance, direction vecteur

		distanceVect[0] = abs(it->x - player.x);
		distanceVect[1]= abs(it->y - player.y);

		
		direction[0] = -(it->x - player.x)/distanceVect[0]; //pour voir la direction du vecteur ennemi-player afin que l'ennemi parte pas dans l'autre sens
		direction[1] = (it->y - player.y)/distanceVect[1];

		//cout << direction[0] << "," << direction[1] << endl;

		distanceNorme = std::sqrt(distanceVect[0]*distanceVect[0] + distanceVect[1] * distanceVect[1]);
		
		if(it->isAlerted)
		{
			// cout << it->x + direction[0] << endl;
			//cout << it->x << "," << it->y << endl;
			if((distanceNorme<=5) && (distanceNorme >1))
			{
				if(distanceVect[0] >= distanceVect[1])
				{
					if(schema[it->x+direction[0]][it->y] != 0)
					{
						it->x += direction[0];

					}

					else
					{
						it->y += direction[1];
					}
				}

				else if(distanceVect[0] < distanceVect[1])
				{
					if(schema[it->x][it->y+direction[1]] != 0)
					{
						it->y += direction[1];
						cout << direction[1] << endl;
					}

					else
					{
						it->x += direction[0];
					}
				}
			//cout << it->x << "," << it->y << endl;
			}

			else {
				it->isAlerted = false;
			}

		}
		else
		{
			if(distanceNorme<=3)
			{
				it->isAlerted=true;
			}
		}
		cout << it->pv<<endl;
	}

}



void Map::DrawObjets(float time)
{
	Cube cube;
	//mat4 MVMatrix;
	glBindTexture(GL_TEXTURE_2D,textures[0]);
	glUniform1i(LocTexture,0); 
	for(auto& objet : objets){
		/*if(objet.id >= itemMesh.size()){
			//Si on voit que l'id est plus grand que la taille du tableau
			//On sait que l'id ne fait pas partie de celui-ci
			//Donc on le charge ! 
		}*/
		MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*objet.x, -5.5+cos(time)/20.+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning, -2*objet.y)); // Translation
		MVMatrix = glm::rotate(MVMatrix, time/5.f, glm::vec3(0.f,1.f,0.f));
		MVMatrix = glm::scale(MVMatrix,glm::vec3(0.2+sin(time+1)/100.,0.2+sin(time+1)/100.,0.2+sin(time+1)/100.)); // Translation
		// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
		MVMatrix = player.camera.getViewMatrix()*MVMatrix;
		glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
		glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
		glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
		glDrawArrays(GL_TRIANGLES,0,(cube.getVertexCount()/6)*5);
	}
	
}


bool Map::CollectObjets()
{

	for(vector<Objet>::iterator it = objets.begin() ; it!=objets.end();it++)
	//for(auto& objet:objets) 
	{
		if((player.x == it->x) && (player.y == it->y))
		{

			player+=(*it);
			objets.erase(it);			

			return true;
		}
	}

	return false;
}



//Gère les textures

void Map::InitTextures(vector<string> addr)
{
	
	

	glUniform1i(LocTexture,0);
	
	unique_ptr<Image> imgPave = loadImage(addr[0]);
	unique_ptr<Image> imgBrique = loadImage(addr[1]);
	unique_ptr<Image> imgDoor = loadImage(addr[2]);
	unique_ptr<Image> imgEau = loadImage(addr[3]);
	unique_ptr<Image> imgVictoire = loadImage(addr[4]);

	
	glGenTextures(1,&this->textures[0]);
    glBindTexture(GL_TEXTURE_2D,this->textures[0]);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,imgPave->getWidth() ,imgPave->getHeight() ,0, GL_RGBA,GL_FLOAT,imgPave->getPixels());
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	 
    glGenTextures(1,&this->textures[1]);
    glBindTexture(GL_TEXTURE_2D,this->textures[1]);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,imgBrique->getWidth() ,imgBrique->getHeight() ,0, GL_RGBA,GL_FLOAT,imgBrique->getPixels());
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 
	glGenTextures(1,&this->textures[2]);
    glBindTexture(GL_TEXTURE_2D,this->textures[2]);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,imgDoor->getWidth() ,imgDoor->getHeight() ,0, GL_RGBA,GL_FLOAT,imgDoor->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glGenTextures(1,&this->textures[3]);
    glBindTexture(GL_TEXTURE_2D,this->textures[3]);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,imgEau->getWidth() ,imgEau->getHeight() ,0, GL_RGBA,GL_FLOAT,imgEau->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glGenTextures(1,&this->textures[4]);
    glBindTexture(GL_TEXTURE_2D,this->textures[4]);
    glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,imgVictoire->getWidth() ,imgVictoire->getHeight() ,0, GL_RGBA,GL_FLOAT,imgVictoire->getPixels());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D,0);
	 
}

void Map::cleanTextures()
{
	glDeleteTextures(1,&this->textures[0]);
    glDeleteTextures(1,&this->textures[1]);
	glDeleteTextures(1,&this->textures[2]);
	glDeleteTextures(1,&this->textures[3]);
}


//Gère la caméra

void Map::BuildMatrix(GLint Id)
{
	LocMVPMatrix = glGetUniformLocation(Id, "uMVPMatrix");
	LocMVMatrix = glGetUniformLocation(Id, "uMVMatrix");
	LocNormalMatrix = glGetUniformLocation(Id, "uNormalMatrix");
			
	LocTexture = glGetUniformLocation(Id, "uTexture");

	ProjMatrix = glm::perspective(glm::radians(70.f), 800.f/600.f,0.1f,100.f );
    MVMatrix= player.camera.getViewMatrix()*glm::translate(glm::mat4(1.f),glm::vec3(0.f,0.f,-5.f));
    NormalMatrix = glm::transpose( glm::inverse(MVMatrix));
}

void Map::UpdateMove(int vitesseF, int vitesseL, int rotateF)
{
	if(!player.isInMovement &&!player.isTurning ){
		if(vitesseF != 0 || vitesseL != 0){
			player.isInMovement=true;
		}
		switch(player.orientation){
			case 2:
				if(vitesseF>0 && (schema[player.x][player.y-1]!=0 && AreDoorUnlocked(player.x,player.y-1) )){
					if(player.drowning<0.4 || schema[player.x][player.y-1]==2){
						player.y--;
					}
					else{
						player.drowning-=0.5;
					}
				}
				else if(vitesseF<0 && schema[player.x][player.y+1]!=0&& AreDoorUnlocked(player.x,player.y+1)){
					if(player.drowning<0.4 || schema[player.x][player.y+1]==2){
						player.y++;
					}
					else{
						player.drowning-=0.5;
					}
				}
				if(vitesseL>0 && schema[player.x+1][player.y]!=0 && AreDoorUnlocked(player.x+1,player.y)){
					if(player.drowning<0.4 || schema[player.x+1][player.y]==2){
						player.x++;
					}
					else{
						player.drowning-=0.5;
					}
				}
					
				else if(vitesseL<0 && schema[player.x-1][player.y]!=0 && AreDoorUnlocked(player.x-1,player.y)){
					if(player.drowning<0.4 || schema[player.x-1][player.y]==2){
						player.x--;
					}
					else{
						player.drowning-=0.5;
					}
				}
					
			break;
			case 3:
				if(vitesseF>0 && schema[player.x-1][player.y]!=0 && AreDoorUnlocked(player.x-1,player.y)){
					if(player.drowning<0.4 || schema[player.x-1][player.y]==2){
						player.x--;
					}
					else{
						player.drowning-=0.5;
					}
					
				}
				else if(vitesseF<0 && schema[player.x+1][player.y]!=0 && AreDoorUnlocked(player.x+1,player.y)){
					if(player.drowning<0.4 || schema[player.x+1][player.y]==2){
						player.x++;
					}
					else{
						player.drowning-=0.5;
					}
					
				}
				if(vitesseL>0 && schema[player.x][player.y-1]!=0&& AreDoorUnlocked(player.x,player.y-1)){
					if(player.drowning<0.4 || schema[player.x][player.y-1]==2){
						player.y--;
					}
					else{
						player.drowning-=0.5;
					}
					
				}
				else if(vitesseL<0 && schema[player.x][player.y+1]!=0&& AreDoorUnlocked(player.x,player.y+1)){
					if(player.drowning<0.4 || schema[player.x][player.y+1]==2){
						player.y++;
					}
					else{
						player.drowning-=0.5;
					}
					
				}
			break;
			case 0:
				if(vitesseF>0 && schema[player.x][player.y+1]!=0&& AreDoorUnlocked(player.x,player.y+1)){
					if(player.drowning<0.4 || schema[player.x][player.y+1]==2){
						player.y++;
					}
					else{
						player.drowning-=0.5;
					}
					
				}
				else if(vitesseF<0 && schema[player.x][player.y-1]!=0&& AreDoorUnlocked(player.x,player.y-1)){
					if(player.drowning<0.4 || schema[player.x][player.y-1]==2){
						player.y--;
					}
					else{
						player.drowning-=0.5;
					}
					
				}
				if(vitesseL>0 && schema[player.x-1][player.y]!=0 && AreDoorUnlocked(player.x-1,player.y)){
					if(player.drowning<0.4 || schema[player.x-1][player.y]==2){
						player.x--;
					}
					else{
						player.drowning-=0.5;
					}
					
				}
				else if(vitesseL<0 && schema[player.x+1][player.y]!=0 && AreDoorUnlocked(player.x+1,player.y)){
					if(player.drowning<0.4 || schema[player.x+1][player.y]==2){
						player.x++;
					}
					else{
						player.drowning-=0.5;
					}
					
			}
			break;
			case 1:
				if(vitesseF>0 && schema[player.x+1][player.y]!=0 && AreDoorUnlocked(player.x+1,player.y)){
					if(player.drowning<0.4 || schema[player.x+1][player.y]==2){
						player.x++;
					}
					else{
						player.drowning-=0.5;
					}
					
				}
				else if(vitesseF<0 && schema[player.x-1][player.y]!=0 && AreDoorUnlocked(player.x-1,player.y)){
					if(player.drowning<0.4 || schema[player.x-1][player.y]==2){
						player.x--;
					}
					else{
						player.drowning-=0.5;
					}
					
				}
				if(vitesseL>0 && schema[player.x][player.y+1]!=0&& AreDoorUnlocked(player.x,player.y+1)){
					if(player.drowning<0.4 || schema[player.x][player.y+1]==2){
						player.y++;
					}
					else{
						player.drowning-=0.5;
					}
					
				}
				else if(vitesseL<0 && schema[player.x][player.y-1]!=0&& AreDoorUnlocked(player.x,player.y-1)){
					if(player.drowning<0.4 || schema[player.x][player.y-1]==2){
						player.y--;
					}
					else{
						player.drowning-=0.5;
					}
					
				}
			break;
			default: 
			break;
			
		}
	}
	if(schema[player.x][player.y]==2 && player.drowning < 2.5f){
		player.drown();
	}
	else if(schema[player.x][player.y]!=2){
		player.drowning=0;
	}

	player.UpdateMove(vitesseF,vitesseL, rotateF);
}




//Initialize de vbos and vaos
void Map::InitShaderDatas(GLuint vao, GLuint vbo){
	 Cube cube;
	 
     glBindBuffer(GL_ARRAY_BUFFER,vbo);

     glBufferData(GL_ARRAY_BUFFER, cube.getVertexCount()*sizeof(ShapeVertex), cube.getDataPointer(), GL_STATIC_DRAW);

     glBindBuffer(GL_ARRAY_BUFFER,0);
	 
	 

     
     glBindVertexArray(vao);


     glEnableVertexAttribArray(VERTEX_ATTRIB_POSITION);
     glEnableVertexAttribArray(VERTEX_ATTRIB_NORMALS);
     glEnableVertexAttribArray(VERTEX_ATTRIB_TEXTURE);

     glBindBuffer(GL_ARRAY_BUFFER,vbo);

     glVertexAttribPointer(VERTEX_ATTRIB_POSITION,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,position)));
     glVertexAttribPointer(VERTEX_ATTRIB_NORMALS,3,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,normal)));
     glVertexAttribPointer(VERTEX_ATTRIB_TEXTURE,2,GL_FLOAT,GL_FALSE,sizeof(ShapeVertex), (const GLvoid *) (offsetof(ShapeVertex,texCoords)));

     glBindBuffer(GL_ARRAY_BUFFER,0);
     glBindVertexArray(0);
}





