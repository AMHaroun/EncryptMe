#include <fstream>
#include <iostream>
#include "DecryptAES.hpp"

/* DecryptFileAES() deals with file operations, actual decryption happens in DecryptAES() 
 * 
 * PARAM 1 - char* fileName -> Name of the file to be decrypted
 * 
 * RETURN - true if the file has been decrypted and false if not;
 *
 */
bool DecryptFileAES(char* fileName)
{
	std::fstream file(fileName, std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Error opening " << fileName << std::endl;
		return false;
	}

	int fileLength = 0;

	file.seekg(0, std::ios::end);
	fileLength = file.tellg();
	file.seekg(0, std::ios::beg);
	if (fileLength % 16)
	{
		fileLength = (fileLength / 16) * 16;
	}

	char* plainText = new char[fileLength];
	char* cipherText = new char[fileLength];

	file.read(cipherText, fileLength);

	int bytesDecrypted = 0;
	bool decrypted = DecryptAES(cipherText, plainText, fileName, fileLength, &bytesDecrypted);
	if (!decrypted)
	{
		std::cerr << "Could not decrypt data" << std::endl;
		return false;
	}

	// Reopening the file to wipe previous data and prepare to write the unencrypted data to it
	file.close();
	file.open(fileName, std::ios::out);
	if (!file.is_open())
	{
		std::cerr << "Error opening " << fileName << std::endl;
		return false;
	}

	file.write(plainText, bytesDecrypted);

	file.close();
	delete[] cipherText;
	delete[] plainText;
	return true;
}
