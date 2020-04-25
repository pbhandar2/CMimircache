#include "reader.h"

int main() { 
	// Get the data reader 
	reader_t* reader = setup_reader("../data/w99_vscsi1.vscsitrace", 
		'v', 'l', BLOCK_UNIT_SIZE, DISK_SECTOR_SIZE, NULL);
	return 0;
}