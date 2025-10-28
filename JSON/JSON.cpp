#include "JSON.h"

#define _WIN32_WINNT 0x0A00  // Windows 10
#include <afxwin.h>
#include <fstream>
#include <afxdlgs.h>  // For CFileDialog
#include <string>
#include <algorithm>
#include <locale>

void replaceString(std::wstring& stringToModify, const std::wstring& stringToSearch, const std::wstring& replacementString)
{
	size_t pos = 0;
	while ((pos = stringToModify.find(stringToSearch, pos)) != std::wstring::npos)
	{
		stringToModify.replace(pos, stringToSearch.length(), replacementString);
		pos += replacementString.length(); // Move past the replacement
	}
}

void makeStringLowercase(std::wstring& stringToLowerCase)
{
	std::locale loc;
	std::transform(stringToLowerCase.begin(), stringToLowerCase.end(), stringToLowerCase.begin(), [&loc](wchar_t c)
	{
		return std::tolower(c, loc);
	});
}

std::wstring getFolder(const std::wstring& path)
{
	size_t pos = path.find_last_of(L"\\/"); // Handles both '/' and '\'
	if (pos != std::wstring::npos)
	{
		return path.substr(0, pos + 1); // Include the trailing slash/backslash
	}
	return L""; // No folder found
}

std::wstring GetModulePathNoExe()
{
	wchar_t buffer[MAX_PATH];
	DWORD length = GetModuleFileNameW(nullptr, buffer, MAX_PATH);
	if (length == 0 || length == MAX_PATH)
	{
		// Handle error or truncation
		return L"";
	}
	std::wstring path(buffer, length);
	return getFolder(path);
}

std::wstring getLoadFilePath()
{
	// Use initially the same folder as the app binary file
	static std::wstring g_wpath = GetModulePathNoExe() + L"data.json";

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

std::wstring getSaveFilePath()
{
	// Use initially the same folder as the app binary file
	static std::wstring g_wpath = GetModulePathNoExe() + L"data.json";

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
