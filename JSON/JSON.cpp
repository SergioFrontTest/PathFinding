#include "JSON.h"

// Include the nlohmann/json library header
#include "../Third Party/JSON/json.hpp"
using json = nlohmann::json;

#include <fstream>
#include <afxdlgs.h>  // For CFileDialog

bool JSON::convertToTerrain(CString jsonDataSource, Terrain* pTerrainDestination)
{
	return true;
}

CString JSON::convertFromTerrain(Terrain* pTerrainSource)
{
	return "";
}

bool JSON::convertToMovements(CString jsonDataSource, Movements* pMovementsDestination)
{
	return true;
}

CString JSON::convertFromMovements(Movements* pMovementsSource)
{
	return "";
}

CString JSON::loadFromFile(CString fullPathFilename)
{
	// read a JSON file
	std::ifstream i("file.json");
	json j;
	i >> j;

	// write prettified JSON to another file
	std::ofstream o("pretty.json");
	o << std::setw(4) << j << std::endl;

	return "";
}

bool JSON::saveToFile(CString fullPathFilename, CString jsonDataSource)
{
	return true;
}

std::wstring GetModulePath()
{
	wchar_t buffer[MAX_PATH];
	DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
	if (length == 0 || length == MAX_PATH)
	{
		// Handle error or truncation
		return L"";
	}
	return std::wstring(buffer, length);
}

std::wstring JSON::getLoadFilePath()
{
	// Use initially the same folder as the app binary file
	static std::wstring g_wpath = GetModulePath();

	// Convert filename string to MFC string for MFC file open dialog
	CString path(g_wpath.c_str());

	// Create an open file dialog for JSON files
	CFileDialog dlg(TRUE, "json", path,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,
		"JSON Files (*.json)|*.json||");

	if (dlg.DoModal() == IDOK)
	{
		// Full path to selected file
		CString path = dlg.GetPathName();

		// Convert filename back to std wide char string
		g_wpath = CT2W(path);

		return g_wpath;
	}

	return L"";
}

std::wstring JSON::getSaveFilePath()
{
	// Use initially the same folder as the app binary file
	static std::wstring g_wpath = GetModulePath() + L"data.json";

	// Convert filename string to MFC string for MFC file open dialog
	CString path(g_wpath.c_str());

	// Create a save file dialog for JSON files
	CFileDialog dlg(FALSE, "json", path,
		OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY,
		"JSON Files (*.json)|*.json||");

	if (dlg.DoModal() == IDOK)
	{
		// Full path to selected file
		CString path = dlg.GetPathName();

		// Convert filename back to std wide char string
		g_wpath = CT2W(path);

		return g_wpath;
	}

	return L"";
}
