/*************************************************************************
Ville  -  description
-------------------
d�but                : 19/10/2015
copyright            : (C) 2015 par Pericas-Belletier
*************************************************************************/

//---------- R�alisation de la classe <Ville> (fichier Ville.cpp) --

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include syst�me
using namespace std;
#include <iostream>

//------------------------------------------------------ Include personnel
#include "Ville.h"

//------------------------------------------------------------- Constantes

//---------------------------------------------------- Variables de classe

//----------------------------------------------------------- Types priv�s


//----------------------------------------------------------------- PUBLIC
//-------------------------------------------------------- Fonctions amies

//----------------------------------------------------- M�thodes publiques
// type Ville::M�thode ( liste de param�tres )
// Algorithme :
//
//{
//} //----- Fin de M�thode


void Ville::StatsJour( int d7 )
{
	// Creation de la structure de retour et variables pratiques
	Vecteur<double> statsRetour;
	int total = 0;

	// Init de la structure d'affichage
	for ( int i = 0; i < 4; i++ )
	{
		statsRetour.insererFin(0);
	}

	// Parcours du tableau de capteurs et remplissage de la structure de retour
	for ( int i = 0; i < nombreCapteurs; i++ )
	{
		for (int j = 0; j < 4; j++)
		{
			statsRetour[j] += (capteurs[i]->DonneesJour(d7))[j];	// TODO: plus rapide a l'execution si on stocke dans une variable ou pas ?
		}
		total += (capteurs[i]->DonneesJour(d7))[4];
	}

	// Creation des stats
	if (total != 0)
	{
		for ( int i = 0; i < 4; i++ )
		{
			statsRetour[i] /= total;
		}
	}

	// Affichage
	for ( int i = 0; i < 4; i++ )
	{
		cout << statsRetour[i] * 100 << "%\r\n";	// TODO: quelle tronche a la fin de ligne ?
	}

} //----- Fin de statsJour

void Ville::EmbouteillageJour( int d7 )
// Algorithme :
{
	// Cr�ation structure pour affichage et variables pratiques
	Vecteur<int> stats;
	Vecteur<int> total;
	
	// Init de la structure d'affichage
	for ( int i = 0; i < 24; i++ )
	{
		stats.insererFin(0);
		total.insererFin(0);
	}

	// Remplissage de la structure d'affichage
	for (int i = 0; i < nombreCapteurs; i++)
	{
		for ( int j = 0; j < 24; j++ )
		{
			stats[j] += (capteurs[i]->EmbouteillageJour(d7))[j];	// TODO: plus rapide a l'execution si on stocke dans une variable ou pas ?
			total[j] += (capteurs[i]->EmbouteillageJour(d7))[j + 24];
		}
	}

	// Creation des stats et affichage
	for ( int i = 0; i < 24; i++ )
	{
		if ( total[i] != 0 )
		{
			stats[i] /= total[i];
		}
		// Affichage
		cout << d7 << " " << i << " " << stats[i] * 100 << "%\r\n";		// TODO: quelle tronche a la fin de ligne ?
	}

} //----- Fin de EmbouteillageJour

void Ville::TempsParcours( int hDebut, int hFin, int d7, Vecteur<int>& idSegments )
// Algorithme :	TODO: complexite en n^4... A ameliorer sans doute
{
	// Cr�ation variables pour affichage et variables pratiques
	int temps;
	int meilleurTemps;
	int meilleureMinute;
	int meilleureHeure;
	int nombreMinutes = (hFin - hDebut + 1) * 60;
	int heureActuelle = hDebut;
	int minuteActuelle = 0;
	Vecteur<Capteur*> capteursSegment;

	// Init variables
	temps = 0;
	meilleurTemps = 9999999;		// TODO : change this
	meilleureHeure = 0;
	meilleureMinute = 0;

	// Prise une fois pour toutes des capteurs qui nous int�ressent
	for ( int i = 0; i < idSegments.GetTaille(); i++ )
	{
		for ( int j = 0; j < NB_MAX_CAPTEURS; j++ )
		{
			if ( capteurs[j]->GetID() == idSegments[i] )
			{
				capteursSegment.insererFin(capteurs[j]);
				break;
			}
		}
	}

	// Recherche du meilleur temps
	for ( int heure = hDebut; heure <= hFin; heure++ )
	{
		for ( int minute = 0; minute < 60; minute++ )
		{
			for (int i = 0; i < idSegments.GetTaille(); i++)
			{
				temps += capteursSegment[i]->TempsSegment(d7, heure, minute);
			}
			// En cas de nouveau meilleur temps : mise a jour
			if (temps < meilleurTemps)
			{
				meilleurTemps = temps;
				meilleureHeure = heure;
				meilleureMinute = minute;
			}
		}
	}

	// Affichage
	cout << d7 << " " << meilleureHeure << " " << meilleureMinute << " " << meilleurTemps << "%\r\n";	// TODO: quelle tronche a la fin de ligne ?

} //----- Fin de TempsParcours

void Ville::AjouterEvenement( int id, Evenement & evenement )
// Algorithme :
{
	for (int i = 0; i < nombreCapteurs; i++)
	{
		if (capteurs[i]->GetID() == id)
		{
			capteurs[i]->Inserer( evenement );
			return;
		}
	}

	CreerCapteur( id );
	capteurs[nombreCapteurs - 1]->Inserer( evenement );

} //----- Fin de AjouterEvenement

//------------------------------------------------- Surcharge d'op�rateurs
Ville &Ville::operator = ( const Ville &uneVille )
// Algorithme :
{
	nombreCapteurs = uneVille.nombreCapteurs;
	for (int i = 0; i < nombreCapteurs; i++)
	{
		capteurs[i] = new Capteur(*uneVille.capteurs[i]);
	}

	return *this;

} //----- Fin de operator =


Capteur& Ville::operator[] ( int idCapteur )
// Algorithme :
{
	return *capteurs[idCapteur];	// TODO: not sure du resultat
} //----- Fin de operator []

//-------------------------------------------- Constructeurs - destructeur
Ville::Ville ( const Ville &uneVille )
// Algorithme :
{
#ifdef MAP
	cout << "Appel au constructeur de copie de <Ville>" << endl;
#endif

	nombreCapteurs = uneVille.nombreCapteurs;
	for ( int i = 0; i < nombreCapteurs; i++ )
	{
		capteurs[i] = new Capteur(*uneVille.capteurs[i]);
	}

} //----- Fin de Ville (constructeur de copie)


Ville::Ville () : nombreCapteurs(0)
// Algorithme :
{
#ifdef MAP
	cout << "Appel au constructeur de <Ville>" << endl;
#endif
	//Rien de plus a faire a priori
} //----- Fin de Ville


Ville::~Ville ()
// Algorithme :
{
#ifdef MAP
	cout << "Appel au destructeur de <Ville>" << endl;
#endif
	for ( int i = 0; i < nombreCapteurs; i++ )
	{
		delete capteurs[i];
	}
} //----- Fin de ~Ville


//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- M�thodes prot�g�es
void Ville::CreerCapteur( int id )
// Algorithme : allocation dynamique d'un nouveau capteur et "insertion" dans le tableau
{
	// La verification de l'existence du capteur a �t� trait�e avant l'appel � cette fonction
	Capteur* capteur = new Capteur( id );
	capteurs[nombreCapteurs++] = capteur;

} //----- Fin de creerCapteur

//------------------------------------------------------- M�thodes priv�es
