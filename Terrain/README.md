Terrain
The terrain map containing walls and target positions.
It also contains an array of TerrainWalk, one per battle unit.

TerrainWalk
The walk path of a battle unit to reach a target position.
When a battle unit is added (when a JSON map file is loaded or when the left mouse button is clicked),
a path is recursively searched near the battle unit position
(in a 4 directions walk,
an internal sub-map stores cells already visited,
then when a target unit cell is detected, an array is recursively and reversely filled with current step position,
and it informs the parent call that the sub-call was successfull, allowing to break further processings).
Then, the battle unit moves one step at a time using the stored path array in TerrainWalk, and taking care of other battle units in the field.

The array of TerrainWalk is used to store battle units movement to JSON file.

TerrainWalk could still be optimized to find a shorter way, by letting all the paths to finish and taking the shortest one, and then by trying the shortest path between all steps of the path.

Something could also be done when battle units are blocked (force a movement when the walk path is blocked, or recompute walk path), or when no target position is found (random walk).
