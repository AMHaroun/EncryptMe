#include <fstream>
#include <string>
#include <iostream>
#include <atltypes.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <nlohmann/json.hpp>
#include "FindKey.hpp"
#include "FindIV.hpp"
#include "GenerateKeyAES.hpp" // for AES_KEYSIZE
#include "GenerateIVAES.hpp" // for AES_IVSIZE

/* Decrypts Data from cipherText and outputs to plainText
 * 
 * PARAM 1 - char* cipherText - the buffer containing the cipher text to be decrypted
 * 
 * PARAM 2 - char* plainText - the buffer that will recieve the resulting plainText
 * 
 * PARAM 3 - const char* fileName - the name of the file to be decrypted
 * 
 * PARAM 4 - int fileLength - the size of the file in bytes
 * 
 * RETURN - returns true if the function successfully decrypts the data false if otherwise
 *
 */
bool DecryptAES(char* cipherText, char* plainText, const char* fileName, int fileLength, int* bytesWritten)
{
	EVP_CIPHER_CTX* cipherContext = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(cipherContext);

	BYTE key[AES_KEYSIZE];
	if (!FindKey(key, fileName))
	{
		std::cerr << "Could not find key" << std::endl;
		return false;
	}

	BYTE iv[AES_IVSIZE];
	if (!FindIV(iv, fileName))
	{
		std::cerr << "Could not find initalization vector" << std::endl;
		return false;
	}

	EVP_DecryptInit(cipherContext, EVP_aes_256_cbc(), key, iv); 
	EVP_CIPHER_CTX_set_padding(cipherContext, EVP_PADDING_PKCS7);
		
	// Here we are decrypting 100 bytes at a time, assuming that the plainText buffer is large enough
	int index = 0;
	int bytesDecrypted;
	for (int i = 0; i < fileLength / 100; i++)
	{
		EVP_DecryptUpdate(cipherContext, (unsigned char*)&plainText[index], &bytesDecrypted, (unsigned char*)&cipherText[index], 100);
		index += bytesDecrypted;
	}
	if (fileLength % 100)
	{
		EVP_DecryptUpdate(cipherContext, (unsigned char*)&plainText[index], &bytesDecrypted, (unsigned char*)&cipherText[index], fileLength % 100);
		index += bytesDecrypted;
	}

	EVP_DecryptFinal(cipherContext, (unsigned char*)&plainText[index], &bytesDecrypted);
	*bytesWritten = index + bytesDecrypted;
	EVP_CIPHER_CTX_free(cipherContext);

	if (ERR_peek_error())
	{
		std::cout << "OpenSSL Error:" << ERR_error_string(ERR_get_error(), NULL) << std::endl;
		return false;
	}

	return true;
}