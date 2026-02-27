
/*
 * gcc cbor_test.c -lcbor
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cbor.h>


int main(int argc, char *argv[])
{
	cbor_item_t *root = cbor_new_definite_map(2);
	cbor_map_add(root, (struct cbor_pair){
			.key = cbor_build_string("key1"), 
			.value = cbor_build_uint32(100)}
			);
	cbor_map_add(root, (struct cbor_pair){
			.key = cbor_build_string("key2"), 
			.value = cbor_build_string("value")}
		    );

	unsigned char *buffer = NULL;
	size_t buffer_size = 0;

	cbor_serialize_alloc(root, &buffer, &buffer_size);
	printf("buffer size: %lu\n", buffer_size);
	for (size_t i = 0; i < buffer_size; i++)
	{
		printf("%c ", buffer[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}
	printf("\n");

	cbor_item_t *item = NULL;
	struct cbor_load_result result;
    
	item = cbor_load(buffer, buffer_size, &result);

	if (cbor_isa_map(item)) 
	{
		printf("Map size: %zu\n", cbor_map_size(item));
		struct cbor_pair *pairs = cbor_map_handle(item);
		printf("pair 1, key: %s, value: %u\n", cbor_bytestring_handle(pairs[0].key), 
				cbor_get_uint32(pairs[0].value));
		printf("pair 2, key: %s, value: %s\n", cbor_bytestring_handle(pairs[1].key), 
				cbor_bytestring_handle(pairs[1].value));
	}
    
	cbor_decref(&item);

	cbor_decref(&root);
	free(buffer);

	return 0;
}
