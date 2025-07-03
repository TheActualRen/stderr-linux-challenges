#include <stdio.h>
#include <string.h>

char table[64] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void binToDec(char bin[]) {
	int dec = 0;

	for (int i = 0; i < 6; i++) {
		dec *= 2;
		dec += bin[i] - 48;
	}


	printf("%d %c\n", dec, table[dec]);
}

const char* charToBin(char c) {
	char bin[8];

	for (int i = 0; i < 8; i++) {
		int num = !!((c << i) & 0x80);
		char ch = (char) num;
		bin[i] = ch;
		printf("%d", !!((c << i) & 0x80));
	}
	printf("\n");
	const char* ptr = &bin;
	return ptr;
}

/*
	In the switch statement, keep track of padding needed
	We can then use the number of padding to ignore last n amount of bytes
	We can iterate through table to check against until we run out of padding
*/

void encode(char text[]) {
	int padding = 0;

	switch (strlen(text) % 3) {
		case 1:
			printf("2 Bytes of Padding\n");
			padding = 2;
			break;
		case 2:
			printf("1 Byte of Padding\n");
			padding = 1;
			break;
		default:
			printf("No padding needed\n");
			break;
	}

	const int len = (strlen(text) + padding) * 8;
	char bin[len];

	for (int i = 0; i < strlen(text); i++) {
		const char* byte = charToBin(text[i]);
		printf("%s\n", byte);
	}
}

int main() {
	char text[] = "Ands";
	encode(text);
		
}
