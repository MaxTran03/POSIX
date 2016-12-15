#include <list.h>
#include <stdlib.h>
#include <stdio.h>


void init_list(struct list_type *l) {
	l->head = NULL;
	l->tail = NULL;
	l->size = 0;
}
	
	
void insert_head(struct list_type *l, void* element) {
	cell* new_cell = malloc(sizeof(cell));
	new_cell->content = element;
	new_cell->previous = NULL;
	new_cell->next = l->head;
	if (l->head != NULL)
		l->head->previous = new_cell;
	l->head = new_cell;
	if (l->tail == NULL)
		l->tail = l->head;
	l->size++;
}


void* extract_head(struct list_type *l) {
	cell* ptr_c = l->head;
	void* element = NULL;
	if(ptr_c != NULL){
		if(l->tail == l->head)
			l->tail = NULL;
		l->head = l->head->next;
		element = ptr_c->content;
		free(ptr_c);
	}
	l->size--;
	return element;	
}


void* extract_tail(struct list_type *l) {
	cell* ptr_c = l->tail;
	void* element = NULL;
	if(ptr_c != NULL){
		if(l->tail == l->head)
			l->head = NULL;
		l->tail = l->tail->previous;
		element = ptr_c->content;
		free(ptr_c);
	}
	l->size--;
	return element;
}


/*int list_size(struct list_type *l) {
	cell* ptr_c = l->head;
	if(l->head == NULL)
		return 0;
	if(l->tail == l->head)
		return 1;
	else{
		int cpt = 1;
		while(ptr_c->next != NULL){
			cpt++;
			ptr_c = ptr_c->next;
		}
		return cpt;
	}
		
}*/

int list_size(struct list_type *l) {
	return l->size;
}
