#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structs.h"
#include <math.h>
#include <SDL2/SDL_ttf.h>

void Init_Circuit(circuit *C)
{
	(*C).nb_colonnes = 0;
	(*C).nb_lignes = 0;
	(*C).matrice = NULL;
	(*C).fenetre = NULL;
	(*C).rendu = NULL ;
	(*C).fond = NULL;
	(*C).Rfond.x = 0;
	(*C).Rfond.y = 0;
	(*C).Rfond.w = largeur_fond;
	(*C).Rfond.h = hauteur_fond;
}



void Init_Voiture(voiture *V)
{
	(*V).Xdep = 0;
	(*V).Ydep =0;
	(*V).angle = 0;
	(*V).vitesse = 0;
	(*V).Rvoiture.x = 0;
	(*V).Rvoiture.y = 0;
}



void Init_Texte(texte *T)
{
	(*T).image = NULL;
	(*T).tex = NULL;
	(*T).couleur.r = 0;
	(*T).couleur.g = 0;
	(*T).couleur.b = 0;
	(*T).Rtexte.x = 0;
	(*T).Rtexte.y =5*hauteur_fond/6;
	(*T).Rtexte.w =largeur_fond/2;
	(*T).Rtexte.h =hauteur_fond/10;
}



void CreationFenetreEtRendu(circuit *C)
{
	(*C).fenetre = SDL_CreateWindow("COURSE DE VOITURE", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,largeur_fond,hauteur_fond,0);
 	(*C).rendu = SDL_CreateRenderer((*C).fenetre, -1, SDL_RENDERER_SOFTWARE);
}



void AffichageTexte(texte *T, circuit *C, const char *text)
{
	TTF_Font *police = TTF_OpenFont("Arimo-Italic-Latin.ttf", 60); //charge la police d'ecriture
	(*T).image = TTF_RenderText_Solid(police, text, (*T).couleur);//affiche le texte dans la variable text a l'ecran
	(*T).tex = SDL_CreateTextureFromSurface((*C).rendu, (*T).image);
	SDL_FreeSurface((*T).image);
	SDL_RenderCopy((*C).rendu, (*T).tex, NULL, &(*T).Rtexte);	
	SDL_DestroyTexture((*T).tex);
	TTF_CloseFont(police);
}



void fermeture(circuit C, zone* tab, char* val, char *chaine, FILE* f)
{
	int i;
	free(tab);
	for(i=0;i<C.nb_lignes;i++)
		free(C.matrice[i]);
	free(C.matrice);

	free(val);
	free(chaine);
	fclose(f);
}



void SDL_Destroy(SDL_Window *w, SDL_Renderer *r,int nbre_textures, SDL_Texture *t, ...)
{
	int i;
	va_list liste;
	va_start(liste, t);
	for(i=0;i<nbre_textures;i++)
	{
		if(t!=NULL)
			 SDL_DestroyTexture(t);
		t = va_arg(liste, SDL_Texture *);
	}
	va_end(liste);
	if(r!=NULL)
		 SDL_DestroyRenderer(r);
	if(w!=NULL)
		 SDL_DestroyWindow(w);

	TTF_Quit();
	SDL_Quit();
}



void SDL_Erreur(const char *message,SDL_Window *w, SDL_Renderer *r, int nbre_textures, SDL_Texture *t, ...)
{
	SDL_Log("%s > %s\n",message, SDL_GetError());
	int i;
	va_list liste;
	va_start(liste, t);
	for(i=0;i<nbre_textures;i++)
	{
		if(t!=NULL)
			 SDL_DestroyTexture(t);
 		t = va_arg(liste, SDL_Texture *);
	}
	SDL_Destroy(w,r,1,NULL);
	exit(EXIT_FAILURE);
}



void affichageCircuit(SDL_Texture *tile, zone *tab, SDL_Rect rect, circuit *C)
{
	int i, j;
	(*C).fond = IMG_LoadTexture((*C).rendu, (*C).chemin_fond);
	SDL_RenderCopy((*C).rendu, (*C).fond, NULL, &(*C).Rfond);
	SDL_DestroyTexture((*C).fond);
	for(i=0;i<(*C).nb_lignes;i++) //place chaque portion de route a la place correpondante et detruit la texture
	{

		for(j=0;j<(*C).nb_colonnes;j++)
		{
			rect.x = largeur_fond / (*C).nb_colonnes *j;
			rect.y = hauteur_fond / (*C).nb_lignes *i;
			if((*C).matrice[i][j] != 0)
			{
				tile = IMG_LoadTexture((*C).rendu, tab[(*C).matrice[i][j]].chemin);
				SDL_RenderCopy((*C).rendu, tile, NULL, &rect);
				SDL_DestroyTexture(tile);
			}
		}
	}
}



void affichageVoiture(voiture *V, SDL_Texture *tile, circuit C)
{
	tile = IMG_LoadTexture(C.rendu, (*V).chemin_voiture);
	SDL_RenderCopyEx(C.rendu,tile, NULL, &(*V).Rvoiture, (*V).angle, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(C.rendu);
}



void EcranAvecTexte(texte *T, voiture *V, SDL_Texture *tile, SDL_Rect rect, circuit *C, zone *tab, const char *text)
{ 
	SDL_RenderClear((*C).rendu);//efface le rendu precedent
	affichageCircuit(tile, tab, rect, C);
	AffichageTexte(T, C, text);
	affichageVoiture(V, tile, *C);
	SDL_DestroyTexture(tile);
}



void EcranDebut(texte *T, voiture *V, SDL_Texture *tile, SDL_Rect rect, circuit *C, zone *tab, const char *text)
{
	EcranAvecTexte(T, V, tile, rect, C, tab, text); 
	SDL_Delay(2000);
}



void genererPos(voiture *V)
{
	float inc_x, inc_y;
	inc_x = (*V).vitesse * sin((*V).angle*pi/180);
	inc_y = -1 * (*V).vitesse * cos((*V).angle*pi/180);

	(*V).Rvoiture.x = (*V).Rvoiture.x + inc_x; 
	(*V).Rvoiture.y = (*V).Rvoiture.y + inc_y;

}



void jeu(voiture V,SDL_Texture *tile, SDL_Rect rect,circuit C, zone *tab, int t0, texte *T) 
{
	double inc_v, inc_angle, Vmax=7;

	SDL_bool jouer = SDL_TRUE; //debut du jeu

	SDL_Event event;
	char affiche[15];
	int i, j;
	int nbre_tour =0;

	while(jouer) //tant qu'on joue
	{

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{

				case SDL_QUIT :   
					jouer = SDL_FALSE;
					break;
					// On ferme la fenetre du jeu

				case SDL_KEYDOWN :

					switch(event.key.keysym.sym)
					{
						case  SDLK_SPACE : //On appuie sur l'espace

								if(V.vitesse<Vmax)
								{	
									inc_v = 1;
									V.vitesse = V.vitesse + inc_v;
								}else{

									V.vitesse = Vmax;
								}
							
								genererPos(&V);	
					 
							continue;

						case SDLK_UP : //On appuie sur la flèche vers le haut

							V.vitesse = 4;
							
								genererPos(&V);
							
							continue;


						case SDLK_DOWN :   //On appuie sur la flèche vers le bas

								V.vitesse = -4;

								genererPos(&V);
							
							continue;


						case SDLK_LEFT :  //On appuie sur la flèche vers la gauch
							inc_angle = -2;
							V.angle = V.angle + inc_angle;
							V.vitesse = 4;
						
							genererPos(&V);
							
							continue;


						case SDLK_RIGHT :
							inc_angle = 2;
							V.angle = V.angle + inc_angle;
							V.vitesse = 4;

							genererPos(&V);
							
							continue;

						default :
							continue;

					}
				case SDL_KEYUP :

					switch(event.key.keysym.sym)
					{
						case  SDLK_SPACE : 		//On relache l'espace
							inc_v = -0.2;
							V.vitesse = V.vitesse + inc_v;

								genererPos(&V);
		
							continue;

						case SDLK_UP :
							V.vitesse = 2;

								genererPos(&V);
								
						
							continue; //On relache la fleche du haut

						case SDLK_LEFT :
							V.vitesse = 0;

								genererPos(&V);
			
						
							continue;	//On relache la fleche de gauche	


						case SDLK_RIGHT : //On relache la fleche de gauche
							V.vitesse = 0;

							
								genererPos(&V);
					
							continue;


						default :
							continue;

					}

				default :
					continue;
			}
		}

	int temps = SDL_GetTicks() - t0; //temps en millisecondes depuis le debut du jeu
	int minute, seconde;

	seconde = temps/1000 %60; 
	minute = temps/60000;

	//Conversion en minutes et secondes

	sprintf(affiche, "%d : %d Tour %d/3", minute, seconde, nbre_tour);
	EcranAvecTexte(T, &V, tile, rect, &C, tab, affiche);
	//Affichage du nombre de tours, du temps et de la position de la voiture
	}
}






