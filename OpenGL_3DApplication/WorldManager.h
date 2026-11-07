#pragma once
#include <vector>
#include <unordered_map>
#include "Utility/Chunk.h"

class WorldManager
{

	// Contains every loaded chunk
	std::unordered_map<Math::vector3, Chunk> loadedChunks;

	// Contains all of the rendered chunks
	std::unordered_map<Math::vector3, Chunk*> renderChunks;

	// Might contain physics objects instead of Chunks
	std::unordered_map<Math::vector3, Chunk*> collisionChunks;


	float renderDistance;
	
	// For determining if a chunk should have collision data
	float collisionDistance;

	int chunckWidth, chunkHeight;
	int chunckSize;

	Shader cubeShader;

	const Transform& playerTransform;
	Math::vector3 prevPlayerPos;

	void LoadChunk(const Math::vector3& pos);
	void UnloadChunk(const Math::vector3& pos);

	// converts grid coordinates to world coordinates. (1,0,0) = (chunckwidth, 0, 0)
	Math::vector3 ChunkGridToPos(const Math::int3& gridCoord) const;

	Math::int3 PosToChunkGrid(const Math::vector3& pos) const;

	void LoadAndUnloadChunks();

	void LoadAndUnloadCollisionChunks();

public:
	WorldManager(Shader& cubeShader, const Transform& playerTransform);
	~WorldManager();

	void LoadLevel();
	void Update();

	std::unordered_map<Math::vector3, Chunk*>& GetRenderChunks()
	{
		return renderChunks;
	}

	std::unordered_map<Math::vector3, Chunk*>& GetCollisionChunks()
	{
		return collisionChunks;
	}

};