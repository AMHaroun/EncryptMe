#include <iostream>
#include <string>
#include <atltypes.h>
#include <openssl/evp.h>

/* Base64 encodes a byte string of given the length of that string in bytes and returns an encoded string
 * 
 * PARAM 1 - BYTE* input - Buffer where the binary data is stored that is to be encoded
 * 
 * PARAM 2 - int length - The length of the binaryData buffer in bytes
 * 
 * RETURN - char* - Returns a char* that contains the base64 encoded version of binaryData (remember to call free()!)
 *
 */


char* Base64Encode(const BYTE* input, int length) {

	const int encodedDataLength = 4 * ((length + 2) / 3); // Approximately how much more data input will take when encoded

	char* output = reinterpret_cast<char*>(calloc(encodedDataLength + 1, 1)); //+1 for the terminating null that EVP_EncodeBlock adds on

	const int dataEncoded = EVP_EncodeBlock(reinterpret_cast<unsigned char*>(output), input, length);
	if (encodedDataLength != dataEncoded) 
	{ 
		std::cerr << "Could not encode data" << std::endl;
	}

	return output;
}
