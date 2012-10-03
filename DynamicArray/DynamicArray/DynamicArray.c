#include "DynamicArray.h"
#include "CommonIncludes.h"

/*
    @description:
        Initializes the darray pointed to by p to the requested capacity.
*/
extern void darray_init(darray *p, size_t item_size, size_t capacity)
{
    p->item_size = item_size;
    p->capacity = 0;
    p->data = NULL;
    if (capacity > 0)
        darray_resize(p, capacity);
}
/*
    @description:
        Releases memory from a previously initialized darray
        pointed to by p, and resets to empty status. The item_size 
        value of the darray is preserved.
*/
extern void darray_reset(darray *p)
{
    free(p->data);
    darray_init(p, p->item_size, 0);
}
/*
    @description:
        Resizes the darray pointed to by p to a new capacity.
        If the new capacity and current capacity are equal, nothing
        is done. If the new capacity is smaller than the current 
        size, the array is truncated to fit the new capacity.
*/
extern bool darray_resize(darray *p, size_t new_capacity)
{
    unsigned char *new_data;
    size_t nitems;
    if (new_capacity == p->capacity)
        return true; /* Don't resize to the same capacity (wasted effort) */
    else {
        new_data = calloc(new_capacity, p->item_size);
        if (!new_data)
            return false; /* Memory allocation failed */
        nitems = p->capacity;
        if (nitems > new_capacity)
            nitems = new_capacity; /* Truncate to a smaller capacity */
        /* Populate the new data array and release the old one */
        memcpy(new_data, p->data, nitems * p->item_size);
        free(p->data);
        /* Apply the changes to the darray */
        p->capacity = new_capacity;
        p->data = new_data;
        return true;
    }
}
/*
    @description:
        Retrieves the item stored at position pos within the 
        darray and returns a pointer to it.
*/
extern void *darray_get(const darray *p, size_t pos)
{
    return &p->data[pos * p->item_size];
}
/*
    @description:
        Sets the item stored at position pos within the darray
        and returns true, or returns false if the position
        is out of range.
*/
extern bool darray_set(darray *p, size_t pos, void *item)
{
    if (pos >= p->capacity)
        return false;
    memcpy(&p->data[pos * p->item_size], item, p->item_size);
    return true;
}


/*
    @description:
        A simple linear search for data 
		-1 : data not present.
		pos: The position where your data is present.(first occurance)
*/
extern int darray_search(darray *d_array, void *data)
{
	size_t i = 0;
	size_t j = 0;
	unsigned char* data_backup_ptr = data;

	if (d_array) {
		for (; i < d_array->capacity; i++) {	
			if (memcmp(&d_array->data[i * d_array->item_size], data, d_array->item_size) == 0) {
				return i;
			}
		}		
	} else {
		assert (0); // Your pointer is NULL!
		return -1; // Doesn't come here anyway.
	}
	return -1; // Make compiler happy :D
}

