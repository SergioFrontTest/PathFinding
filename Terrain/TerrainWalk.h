#pragma once

#include <vector>

constexpr int TerrainWalkNotVisited = -1;
constexpr int TerrainWalkWall = -2;

struct WalkPosition
{
	int x = TerrainWalkNotVisited;
	int y = TerrainWalkNotVisited;
};

class Terrain;

class TerrainWalk
{
public:
	TerrainWalk();
	virtual ~TerrainWalk();

	// Map data
	inline int getWidth();
	inline int getHeight();
	inline WalkPosition getAt(int x, int y);
	inline void setAt(int x, int y, int xPrevious, int yPrevious);
	bool isReachablePosition(int x, int y);
	bool isTargetPosition(int x, int y);
	void clearMap();
	void resize(const int mapwidth, const int mapheight);

	// Used by JSON file save operation
	std::vector<WalkPosition>* getWalkPositionsToTarget();

	// Walk
	bool computeWalkPath(Terrain* pTerrain, int startingPositionX, int startingPositionY);
	bool processWalkStep(int xPrevious, int yPrevious, int x, int y);
	void addPathToTargetPosition(int x, int y);

	// Real-time walk
	WalkPosition getDisplayWalkStep();
	bool doDisplayWalkStep(); // Returns true if the target position has been reached

protected:
	// Walk variables
	std::vector<std::vector<WalkPosition>> m_mapPreviousWalkPositions;
	int m_startingPositionX = -1;
	int m_startingPositionY = -1;

	// Path to target variables
	std::vector<WalkPosition> m_walkPositionsToTarget;
	int m_currentWalkStepInPath = 0;

	Terrain* m_pTerrain;
};
