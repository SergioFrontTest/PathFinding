#include "Terrain.h"

#include "../JSON/JSONTerrain.h"
#include "../JSON/JSONTerrainWalk.h"
using namespace JSONTerrain;
using namespace JSONTerrainWalk;

Terrain::Terrain()
{
	// Initialize default map
	std::vector<int> data = {
		-1, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, 3, -1, -1, -1, -1, 3, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, 3, -1, 3, -1, -1, -1, -1, -1, -1, 3, -1,
		-1, -1, 3, 3, 3, 3, -1, -1, -1, -1, 3, -1, 3, 3, 3, 3, 3, -1, 3, -1, -1,
		3, -1, 3, -1, -1, -1, -1, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, 3, -1, 3, -1, -1, -1, -1, -1, 3, 3, -1, -1, -1, 3, 3, 3, -1, -1,
		-1, -1, 3, -1, -1, -1, 3, 3, 3, 3, 3, 3, -1, -1, 3, -1, 3, -1, 3, 3, 3,
		-1, -1, 3, 3, 3, -1, -1, -1, 3, 3, 3, -1, -1, 3, -1, 3, 3, 3, -1, -1,
		-1, -1, 3, -1, -1, 3, -1, -1, 3, 3, -1, -1, -1, -1, -1, -1, -1, 3, 3,
		-1, 3, -1, 3, 3, -1, -1, -1, 3, -1, -1, -1, -1, -1, -1, 3, 3, 3, 3, -1,
		3, 3, -1, -1, -1, -1, -1, 3, -1, -1, -1, 3, -1, 3, -1, -1, 3, -1, -1,
		-1, -1, 3, -1, 3, 3, 3, -1, 3, -1, -1, -1, -1, 3, -1, -1, -1, -1, -1, 3,
		3, 3, -1, 0, 3, 3, 3, -1, -1, 3, -1, -1, -1, -1, 3, -1, -1, -1, 3, -1,
		3, -1, -1, -1, -1, -1, -1, -1, -1, 3, 3, 3, -1, 3, 3, -1, -1, -1, -1,
		-1, -1, 3, 3, -1, -1, -1, 3, 3, -1, -1, 3, -1, 3, -1, -1, -1, 3, 3, 3,
		-1, 3, 3, -1, -1, -1, -1, 3, -1, -1, 0, -1, -1, -1, -1, 3, 3, -1, -1,
		-1, 3, -1, -1, 3, -1, -1, 3, -1, 3, -1, -1, 3, -1, 3, -1, -1, -1, -1,
		-1, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, 3, -1, 3, 3,
		-1, 3, 3, -1, -1, -1, 3, -1, 3, -1, 3, -1, -1, 3, -1, 3, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, 3, -1, -1, 3, -1, -1, 3, 3, -1, -1, 3,
		-1, 3, -1, 3, 3, -1, 3, -1, 3, 3, 3, 3, 3, 3, 3, 3, 3, -1, -1, 3, 3, 3,
		-1, -1, 3, -1, -1, -1, 3, -1, -1, -1, -1, 3, -1, -1, 3, -1, 3, -1, 3,
		-1, 3, -1, -1, -1, -1, -1, -1, 3, 3, -1, -1, -1, -1, -1, 3, 3, 3, 3, 3,
		3, -1, -1, -1, 3, 3, 3, 3, -1, 3, -1, 3, -1, 3, -1, -1, -1, -1, 3, 3,
		-1, -1, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, 3, -1, -1, -1, -1, -1,
		-1, 3, -1, 3, -1, 3, -1, 3, -1, -1, -1, 3, 3, -1, -1, -1, -1, -1, 3, 3,
		3, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, 3, -1, 3, -1, 3, -1, 3,
		-1, -1, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, 3, 3, -1, -1, 3, -1, 3, -1, 3, -1, -1, 3, 3, -1, -1,
		-1, 3, -1, -1, 3, -1, -1, 3, -1, -1, -1, -1, 3, 3, 3, 3, 3, 3, -1, -1,
		-1, 3, -1, 3, -1, 3, -1, -1, 3, -1, -1, -1, -1, 3, -1, -1, 3, 3, -1, 3,
		3, -1, 3, 3, 3, -1, -1, -1, -1, -1, -1, 3, -1, -1, -1, -1, -1, 3, -1,
		-1, -1, -1, -1, 3, 3, 3, -1, -1, -1, 3, -1, -1, 3, -1, 3, -1, -1, -1,
		-1, -1, -1, -1, -1, 3, 3, -1, -1, 3, 3, 3, -1, 3, 3, 3, 3, -1, 3, -1,
		-1, -1, -1, 3, 3, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, 3, -1, -1, 3,
		3, -1, -1, -1, -1, -1, 3, -1, -1, 3, -1, 3, 3, 3, 3, -1, -1, 3, -1, 3,
		3, 3, 3, -1, -1, 3, -1, -1, 3, 3, -1, -1, 3, 3, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, 3, -1, -1, 3, -1, -1, -1, 3, 3, -1, -1, 3, -1, 3, 3, -1, 3,
		-1, 3, -1, -1, -1, 3, 3, -1, -1, -1, -1, 3, 3, -1, -1, 3, -1, -1, 3, -1,
		-1, -1, -1, 3, -1, -1, 3, -1, -1, -1, -1, 3, -1, 3, -1, -1, -1, -1, 3,
		3, -1, -1, -1, -1, 3, 3, -1, 3, -1, -1, 3, -1, -1, -1, -1, 3, -1, -1,
		-1, 3, 3, 3, 3, -1, -1, 3, -1, -1, -1, -1, -1, 3, -1, -1, 8, -1, -1,
		3, -1, 3, -1, -1, -1, 3, 3, -1, -1, -1, -1, 3, -1, -1, -1, -1, -1, -1,
		3, 3, -1, 3, 3, 3, 3, 3, -1, -1, -1, -1, -1, 3, 3, 3, -1, -1, -1, -1, 3,
		-1, -1, -1, -1, -1, -1, 3, 3, 3, 3, 3, 3, -1, -1, -1, -1, 3, -1, -1, -1,
		-1, 3, 3, -1, 3, -1, -1, -1, 3, -1, -1, -1, 3, -1, -1, -1, -1, -1, 3,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, 3, -1, 3, 3, 3, -1, -1, -1, 3, -1,
		-1, -1, 3, -1, -1, -1, 3, 3, -1, -1, -1, 3, 3, 3, -1, 3, -1, -1, -1, 3,
		-1, -1, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, 3, -1, -1, -1, -1,
		3, 3, 3, -1, -1, -1, -1, -1, 3, 3, 3, 3, 3, -1, -1, 3, -1, -1, -1, -1,
		-1, 3, 3, 3, 3, 3, 3, -1, -1, -1, -1, -1, -1, -1, 3, 3, 3, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, 3, -1, -1, -1, -1, -1, 3, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1
	};

	const int width = 32;
	const int height = 32;
	m_map.resize(height, std::vector<int>(width));

	for (int row = 0; row < height; ++row)
	{
		for (int col = 0; col < width; ++col)
		{
			m_map[col][row] = data[row * width + col]; // transposed
		}
	}

	initializeTargetPositionsAndBattleUnits();
}

Terrain::~Terrain()
{
}

int Terrain::getWidth()
{
	return static_cast<int>(m_map.size());
}

int Terrain::getHeight()
{
	return static_cast<int>(m_map[0].size());
}

int Terrain::getAt(int x, int y)
{
	if (x < 0 || x >= getWidth() ||
		y < 0 || y >= getHeight())
	{
		return MapValue_OutsideTerrain;
	}
	return m_map[x][y];
}

void Terrain::setAt(int x, int y, int cellValue)
{
	if (x < 0 || x >= getWidth() ||
		y < 0 || y >= getHeight())
	{
		ASSERT(FALSE); // Outside boundaries
		return;
	}
	m_map[x][y] = cellValue;
}

bool Terrain::isReachablePosition(int x, int y)
{
	int value = getAt(x, y);
	if (value == MapValue_StartingBattleUnitPosition ||
		value == MapValue_TargetPostion ||
		value == MapValue_ReachableTerrainPosition)
	{
		return true;
	}
	return false;
}

TerrainWalk* Terrain::isBattleUnitPosition(int x, int y)
{
	for (TerrainWalk& terrainWalk : m_terrainWalks)
	{
		WalkPosition battleUnitXY = terrainWalk.getDisplayWalkStep();
		if (battleUnitXY.x == x && battleUnitXY.y == y)
			return &terrainWalk;
	}

	return nullptr;
}

void Terrain::clearMap()
{
	for (auto& column : m_map)
	{
		for (auto& cell : column)
		{
			cell = MapValue_ReachableTerrainPosition;
		}
	}
}

void Terrain::resize(const int mapwidth, const int mapheight)
{
	m_map.resize(mapwidth); // Number of rows
	for (auto& row : m_map)
	{
		row.resize(mapheight); // Number of columns per row
	}
}

std::vector<std::vector<int>>* Terrain::getMap()
{
	return &m_map;
}

std::vector<TerrainWalk>* Terrain::getTerrainWalks()
{
	return &m_terrainWalks;
}

void Terrain::getCellToTerrainImageColumnAndRow(int cell, int& imageColumn, int& imageRow)
{
	switch (cell)
	{
	case MapValue_TargetPostion :
		imageColumn = 0;
		imageRow = 4;
		break;

	case MapValue_OutsideTerrain:
	case MapValue_ElevatedTerrainPosition:
		imageColumn = 4;
		imageRow = 0;
		break;

	case MapValue_StartingBattleUnitPosition:
	case MapValue_ReachableTerrainPosition:
	default:
		imageColumn = 1;
		imageRow = 0;
		break;
	}
}

void Terrain::getCellToTerrainImageColumnAndRowBattleUnit(int& imageColumn, int& imageRow)
{
	imageColumn = 5;
	imageRow = 0;
}

bool Terrain::loadFromFileUsingDialogBox()
{
	return JSONTerrain::loadTerrainFromFileDialogBox(this);
}

//bool Terrain::saveToFileUsingDialogBox()
//{
//	return JSONTerrain::saveTerrainToFileDialogBox(this);
//}

bool Terrain::saveWalksToFileUsingDialogBox()
{
	return JSONTerrainWalk::saveTerrainWalksToFileDialogBox(this);
}


void Terrain::addBattleUnit(int x, int y)
{
	if (x < 0 || x >= getWidth() ||
		y < 0 || y >= getHeight())
	{
		return; // Outside limits
	}

	// If a battle unit can be added at this position
	if (isReachablePosition(x, y) &&
		isBattleUnitPosition(x, y) == nullptr)
	{
		// Add a walk path for it and compute its path to a target position
		TerrainWalk terrainWalk;
		terrainWalk.computeWalkPath(this, x, y);
		m_terrainWalks.push_back(terrainWalk);
	}
}

void Terrain::initializeTargetPositionsAndBattleUnits()
{
	// Clear all battle units walks
	m_terrainWalks.clear();
	int positionY = 0;

	// Search in all map cells for a battle unit, then add a terrain walk for each of them and compute their walk path to a target position
	for (auto& column : m_map)
	{
		int positionX = 0;
		for (auto& cell : column)
		{
			if (cell == MapValue_StartingBattleUnitPosition)
			{
				TerrainWalk terrainWalk;
				terrainWalk.computeWalkPath(this, positionX, positionY);
				m_terrainWalks.push_back(terrainWalk);
			}
			positionX++;
		}
		positionY++;
	}
}
