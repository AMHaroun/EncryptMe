#include <iostream>
#include <openssl/evp.h>
#include <atltypes.h>

/* Similar to Base64Encode, please read Base64Encode's documentation for more context on this function
 *
 */

BYTE* Base64Decode(const char* input, int length) 
{

	const int decodedDataLength = 3 * length / 4;

	BYTE* output = reinterpret_cast<BYTE*>(calloc(decodedDataLength + 1, 1));

	const int dataDecoded = EVP_DecodeBlock(output, reinterpret_cast<const BYTE*>(input), length);

	if (decodedDataLength != dataDecoded) 
	{ 
		std::cerr << "Could not decode data" << std::endl; 
	}

	return output;
}