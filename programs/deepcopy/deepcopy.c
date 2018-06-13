#include<stdlib.h>
#include<stdio.h>
 
typedef struct elem{
	int data;
	struct elem* next;
}cell;
 
typedef cell* list;
 
list addToList(list *a,int num){
	list temp, holder;
 
	if(*a==NULL){
		*a = (list)malloc(sizeof(cell));
		(*a)->data = num;
		(*a)->next = NULL;
		return *a;
	}
	else{
		temp = *a;
 
		while(temp->next!=NULL)
			temp = temp->next;
 
		holder = (list)malloc(sizeof(cell));
		holder->data = num;
		holder->next = NULL;
 
		temp->next = holder;
		return holder;
	}
}
 
list copyList(list a){
	list b, tempA, tempB, temp;
 
	if(a!=NULL){
		b = (list)malloc(sizeof(cell));
		b->data = a->data;
		b->next = NULL;
 
		tempA = a->next;
		tempB = b;
 
		while(tempA!=NULL){
			temp = (list)malloc(sizeof(cell));
			temp->data = tempA->data;
			temp->next = NULL;
 
			tempB->next = temp;
			tempB = temp;
 
			tempA = tempA->next;
		}
	}
 
	return b;
}
 
void printList(list a){
	list temp = a;
 
	while(temp!=NULL){
		printf("%d,",temp->data);
		temp = temp->next;
	}
	printf("\b");
}
 
int main()
{
	list a,b,tmp;
	int i;
	tmp = a;
	for(i=0;i<100000000;i++)
		tmp = addToList(&tmp,i);
 
	/* printf("List a is : "); */
 
	/* printList(a); */
 
	b = copyList(a);
 
	free(a);
 
	/* printf("\nList a destroyed, List b is : "); */
 
	/* printList(b); */
 
	return 0;
}
 
