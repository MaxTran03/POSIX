#include <list.h>
#include <stdlib.h>
#include <stdio.h>

struct list_type l;

int init_queue(){
	init_list(&l);
	return 0;
}

int queue(void* element){
	insert_head(&l, element);
	return 0;
}

void* dequeue(){
	return extract_tail(&l);
}

int size(){
	return list_size(&l);
}
