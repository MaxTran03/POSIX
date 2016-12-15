#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>

#define NB_MAX_FILE 20
/*Fonctions de la bibliothèque de gestion des fichiers indexés 
Variable globale
Tableau tel que size_file[fd] contient la taille du type des données stockées dans le fichier de descripteur fd*/
int size_file [NB_MAX_FILE];


/**
 * Ouvre un fichier indexe.
 * Fichier cree ou ecrase => ecrire la taille de l'index en debut de fichier
 * Sinon => lire la taille de l'index en debut de fichier
 * 	path   nom du fichier a ouvrir
 * 	flags  mode d'ouverture du fichier
 * 	ind    taille de la structure index (ne sert que si le fichier est cree ou ecrase)
 * 	mode   droits d'acces au fichier
 * Renvoie le descripteur du fichier ouvert, -1 en cas de pb.
 */
int indopen(const char *path, int flags, int ind, mode_t mode)
{
	int desc ;
   int buf;
   int existe = 0 ;
   /* Fichier existe ? */
   existe = !(access(path, F_OK) == -1);

	if( (desc= open(path, flags, mode))==-1 )
	{
		perror("Erreur d'ouverture\n") ;
		return -1 ;
	}
	if ( ((existe == 1) && ((flags & O_TRUNC) == O_TRUNC)) || ((existe == 0) && ((flags & O_CREAT) == O_CREAT)) ) /*fichier écrasé ou non existant */
	{
		size_file[desc]= ind ;
		if(write(desc,&ind,sizeof(int)) ==-1)
		{
			perror("write") ;
			return EXIT_FAILURE ;
		}
	}
	else
	{
      if(read(desc,&buf,sizeof(int)) ==-1)
		{
			perror("read") ;
			return EXIT_FAILURE ;
		}	
      else
      {
         size_file[desc] = buf;
      }
	}
	return desc ;
}

/**
 * Ferme un fichier indexe.
 * 	fd  le descripteur du fichier a fermer
 * Renvoie -1 en cas de pb, 0 sinon.
 */
int indclose(int fd)
{
	return close(fd) ;
}

/**
 * Lit des donnees dans un fichier indexe.
 *	fd	descripteur du fichier lu
 *	buf     ptr vers les donnees lues
 *	nbytes  nb d'octets a lire 
 *		(nbytes doit etre >= a la taille d'index)
 * Renvoie le nb d'octets lus, 0 si fin de fichier, -1 en cas de pb.
 */
/*Fonction booléenne qui renvoie true (1) si nbytes est >= la taille du fichier de descripteur fd, sinon retourne 0 
int nbytes_valable(int fd, size_t nbytes)
{
	if (read(fd,&(size_file[fd]), sizeof(int))!= -1)
	{
		if( nbytes >= size_file[fd]) 
		{
			return 1 ;
		}	
	}
	errno= EINVAL;  
	return 0 ;
}* INUTILE */
ssize_t indread(int fd, void *buf, size_t nbytes)
{
	ssize_t nb_octets_lus=-1 ;
   struct stat koko ;
   if(fstat(fd, &koko)==-1)
      return -1;
   if( S_ISREG(koko.st_mode)!=0)
   {
       if (nbytes >= size_file[fd])
      {
         if((nb_octets_lus=read(fd, buf,nbytes))==-1)
         {
            perror("read") ;
            return -1 ;
         }
         else
         {
            if(nb_octets_lus < nbytes) /*Si je suis arrivée à la fin du fichier*/
            { 
               return 0 ;
            }	
         }
      }
      else
      {
         return -1 ;
      }
   }
	return nb_octets_lus ;		
}

/**
 * Ecrit des donnees dans un fichier indexe.
 *	fd	descripteur du fichier modifie
 *	buf     ptr vers les donnees la ecrire
 *	nbytes  nb d'octets a ecrire 
 *		(nbytes doit etre >= a la taille d'index)
 * Renvoie le nb d'octets ecrits, -1 en cas de pb.
 */
ssize_t indwrite(int fd, const void *buf, size_t nbytes)
{
	ssize_t nb_octets_ecrits=0 ;
	/*if( nbytes_valable(fd, nbytes) !=0)*/
   if (nbytes >= size_file[fd])
	{
		if((nb_octets_ecrits=write(fd, buf,nbytes))==-1)
		{
			perror("write") ;
			return -1 ;
		}
	}
   else
   {
      return -1 ;
   }  
	return nb_octets_ecrits ;
}
/**
 * Deplace le curseur dans un fichier indexe.
 *	fd	descripteur du fichier
 *	offset  deplacement a effectuer (en nb de structures index)
 *	whence  positionnement initial du curseur 
 * Renvoie le deplacement effectif du curseur (en nb d'octets), -1 en cas de pb.
 */
off_t indlseek(int fd, off_t offset, int whence)
{
	off_t position ;
	if(whence == SEEK_CUR || whence == SEEK_END)
	{
		position= offset*size_file[fd] ;
	}
	else
	{
		if(whence==SEEK_SET)
		{
			position= sizeof(int)+ offset* size_file[fd] ;
		}
	}
	return lseek(fd,position, whence) ;	
}

/**
 * Recherche un enregistrement dans un fichier indexe.
 *	fd	descripteur du fichier
 *	cmp	fonction de comparaison qui renvoie 1 si les deux arguments sont égaux, 0 sinon
 *	key	cle de recherche
 * Renvoie le 1er elt trouve pr lequel la comparaison est correcte, NULL sinon.
 */
void *indsearch(int fd, int (*cmp)(void *, void *), void *key)
{
	void * tampon= malloc(size_file[fd]) ;
	while( read(fd, tampon, size_file[fd])>0)
	{
		if(cmp(tampon,key)==1)
		{
         free(tampon);
			return tampon ;		
		}	
	}
   free(tampon);
 	return NULL ;
}
/**
 * Change l'indexation d'un fichier.
 * Les donnees sont recopiees dans un nouveau fichier avec les memes droits d'acces.
 * 	oldfic	fichier a reindexer
 *	newfic	fichier resultat
 *	newind	nouvelle taille de structure index
 * Si la nouvelle taille est < a la precedente, les donnees supplementaires sont perdues.
 * 	ex : 	'ftoto' contient des struct toto { int pi; char ps[2]; };
 *		'ftata' contient des struct tata { int pj; };
 *		indxchg("ftoto", "ftata", sizeof(struct tata));
 *		=> les donnees correspondant a ps seront perdues
 * Renvoie -1 en cas de pb, 0 sinon.
 */
int indxchg(char *oldfic, char *newfic, unsigned int newind)
{
	struct stat stat_info ;
	stat(oldfic, &stat_info) ;
   /*Création du nouveau fichier de nom newfic, avec les mêmes droits*/
	int new = indopen(newfic,O_CREAT | O_TRUNC | O_WRONLY,newind,stat_info.st_mode) ;
   
   int old = indopen(oldfic,O_RDONLY,newind,stat_info.st_mode) ;
   if((old==-1) || (new==-1))
   {
      perror("Erreur à l'ouverture du fichier") ;
      return -1 ;
   }
   /*RECOPIAGE DES DONNEES*/
   size_t size_buf= (size_file[old] > newind)?size_file[old]:newind ;
   void * buffer= malloc(size_buf) ;
   memset(buffer,0,size_buf) ; /*initialisation du buffer à 0*/
	while(indread(old, buffer, size_file[old])>0)
   {
      indwrite(new, buffer,(size_t)newind) ;      
   }
   free(buffer);
	return 0 ;
}

