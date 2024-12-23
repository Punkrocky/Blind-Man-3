#pragma once

#define TILES_PER_CHUNK 33
#define CHUNK_PER_WORLD 2
#define TILES_PER_WORLD (TILES_PER_CHUNK * CHUNK_PER_WORLD)

#define TILES_PER_CHUNK_SQRD (TILES_PER_CHUNK * TILES_PER_CHUNK)
#define CHUNK_PER_WORLD_SQRD (CHUNK_PER_WORLD * CHUNK_PER_WORLD)
#define TILES_PER_WORLD_SQRD (TILES_PER_WORLD * TILES_PER_WORLD)

#define TILE_HALF_SIZE 0.50f   //!< Half size of each tile
#define OBJECT_HALF_SIZE 0.05f //!< Half size of individual objects

#define VERTEX_COUNT 4
#define INDEX_COUNT 6

#define SEA_LEVEL 0

#define TEXTURE_PATH "../../Assets/Textures/"
#define SHADER_PATH "../../Assets/Shaders/"
