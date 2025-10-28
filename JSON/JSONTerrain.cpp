#include "JSONTerrain.h"
#include "../Terrain/Terrain.h"
#include "../JSON/JSON.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <optional>
#include <stdexcept>
#include <filesystem> // <-- REQUIRED for robust wide-string path handling

// Include the nlohmann/json library header
#include "../Third Party/JSON/json.hpp"
// Define the namespace for convenience
using json = nlohmann::json;

namespace JSONTerrain {

// --- 1. Define C++ Structures ---

struct TileSet {
	std::string name;
	std::string image;
	int imagewidth = 0;
	int imageheight = 0;
	int tilewidth = 0;
	int tileheight = 0;
};

struct Layer {
	std::string name;
	// 'tileset' is optional in the JSON structure (e.g., 'world' layer doesn't have it)
	std::optional<std::string> tileset;
	std::vector<int> data;
};

struct Canvas {
	int width = 0;
	int height = 0;
};

struct MapData {
	std::vector<Layer> layers;
	std::vector<TileSet> tilesets;
	Canvas canvas;
};

// --- 2. Implement nlohmann/json Deserialization (from_json) ---

// Custom deserializer for TileSet
void from_json(const json& j, TileSet& t) {
	j.at("name").get_to(t.name);
	j.at("image").get_to(t.image);
	j.at("imagewidth").get_to(t.imagewidth);
	j.at("imageheight").get_to(t.imageheight);
	j.at("tilewidth").get_to(t.tilewidth);
	j.at("tileheight").get_to(t.tileheight);
}

// Custom deserializer for Layer
void from_json(const json& j, Layer& l) {
	j.at("name").get_to(l.name);
	// Check if the optional 'tileset' field exists
	if (j.contains("tileset") && !j["tileset"].is_null()) {
		l.tileset = j.at("tileset").get<std::string>();
	}
	else {
		l.tileset = std::nullopt;
	}
	j.at("data").get_to(l.data);
}

// Custom deserializer for Canvas
void from_json(const json& j, Canvas& c) {
	j.at("width").get_to(c.width);
	j.at("height").get_to(c.height);
}

// Custom deserializer for MapData (main structure)
void from_json(const json& j, MapData& m) {
	j.at("layers").get_to(m.layers);
	j.at("tilesets").get_to(m.tilesets);
	j.at("canvas").get_to(m.canvas);
}

// Helper to convert wstring to string for error reporting only
std::string wstring_to_string(const std::wstring& ws) {
	// Note: For actual file I/O, we use std::filesystem.
	// This is a simple conversion for console output (cerr/cout) which expects narrow characters.
	return std::string(ws.begin(), ws.end());
}

///**
// * @brief Writes the raw JSON string to a file for testing the file read operation.
// * @param filename The name of the file to write to (as std::wstring).
// * @param content The JSON content string.
// */
//void setup_test_file(const std::wstring& filename, const std::string& content) { // <-- Updated signature
//    try {
//        // Use std::filesystem::path to ensure proper handling of wide characters
//        std::ofstream outfile(std::filesystem::path(filename));
//        if (!outfile.is_open()) {
//            throw std::runtime_error("Failed to open file for writing: " + wstring_to_string(filename));
//        }
//        outfile << content;
//        outfile.close();
//        std::wcout << L"[INFO] Test file '" << filename << L"' created successfully." << std::endl; // Use wcout for wstring output
//    }
//    catch (const std::exception& e) {
//        std::cerr << "[ERROR] File setup failed: " << e.what() << std::endl;
//    }
//}

/**
 * @brief Attempts to load and parse the map data from a file.
 * @param filename The name of the file to read (as std::wstring).
 * @return An optional MapData object, populated if successful.
 */
std::optional<MapData> load_map_data(const std::wstring& filename) { // <-- Updated signature
	std::ifstream input_file;
	MapData map_data;
	std::string filename_str = wstring_to_string(filename); // For error reporting only

	// --- File Operation Try/Catch ---
	try {
		// Use std::filesystem::path to open the file
		input_file.open(std::filesystem::path(filename));
		if (!input_file.is_open()) {
			throw std::runtime_error("File not found or permission denied: " + filename_str);
		}

		// --- JSON Parsing Try/Catch ---
		try {
			json j;
			input_file >> j; // Read and parse JSON from the stream

			// Directly deserialize the JSON object into the C++ struct
			map_data = j.get<MapData>();

			//std::cout << "[SUCCESS] Map data successfully loaded and parsed." << std::endl;
			return map_data;

		}
		catch (const json::exception& e) {
			// Catch nlohmann/json specific errors (e.g., malformed JSON, wrong types)
			std::cerr << "[ERROR] JSON Parsing Failed: " << e.what() << std::endl;
			// Provide context on where the error occurred
			if (e.id == 302) { // Specific ID for type mismatch
				std::cerr << "       (Check for type mismatch or missing required keys in JSON structure.)" << std::endl;
			}
		}
	}
	catch (const std::runtime_error& e) {
		// Catch file I/O errors
		std::cerr << "[ERROR] File Operation Failed: " << e.what() << std::endl;
	}
	catch (const std::exception& e) {
		// Catch other unexpected errors
		std::cerr << "[ERROR] An unexpected error occurred: " << e.what() << std::endl;
	}

	// If any error occurred, return an empty optional
	return std::nullopt;
}

bool loadTerrainFromFileDialogBox(Terrain* pTerrain)
{
	std::wstring path = getLoadFilePath();
	if (path.length() > 0)
	{
		// Read JSON file
		 
		// Load the map data using the wstring filename
		auto map_data_opt = load_map_data(path);

		// Check the result
		if (map_data_opt.has_value())
		{
			const auto& map_data = map_data_opt.value();
			//std::cout << "\n[RESULT] Map Data successfully parsed into structs:" << std::endl;
			//std::cout << "  Canvas Size: " << map_data.canvas.width << "x" << map_data.canvas.height << std::endl;
			//std::cout << "  Layers Found: " << map_data.layers.size() << std::endl;
			//std::cout << "  Tilesets Found: " << map_data.tilesets.size() << std::endl;

			// Print details of the first layer
			if (!map_data.layers.empty())
			{
				const auto& first_layer = map_data.layers[0];
				//std::cout << "  First Layer Name: " << first_layer.name << std::endl;
				//std::cout << "  First Layer Tileset: " << (first_layer.tileset.has_value() ? *first_layer.tileset : "N/A (optional)") << std::endl;
				//std::cout << "  First Layer Data Size: " << first_layer.data.size() << " tiles" << std::endl;

				// --- Parse (x,y) coordinates for the first layer ---
				//std::cout << "\n  --- Parsing First Layer (x, y) Tiles ---" << std::endl;
				const auto& layer_data = first_layer.data;

				const size_t canvas_width = map_data.canvas.width;
				const size_t canvas_height = map_data.canvas.height;
				const size_t map_width = canvas_width / TerrainCellWidth;
				const size_t map_height = canvas_height / TerrainCellHeight;
				pTerrain->resize(static_cast<int>(map_width), static_cast<int>(map_height));
				pTerrain->clearMap();
				if (layer_data.size() != map_width * map_height)
				{
					AfxMessageBox("Failed to load or parse map data, incorrect map cells data count.", MB_ICONERROR | MB_OK);
					return false;
				}

				for (size_t i = 0; i < layer_data.size(); ++i)
				{
					// Convert 1D index 'i' to 2D coordinates (x, y)
					// x (column) = index % width
					const size_t x = i % map_width;
					// y (row) = index / width (integer division)
					const size_t y = i / map_width;
					const int tile_id = layer_data[i];
					pTerrain->setAt(static_cast<int>(x), static_cast<int>(y), tile_id);
				}

				pTerrain->initializeTargetPositionsAndBattleUnits();
			}
		}
		else
		{
			//std::cout << "\n[RESULT] Failed to load or parse map data." << std::endl;
			AfxMessageBox("Failed to load or parse map data.", MB_ICONERROR | MB_OK);
			return false;
		}

		return true;
	}

	return false;
}

//bool saveTerrainToFileDialogBox(Terrain* pTerrain)
//{
//	std::wstring path = JSON::getSaveFilePath();
//	if (path.length() > 0)
//	{
//		json data;
//
//		// Write prettified JSON to another file
//		std::ofstream stream(path);
//		stream << std::setw(4) << data << std::endl;
//
//		return true;
//	}
//
//	return false;
//}

} // namespace JSONTerrain
