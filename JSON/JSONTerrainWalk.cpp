#include "JSONTerrainWalk.h"
#include "../Terrain/Terrain.h"
#include "../Terrain/TerrainWalk.h"
#include "../JSON/JSON.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <filesystem>
#include <map>
#include <functional>

// Include the nlohmann/json library header
#include "../Third Party/JSON/json.hpp"
// Define the namespace for convenience
using json = nlohmann::json;

namespace JSONTerrainWalk {

	// --- Define C++ Structures for Export ---

	struct UnitWalkPosition {
		int x = -1;
		int y = -1;
	};

	// Represents the list of positions (a path) for a SINGLE unit
	struct SingleUnitWalkMap {
		// Single vector representing a list/path of positions
		std::vector<UnitWalkPosition> walk_positions;
	};

	struct MultiUnitWalkData {
		// This is the top-level container: a list of maps, one for each battle unit
		std::vector<SingleUnitWalkMap> unit_walk_maps;
	};

	// --- Implement nlohmann/json Serialization (to_json) ---

	/**
	 * @brief Custom serializer for UnitWalkPosition (x, y coordinates).
	 * Creates JSON object: { "x": -1, "y": -1 }
	 */
	void to_json(json& j, const UnitWalkPosition& wp) {
		j = json::object({
			{"x", wp.x},
			{"y", wp.y}
			});
	}

	/**
	 * @brief Custom serializer for the SingleUnitWalkMap.
	 * Creates JSON object: { "walk_positions": [{...}, {...}, ...] }
	 */
	void to_json(json& j, const SingleUnitWalkMap& suwm) {
		// The nested vectors were replaced; now we serialize the flat list under the key "walk_positions"
		j["walk_positions"] = suwm.walk_positions;
	}

	/**
	 * @brief Custom serializer for the top-level structure (MultiUnitWalkData).
	 * Creates JSON object: { "unit_walk_maps": [suwm1, suwm2, ...] }
	 */
	void to_json(json& j, const MultiUnitWalkData& muwd) {
		j["battle_units"] = muwd.unit_walk_maps;
	}

	// --- 3. Utility Functions ---

	// Helper to convert wstring to string for error reporting only
	std::string wstring_to_string(const std::wstring& ws) {
		return std::string(ws.begin(), ws.end());
	}

	/**
	 * @brief Serializes MultiUnitWalkData to JSON and exports it to a file.
	 * @param data The C++ struct to serialize.
	 * @param filename The output filename (as std::wstring).
	 * @return True if export succeeded, false otherwise.
	 */
	bool export_multi_unit_data(const MultiUnitWalkData& data, const std::wstring& filename) {
		try {
			// 1. Serialize the C++ struct to a nlohmann::json object
			json j = data; // Triggers the MultiUnitWalkData::to_json overload

			// 2. Open the file using std::filesystem::path
			std::ofstream output_file;
			output_file.open(std::filesystem::path(filename));
			if (!output_file.is_open())
			{
				throw std::runtime_error("Failed to open file for writing: " + wstring_to_string(filename));
			}

			// 3. Write the JSON to the file, using 4 spaces for pretty printing
			output_file << j.dump(4) << std::endl;
			output_file.close();

			std::wcout << L"[SUCCESS] Multi-Unit Walk Map data exported to '" << filename << L"'" << std::endl;
			return true;

		}
		catch (const std::exception& e) {
			std::cerr << "[ERROR] Export failed: " << e.what() << std::endl;
			return false;
		}
	}

bool saveTerrainWalksToFileDialogBox(Terrain* pTerrain)
{
	std::wstring path = JSON::getSaveFilePath();
	if (path.length() > 0)
	{
		// Transfer TerrainWalk arrays to the JSON arrays
		auto pTerrainWalks = pTerrain->getTerrainWalks();

		MultiUnitWalkData walkData;
		for (auto terrainWalk : *pTerrainWalks)
		{
			SingleUnitWalkMap oneUnitWalk;
			for (WalkPosition& walkPosition : *terrainWalk.getWalkPositionsToTarget())
			{
				UnitWalkPosition unitWalkPosition;
				unitWalkPosition.x = walkPosition.x;
				unitWalkPosition.y = walkPosition.y;
				//oneUnitWalk.walk_positions.push_back(unitWalkPosition);
				// The unit walk is stored in memory in reverse order, save it to file in the correct order
				oneUnitWalk.walk_positions.insert(oneUnitWalk.walk_positions.begin(), unitWalkPosition);
			}
			walkData.unit_walk_maps.push_back(oneUnitWalk);
		}

		return export_multi_unit_data(walkData, path);
	}

	return false;
}

} // namespace JSONTerrainWalk
