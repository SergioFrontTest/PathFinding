#pragma once

#define _WIN32_WINNT 0x0A00  // Windows 10
#include <afxwin.h>
#include <string>

class Terrain;
class Movements;

class JSON {
public:
	// Terrain conversions
	static bool convertToTerrain(CString jsonDataSource, Terrain* pTerrainDestination);
	static CString convertFromTerrain(Terrain* pTerrainSource);

	// Movements history conversions
	static bool convertToMovements(CString jsonDataSource, Movements* pMovementsDestination);
	static CString convertFromMovements(Movements* pMovementsSource);

	// File operations
	static CString loadFromFile(CString fullPathFilename);
	static bool saveToFile(CString fullPathFilename, CString jsonDataSource);

	// File operations using common file open/save dialog box
	static std::wstring getLoadFilePath();
	static std::wstring getSaveFilePath();
};