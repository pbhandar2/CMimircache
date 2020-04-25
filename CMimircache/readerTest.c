#include "reader.h"

#define BLOCK_UNIT_SIZE 16 * 1024    // 16 * 1024
#define DISK_SECTOR_SIZE 512 // 512

int main() { 
	// Get the data reader 
	reader_t* reader = setup_reader("../data/w99_vscsi1.vscsitrace", 
		'v', 'l', BLOCK_UNIT_SIZE, DISK_SECTOR_SIZE, NULL);

	// read each request of the trace 
	request_t* cp = new_req_struct();
	cp->label_type='l';

    read_one_element(reader, cp);
    while (cp->valid){
    	*(gint64*)(cp->label_ptr) = (gint64) (*(gint64*)(cp->label_ptr) * DISK_SECTOR_SIZE/BLOCK_UNIT_SIZE);
    	printf("%ld \n", cp->label_ptr);
    	break;
		//Mithril_add_element(mithril_cache, cp);
    	read_one_element(reader, cp);
	}

	return 0;
}