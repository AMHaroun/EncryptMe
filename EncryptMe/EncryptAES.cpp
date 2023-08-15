#include <atltypes.h>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "GenerateKeyAES.hpp"
#include "GenerateIVAES.hpp"

/* Encrypts plainText and writes the encrypted data to cipherText 
 *	
 * PARAM 1 - char* plainText - the buffer holding the plain text to be encrypted
 * 
 * PARAM 2 - char* cipherText - the buffer to recieve the cipher text output (Should be slightly larger than plainText to hold padding)
 * 
 * PARAM 3 - int* bytesWritten - recieves the amount of bytees written to cipherText
 * 
 * PARAM 4 - const char* fileName - the file name of the file that is being encrypted
 * 
 * PARAM 5 - int plainTextLength - the size of the plainText in bytes
 * 
 * RETURN - returns true if plainText was encrypted and put into cipherText and false if otherwise
 *
 *
 */
bool EncryptAES(char* plainText, char* cipherText, int* bytesWritten, const char* fileName, int plainTextLength)
{
	BYTE key[AES_KEYSIZE];
	if (!GenerateKeyAES(key, fileName))
	{
		std::cerr << "Could not generate key" << std::endl;
		return false;
	}


	BYTE iv[AES_IVSIZE];
	if (!GenerateIVAES(iv, fileName))
	{
		std::cerr << "Could not generate initalization vector" << std::endl;
		return false;
	}


	EVP_CIPHER_CTX* cipherContext = EVP_CIPHER_CTX_new();
	EVP_CIPHER_CTX_init(cipherContext);
	EVP_EncryptInit(cipherContext, EVP_aes_256_cbc(), key, iv);
	EVP_CIPHER_CTX_set_padding(cipherContext, EVP_PADDING_PKCS7);

	// Here we are encrypting 100 bytes at a time, assuming that the cipherText buffer is large enough for padding
	int index = 0;
	int bytesEncrypted;
	for (int i = 0; i < plainTextLength / 100; i++)
	{
		EVP_EncryptUpdate(cipherContext, (unsigned char*)&cipherText[index], &bytesEncrypted, (unsigned char*) & plainText[index], 100);
		index += bytesEncrypted;
	}
	if (plainTextLength % 100)
	{
		EVP_EncryptUpdate(cipherContext, (unsigned char*)&cipherText[index], &bytesEncrypted, (unsigned char*)&plainText[index], plainTextLength % 100);
		index += bytesEncrypted;
	}

	EVP_EncryptFinal(cipherContext, (unsigned char*)&cipherText[index], &bytesEncrypted);
	*bytesWritten = index + bytesEncrypted;
	EVP_CIPHER_CTX_free(cipherContext);

	
	if (ERR_peek_error())
	{
		std::cout << "OpenSSL Error:" << ERR_error_string(ERR_get_error(), NULL) << std::endl;
		EVP_CIPHER_CTX_free(cipherContext);
		return false;
	}
	

	return true;
}