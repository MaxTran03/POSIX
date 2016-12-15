#define _XOPEN_SOURCE 700
#define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <myqueue.h>

struct mq_attr *attrs[100];
sem_t *semread;
sem_t *semwrite;
sem_t *mutex;

void translate_msg(msg *msg, int longueur){
	int i;
	for(i=0; i<longueur-1; i++){
		msg[i] = msg[i+1];
	}
}

int prio_msg(msg *msg, int longueur, unsigned priorite){
	int i;
	
	if(longueur == 0)
		return 0;
	if(priorite >= msg[longueur-1].prio)
		return longueur;
		
	for(i=longueur-1; i>=0; i--){
		if(priorite < msg[i].prio)
			msg[i+1] = msg[i];
		else
			return i+1;
	}
	
	return 0;
}

char swrite[32] = "write";
char sread[32] = "read";

mqd_t mq_open(const char *nom, int mq_flags, int mode){
	if((mutex = sem_open("mutex",O_CREAT|O_RDWR,S_IRWXU,1)) == SEM_FAILED){
		if(errno == EEXIST){
			mutex = sem_open("mutex", O_RDWR);
		}
	}
	
	sem_wait(mutex);
	
	int fd;
	if((fd = shm_open(nom, mq_flags, mode))==-1){
		perror("shm_open");
		return -1;
	}
	
	if((mq_flags & O_CREAT) == O_CREAT)
		if(ftruncate(fd,sizeof(struct mq_attr))==-1){
			perror("ftruncate");
			return -1;
		}
	
	attrs[fd] = mmap(NULL, sizeof(struct mq_attr), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	
	attrs[fd]->mq_maxmsg = 32;
	attrs[fd]->mq_flags = mq_flags;
	attrs[fd]->mq_msgsize = sizeof(msg_data);
	attrs[fd]->mq_curmsgs = 0;
	
	strcpy(attrs[fd]->blocked_writers,swrite);
	strcpy(attrs[fd]->blocked_readers,sread);
	
	attrs[fd]->nb_blocked_writers = 0;
	attrs[fd]->nb_blocked_readers = 0;
	
	attrs[fd]->subscriber = -1;
	attrs[fd]->notification_sig = -1;
	
	if((semread = sem_open(attrs[fd]->blocked_readers,O_CREAT|O_RDWR,S_IRWXU,0)) == SEM_FAILED){
		if(errno == EEXIST){
			semread = sem_open(attrs[fd]->blocked_readers, O_RDWR);
		}
	}
	
	if((semwrite = sem_open(attrs[fd]->blocked_writers,O_CREAT|O_RDWR,S_IRWXU,0)) == SEM_FAILED){
		if(errno == EEXIST){
			semwrite = sem_open(attrs[fd]->blocked_writers, O_RDWR);
		}
	}
	
	sem_post(mutex);
	
	return fd;
}


int mq_close(mqd_t mqdes){
	sem_close(semwrite);
	sem_close(semread);
	sem_close(mutex);
	return munmap(attrs[mqdes], sizeof(struct mq_attr));
}


int mq_unlink(const char* name){
	sem_unlink(sread);
	sem_unlink(swrite);
	sem_unlink("mutex");
	return shm_unlink(name);
}

int mq_getattr(mqd_t mqdes, struct mq_attr *mqstat ){
	mqstat = attrs[mqdes];
	return 0;
}



int mq_setattr(mqd_t mqdes, struct mq_attr * mqstat, struct mq_attr * omqstat){
	sem_wait(mutex);
	bcopy(attrs[mqdes], omqstat, sizeof(struct mq_attr));
	attrs[mqdes]->mq_flags = mqstat->mq_flags;
	sem_post(mutex);
	
	return 0;
}

int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio){
	if((mutex = sem_open("mutex",O_CREAT|O_RDWR,S_IRWXU,1)) == SEM_FAILED){
		if(errno == EEXIST){
			mutex = sem_open("mutex", O_RDWR);
		}
	}
	
	if(msg_len > attrs[mqdes]->mq_msgsize){
		printf("msg_len était plus grand que l'attribut mq_msgsize de la file de messages\n");
		return -1;
	}
	
	if(msg_prio > 32){
		printf("Il faut que la priorité soit entre 0 et 32");
		return -1;
	}
	
	if(attrs[mqdes]->mq_curmsgs == attrs[mqdes]->mq_maxmsg){
		if((attrs[mqdes]->mq_flags&O_NONBLOCK) != O_NONBLOCK){
			attrs[mqdes]->nb_blocked_writers++;
			sem_wait(semwrite);
			attrs[mqdes]->nb_blocked_writers--;
		}else{
			printf("La file est pleine et le drapeau O_NONBLOCK était défini pour la file de messages référencée par le descripteur mqdes.\n");
			return -1;
		}
	}
	
	sem_wait(mutex);
	
	int p = prio_msg(attrs[mqdes]->queue, attrs[mqdes]->mq_curmsgs, msg_prio);
	printf("__%d\n",p);
	strncpy(attrs[mqdes]->queue[p].data, msg_ptr, msg_len);
	attrs[mqdes]->queue[p].prio = msg_prio;
	attrs[mqdes]->queue[p].length = msg_len;
	attrs[mqdes]->mq_curmsgs++;
	
	if(attrs[mqdes]->subscriber>0){
			kill(attrs[mqdes]->subscriber, attrs[mqdes]->notification_sig);
			attrs[mqdes]->subscriber = -1;
			attrs[mqdes]->notification_sig = -1;
			sem_post(semread);
	}else if(attrs[mqdes]->mq_curmsgs != 0 && attrs[mqdes]->nb_blocked_readers>0){
			sem_post(semread);
	}
	
	sem_post(mutex);
	
	return 0;
}

ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio){
	if((mutex = sem_open("mutex",O_CREAT|O_RDWR,S_IRWXU,1)) == SEM_FAILED){
		if(errno == EEXIST){
			mutex = sem_open("mutex", O_RDWR);
		}
	}
	
	if(msg_len > attrs[mqdes]->mq_msgsize){
		printf("msg_len était plus grand que l'attribut mq_msgsize de la file de messages\n");
		return -1;
	}
	
	if(attrs[mqdes]->mq_curmsgs == 0){
		if((attrs[mqdes]->mq_flags&O_NONBLOCK) != O_NONBLOCK){
			attrs[mqdes]->nb_blocked_readers++;
			sem_wait(semread);
			attrs[mqdes]->nb_blocked_readers--;
		}else{
			printf("La file est vide et le drapeau O_NONBLOCK était défini pour la file de messages référencée par le descripteur mqdes.\n");
			return -1;
		}		
	}
	
	sem_wait(mutex);
	
	strncpy(msg_ptr, attrs[mqdes]->queue[0].data, attrs[mqdes]->queue[0].length);
	msg_prio[0] = attrs[mqdes]->queue[0].prio;
		
	translate_msg(attrs[mqdes]->queue, attrs[mqdes]->mq_curmsgs);
	
	attrs[mqdes]->mq_curmsgs--;
	
	if(attrs[mqdes]->mq_curmsgs != attrs[mqdes]->mq_maxmsg && attrs[mqdes]->nb_blocked_writers>0){
			sem_post(semwrite);
	}
	
	sem_post(mutex);
	
	return attrs[mqdes]->queue[0].length;
}

mqd_t mq_notify(mqd_t mqdes, const struct sigevent *notification){
	pid_t pidsub = attrs[mqdes]->subscriber;
	int notify = notification->sigev_notify;
	
	if(notify != SIGEV_NONE && notify != SIGEV_SIGNAL && notify != SIGEV_THREAD){
		printf("sigev_notify n'est pas l'une  des  valeurs  permises\n");
		return -1;
	}
	
	if(pidsub == getpid() && notification == NULL){
		attrs[mqdes]->subscriber = -1;
		attrs[mqdes]->notification_sig = -1;
		return 0;
	}
	
	if(pidsub > 0){
		printf("Un autre processus est déjà enregistré pour recevoir les notifications de cette file de messages.\n");
		return -1;
	}
	
	attrs[mqdes]->subscriber = getpid();
	attrs[mqdes]->notification_sig = notification->sigev_signo;
	
	return 0;
}

