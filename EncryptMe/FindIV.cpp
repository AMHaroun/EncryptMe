#include <atltypes.h>
#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <nlohmann/json.hpp>
#include "GenerateIVAES.hpp" // for AES_IVSIZE
#include "Base64Decode.hpp"

/* Find a object in the "files" array in secretkeys.json that matches fileName, and pull the encoded intialization vector information decode it and store it in BYTE* iv
 *
 * PARAM 1 - BYTE* iv - Buffer where the decoded initalization vector will be stored, this should be big enough for the decoded initalization vector
 *
 * PARAM 2 - const char* fileName - The name of the file, used to look for a fileName object in secretkeys.json
 *
 * RETURN - Returns true or false, true if the initalization vector has been found and stored in BYTE* iv and false if it has not been found
 *
 */
bool FindIV(BYTE* iv, const char* fileName)
{
	// We are under the assumption that secretkeys.json exsits. we will not be doing any correction if it does not exsist or if the json data is not structured correctly
	std::fstream secretKeys("secretkeys.json", std::ios::in);
	if (!secretKeys.is_open())
	{
		std::cerr << "Could not open secretkeys.json" << std::endl;
		return false;
	}

	std::string jsonFileName(fileName);
	std::string encodedIV;

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
				encodedIV = item["iv"];

				BYTE* decodedIV = Base64Decode(encodedIV.c_str(), encodedIV.length());
				memcpy(iv, decodedIV, AES_IVSIZE);
				free(decodedIV);

				return true;
			}
		}
	}

	// reaching this point we could not find the key, either there is not a object in the "files" array for our fileName or the json file is empty
	return false;
}