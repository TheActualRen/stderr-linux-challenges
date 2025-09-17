#include <stdio.h>

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

void encode(const unsigned char *input) {
	const int inputLen = getsizeof(input);

	const int encodedLen = ((inputLen + 2) / 3) * 4;
	const int totalBits = encodedLen * 6;
	const int padding = (3 - (inputLen % 3)) % 3;

	char bin[totalBits + 1];
	bin[totalBits] = '\0';

	for (int i = 0; i < inputLen; i++) {
		charToByte(input[i], bin, i * 8);
	}

	char buf[6];
	char encoded[encodedLen + 1];
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
	printf("Encoded: %s\n", encoded);
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

void decode(const unsigned char* encoded) {
	// remove the padding if any
	// convert the char to int using the table
	// put the num into 6 bit binary charToBin()
	// go over 8 bits 
	// convert to char
	
	int len = getsizeof(encoded);

	while (encoded[len - 1] == '=') {
		len--;
	}

	char bin[len * 6 + 1];
	int k = 0;

	for (int i = 0; i < len; i++) {
		int index = getTableIndex(encoded[i]);
		if (index == -1) {
			continue;
		}

		for (int j = 5; j >= 0; j--) {
			bin[k++] = (index & (1 << j)) ? '1' : '0';
		}
	}
	bin[k] = '\0';

	int totalBytes = k / 8;
	char output[totalBytes + 1];

	for (int i = 0; i < totalBytes; i++) {
		output[i] = binToByte(&bin[i * 8]);
	}
	output[totalBytes] = '\0';

	printf("Decoded: %s\n", output);

	/*FILE *f = fopen("input.gz", "wb");*/
	/*if (!f) {*/
	/*	perror("Failed to open file");*/
	/*	return;*/
	/*}*/
	/*fwrite(output, 1, totalBytes, f);*/
	/*fclose(f);*/
}


int main() {
	const unsigned char encoded_text[] = "H4sIAAAAAAACAysuSUktKopPyVbILFYIcvR0ycxLVyjPLMlQSFQoSCwqqVTIT1Mw1QMA1OS3RycAAAA=";
	decode(encoded_text);

	return 0;
}
