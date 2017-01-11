// main.c
//
// Andrew Gilpin
// agg1@cec.wustl.edu
//
// LI Haocheng
// hcli@cse.cuhk.edu.hk

// This file contains the example program used in the gdb debugging
// tutorial. The tutorial can be found on the web at
// http://students.cec.wustl.edu/~agg1/tutorial/

#include <stdio.h>

int number_instantiated = 0;

struct Node {
	int value_;
	Node<T> *next_;
};


struct LinkedList {
	Node *head_;
};

// returns 0 on success, -1 on failure
int insert(LinkedList *l, const int &new_item) {
	Node *n = malloc(sizeof(Node));
	if(!n) { return -1; }
	++number_instantiated;
	n->next_ = l->head_;
	n->value_ = new_item;
	l->head_ = n;
	return 0;
}

// returns 0 on success, -1 on failure
int remove(LinkedList *l, const int &item_to_remove) {
	Node *marker = l->head_;
	Node *temp = NULL;  // temp points to one behind as we iterate

	while(marker) {
		if(marker->value_ == item_to_remove) {
			if(!temp) { // marker is the first element in the list
				if(!(marker->next_)) {
					l->head_ = NULL;
					free(marker); // marker is the only element in the list
					--number_instantiated;
					marker = NULL;
				} else {
					Node *n = malloc(sizeof(Node));
					if(!n) { return -1; }
					++number_instantiated;
					n->next_ = marker->next_;
					n->value_ = marker->value_;
					l->head_ = n;

					free(marker);
					--number_instantiated;
					marker = 0;
				}
				return 0;
			} else {
				temp->next_ = marker->next_;
				free(temp);
				--number_instantiated;
				temp = NULL;
				return 0;
			}
		}
		marker = NULL;  // reset the marker
		temp = marker;
		marker = marker->next_;
	}

	return -1;	// failure
}

void print(LinkedList *l) {
	Node *marker = l->head_;
	while(marker) {
		printf("%d\n", marker->value_);
		marker = marker->next_;
	}
}

void delete_nodes(LinkedList *l) {
	Node *marker = l->head_;
	while(marker) {
		Node *temp = marker;
		free(marker);
		--number_instantiated;
		marker = temp->next_;
	}
}


int main(int argc, char **argv) {
	LinkedList *list = malloc(LinkedList);
	list->head_ = NULL;

	insert(list, 1);
	insert(list, 2);
	insert(list, 3);
	insert(list, 4);

	printf("The fully created list is:\n");
	print(list);

	printf("\nNow removing elements:\n");
	remove(list, 4);
	print(list);
	printf("\n");

	remove(list, 1);
	print(list);
	printf("\n");

	remove(list, 2);
	print(list);
	printf("\n");

	remove(list, 3);
	print(list);

	delete_nodes(list);
	free(list);
	list = NULL;

	return 0;
}
