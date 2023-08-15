#pragma once
#include <atltypes.h>
#define AES_IVSIZE 16 // or 256 bits

// This is approximately how many bytes an AES iv would take when encoded in base64
#define AES_IVENCODEDLENGTH 38

bool GenerateIVAES(BYTE* iv, const char* fileName);