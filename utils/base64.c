#include <stdio.h>
#include <stdlib.h>

char table[65] = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	"abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

int getsizeof(const unsigned char* str) {
	int len = 0;
	while (str[len] != '\0') {
		len++;
	}

	return len;
}

int binToDen(char* bin) {
	int header = 32, value = 0;

	for (int i = 0; i < 6; i++) {
		if (bin[i] == '1') {
			value += header;
		}
		header /= 2;
	}

	return value;
}

void charToByte(char c, char* bin, int idx) {
	int mask = 0b10000000;

	for (int i = idx; i < idx + 8; i++) {
		bin[i] = c & mask ? '1' : '0';
		mask >>= 1;
	}
}

int readFile(const char* filename, unsigned char** buffer) {
	FILE* f = fopen(filename, "rb");

	if (!f) {
		perror("Failed to open file");
		return -1;
	}

	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	rewind(f);

	*buffer = malloc(size);
	
	if (!*buffer) {
		perror("Memory allocation failed");
		fclose(f);
		return -1;
	}

	fread(*buffer, 1, size, f);
	fclose(f);
	return size;
}

const unsigned char* encode(const unsigned char *input, int* outputLen) {
	const int inputLen = getsizeof(input);

	const int encodedLen = ((inputLen + 2) / 3) * 4;
	const int totalBits = encodedLen * 6;
	const int padding = (3 - (inputLen % 3)) % 3;

	char bin[totalBits + 1];

	for (int i = 0; i < totalBits; i++) {
		bin[i] = '0';
	}

	bin[totalBits] = '\0';

	for (int i = 0; i < inputLen; i++) {
		charToByte(input[i], bin, i * 8);
	}

	char buf[6];
	/*char encoded[encodedLen + 1];*/

	unsigned char *encoded = malloc(encodedLen + 1);
	if (!encoded) {
		return NULL;
	}

	int j = 0;
	for (int i = 0; i < totalBits; i++) {
		buf[i % 6] = bin[i];

		if ((i + 1) % 6 == 0) {
			encoded[j++] = table[binToDen(buf)];
		}
	}

	for (int i = 0; i < padding; i++) {
		encoded[encodedLen - 1 - i] = '=';
	}

	encoded[encodedLen] = '\0';

	*outputLen = encodedLen;
	return encoded;
}

int getTableIndex(char c) {
	for (int i = 0; i < 64; i++) {
		if (table[i] == c) {
			return i;
		}
	}

	return -1;
}

void charToBin(char c, char* bin, int idx) {
	int mask = 0b100000;
	for (int i = idx; i < idx + 8; i++) {
		bin[i] = c & mask ? '1' : '0';
		mask >>= 1;
	}
}

char binToByte(char* bin) {
	int header = 128, value = 0;

	for (int i = 0; i < 8; i++) {
		if (bin[i] == '1') {
			value += header;
		}
		header /= 2;
	}

	return (char)value;
}

void writeToFile(const unsigned char* output, int length, const char* filename) {
	FILE *f = fopen(filename, "wb");
	if (!f) {
		perror("Failed to open file");
		return;
	}
	fwrite(output, 1, length, f);
	fclose(f);

}

const unsigned char* decode(const unsigned char* input, int* outputLen) {
	int inputLen = getsizeof(input);

	while (input[inputLen - 1] == '=') {
		inputLen--;
	}

	char bin[inputLen * 6 + 1];
	int k = 0;

	for (int i = 0; i < inputLen; i++) {
		int index = getTableIndex(input[i]);
		if (index == -1) {
			continue;
		}

		for (int j = 5; j >= 0; j--) {
			bin[k++] = (index & (1 << j)) ? '1' : '0';
		}
	}
	bin[k] = '\0';

	int totalBytes = k / 8;
	unsigned char* decoded = malloc(totalBytes + 1);

	for (int i = 0; i < totalBytes; i++) {
		decoded[i] = binToByte(&bin[i * 8]);
	}
	decoded[totalBytes] = '\0';

	*outputLen = totalBytes;
	return decoded;
}


void printBinary(unsigned char byte) {
	for (int i = 7; i >= 0; i--) {
		printf("%c", (byte & (1 << i)) ? '1' : '0');
	}
}

void printBinaryBuffer(const unsigned char* data, int len) {
	for (int i = 0; i < len; i++) {
		printBinary(data[i]);
		printf(" ");
	}
	printf("\n");
}


int main() {
	/*const unsigned char text[] = "H4sIAAAAAAACAysuSUktKopPyVbILFYIcvR0ycxLVyjPLMlQSFQoSCwqqVTIT1Mw1QMA1OS3RycAAAA=";*/
	/**/
	/*int decodedLen;*/
	/*const unsigned char* decoded = decode(text, &decodedLen);*/
	/*printBinaryBuffer(decoded, decodedLen);*/
	/*writeToFile(decoded, decodedLen, "decoded.txt");*/
	/**/
	/*free((void*)decoded);*/
	/**/
	/*unsigned char* data;*/
	/*int size = readFile("input.gz", &data);*/
	/*if (size < 0) return 1;*/
	/**/
	/*int encodedLen;*/
	/*const unsigned char* encoded = encode(data, &encodedLen);*/
	/*writeToFile(encoded, encodedLen, "other.gz");*/
	/**/
	/*free(data);*/
	/**/
	/*return 0;*/
	
	const unsigned char text[] = "SGVsbG8gV29ybGQ=";
	int decodedLen;
	const unsigned char* decoded = decode(text, &decodedLen);
	printf("Decoded: %s\n", decoded);
	writeToFile(decoded, decodedLen, "decoded.txt");
	free((void*)decoded);

}
