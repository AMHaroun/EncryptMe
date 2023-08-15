#include <atltypes.h>
#include <fstream>
#include <iostream>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <nlohmann/json.hpp>
#include "GenerateKeyAES.hpp" // for AES_KEYSIZE and AES_KEYENCODEDLENGTH
#include "Base64Encode.hpp"

/* Generate a random key, base64 encode it and store it in secretskeys.json for later decryption
 * I recommend you to run the program and look at the secretkeys.json file that gets generated to get better context of this code
 * 
 * PARAM 1 - BYTE* key - Buffer where the random key will be stored, it is not base64 encoded and should be 32 bytes in size for AES keys
 * 
 * PARAM 2 - const char* fileName - The name of the file, this is used to create an entry for that file's key and initalization vector in secretkeys.json
 * 
 * RETURN - Return true or false, true if the key has been recorded in secretkeys.json and false if it has not
 * 
 * GOOD ADDITIONS -> It would be good to handle the case where the secretkeys.json file exsists but is completely empty, but this isn't a priority as
 * secretkeys.json isn't meant to be a file that users should interact with
 *
 */
bool GenerateKeyAES(BYTE* key, const char* fileName)
{
	// Generate Random Key
	RAND_bytes(key, AES_KEYSIZE);

	// Data to write to json file
	std::string jsonKey = Base64Encode(key, AES_KEYSIZE);
	std::string jsonFileName(fileName);

	std::fstream secretKeys("secretkeys.json", std::ios::in);
	if (secretKeys.is_open()) // secretkeys.json allready exsists and we need to add our key
	{
		nlohmann::json jsonData;
		secretKeys >> jsonData;
		secretKeys.close();

		// if a object for this file allready exsists we will update the key
		if (jsonData.contains("files") && jsonData["files"].is_array())
		{
			for (auto& item : jsonData["files"])
			{
				if (item.contains("fileName") && item["fileName"] == jsonFileName)
				{
					item["key"] = jsonKey;
					secretKeys.open("secretkeys.json", std::ios::out);
					if (!secretKeys.is_open())
					{
						std::cerr << "Could not open secretkeys.json" << std::endl;
						return false;
					}

					// std::setw(4) pretty prints to the jsonfile
					secretKeys << std::setw(4) << jsonData;
					secretKeys.close();
					return true;
				}
			}
		}

		// if we reach this point, there is not a exsisting object in the "files" array so let's create one and include our key information
		jsonData["files"].push_back(
			{
			{"fileName", jsonFileName},
			{"encryption", "AES"},
			{"key", jsonKey},
			{"iv", nullptr}
			}
		);

		secretKeys.open("secretkeys.json", std::ios::out);
		if (!secretKeys.is_open())
		{
			std::cerr << "Could not open secretkeys.json" << std::endl;
			return false;
		}
		
		secretKeys << std::setw(4) << jsonData;
		secretKeys.close();
	}
	else
	{
		// There is not an exsisting secretkeys.json file let's create one with a json "files" array and add a object with our key information
		secretKeys.open("secretkeys.json", std::ios::out);
		if (!secretKeys.is_open())
		{
			std::cout << "Could not create secretkeys.json" << std::endl;
			return false;
		}

		nlohmann::json jsonData;
		jsonData =
		{
			{"files", nlohmann::json::array({
			{
				{"fileName", jsonFileName},
				{"encryption", "AES"},
				{"key", jsonKey},
				{"iv", nullptr}
			}
		})}
		};

		secretKeys << std::setw(4) << jsonData;
		secretKeys.close();
	}

	return true;

}
