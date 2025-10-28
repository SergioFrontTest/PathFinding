# CMakeList.txt : CMake project for PathFinding, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project (PathFinding)

# Enable MFC
set(CMAKE_MFC_FLAG 1)  # 1 for static, 2 for shared

# Add source to this project's executable.
add_executable (PathFinding WIN32
	# Process
	"PathFinding.cpp"
	"PathFinding.h"

	# UI
	"UI/UI.h"
	"UI/UI.cpp"
	"UI/Resources.h"
	"UI/Resources.rc" "Resources/"
	"UI/UIDirect2D.h"
	"UI/UIDirect2D.cpp"
	"UI/Image.h"
	"UI/Image.cpp"
	"UI/IUIDraw.h"
	"UI/IUIDraw.cpp"
	"UI/MainFrameWnd.h"
	"UI/MainFrameWnd.cpp"

	# JSON
	"Third Party/JSON/json.hpp"
	"JSON/JSON.h"
	"JSON/JSON.cpp"
	"JSON/JSONTerrain.h"
	"JSON/JSONTerrain.cpp"
	"JSON/JSONTerrainWalk.h"
	"JSON/JSONTerrainWalk.cpp"

	# Terrain
	"Terrain/Terrain.h"
	"Terrain/Terrain.cpp"
	"Terrain/TerrainWalk.h"
	"Terrain/TerrainWalk.cpp"
)

# Enable MFC for the target
set_target_properties(PathFinding PROPERTIES
    USE_WIN32_MFC TRUE
)
 
# Apply compile definitions AFTER the target is defined
target_compile_definitions(PathFinding PRIVATE _AFXDLL)