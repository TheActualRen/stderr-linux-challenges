#include "header.h"

/*
	- To make something that the actual gunzip utility can decode, 
	- we need to write it in the format of a .gz file
	- A .gz file acts like a container in which the gunzip utility recongnises

	Key Idea:
	- The gzip header is a fixed 10-byte sequence
	- Bytes 1-2 are MAGIC numbers
	- Byte 3 is the compression method
		- I think gzip only recongnises DELFATE though
	- Byte 4 is flags which we dont need to use, so we set to 0
	- Bytes 5-8 are modification time which we dont really need, so set to 0
	- Byte 9 is extra flags which wedont need, so set to 0
	- Byte 10 indicates a bunch of OSs but the only one that really matters is UNIX

	- This header must be presentfor every gzip file
*/

void write_gzip_header(FILE* out) {
	unsigned char header[10] = {
		0x1F, // MAGIC BYTE 1
		0x8B, // MAGIC BYTE 2
		0x08, // Compression Method: DEFLATE
		0x00, // Flags, set to 0
		0x00, 0x00, 0x00, 0x00, // mTime, set to 0
		0x00, // XFL, not worth setting tbh
		0x03 // Specify UNIX OS
	};
	fwrite(header, 1, sizeof(header), out);
}
