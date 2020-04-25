#include "Mithril.h"
#include "reader.h"

#define BLOCK_UNIT_SIZE 0    // 16 * 1024, not sure what this means 
#define DISK_SECTOR_SIZE 0  // 512

int main() { 

	// Set the cache size in bytes (CHECK!) to 1MB
	guint64 cache_size = 1000000;

	// data type is long 
	char data_type = 'l';

	// block size is 512 bytes 
	guint64 block_size = 512;

	// not sure what this is, but segfault without this var 
	gdouble max_metadata_size = 10240;

	Mithril_init_params_t mithril_init;
	mithril_init.cache_type = "LRU";
	mithril_init.lookahead_range=100; // how many request ahead do you want to consider 
	mithril_init.block_size = block_size;
	mithril_init.max_support=200;
	mithril_init.min_support=1;
	mithril_init.confidence=1;
	mithril_init.pf_list_size=4; // number of association that can be stored 
	mithril_init.max_metadata_size=max_metadata_size; // CHECK 
	mithril_init.sequential_type=0;
	mithril_init.mining_threshold=10;
	mithril_init.output_statistics=1;

	cache_t* mithril_cache=Mithril_init(cache_size, data_type, block_size, &mithril_init);

	// Get the data reader 
	reader_t* reader = setup_reader("../data/w99_vscsi1.vscsitrace", 
		'v', 'l', BLOCK_UNIT_SIZE, DISK_SECTOR_SIZE, NULL);

	printf("Trace file has %d reqs\n", get_num_of_req(reader));

	// read each request of the trace 
	request_t* cp = new_req_struct();
	cp->label_type='l';

    read_one_element(reader, cp);
    while (cp->valid){
		gboolean add_bool = Mithril_add_element(mithril_cache, cp);
		//printf("%ld, %d \n", *(guint64*)(cp->label_ptr), add_bool);
    	read_one_element(reader, cp);
	}

	printf("Done");

	//print_mining_table(mithril_cache);

	Mithril_params_t *mithril_results = (Mithril_params_t *)(mithril_cache->cache_params);
	GHashTable *prefetch_hashtable = mithril_results->prefetch_hashtable;
	GList *keys = g_hash_table_get_keys(prefetch_hashtable);

	GHashTableIter iter;
	gpointer key, value;

	g_hash_table_iter_init (&iter, prefetch_hashtable);

	gint32 ptable_cur_row = mithril_results->ptable_cur_row;
	gint64 **ptable_array = mithril_results->ptable_array;

	printf("Current row is %d\n", ptable_cur_row);



	for (int i=0; i<ptable_cur_row; i++) {
		//gint64 test = ptable_array[i][0]
		printf("Row 0, %d, %d, %d, %d, %d, %d\n", ptable_array[0][0], ptable_array[0][1], ptable_array[0][2], ptable_array[0][3], ptable_array[0][4], ptable_array[0][5]);
		printf("Row %d, %d, %d\n", i, ptable_array[i][0], ptable_array[i][1]);
		break;
	}

	// printf("Row 0, %d, %d\n", ptable_array[0][0], ptable_array[0][1]);
	// printf("Row 1, %d, %d\n", ptable_array[1][0], ptable_array[1][1]);
	// printf("Row 2, %d, %d\n", ptable_array[2][0], ptable_array[2][1]);

	// printf("Iterating over hash table with %d keys\n", g_list_length(keys));

	// while (g_hash_table_iter_next (&iter, &key, &value)) 
	//   {
 //    	//printf("Key: %ld\n", *(long *) key);
	//   }

} 