#pragma once
#include <atltypes.h>
#define AES_KEYSIZE 32 // or 256 bits
#define AES_KEYENCODEDLENGTH 48 // approximately how much space an encoded AES key will take up

bool GenerateKeyAES(BYTE* key, const char* fileName);
