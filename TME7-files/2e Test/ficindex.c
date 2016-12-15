/****** TME7 ******/
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
int indopen(const char *path, int flags, int ind, mode_t mode){
	int fd;

	if((fd = open(path, flags, mode)) == -1){
		return -1;
	}

	if(read(fd, &ind, sizeof(int)) == 0){
		write(fd, &ind, sizeof(int)); 
	}
	
	return fd;  
}


/**
 * Ferme un fichier indexe.
 * 	fd  le descripteur du fichier a fermer
 * Renvoie -1 en cas de pb, 0 sinon.
 */
int indclose(int fd){
	return close(fd);
}


/**
 * Lit des donnees dans un fichier indexe.
 *	fd	descripteur du fichier lu
 *	buf     ptr vers les donnees lues
 *	nbytes  nb d'octets a lire 
 *		(nbytes doit etre >= a la taille d'index)
 * Renvoie le nb d'octets lus, 0 si fin de fichier, -1 en cas de pb.
 */
ssize_t indread(int fd, void *buf, size_t nbytes){
	int index;
	
	/* On remet l'offset en début de fichier */
	off_t currentPos = lseek(fd, 0, SEEK_CUR);
	
	if(lseek(fd, 0, SEEK_SET) == -1){
		perror("Impossible de se déplacer dans le fichier");
		return -1;
	}
  
	read(fd, &index, sizeof(int));
  
	if(lseek(fd, currentPos, SEEK_SET) == -1){
		perror("Impossible de continuer le déplacement dans le fichier");
		return -1;
	}
  
	if(nbytes < index){
		perror("-> indread: Nbytes est inférieur à la taille de l'index");
		return -1;
	}else{
		return read(fd, buf, index);
	}
}


/**
 * Ecrit des donnees dans un fichier indexe.
 *	fd	descripteur du fichier modifie
 *	buf     ptr vers les donnees la ecrire
 *	nbytes  nb d'octets a ecrire 
 *		(nbytes doit etre >= a la taille d'index)
 * Renvoie le nb d'octets ecrits, -1 en cas de pb.
 */
ssize_t indwrite(int fd, const void *buf, size_t nbytes){
	int index;
	off_t currentPos = lseek(fd, 0, SEEK_CUR);
	
	if(lseek(fd, 0, SEEK_SET) == -1){
		perror("Impossible de se déplacer dans le fichier");
		return -1;
	}

	read(fd, &index, sizeof(int));
  
	if(lseek(fd, currentPos, SEEK_SET) == -1 ){
		perror("Impossible de continuer le déplacement dans le fichier");
		return -1;
	}
  
	if(nbytes < index){
		perror("-> indwrite: Nbytes est inférieur à la taille de l'index");
		return -1;
	}else{
		return write(fd, buf, nbytes);  
	}
}


/**
 * Deplace le curseur dans un fichier indexe.
 *	fd	descripteur du fichier
 *	offset  deplacement a effectuer (en nb de structures index)
 *	whence  positionnement initial du curseur 
 * Renvoie le deplacement effectif du curseur (en nb d'octets), -1 en cas de pb.
 */
off_t indlseek(int fd, off_t offset, int whence){
	/* Littéralement: whence = "d'où", provenance */
	int index;
	off_t currentPos = lseek(fd, 0, SEEK_CUR);
 
	lseek(fd, 0, SEEK_SET);
	read(fd, &index, sizeof(int));
  
	if(lseek(fd, currentPos, SEEK_SET) == -1 ){
		perror("Impossible de se déplacer dans le fichier");
		return -1;
	}
	offset = offset * index;

	if(whence == SEEK_SET){
		/* pour éviter le descripteur */
		offset += sizeof(int);
	}
	return lseek(fd,offset,whence);
}


/**
 * Recherche un enregistrement dans un fichier indexe.
 *	fd	descripteur du fichier
 *	cmp	fonction de comparaison qui renvoie 1 si les deux arguments sont égaux, 0 sinon
 *	key	cle de recherche
 * Renvoie le 1er elt trouve pr lequel la comparaison est correcte, NULL sinon.
 */
void *indsearch(int fd, int (*cmp)(void *, void *), void *key){
	void *word;
	int index;
 
	off_t currentPos = lseek(fd, 0, SEEK_CUR);

	if(lseek(fd, 0, SEEK_SET) == -1){
		perror("Impossible de se déplacer dans le fichier");
		return NULL;
	}
  
	read(fd, &index, sizeof(int));
	word = malloc(index);
  
	if(lseek(fd, currentPos, SEEK_SET) == -1){
		perror("Impossible de continuer le déplacement dans le fichier");
		return NULL;
	}

	int test;
	while((test = read(fd, word, index)) != 0){
		if(test == -1)
			return NULL;
		if(cmp(word, key) == 1){
			return word;
		}
	}

	return NULL;
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
