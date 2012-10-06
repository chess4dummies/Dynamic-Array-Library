#include "CommonIncludes.h"
#include "DynamicArray.h"

#define N 4

void example()
{
	darray a;
    size_t i, j;
    int k = 1;		
	unsigned int data = 1;
	unsigned char* hash;
	unsigned char* all_occurances;
    
	darray_init(&a, 4, 3);

	for (j = 0; j < N; ++j, ++k)
		darray_set(&a, j, &data);
	
	for (i = 0; i < a.capacity; ++i)
		printf("\n%d", a.data[i * a.item_size]);

	/* Lets give 100 bytes for now. TODO: change size as per hash algo.
	 * i.e for eg: Sha1 uses 40 bytes
	 */	
	hash = (unsigned char*)calloc(100, 1); 
	darray_get_hash(&a, &hash);

	darray_find_all_occurances(&a, &data, &all_occurances);

	printf("\nThe result of your search is %d", darray_search(&a, &data));
}

int main(void)
{
   example();
   return 0;
}