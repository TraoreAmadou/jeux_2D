#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structs.h"
#include <math.h>
#include <SDL2/SDL_ttf.h>

          /*----------------------------------------------------------------------*/

int main(int argc, char **argv)
{
	FILE *fichier = NULL;
	fichier = fopen(argv[1], "r");   //Ouverture du fichier

	if(fichier == NULL || argc != 2) //Verification de l'ouverture du fichier et du nombre d'arguments en ligne de commande
		{
			printf("\t\tPOUR JOUER, SAISISSEZ APRES L'EXECUTABLE LE NOM DU FICHIER CONTENANT LES ELEMENTS DU CIRCUIT\n\t\tEXEMPLE SUR LINUX : ./prog texte.txt SI VOUS AVEZ NOMME LE FICHIER \n\t\tEXECUTABLE prog ET LE FICHIER DU CIRCUIT texte.txt\n\t\t prog texte.txt POUR LE MEME EXEMPLE SUR WINDOWS\n");
			return 0;
		}
	char *chaine, *val;
	int lecture_max = 50, i, j=0, cpt=0; 
	char *delimiteur={" \n\t\r"};

	chaine = (char*)malloc(sizeof(char)*lecture_max);
	val = (char*)malloc(sizeof(char)*lecture_max);


	zone *tab;


	voiture V;
	Init_Voiture(&V);

	circuit C;
	Init_Circuit(&C);


	while( fgets(chaine, lecture_max, fichier)!= NULL)// tant qu'on peut lire une ligne du fichier soit tant que le fichier n'est pas vide
	{
		val = strtok(chaine,delimiteur);//On extrait le premier mot d'une ligne
		
		if(strcmp("TAILLE",val)==0)
		{
			val = strtok(NULL,delimiteur);
			C.nb_colonnes = atoi(val);
			val = strtok(NULL,delimiteur);
			C.nb_lignes = atoi(val);
			C.matrice = (int**)malloc(sizeof(int*)*C.nb_lignes);


			for(i=0;i<C.nb_lignes;i++)
				C.matrice[i] = (int *)malloc(sizeof(int)*C.nb_colonnes);

		}
		/*Recuperation de la taille et allocation de memoire pour la 
		recuperation de la matrice representant les positions 
		des differents elements du circuit*/

		else if(strcmp("FOND",val)==0)
		{

			val = strtok(NULL,delimiteur);
			strcpy(C.chemin_fond,val);
		}

		//Recuperation du chemin de l'image de fond

		else if(strcmp("NBZONES",val)==0)
		{
			val = strtok(NULL,delimiteur);
			tab = malloc(sizeof(zone)*atoi(val));
		}

		//Recuperation du nombre de zones


		else if(strcmp("VOITURE",val)==0)
		{
			val = strtok(NULL,delimiteur);
			strcpy(V.chemin_voiture,val);
		}


		//Recuperation du chemin de l'image de la voiture


		else if(strcmp("ZONE",val)==0)
		{

			val = strtok(NULL,delimiteur);
			tab[j].numero = atoi(val);
			val = strtok(NULL,delimiteur);
			strcpy(tab[j].chemin,val);
			j++;
		}

		//Recuperation du chemin des differents elements du circuit


		else if(strcmp("DEPART",val)==0)
		{
			val = strtok(NULL,delimiteur);
			V.Xdep = atoi(val);
			val = strtok(NULL,delimiteur);
			V.Ydep = atoi(val);

		}

		//Recuperation des coordonnees de la voiture au depart 


		else if(strcmp("ARRIVEE",val)==0)
		{
			val = strtok(NULL,delimiteur);

		}


		//Recuperation des coordonnees de la voiture a l'arrivee


		else if(strcmp("ANGLE_DEPART",val)==0)
		{
			val = strtok(NULL,delimiteur);
			V.angle = atof(val);
			printf("%f & %f", sin(V.angle*pi/180), cos(V.angle*pi/180));

		}

		//Recuperation de l'angle de la voiture


		else //Recuperation du reste de fichier soit la matrice de circuit
		{	
			j = 0;
			C.matrice[cpt][j] = atoi(val);
			while(val != NULL)
			{
				j++;
				val = strtok(NULL, delimiteur);
				if(val != NULL)
					C.matrice[cpt][j] = atoi(val);
			}
			cpt++;

		}
	}


 	SDL_Texture *tile = NULL;
 	SDL_Rect rectangle_tile={0 ,0 ,largeur_fond/C.nb_colonnes,hauteur_fond/C.nb_lignes}; 


 	if(SDL_Init(SDL_INIT_VIDEO) != 0)
 	{

		fermeture(C, tab, val, chaine, fichier);
 		SDL_Erreur("ERREUR : Initialisation SDL echouee",NULL,NULL,1,NULL);
 	}

	TTF_Init();

	CreationFenetreEtRendu(&C);

 	if(C.fenetre == NULL || C.rendu == NULL)
 	{

		fermeture(C, tab, val, chaine, fichier);
 		SDL_Erreur("ERREUR : Initialisation SDL echouee",C.fenetre,C.rendu,1,NULL);
 	}

	texte T;
	Init_Texte(&T);


	V.Rvoiture.x = (4*rectangle_tile.w/3) *V.Xdep; //abcisse de la voiture au depart
	V.Rvoiture.y = (rectangle_tile.h+10)*V.Ydep; //ordonnee de la voiture au depart

	V.Rvoiture.w = rectangle_tile.w/4;
	V.Rvoiture.h = 319*V.Rvoiture.w/158;


	EcranDebut(&T, &V, tile, rectangle_tile, &C, tab, "Bienvenue dans cette course de voiture");//Affiche le texte entre quotes sur l'ecran de jeu


	EcranDebut(&T, &V, tile, rectangle_tile, &C, tab, "Utilisez les fleches pour diriger la voiture");


	EcranDebut(&T, &V, tile, rectangle_tile, &C, tab, "Et l'espace si vous voulez accelerer");
	

	EcranDebut(&T, &V, tile, rectangle_tile, &C, tab, "Pour gagner, c'est simple");


	EcranDebut(&T, &V, tile, rectangle_tile, &C, tab, "Faites 3 tours en 2 minutes 30");



	char infos[30]; 
	int compteur = 3;

	while(compteur >= 0)
	{
		if(compteur==0)
		{
			strcpy(infos,"Gooo");
		}else{
			sprintf(infos, "Depart dans : %d", compteur);
		}

		EcranAvecTexte(&T, &V, tile, rectangle_tile, &C, tab, infos);
		SDL_Delay(1000);
		compteur --;
	}


	int  t0 = SDL_GetTicks();

	T.Rtexte.w = largeur_fond /5;
	T.Rtexte.y = 0;
	
	jeu(V, tile, rectangle_tile, C, tab, t0, &T);

	fermeture(C, tab, val, chaine, fichier);

	SDL_Destroy(C.fenetre,C.rendu,3,C.fond,T.tex, tile);
 
	
}