#include "Mithril.h"
#include "reader.h"

#define BLOCK_UNIT_SIZE 0    // 16 * 1024
#define DISK_SECTOR_SIZE 0  // 512

int main() { 

	// Set the cache size in bytes (CHECK!) to 1MB
	guint64 cache_size = 1000000;

	// data type is long 
	char data_type = 'l';

	// block size is 512 bytes 
	guint64 block_size = 512;

	// not sure what this is, but segfault without this var 
	gdouble max_metadata_size = 1024;

	Mithril_init_params_t mithril_init;
	mithril_init.cache_type = "LRU";
	mithril_init.lookahead_range=2; // how many request ahead do you want to consider 
	mithril_init.block_size = block_size;
	mithril_init.max_support=200;
	mithril_init.min_support=1;
	mithril_init.confidence=1;
	mithril_init.pf_list_size=51; // CHECK
	mithril_init.max_metadata_size=max_metadata_size; // CHECK 
	mithril_init.sequential_type=0;

	cache_t* mithril_cache=Mithril_init(cache_size, data_type, block_size, &mithril_init);

	// Get the data reader 
	reader_t* reader = setup_reader("../data/trace.vscsi", 
		'v', 'l', BLOCK_UNIT_SIZE, DISK_SECTOR_SIZE, NULL);

	// read each request of the trace 
	request_t* cp = new_req_struct();
	cp->label_type='l';

    read_one_element(reader, cp);
    while (cp->valid){
    	//printf("HERER, %ld \n", cp->label_ptr);
		Mithril_add_element(mithril_cache, cp);
    	read_one_element(reader, cp);
	}

	Mithril_params_t *mithril_results = (Mithril_params_t *)(mithril_cache->cache_params);
	GHashTable *prefetched_hashtable_Mithril = mithril_results->prefetched_hashtable_Mithril;
	GList *keys = g_hash_table_get_keys(prefetched_hashtable_Mithril);

	GHashTableIter iter;
	gpointer key, value;

	g_hash_table_iter_init (&iter, prefetched_hashtable_Mithril);

	printf("Iterating over hash table");
	while (g_hash_table_iter_next (&iter, &key, &value)) 
	  {
	    printf("Key: %d", key);
	  }

} 