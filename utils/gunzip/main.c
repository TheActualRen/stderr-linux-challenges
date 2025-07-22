#include "gunzip.h"

#include <stdio.h>

int main() {

	/*gunzip_encode("input.txt", "output.gz", DEFLATE_BLOCK_STORED);*/
	/*printf("Success. STORED UNCOMPRESSED compression complete.");*/
	
	gunzip_encode("input.txt", "final.gz", DEFLATE_BLOCK_FIXED);
	printf("Sucess. FIXED CODE compression complete.");

	return 0;
}
