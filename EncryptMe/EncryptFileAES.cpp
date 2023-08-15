#include <fstream>
#include <iostream>
#include "EncryptAES.hpp"

/* EncryptFileAES() deals with file operations, EncryptAES() does the actual encryption of data 
 *
 *	PARAM 1 - char* fileName -> The name of the file that is to be encrypted
 * 
 *	RETURN - returns true if file has been encrypted anf false if it has not been encrypted
 *
 */
bool EncryptFileAES(char* fileName)
{
	std::fstream file;
	file.open(fileName, std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Could not open " << fileName << std::endl;
		return false;
	}
	
	int fileLength = 0;
	file.seekg(0, std::ios::end);
	fileLength = file.tellg();
	file.seekg(0, std::ios::beg);

	char* plainText = new char[fileLength];
	char* cipherText = new char[fileLength + 20]; // for padding
	file.read(plainText, fileLength);
	
	int bytesEncrypted = 0;
	bool encrypted = EncryptAES(plainText, cipherText, &bytesEncrypted, fileName, fileLength);
	if (!encrypted)
	{
		std::cerr << "Could not encrypt data" << std::endl;
		return false;
	}


	// Closing the file to reopen the file with no data inside so we can replace it with cipher text
	file.close();
	file.open(fileName, std::ios::out);
	if (!file.is_open())
	{
		std::cerr << "Could not open " << fileName << std::endl;
		return false;
	}

	file.write(cipherText, bytesEncrypted);
	file.close();

	delete[] plainText;
	delete[] cipherText;

	return true;
}