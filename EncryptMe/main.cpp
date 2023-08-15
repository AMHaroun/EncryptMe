#include <iostream>
#include <Windows.h>
#include "EncryptFileAES.hpp"
#include "DecryptFileAES.hpp"
#include "IsContextMenuEntryPresent.hpp"
#include "AddContextMenuEntry.hpp"

/* The program is to be invoked by the windows context menu, so a file name should be passed in as the second argument */
int main(int argc, char* argv[])
{
	if (!IsContextMenuEntryPresent())
	{
		void AddContextMenuEntry();
	}


	if (argc < 2)
	{
		std::cerr << "Invalid number of arguments" << std::endl;
		return EXIT_FAILURE;
	}
	
	if (argc > 2)
	{
		int result = strcmp(argv[2], "-D");
		if (result == 0)
		{
			DecryptFileAES(argv[1]);
			return EXIT_SUCCESS;
		}
	}
	
	EncryptFileAES(argv[1]);

	return EXIT_SUCCESS;
}

