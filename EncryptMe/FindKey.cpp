#include <atltypes.h>
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <nlohmann/json.hpp>
#include "GenerateKeyAES.hpp" // for AES_KEYSIZE and AES_KEYENCODEDLENGTH
#include "Base64Decode.hpp"

/* Find a object in the "files" array in secretkeys.json that matches fileName, and pull the encoded key information decode it and store it in BYTE* key
 * 
 * PARAM 1 - BYTE* key - Buffer where the decoded key will be stored, this should be big enough for the decoded key 
 * 
 * PARAM 2 - const char* fileName - The name of the file, used to look for a fileName object in secretkeys.json
 * 
 * RETURN - Returns true or false, true if the key has been found and stored in BYTE* key and false if it has not been found
 *
 */
bool FindKey(BYTE* key, const char* fileName)
{
	// We are under the assumption that secretkeys.json exsits. we will not be doing any correction if it does not exsist or if the json data is not structured correctly
	std::fstream secretKeys("secretkeys.json", std::ios::in);
	if (!secretKeys.is_open())
	{
		std::cerr << "Could not open secretkeys.json" << std::endl;
		return false;
	}

	std::string jsonFileName(fileName);
	std::string encodedKey;

	nlohmann::json jsonData;
	secretKeys >> jsonData;
	secretKeys.close();

	// Let's now search for an object in the "files" array that matches our fileName
	if (jsonData.contains("files") && jsonData["files"].is_array())
	{
		for (auto& item : jsonData["files"])
		{
			if (item.contains("fileName") && item["fileName"] == jsonFileName)
			{
				encodedKey = item["key"];

				BYTE* decodedKey = Base64Decode(encodedKey.c_str(), encodedKey.length());
				memcpy(key, decodedKey, AES_KEYSIZE);
				free(decodedKey);

				return true;
			}
		}
	}

	// reaching this point we could not find the key, either there is not a object in the "files" array for our fileName or the json file is empty
	return false;
}