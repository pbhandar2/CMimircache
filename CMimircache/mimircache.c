#include "Mithril.h"
#include "reader.h"

#define BLOCK_UNIT_SIZE 0    // 16 * 1024
#define DISK_SECTOR_SIZE 0  // 512

int main() { 

	// Cache Size in 10Mi
	guint64 cache_size = 10000000;

	// data type is long 
	char data_type = 'l';

	// block size is 512 bytes 
	guint64 block_size = 512;

	// cap for metadata size 
	gdouble max_metadata_size = 10240;

	Mithril_init_params_t mithril_init;
	mithril_init.cache_type = "LRU";
	mithril_init.lookahead_range=200; // how many request ahead do you want to consider 
	mithril_init.block_size = block_size;
	mithril_init.max_support=10;
	mithril_init.min_support=1;
	mithril_init.confidence=1;
	mithril_init.pf_list_size=1; // number of association that can be stored 
	mithril_init.max_metadata_size=max_metadata_size;  
	mithril_init.sequential_type=0; // no sequential access, so only 
	mithril_init.mining_threshold=100;
	mithril_init.output_statistics=1;

	// initiate MITHRIL 
	cache_t* mithril_cache=Mithril_init(cache_size, 
		data_type, block_size, &mithril_init);

	// initiate the reader  
	reader_t* reader = setup_reader("../data/w99_vscsi1.vscsitrace", 
		'v', 'l', BLOCK_UNIT_SIZE, DISK_SECTOR_SIZE, NULL);

	// get the number of reuests in file 
	gint64 num_req = get_num_of_req(reader);

	// read each request of the trace 
	request_t* cp = new_req_struct();
	cp->label_type='l';

	// training set size 
	gint64 training_set_size = (gint64) ((double)num_req/2.0);
	gint64 cur_request_num = 0;

    read_one_element(reader, cp);
    while (cp->valid){
		gboolean add_bool = Mithril_add_element(mithril_cache, cp);
    	read_one_element(reader, cp);
    	if (cur_request_num >= training_set_size) {
    		break;
    	}
    	cur_request_num++;
	}

	Mithril_params_t *mithril_results = (Mithril_params_t *)(mithril_cache->cache_params);

	gint32 ptable_cur_row = mithril_results->ptable_cur_row;
	gint64 **ptable_array = mithril_results->ptable_array;

	gint dim1, dim2;
	for (int i=0; i<ptable_cur_row; i++) {
	    dim1 = (gint)floor(i/(double)PREFETCH_TABLE_SHARD_SIZE);
	    dim2 = i % PREFETCH_TABLE_SHARD_SIZE *
	           (mithril_results->pf_list_size + 1);
	    gint cur_block = ptable_array[dim1][dim2];
	    if (cur_block != 0) {
		    printf("%d,", cur_block);
		    for (int j=1; j<mithril_results->pf_list_size+1; j++) {
		    	gint associated_block = ptable_array[dim1][dim2+j];
		    	if (associated_block != 0) printf("%d", associated_block);
		    }
		    printf("\n");
		}
	    
	}

} 