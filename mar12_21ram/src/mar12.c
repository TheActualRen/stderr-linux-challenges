#include <stdio.h>
#include <stdlib.h>
#include "../../utils/base64.h"


int main() {
	char unsigned startpoint[] =
		"=AAAAcyR3SO1AMQ1wM1TITVqqwCSoQFSQlMLPjyVLxcy0RvcIYFLIbVyPpoKtkUSusyACAAAAAAAIs4H";
	const int length = getsizeof(startpoint);

	char unsigned reverse[length + 1];
	reverse[length] = '\0';

	for (int i = 0; i < length; i++) {
		const int j = length - 1 - i;
		reverse[i] = startpoint[j];
	}

	int decodedLen;
	const unsigned char* decoded = decode(reverse, &decodedLen);
	printBinaryBuffer(decoded, decodedLen);
	writeToFile(decoded, decodedLen, "../myfile.gz");
	free((void*)decoded);

	printf("Process Complete\n");
}


