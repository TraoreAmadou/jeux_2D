
#ifndef STRUCTS_H 
#define STRUCTS_H


#define largeur_fond 1000
#define hauteur_fond 800

#define pi 3.14159

typedef struct zone
{
	int numero;
	char chemin[50];
}zone;//Declaration d'une variable zone avec toutes les caracteristiques d'une zone dans le jeu


typedef struct circuit
{
	int nb_colonnes;
	int nb_lignes;
	int **matrice;
	SDL_Window	*fenetre;
	SDL_Renderer *rendu;
	SDL_Texture *fond;
	SDL_Rect Rfond;
	char chemin_fond[50];
}circuit;  //Declaration d'une variable circuit avec toutes les caracteristiques du circuit du jeu et de l'affichage du fond


typedef struct voiture
{
	int Xdep;
	int Ydep;
	double angle;
	double vitesse;
	SDL_Rect Rvoiture;
	char chemin_voiture[50];
}voiture;  //Declaration d'une variable voiture avec toutes les caracteristiques de la voiture du jeu


typedef struct texte
{
	SDL_Surface *image;
	SDL_Texture *tex;
	SDL_Color couleur;
	SDL_Rect Rtexte;
}texte; //Declaration d'une variable texte avec les elements du texte qui va etre affiche sur le jeu


void Init_Circuit(circuit *C);//Initialise les elements de la structure circuit a 0 ou NULL



void Init_Voiture(voiture *V);//Initialise les elements de la structure voiture a 0 ou NULL



void Init_Texte(texte *T);//Initialise les elements de la structure texte a 0 ou NULL



void CreationFenetreEtRendu(circuit *C);//Cree une fentre et un rendu



void AffichageTexte(texte *T, circuit *C, const char *text);//Affiche du texte sur l'ecran du jeu



void fermeture(circuit C, zone* tab, char* val, char *chaine, FILE* f);//Libere toute la memoire allouee avec malloc et ferme le fichier du circuit



void SDL_Destroy(SDL_Window *w, SDL_Renderer *r,int nbre_textures, SDL_Texture *t, ...);//Detruit toutes les structures SDL creees puis quitte SDL



void SDL_Erreur(const char *message,SDL_Window *w, SDL_Renderer *r, int nbre_textures, SDL_Texture *t, ...);//Affiche une erreur s'il y'en a puis quitte proprement SDL


void affichageCircuit(SDL_Texture *tile, zone *tab, SDL_Rect rect, circuit *C);//Affiche le circuit


void affichageVoiture(voiture *V, SDL_Texture *tile, circuit C);//Affiche la voiture


void EcranAvecTexte(texte *T, voiture *V, SDL_Texture *tile, SDL_Rect rect, circuit *C, zone *tab, const char *text);//Affiche l'ecran avec un texte


void EcranDebut(texte *T, voiture *V, SDL_Texture *tile, SDL_Rect rect, circuit *C, zone *tab, const char *text);//Affiche l'ecran avec un texte qui disparait en 2s


void genererPos(voiture *V); //fonction qui genere la position de la voiture

void jeu(voiture V,SDL_Texture *tile, SDL_Rect rect,circuit C, zone *tab, int t0, texte *T); //Fonction qui gere tous les evenements du jeu : temps, tours, deplacement de la voiture...




#endif