#include "TerrainWalk.h"
#include "Terrain.h"

TerrainWalk::TerrainWalk()
{
	resize(TerrainDefaultWidth, TerrainDefaultHeight);
	clearMap();
}

TerrainWalk::~TerrainWalk()
{
}

int TerrainWalk::getWidth()
{
	return static_cast<int>(m_mapPreviousWalkPositions.size());
}

int TerrainWalk::getHeight()
{
	return static_cast<int>(m_mapPreviousWalkPositions[0].size());
}

WalkPosition TerrainWalk::getAt(int x, int y)
{
	if (x < 0 || x >= getWidth() ||
		y < 0 || y >= getHeight())
	{
		WalkPosition wall;
		wall.x = TerrainWalkWall;
		wall.y = TerrainWalkWall;
		return wall;
	}
	return m_mapPreviousWalkPositions[x][y];
}

void TerrainWalk::setAt(int x, int y, int xPrevious, int yPrevious)
{
	if (x < 0 || x >= getWidth() ||
		y < 0 || y >= getHeight())
	{
		return;
	}
	WalkPosition walkPosition;
	walkPosition.x = xPrevious;
	walkPosition.y = yPrevious;
	m_mapPreviousWalkPositions[x][y] = walkPosition;
}

bool TerrainWalk::isReachablePosition(int x, int y)
{
	ASSERT(m_pTerrain);
	if (!m_pTerrain->isReachablePosition(x, y))
		return false;

	WalkPosition walkStep = getAt(x, y);
	return (walkStep.x == TerrainWalkNotVisited
		|| walkStep.y == TerrainWalkNotVisited);
}

bool TerrainWalk::isTargetPosition(int x, int y)
{
	ASSERT(m_pTerrain);
	int cellTerrain = m_pTerrain->getAt(x, y);
	return (cellTerrain == MapValue_TargetPostion);
}

void TerrainWalk::clearMap()
{
	WalkPosition notVisited;

	for (auto& column : m_mapPreviousWalkPositions)
	{
		for (auto& cell : column)
		{
			cell = notVisited;
		}
	}
}

void TerrainWalk::resize(const int mapwidth, const int mapheight)
{
	m_mapPreviousWalkPositions.resize(mapwidth); // Number of rows
	for (auto& row : m_mapPreviousWalkPositions)
	{
		row.resize(mapheight); // Number of columns per row
	}
}

std::vector<WalkPosition>* TerrainWalk::getWalkPositionsToTarget()
{
	return &m_walkPositionsToTarget;
}

void TerrainWalk::addPathToTargetPosition(int x, int y)
{
	WalkPosition walkPosition;
	walkPosition.x = x;
	walkPosition.y = y;
	m_walkPositionsToTarget.push_back(walkPosition);
}

bool TerrainWalk::doDisplayWalkStep()
{
	if (m_currentWalkStepInPath == 0)
		return true;

	if (m_currentWalkStepInPath > 0)
	{
		int nextStepInPath = m_currentWalkStepInPath - 1;
		WalkPosition nextWalkPosition = m_walkPositionsToTarget[nextStepInPath];

		ASSERT(m_pTerrain);
		TerrainWalk* pTerrainWalkAtNextPosition = m_pTerrain->isBattleUnitPosition(nextWalkPosition.x, nextWalkPosition.y);
		if (pTerrainWalkAtNextPosition != nullptr &&
			pTerrainWalkAtNextPosition != this )
		{
			// Wait next move
			return false;
		}

		m_currentWalkStepInPath--;
	}

	return false;
}

WalkPosition TerrainWalk::getDisplayWalkStep()
{
	WalkPosition walkPosition;
	walkPosition.x = -1; // Hide in display by default
	walkPosition.y = -1; // Hide in display by default

	if (m_walkPositionsToTarget.size() <= 0)
	{
		// A valid path was not found, but we need to display the battle unit on screen
		walkPosition.x = m_startingPositionX;
		walkPosition.y = m_startingPositionY;
	}
	else
	{
		if (m_currentWalkStepInPath >= 0)
			return m_walkPositionsToTarget[m_currentWalkStepInPath];
	}

	return walkPosition;
}

bool TerrainWalk::processWalkStep(int xPrevious, int yPrevious, int x, int y)
{
	if (!isReachablePosition(x, y))
	{
		setAt(x, y, xPrevious, yPrevious);
		return false;
	}

	// Indicate a visited cell
	setAt(x, y, xPrevious, yPrevious);

	// Have we reached the target ?
	if (isTargetPosition(x, y))
	{
		addPathToTargetPosition(x, y); // Store successful step reversely
		return true;
	}

	if (processWalkStep(x, y, x, y - 1)) // Walk up
	{
		addPathToTargetPosition(x, y); // Store successful step reversely
		return true;
	}
	if (processWalkStep(x, y, x + 1, y)) // Walk right
	{
		addPathToTargetPosition(x, y); // Store successful step reversely
		return true;
	}
	if (processWalkStep(x, y, x, y + 1)) // Walk down
	{
		addPathToTargetPosition(x, y); // Store successful step reversely
		return true;
	}
	if (processWalkStep(x, y, x - 1, y)) // Walk left
	{
		addPathToTargetPosition(x, y); // Store successful step reversely
		return true;
	}

	return false;
}

bool TerrainWalk::computeWalkPath(Terrain* pTerrain, int startingPositionX, int startingPositionY)
{
	m_pTerrain = pTerrain;
	ASSERT(m_pTerrain);

	m_startingPositionX = startingPositionX;
	m_startingPositionY = startingPositionY;
	
	clearMap();
	m_walkPositionsToTarget.clear();
	
	bool bPathFound = processWalkStep(m_startingPositionX, m_startingPositionY, m_startingPositionX, m_startingPositionY);
	
	m_currentWalkStepInPath = static_cast<int>(m_walkPositionsToTarget.size() - 1); // Start from the last one, because we stored the path reversely
	
	return bPathFound;
}
