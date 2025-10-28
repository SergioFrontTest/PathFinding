#pragma once

#include "TerrainWalk.h"

#define _WIN32_WINNT 0x0A00  // Windows 10
#include <afxwin.h>
#include <vector>

// Sub-images count in the loaded terrain image
constexpr int TerrainSubImagesCountX = 16;
constexpr int TerrainSubImagesCountY = 16;

// Default map size
constexpr int TerrainDefaultWidth = 32;
constexpr int TerrainDefaultHeight = 32;

// Default screen size
constexpr int TerrainCellWidth = 32;
constexpr int TerrainCellHeight = 32;
constexpr int TerrainDefaultScreenWidth = TerrainDefaultWidth * TerrainCellWidth;
constexpr int TerrainDefaultScreenHeight = TerrainDefaultHeight * TerrainCellHeight;

// Map cell values
constexpr int MapValue_OutsideTerrain = -1000;
constexpr int MapValue_StartingBattleUnitPosition = 0;
constexpr int MapValue_TargetPostion = 8;
constexpr int MapValue_ElevatedTerrainPosition = 3;
constexpr int MapValue_ReachableTerrainPosition = -1;

class Terrain
{
public:
	Terrain();
	virtual ~Terrain();

	// Map data
	int getWidth();
	int getHeight();
	int getAt(int x, int y);
	void setAt(int x, int y, int cellValue);
	bool isReachablePosition(int x, int y);
	TerrainWalk* isBattleUnitPosition(int x, int y); // Returns the battle unit walk of the unit at this position. It returns nullptr if no battle unit is at this position.
	void clearMap();
	void resize(const int mapwidth, const int mapheight);

	// Used by display classes
	std::vector<std::vector<int>>* getMap();
	std::vector<TerrainWalk>* getTerrainWalks();

	// Convert the terrain cell type to the source terrain image's block column and row (or line), to determine image coordinates
	void getCellToTerrainImageColumnAndRow(int cell, int& imageColumn, int& imageRow);
	void getCellToTerrainImageColumnAndRowBattleUnit(int& imageColumn, int& imageRow);

	// File operations
	bool loadFromFileUsingDialogBox();
	//bool saveToFileUsingDialogBox();
	bool saveWalksToFileUsingDialogBox();

	void addBattleUnit(int x, int y);
	void initializeTargetPositionsAndBattleUnits();

protected:
	std::vector<TerrainWalk> m_terrainWalks;
	std::vector<std::vector<int>> m_map;
};
