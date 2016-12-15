#include <list.h>
#include <stdlib.h>
#include <stdio.h>

struct list_type l;

int init_stack(){
	init_list(&l);
	return 0;
}

int push(void * element){
	insert_head(&l, element);
	return 0;
}

void* pop(){
	return extract_head(&l);
}

int size(){
	return list_size(&l);
}
