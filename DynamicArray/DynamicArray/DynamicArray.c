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




static bool do_heap(darray *p, size_t i, size_t n, int (*cmp)(const void*, const void*));
/*
    @description:
        Swaps two elements in the darray pointed to by p. If the
        swap could not be completed, false is returned. Otherwise
        true is returned.
*/
extern bool darray_swap(darray *p, size_t pos1, size_t pos2)
{
    if (pos1 >= p->capacity || pos2 >= p->capacity)
        return false; /* Out of range index */
    else {
#if __STDC_VERSION__ >= 199901L
        /*
            VLAs can be risky due to the runtime size not under programmer
            control. So if the size is greater than a threshold, use
            dynamic allocation. That should rarely happen here, if ever, 
            because the item_size is assumed to be the number of bytes 
            in an object.
        */
#define VLA_LIMIT 1024
        unsigned char temp_base[p->item_size < VLA_LIMIT ? p->item_size : 1];
        bool dynamic_temp = false;
        void *temp = temp_base;
        if (p->item_size >= VLA_LIMIT) {
            temp = malloc(p->item_size);
            dynamic_temp = true;
        }
#undef VLA_LIMIT
#else /* __STDC_VERSION__ */
        /*
            Prior to C99 we can't take advantage of a stack-based
            array with a runtime size. At least we can't without using
            something nonportable like alloca(). To keep the library
            portable, malloc() is used, despite slowness.
        */
        void *temp = malloc(p->item_size);
        bool dynamic_temp = false;
#endif /* __STDC_VERSION__ */
        void *q = &p->data[pos1 * p->item_size];
        void *r = &p->data[pos2 * p->item_size];
        if (!temp)
            return false; /* Failed dynamic allocation */
        memcpy(temp, q, p->item_size);
        memcpy(q, r, p->item_size);
        memcpy(r, temp, p->item_size);
        if (dynamic_temp)
            free(temp);
        return true;
    }
}
/*
    @description:
        Sorts the darray pointed to by p according to the comparison
        rules specified within the function pointed to by cmp. If
        the sort could not be completed, false is returned. Otherwise,
        true is returned.
*/
extern bool darray_sort(darray *p, int (*cmp)(const void*, const void*))
{
    size_t n = p->capacity;
    size_t i = n / 2;
    /* Make the entire array a valid heap */
    while (i-- > 0) {
        if (!do_heap(p, i, n, cmp))
            return false;
    }
    while (--n < (size_t)-1) {
        /* Place the maximum value and fix the heap for remaining elements */
        if (!darray_swap(p, 0, n) || !do_heap(p, 0, n, cmp))
            return false;
    }
    return true;
}
/*
    @description:
        Heapify helper for darray_sort.
*/
static bool do_heap(darray *p, size_t i, size_t n, int (*cmp)(const void*, const void*))
{
    void *temp = malloc(p->item_size);
    unsigned char *base = p->data;
    size_t size = p->item_size;
    size_t k;
    if (!temp)
        return false; /* Failed dynamic allocation */
    memcpy(temp, &base[i * size], size);
    for (k = i * 2 + 1; k < n; k = i * 2 + 1) {
        if (k + 1 < n && cmp(&base[k * size], &base[(k + 1) * size]) < 0)
            ++k;
        if (cmp(temp, &base[k * size]) >= 0)
            break;
        memcpy(&base[i * size], &base[k * size], size);
        i = k;
    }
    memcpy(&base[i * size], temp, size);
    free(temp);
    return true;
}


/*
    @description:
        A simple insertion sort.
*/
extern bool darray_insertion_sort(darray *d_array, int (*cmp)(const void*, const void*))
{    
    size_t i, j;
	unsigned char *temp; 
    
	for (i = 1; i < d_array->capacity; i++) {
		temp = &d_array->data[i * d_array->item_size]; // compare data		
		j = i-1;
		
		while (cmp(temp, &d_array->data[j * d_array->item_size]) && j>=0) {
			darray_swap(d_array, j+1, j);			
			j--;
		}	
		memcpy(&d_array->data[(j+1) * d_array->item_size], &d_array->data[j * d_array->item_size], d_array->item_size);		
	}

    return true;
}

/*
    @description:
        This function copies all occurances of some data into a separate byte stream.
*/
extern void darray_find_all_occurances(darray *d_array, void* data, unsigned char** all_occurances)
{    
    size_t i, count = 0;	

	if (d_array) {
		for (i = 0; i < d_array->capacity; i++) {
			if (memcmp(&d_array->data[i * d_array->item_size], data, d_array->item_size) == 0) {
				count++;
			}
		}    
		
		if (count == 0) {
			*all_occurances = NULL;
			return;
		}

		*all_occurances = (unsigned char*)calloc(count, d_array->item_size);

		for (i = 0; i < d_array->capacity; i++) {
			if (memcmp(&d_array->data[i * d_array->item_size], data, d_array->item_size) == 0) {
				memcpy(*all_occurances, &d_array->data[i * d_array->item_size], d_array->item_size);				
				*all_occurances += d_array->item_size;				
			}
		}   

		*all_occurances -= count * d_array->item_size;
	} else {
		assert(0);
	}
}


/*
    @description:
        This function gets you a hash for your data. 
		Current hashing algorithm: None!
		Maybe we could have another parameter indicating which hashing algo needs to be used as well.
*/
extern void darray_get_hash(darray *d_array, unsigned char** hash)
{    
    size_t i;	
    
	*hash = d_array->data;

	if (d_array) {
		for (i = 0; i < d_array->capacity; i++) {
			// TODO: Use a hashing algo to get hash.
		}    
	} else {
		assert(0);
	}
}