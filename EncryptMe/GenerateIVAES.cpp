#include <atltypes.h>
#include <fstream>
#include <iostream>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <nlohmann/json.hpp>
#include "GenerateIVAES.hpp" // for AES_IVSIZE and AES_IVENCODEDLENGTH
#include "Base64Encode.hpp"

/* Generate a random initalization vector, base64 encode it and store it in secretkeys.json for later decryption similar to GenerateKeyAES()
 * I recommend you to run the program and look at the secretkeys.json file that gets generated to get better context of this code
 * 
 * PARAM 1 - BYTE* IV - Buffer where the random iv will be stored, it is not base64 encoded and should be 32 bytes in size for AES iv's
 *
 * PARAM 2 - const char* fileName - The name of the file, this is used to create an entry for that file's key and initalization vector in secretkeys.json
 *
 * RETURN - Return true or false, true if the iv has been recorded in secretkeys.json and false if it has not
 *
 * GOOD ADDITIONS -> It would be good to handle the case where the secretkeys.json file exsists but is completely empty, but this isn't a priority as
 * secretkeys.json isn't meant to be a file that users should interact with
 *
 */
bool GenerateIVAES(BYTE* iv, const char* fileName)
{
	// Generate random initalization vector
	RAND_bytes(iv, AES_IVSIZE);

	// Data to write to json file
	std::string jsonIV = Base64Encode(iv, AES_IVSIZE);
	std::string jsonFileName(fileName);

	std::fstream secretKeys("secretkeys.json", std::ios::in);
	if (secretKeys.is_open()) // secretkeys.json allready exsists and we need to add our initalization vector
	{
		nlohmann::json jsonData;
		secretKeys >> jsonData;
		secretKeys.close();

		// if a object for this file allready exsists we will update the initalization vector
		if (jsonData.contains("files") && jsonData["files"].is_array())
		{
			for (auto& item : jsonData["files"])
			{
				if (item.contains("fileName") && item["fileName"] == jsonFileName)
				{
					item["iv"] = jsonIV;
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

		// if we reach this point, there is not a exsisting object in the "files" array so let's create one and include our initalization vector information
		jsonData["files"].push_back(
			{
			{"fileName", jsonFileName},
			{"encryption", "AES"},
			{"key", nullptr},
			{"iv", jsonIV}
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
		// There is not an exsisting secretkeys.json file let's create one with a json "files" array and add a object with our initalization vector information
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
				{"key", nullptr},
				{"iv", jsonIV}
			}
		})}
		};

		secretKeys << std::setw(4) << jsonData;
		secretKeys.close();
	}

	return true;

}