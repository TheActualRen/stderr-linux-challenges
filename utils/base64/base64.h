#ifndef BASE64_H
#define BASE64_H

int getsizeof(const unsigned char* str);
int binToDen(char* bin);
void charToByte(char c, char* bin, int idx);
void encode(const unsigned char* input, int* outputLen);

int getTableIndex(char c);
void charToBin(char c, char* bin, int idx);
char binToByte(char* bin);
const unsigned char* decode(const unsigned char* input, int* outputLen);

void writeToFile(const unsigned char* output, int length, const char* filename);
int readFile(const char* filename, unsigned char** buffer);

void printBinary(unsigned char byte);
void printBinaryBuffer(const unsigned char* data, int len);

#endif
