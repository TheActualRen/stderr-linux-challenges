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

int binToDen(char *bin) {
	int header = 32, value = 0;

	for (int i = 0; i < 6; i++) {
		if (bin[i] == '1') {
			value += header;
		}
		header /= 2;
	}

	return value;
}

void charToBin(char c, char *bin, int idx) {
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
		charToBin(input[i], bin, i * 8);
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

int main() {
	const unsigned char text[] = "";
	encode(text);

	return 0;
}
