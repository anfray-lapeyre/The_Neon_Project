/*
* 					  Anfray Lapeyre - Noemie Busquet - Marion Prigent
*									The Neon Project
*					 				    02/01/2017
*				      				  OPENGL - IMAC2
*/

#include "tile_map.hpp"


#define CACHE_SIZE 5000


using namespace std;
using namespace glm;
using namespace glimac;
/********************************************************************************************************************************************
																SOUS-FONCTIONS
	*************************************************************************************************************************************************/	
/*
*Charge les donn�es sur le tileset et les stocke dans props
*Cette fonction est appel�e en premi�re
*/
void Map::ChargerMap_donnees(FILE* F) 
{
	int numtile,i,j;
	char buf[CACHE_SIZE];  // un buffer, petite m�moire cache
	char buf2[CACHE_SIZE];  // un buffer, petite m�moire cache
	int infos[7];
	fscanf(F,"%d",&this->nbLvls);
	for(int i=0;i<actual_lvl;i++){
		fscanf(F,"%s",buf); // #Titre
		fscanf(F,"%s",buf); // Adresse du niveau
		fscanf(F,"%d",&numtile); // Adresse du niveau
		for(int i=0;i<6; i++){ //Textures
			fscanf(F,"%s",buf); 
		}
		
		fscanf(F,"%d",&this->nbObjets);
		
		for(int i=0;i<nbObjets;i++){

			fscanf(F,"%d:%d:%d:%s %d:%d:%s",&infos[0],&infos[1],&infos[2],buf,&infos[3],&infos[4],buf2);
		}
		fscanf(F,"%d",&this->nbEnnemis);
		for(int i=0;i<nbEnnemis;i++){
			fscanf(F,"%d:%d:%d:%s %d:%d:%d:%d:%s",&infos[0],&infos[1],&infos[2],buf,&infos[3],&infos[4],&infos[5],&infos[6],buf2);
		}
	}
	fscanf(F,"%s",buf); // #Titre
	strcpy(this->nom,buf);
	fscanf(F,"%s",buf); // Adresse du niveau
	strcpy(this->adresseNiveau,buf);
	fscanf(F,"%d",&numtile); // Adresse du niveau
	player.orientation=numtile;
	//player.camera.rotateLeft(-numtile*90);
	
	vector<string> addrs;//Adresses texture
	for(int i=0;i<6; i++){
		fscanf(F,"%s",buf); 
		addrs.push_back(buf);
	}

	InitTextures(addrs);
	
	
	fscanf(F,"%d",&this->nbObjets);

	
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
	if(actual_lvl <1){
		music = Mix_LoadWAV("assets/music/CaravanPalace-LoneDigger.wav");
	}else{
		music = Mix_LoadWAV("assets/music/phantom.wav");

	}
    musicBeep = Mix_LoadWAV("assets/music/pew.wav");
    musicLoot = Mix_LoadWAV("assets/music/laser.wav");

    Mix_AllocateChannels(3);
    
    
    Mix_VolumeChunk(music,MIX_MAX_VOLUME/2.);
    Mix_VolumeChunk(musicBeep,MIX_MAX_VOLUME/2.);   
    Mix_VolumeChunk(musicLoot,MIX_MAX_VOLUME/2.);

    Mix_PlayChannel(0,music,-1);
}

/*
*Charge les donn�es concernant la composition du niveau
*Cette fonction est appel�e en second
*/
void Map::ChargerMap_level(ifstream& F)
{
	int i,j;
	char buf[CACHE_SIZE];  // un buffer, petite m�moire cache
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


void Map::DrawMur(float time, float life){
	Cube cube;
	//mat4 MVMatrix;
	glBindTexture(GL_TEXTURE_2D,textures[0]);
	glUniform1i(LocTexture,0); 
	for(int i=0;i<this->nbtilesY; i++){ //MUR
		for(int j=0;j<this->nbtilesX; j++){
			if(schema[i][j]==0){
				
				 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -5+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning-life/100.f, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
				 
				  MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -7+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning-life/100.f, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
			}
		}
	}
	
	
	glBindTexture(GL_TEXTURE_2D,textures[5]);
	for(int i=0;i<this->nbtilesY; i++){ //MUR DU HAUT
		for(int j=0;j<this->nbtilesX; j++){
			if(schema[i][j]==0){
				
				 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -5+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning+2-life/100.f, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
				 
			}
		}
	}
}


void Map::DrawEau(float time, float life){
	Cube cube;
	
	glBindTexture(GL_TEXTURE_2D,textures[2]);
	for(int i=0;i<this->nbtilesY; i++){ //EAU
		for(int j=0;j<this->nbtilesX; j++){
			
			if(schema[i][j]==2){
				
				 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -7+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning-life/100.f, -2*j)); // Translation
				 MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
				 // MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -7+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 // glDrawArrays(GL_LINES,0,cube.getVertexCount());
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);
			}
		}
	}
}


void Map::DrawSol(float time, float life){
	Cube cube;
	glBindTexture(GL_TEXTURE_2D,textures[1]);
	for(int i=0;i<this->nbtilesY; i++){ //SOL
		for(int j=0;j<this->nbtilesX; j++){
			
			if(schema[i][j]==1 ||schema[i][j]==3 ||schema[i][j]==4){
				 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -7+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning-life/100.f, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
			}
		}
	}
}


void Map::DrawAutre(float time, float life){
	Cube cube;
	glBindTexture(GL_TEXTURE_2D,textures[4]);
	for(int i=0;i<this->nbtilesY; i++){ //SOL VICTOIRE
		for(int j=0;j<this->nbtilesX; j++){
			
			if(schema[i][j]==5){
				 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*i, -7+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning-life/100.f, -2*j)); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount());
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
}



//Dessine la carte en 3D 
void Map::DrawMap(float time){
	float life = (sin(glm::radians(20.f-player.pv)*time)*sin(glm::radians((20.f-player.pv)*time)))*(20.1f-player.pv)/5.f;
	ProjMatrix = glm::perspective(glm::radians(70.f+sin(time)*sin(time)+life), 800.f/600.f,0.1f,100.f );
	
	DrawMur(time,life);
	DrawEau(time,life);
	DrawSol(time,life);
	DrawAutre(time,life);
	
	DrawEnnemis(time);
	DrawObjets(time);
	
}


void Map::DrawEnnemis(float time)
{
	Cube cube;
	//mat4 MVMatrix;
	glBindTexture(GL_TEXTURE_2D,textures[0]);
	glUniform1i(LocTexture,0); 
	for(auto& ennemi : ennemis){
		//On s'occupe d'abord des objets
		MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2*ennemi.realX-sin(time*10)/50., -5.25-sin(time*10)/50.+((int)player.isInMovement)*VITESSE_DEPLACEMENT*2*sin(time*M_PI*4)+player.drowning, -2*ennemi.realY+sin(time*13)/50.)); // Translation
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




void Map::InitTexturesMenu()
{
	
	

	glUniform1i(LocTexture,0);
	
	unique_ptr<Image> img[7];
	img[0]	= loadImage("assets/textures/menu/menufond3.jpg");
	img[1] = loadImage("assets/textures/menu/lose.jpg");
	img[2] = loadImage("assets/textures/menu/win.jpg");
	img[3] = loadImage("assets/textures/menu/play.jpg");
	img[4] = loadImage("assets/textures/menu/replay.jpg");
	img[5] = loadImage("assets/textures/menu/quit.jpg");
	img[6] = loadImage("assets/textures/menu/menu.jpg");

	for(int i=0;i<7;i++){
		glGenTextures(1,&this->texturesMenu[i]);
		glBindTexture(GL_TEXTURE_2D,this->texturesMenu[i]);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,img[i]->getWidth() ,img[i]->getHeight() ,0, GL_RGBA,GL_FLOAT,img[i]->getPixels());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	

    glBindTexture(GL_TEXTURE_2D,0);
	 
}


/********************************************************************************************************************************************
																FONCTIONS AUTRES
	*************************************************************************************************************************************************/	


/*****************************************************************
							INITIALISATION
	****************************************************************/	




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





//G�re les textures

void Map::InitTextures(vector<string> addr)
{
	
	

	glUniform1i(LocTexture,0);
	unique_ptr<Image> img[6];
	img[0]	 = loadImage(addr[0]);
	img[1] = loadImage(addr[1]);
	img[2] = loadImage(addr[2]);
	img[3] = loadImage(addr[3]);
	img[4] = loadImage(addr[4]);
	img[5] = loadImage(addr[5]);

	for(int i=0;i<6;i++){
		glGenTextures(1,&this->textures[i]);
		glBindTexture(GL_TEXTURE_2D,this->textures[i]);
		glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA,img[i]->getWidth() ,img[i]->getHeight() ,0, GL_RGBA,GL_FLOAT,img[i]->getPixels());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	
	InitTexturesMenu();
	 
}




/*
*Charge les donn�es dans la structure de carte gr�ce � l'adresse vers un fichier texte
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
	strcpy(this->adresseData,level);
	ChargerMap_donnees(F);

	fclose(F);
	
	ifstream F2;
	F2.open(this->adresseNiveau, std::ios::binary);

	ChargerMap_level(F2);
	
	F2.clear();
    F2.close();
	
	
}

/*
*Recharge les donn�es dans la structure de carte gr�ce � l'adresse vers un fichier texte
*/
void Map::ReloadLevel()
{

	for(int i=0; i<ennemis.size();i++){
		ennemis.pop_back();
	}
	for(int i=0; i<objets.size();i++){
		objets.pop_back();
	}
	FILE* F;
	F = fopen(adresseData,"r");
	if (!F)
	{

		printf("fichier %s introuvable !! \n",adresseData);
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
	
	player.revive();
}






//G�re la cam�ra

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


void Map::LoadMeshes()
{
	//Assimp �tant le fruit de l'union entre un d�mon et un �tron, cette partie n'est pas impl�ment�e
	
	//On s'occupe d'abord des objets
	
	
	// cout << ennemis[0].modele << endl;
	// ennemis[0].modele3D=Model(ennemis[0].modele);
	//Puis des ennemis
	// for(auto& ennemi:ennemis){
		// ennemi.modele3D=Model(ennemi.modele);
	// }
}



/*****************************************************************
							UPDATE
	****************************************************************/	



void Map::UpdateMove(int vitesseF, int vitesseL, int rotateF)
{
	if(!player.isInMovement &&!player.isTurning ){
		if(vitesseF != 0 || vitesseL != 0){
			player.isInMovement=true;
			player.moveEnnemi=true;
		}
		switch(player.orientation){
			case 2:
				if(vitesseF>0 && (schema[player.x][player.y-1]!=0 && AreDoorUnlocked(player.x,player.y-1) )){
					if(player.drowning<0.2 || schema[player.x][player.y-1]==2){
						player.y--;
					}
					else{
						player.drowning-=0.2;
					}
				}
				else if(vitesseF<0 && schema[player.x][player.y+1]!=0&& AreDoorUnlocked(player.x,player.y+1)){
					if(player.drowning<0.2 || schema[player.x][player.y+1]==2){
						player.y++;
					}
					else{
						player.drowning-=0.2;
					}
				}
				if(vitesseL>0 && schema[player.x+1][player.y]!=0 && AreDoorUnlocked(player.x+1,player.y)){
					if(player.drowning<0.2 || schema[player.x+1][player.y]==2){
						player.x++;
					}
					else{
						player.drowning-=0.2;
					}
				}
					
				else if(vitesseL<0 && schema[player.x-1][player.y]!=0 && AreDoorUnlocked(player.x-1,player.y)){
					if(player.drowning<0.2 || schema[player.x-1][player.y]==2){
						player.x--;
					}
					else{
						player.drowning-=0.2;
					}
				}
					
			break;
			case 3:
				if(vitesseF>0 && schema[player.x-1][player.y]!=0 && AreDoorUnlocked(player.x-1,player.y)){
					if(player.drowning<0.2|| schema[player.x-1][player.y]==2){
						player.x--;
					}
					else{
						player.drowning-=0.2;
					}
					
				}
				else if(vitesseF<0 && schema[player.x+1][player.y]!=0 && AreDoorUnlocked(player.x+1,player.y)){
					if(player.drowning<0.2 || schema[player.x+1][player.y]==2){
						player.x++;
					}
					else{
						player.drowning-=0.2;
					}
					
				}
				if(vitesseL>0 && schema[player.x][player.y-1]!=0&& AreDoorUnlocked(player.x,player.y-1)){
					if(player.drowning<0.2 || schema[player.x][player.y-1]==2){
						player.y--;
					}
					else{
						player.drowning-=0.2;
					}
					
				}
				else if(vitesseL<0 && schema[player.x][player.y+1]!=0&& AreDoorUnlocked(player.x,player.y+1)){
					if(player.drowning<0.2 || schema[player.x][player.y+1]==2){
						player.y++;
					}
					else{
						player.drowning-=0.2;
					}
					
				}
			break;
			case 0:
				if(vitesseF>0 && schema[player.x][player.y+1]!=0&& AreDoorUnlocked(player.x,player.y+1)){
					if(player.drowning<0.2 || schema[player.x][player.y+1]==2){
						player.y++;
					}
					else{
						player.drowning-=0.2;
					}
					
				}
				else if(vitesseF<0 && schema[player.x][player.y-1]!=0&& AreDoorUnlocked(player.x,player.y-1)){
					if(player.drowning<0.2 || schema[player.x][player.y-1]==2){
						player.y--;
					}
					else{
						player.drowning-=0.2;
					}
					
				}
				if(vitesseL>0 && schema[player.x-1][player.y]!=0 && AreDoorUnlocked(player.x-1,player.y)){
					if(player.drowning<0.2 || schema[player.x-1][player.y]==2){
						player.x--;
					}
					else{
						player.drowning-=0.2;
					}
					
				}
				else if(vitesseL<0 && schema[player.x+1][player.y]!=0 && AreDoorUnlocked(player.x+1,player.y)){
					if(player.drowning<0.2 || schema[player.x+1][player.y]==2){
						player.x++;
					}
					else{
						player.drowning-=0.2;
					}
					
			}
			break;
			case 1:
				if(vitesseF>0 && schema[player.x+1][player.y]!=0 && AreDoorUnlocked(player.x+1,player.y)){
					if(player.drowning<0.2 || schema[player.x+1][player.y]==2){
						player.x++;
					}
					else{
						player.drowning-=0.2;
					}
					
				}
				else if(vitesseF<0 && schema[player.x-1][player.y]!=0 && AreDoorUnlocked(player.x-1,player.y)){
					if(player.drowning<0.2 || schema[player.x-1][player.y]==2){
						player.x--;
					}
					else{
						player.drowning-=0.2;
					}
					
				}
				if(vitesseL>0 && schema[player.x][player.y+1]!=0&& AreDoorUnlocked(player.x,player.y+1)){
					if(player.drowning<0.2 || schema[player.x][player.y+1]==2){
						player.y++;
					}
					else{
						player.drowning-=0.2;
					}
					
				}
				else if(vitesseL<0 && schema[player.x][player.y-1]!=0&& AreDoorUnlocked(player.x,player.y-1)){
					if(player.drowning<0.2 || schema[player.x][player.y-1]==2){
						player.y--;
					}
					else{
						player.drowning-=0.2;
					}
					
				}
			break;
			default: 
			break;
			
		}
	}
	if(schema[player.x][player.y]==2 && player.drowning < .8f){
		player.drown();
	}
	else if(schema[player.x][player.y]!=2){
		player.drowning=0;
	}

	player.UpdateMove(vitesseF,vitesseL, rotateF);
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






void Map::UpdateEnnemis(bool tour)
{
	
	for(vector<Ennemi>::iterator it = ennemis.begin() ; it!=ennemis.end();it++)
	{
		Update(tour,this,*it);
	}

}


void Map::AttackEnnemis()
{
	
	for(vector<Ennemi>::iterator it = ennemis.begin() ; it!=ennemis.end();it++)
	{
		if((player.x+glm::sin(glm::radians(player.orientation*90.f+180.f))  == it->x) && (player.y+glm::cos(glm::radians(player.orientation*90.f+180.f))  == it->y))
		{
			it->pv = it->pv - player.attaque;
			
		}

		if(it->pv <= 0) 
		{
			ennemis.erase(it);
			if(ennemis.size()==0)
				break;
		}
	}
}











/*****************************************************************
							DRAW
	****************************************************************/		
	
	
	
	
	/*
*Affiche la carte uniquement dans les parties couvertes par la cam�ra
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









//Draws the menu
void Map::DrawMenu(float time, int hover){
	Cube cube;
	//mat4 MVMatrix;
	glBindTexture(GL_TEXTURE_2D,texturesMenu[0]);
	float floating = sin(time)/50.;
	glUniform1i(LocTexture,0);
	
	mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0,0, -4.f)); // Translation
	MVMatrix = glm::scale(MVMatrix, glm::vec3((3.5)*1.333,3.5,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);
	
	//mat4 MVMatrix;
	glBindTexture(GL_TEXTURE_2D,texturesMenu[6]);
	MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0,1.5, -3.95f)); // Translation
	MVMatrix = glm::scale(MVMatrix, glm::vec3((2)*1.333,2,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);
	
	glBindTexture(GL_TEXTURE_2D,texturesMenu[3]);
	

	
	MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2.f,-2.f, -3.9f)); // Translation
	if(hover == 1)
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.5+sin(2*M_PI*time)/25.)*1.333,1.5+sin(2*M_PI*time)/25.,1.));
	else
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.4+floating)*1.333,1.4+floating,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);

	glBindTexture(GL_TEXTURE_2D,texturesMenu[5]);
	
	MVMatrix = glm::translate(glm::mat4(1), glm::vec3(2.f,-1.8f, -3.9f)); // Translation
	if(hover == 2)
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.5+sin(2*M_PI*time)/25.)*1.333,1.5+sin(2*M_PI*time)/25.,1.));
	else
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.4+floating)*1.333,1.4+floating,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
	
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);
	
}
//Draws the victory menu
void Map::DrawVictoire(float time, int hover){
	Cube cube;
	//mat4 MVMatrix;
	glBindTexture(GL_TEXTURE_2D,texturesMenu[0]);
	float floating = sin(time)/50.;
	glUniform1i(LocTexture,0);
	
	mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0,0, -4.f)); // Translation
	MVMatrix = glm::scale(MVMatrix, glm::vec3((3.5)*1.333,3.5,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);
	
	//mat4 MVMatrix;
	glBindTexture(GL_TEXTURE_2D,texturesMenu[2]);
	MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0,1.5, -3.95f)); // Translation
	MVMatrix = glm::scale(MVMatrix, glm::vec3((2)*1.333,2,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);
	
	glBindTexture(GL_TEXTURE_2D,texturesMenu[4]);
	

	
	MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2.f,-2.f, -3.9f)); // Translation
	if(hover == 1)
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.5+sin(2*M_PI*time)/25.)*1.333,1.5+sin(2*M_PI*time)/25.,1.));
	else
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.4+floating)*1.333,1.4+floating,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);

	glBindTexture(GL_TEXTURE_2D,texturesMenu[5]);
	
	MVMatrix = glm::translate(glm::mat4(1), glm::vec3(2.f,-1.8f, -3.9f)); // Translation
	if(hover == 2)
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.5+sin(2*M_PI*time)/25.)*1.333,1.5+sin(2*M_PI*time)/25.,1.));
	else
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.4+floating)*1.333,1.4+floating,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
	
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);
}


//Draws the victory menu
void Map::DrawDefaite(float time, int hover){
	Cube cube;
	//mat4 MVMatrix;
	glBindTexture(GL_TEXTURE_2D,texturesMenu[0]);
	float floating = sin(time)/50.;
	glUniform1i(LocTexture,0);
	
	mat4 MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0,0, -4.f)); // Translation
	MVMatrix = glm::scale(MVMatrix, glm::vec3((3.5)*1.333,3.5,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);
	
	//mat4 MVMatrix;
	glBindTexture(GL_TEXTURE_2D,texturesMenu[1]);
	MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0,1.5, -3.95f)); // Translation
	MVMatrix = glm::scale(MVMatrix, glm::vec3((2)*1.333,2,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);
	
	glBindTexture(GL_TEXTURE_2D,texturesMenu[4]);
	

	
	MVMatrix = glm::translate(glm::mat4(1), glm::vec3(-2.f,-2.f, -3.9f)); // Translation
	if(hover == 1)
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.5+sin(2*M_PI*time)/25.)*1.333,1.5+sin(2*M_PI*time)/25.,1.));
	else
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.4+floating)*1.333,1.4+floating,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);

	glBindTexture(GL_TEXTURE_2D,texturesMenu[5]);
	
	MVMatrix = glm::translate(glm::mat4(1), glm::vec3(2.f,-1.8f, -3.9f)); // Translation
	if(hover == 2)
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.5+sin(2*M_PI*time)/25.)*1.333,1.5+sin(2*M_PI*time)/25.,1.));
	else
		MVMatrix = glm::scale(MVMatrix, glm::vec3((1.4+floating)*1.333,1.4+floating,1.));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(-1.f,0.f,0.f));
	MVMatrix = glm::rotate(MVMatrix,glm::radians(90.f),glm::vec3(0.f,0.f,-1.f));
	glUniformMatrix4fv(LocMVPMatrix,1,GL_FALSE,glm::value_ptr(getProjMatrix() * MVMatrix));
	glUniformMatrix4fv(LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
	
	glDrawArrays(GL_TRIANGLES,0,cube.getVertexCount()/6);
}



void Map::DrawMeshes(GLint id, float time){//, Model m){
	 // MVMatrix = glm::translate(glm::mat4(1), glm::vec3(0,0,-5*sin(time))); // Translation
				// MVMatrix = glm::rotate(MVMatrix, time, glm::vec3(1.,0.,0.));
				// MVMatrix = player.camera.getViewMatrix()*MVMatrix;
				 // glUniformMatrix4fv (LocMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix * MVMatrix));
				 // glUniformMatrix4fv (LocMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
				 // glUniformMatrix4fv (LocNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
				 
				 // m.Draw();
}









	
	
	
	
	
	
	
	
	
	
	
	
/*****************************************************************
							DESTRUCTEUR
	****************************************************************/	
	/*
*Nettoie la m�moire de tous les pointeurs de la carte
*/
Map::~Map()
{

	free(this->schema);
}


void Map::cleanTextures()
{
	glDeleteTextures(1,&this->textures[0]);
    glDeleteTextures(1,&this->textures[1]);
	glDeleteTextures(1,&this->textures[2]);
	glDeleteTextures(1,&this->textures[3]);
	glDeleteTextures(1,&this->textures[4]);
	glDeleteTextures(1,&this->textures[5]);
	
	glDeleteTextures(1,&this->texturesMenu[0]);
    glDeleteTextures(1,&this->texturesMenu[1]);
	glDeleteTextures(1,&this->texturesMenu[2]);
	glDeleteTextures(1,&this->texturesMenu[3]);
	glDeleteTextures(1,&this->texturesMenu[4]);
	glDeleteTextures(1,&this->texturesMenu[5]);
	glDeleteTextures(1,&this->texturesMenu[6]);
}


	
	


