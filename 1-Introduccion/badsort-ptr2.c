#include <stdio.h>

typedef struct {
	char data[4096];
	int key;
} item;

item array[] = {
	{"bill", 3},
	{"neil", 4},
	{"john", 2},
	{"rick", 5},
	{"alex", 1},
};

void sort(item *begin, int n) {
	int i = 0, j = 0;
	int sorted = 1;
	item* curr;

	for(; i < n && sorted != 0; i++) {
		sorted = 0;
		curr = begin;
		j = n-1;
		do {
			if( curr->key > (curr+1)->key) {
				item t = *curr;
				*curr  = *(curr+1);
				*(curr+1) = t;
				sorted++;
			}
			curr = curr+1;
		} while ( --j > 0 );
	}
}

int main() {
	int i;
	sort(array,5);
	for(i = 0; i < 5; i++)
		printf("array[%d] = {%s, %d}\n",
		 i, array[i].data, array[i].key);
	return 0;
}
