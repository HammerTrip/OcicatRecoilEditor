#include "platform.h"

#ifdef _WIN32

#include <windows.h>

std::string pf_get_save_file () {
	OPENFILENAME f;
	char filename [MAX_PATH];

	memset(&f, 0, sizeof(f));
	memset(filename, 0, sizeof(filename));

	f.lStructSize = sizeof(f); 
	f.hwndOwner = NULL;
	f.lpstrFilter = TEXT("Json File (*.json)\0*.json\0All Files (*.*)\0*.*\0");
	f.lpstrFile = filename;
	f.nMaxFile = MAX_PATH;
	f.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	f.lpstrDefExt = TEXT("json");

	GetSaveFileName(&f);

	std::string result = std::string(filename);
	return result;
}

std::string pf_get_open_file () {
	OPENFILENAME f;
	char filename [MAX_PATH];

	memset(&f, 0, sizeof(f));
	memset(filename, 0, sizeof(filename));

	f.lStructSize = sizeof(f); 
	f.hwndOwner = NULL;
	f.lpstrFilter = TEXT("Json File (*.json)\0*.json\0All Files (*.*)\0*.*\0");
	f.lpstrFile = filename;
	f.nMaxFile = MAX_PATH;
	f.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	f.lpstrDefExt = TEXT("json");

	GetOpenFileName(&f);

	std::string result = std::string(filename);
	return result;
}

#endif