#include "CommonIncludes.h"
#include "DynamicArray.h"

#define N 4

void example()
{
	darray a;
    size_t i, j;
    int k = 1;		
	unsigned int data = 0xffffffff;
    
	darray_init(&a, 4, 3);

	for (j = 0; j < N; ++j, ++k)
		darray_set(&a, j, &k);
	
	for (i = 0; i < a.capacity; ++i)
		printf("\n%d", a.data[i * a.item_size]);

	printf("\nThe result of your search is %d", darray_search(&a, &data));
}

int main(void)
{
   example();
   return 0;
}