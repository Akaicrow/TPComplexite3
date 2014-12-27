#include "main.h"
#include "gestionLitterauxEtClauses.h"

int lecture(char* cheminFichier,Clause **tabClauses,Litteral **tabLitteraux,int *nbClauses,int *nbLitteraux)
{
	//Déclaration de variables
	FILE *fichier=NULL;
	int i,j,k,valeurLitteral;
	char ligne[256],nombre[10],nombre2[10];
	
	//Ouverture du fichier en mode lecture
	fichier=fopen(cheminFichier,"r");
	if(fichier==NULL)
	{
		fprintf(stderr,"fichier %s non lu\n",cheminFichier);
		return -1;
	}
	
	//Ignorer les 7 première lignes des fichiers
	for(i=0;i<7;i++) fgets(ligne,255,fichier);
	
	//Lecture de la ligne du nombre de littéraux et du nombre de clauses
	fgets(ligne,255,fichier);
	
	//Test que la ligne commence bien par un p dans le cas inverse le fichier a un fomat non supporté
	if(ligne[0]!='p')
	{
		fprintf(stderr,"le format du fichier %s est erroné\n",cheminFichier);
		return -1;
	}
	
	//Saut des caractères jusqu'au premier chiffre (sauf 0)
	i=1;
	while(ligne[i]<'1' || ligne[i]>'9') i++;
	
	//Lecture du nombre de littéraux
	j=0;
	while(ligne[i]!=' ')
	{
		nombre[j]=ligne[i];
		i++;
		j++;
	}
	nombre[j]='\0';
	*nbLitteraux=(int)strtol(nombre,NULL,10);
	
	//Saut des caractères jusqu'au premier chiffre (sauf 0)
	while(ligne[i]<'1' || ligne[i]>'9') i++;
	
	//Lecture du nombre de clauses
	j=0;
	while(ligne[i]!=' ')
	{
		nombre2[j]=ligne[i];
		i++;
		j++;
	}
	nombre2[j]='\0';
	*nbClauses=(int)strtol(nombre2,NULL,10);
	
	//Allocation de la table de clauses
	*tabClauses=(Clause*)malloc(*nbClauses*sizeof(Clause));
	
	//Allocation de la table de littéraux
	*tabLitteraux=(Litteral*)malloc((*nbLitteraux+1)*sizeof(Litteral));
	
	//Initialisation des valeurs de vérité et des putre de tout les littéraux aux valeurs par défaut
	for(i=1;i<=*nbLitteraux;i++)
	{
		(*tabLitteraux)[i].valeurDeVerite=0;//0 <=> valeur de vérité inconnue
		(*tabLitteraux)[i].purte='0';//'0' pureté inconnue
	}
	
	//Lecture des clauses et variables 
	for(i=0;i<*nbClauses;i++)
	{
		fgets(ligne,255,fichier);
		(*tabClauses)[i].nbLitteraux=0;
		(*tabClauses)[i].tete=NULL;
		j=0;k=0;
		while(ligne[j]!='\n')
		{
			if(ligne[j]==' ')
			{
				if(j>0) //Si l'espace n'est pas en début de ligne
				{
					//Ajouter le nombre a la liste des litteraux de la clause
					nombre[k]='\0';
					valeurLitteral=(int)strtol(nombre,NULL,10);
					if(valeurLitteral)//la variable valeurLitteral est différente de 0
					{
						if(ajouterLitteral(&((*tabClauses)[i]),valeurLitteral)==0)//si l'ajoute du littéral a la clause s'est bien passé
						{
							//Mettre a jour purte initiale
							if(valeurLitteral<0)
							{
								if((*tabLitteraux)[-1*valeurLitteral].purte=='0') (*tabLitteraux)[-1*valeurLitteral].purte='n';
								else if((*tabLitteraux)[-1*valeurLitteral].purte=='p') (*tabLitteraux)[-1*valeurLitteral].purte='i';
							}
							else //La variable valeurLitteral > 0 (strictement)
							{
								if((*tabLitteraux)[valeurLitteral].purte=='0') (*tabLitteraux)[valeurLitteral].purte='p';
								else if((*tabLitteraux)[valeurLitteral].purte=='n') (*tabLitteraux)[valeurLitteral].purte='i';
							}
						}
					}
					k=0;
				}
			} 
			else if(((ligne[j]>='0')&&(ligne[j]<='9'))||(ligne[j]=='-'))//si c'est un chiffre ou '-'
			{
				if(ligne[j]=='-')
				{
					if((j-1>0)&&(ligne[j-1]!=' ')) //Afin d'éviter d'avoir des '-' au mileu d'un nombre
					{
						fprintf(stderr,"Fichier erronné (- précédé par autre chose que blanc), clause %d erronée\n",i);
						break;
					}
					else
					{
						k=1;
						nombre[0]='-';
					}
				}
				else //C'est un chiffre
				{
					nombre[k]=ligne[j];
					k++;
				}	
			}
			else //C'est ni un blanc ni un chiffre ni un '-' ni un retour a la ligne 
			{
				fprintf(stderr,"caractère non pris en charge dans la clause %d, clause %d erronnée\n",i,i);
				break;
			}
			j++;
		}
	} 
	return 0;
}
