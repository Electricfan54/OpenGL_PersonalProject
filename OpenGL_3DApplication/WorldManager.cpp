#include "WorldManager.h"



void WorldManager::LoadChunk(const Math::vector3& pos)
{
    if (loadedChunks.contains(pos))
        return;

	Chunk newChunk;
	newChunk.cube = Cube(cubeShader);

    Cube instancedCube = newChunk.cube;

    instancedCube.SetPosition(pos);
    instancedCube.Color({ .2f, 0.7f, .2f, 1.0f });

    // Creating the model array for instancing
    std::vector<Math::mat4f> modelVector;
    std::vector<PhysicsObject>& colliderVector = newChunk.colliderVector;
    float instanceOffsetX = pos.x, instanceOffsetY = pos.y, instanceOffsetZ = pos.z;
    for (int z = 0; z < chunckWidth; z++)
    {
        for (int x = 0; x < chunckWidth; x++)
        {
            for (int y = 0; y < chunkHeight; y++)
            {
                Transform objectTransform;
                objectTransform.Position({(float)x + instanceOffsetX, -(float)y + instanceOffsetY, -(float)z + instanceOffsetZ });
                objectTransform.Scale(2.0f, 2.0f, 2.0f);

                modelVector.push_back(objectTransform.GetMatrix().Transposed());

                PhysicsObject objectCollider(&objectTransform);
                Math::vector3 tempVec = Math::vector3(instancedCube.GetCenterPosition().x, instancedCube.GetCenterPosition().y, instancedCube.GetCenterPosition().z) + Math::vector3(x, -y, -z);
                tempVec.z = -tempVec.z;
                tempVec.x = -tempVec.x;
                objectCollider.GetCollider().SetCenterPos(tempVec);
                colliderVector.push_back(objectCollider);
            }
        }
    }

    // Setting the instance model array
    instancedCube.SetupInstancing(&modelVector[0], modelVector.size() * sizeof(Math::mat4f), modelVector.size());

    newChunk.cube = instancedCube;

    loadedChunks.try_emplace(pos, newChunk);
    renderChunks.try_emplace(pos, &loadedChunks[pos]);
    collisionChunks.try_emplace(pos, &loadedChunks[pos]);
}

void WorldManager::UnloadChunk(const Math::vector3& pos)
{
    if (!loadedChunks.contains(pos))
        return;

    loadedChunks.erase(pos);
    renderChunks.erase(pos);
    collisionChunks.erase(pos);
}

Math::vector3 WorldManager::ChunkGridToPos(const Math::int3& gridCoord) const
{
    return { (float)gridCoord.x * -chunckWidth, (float)gridCoord.y * chunkHeight, (float)gridCoord.z * -chunckWidth };
}

Math::int3 WorldManager::PosToChunkGrid(const Math::vector3& pos) const
{
    return { (int)(pos.x / chunckWidth), (int)(pos.y / chunkHeight), (int)(pos.z / chunckWidth) };
}

WorldManager::WorldManager(Shader& cubeShader, const Transform& playerTransform)
	: renderDistance(100.0f), collisionDistance(10.0f), chunckWidth(16), chunkHeight(16), chunckSize(chunckWidth * chunckWidth* chunkHeight),
	cubeShader(cubeShader), playerTransform(playerTransform), prevPlayerPos(0,0,0)
{
	//LoadChunk({0,0,0});
	//LoadChunk({-(float)chunckWidth,0,(float)chunckWidth});
	//LoadChunk({-(float)chunckWidth,0,0});
	//LoadChunk({0,0, (float)chunckWidth});

    //UnloadChunk({ -(float)chunckWidth,0,(float)chunckWidth });
}

WorldManager::~WorldManager()
{
}

void WorldManager::LoadLevel()
{
    LoadChunk({ 0,0,0 });
    LoadChunk({-(float)chunckWidth,0,(float)chunckWidth});
    LoadChunk({-(float)chunckWidth,0,0});
    LoadChunk({0,0, (float)chunckWidth});
}

void WorldManager::Update()
{

    LoadAndUnloadChunks();

    prevPlayerPos = playerTransform.Position();
}

void WorldManager::LoadAndUnloadChunks()
{

    int loadRadius = 1;

    Math::int3 playerGridPos = PosToChunkGrid(playerTransform.Position());

    for (int x = -loadRadius; x <= loadRadius; x++)
    {
        for (int z = -loadRadius; z <= loadRadius; z++)
        {
            LoadChunk(ChunkGridToPos({ playerGridPos.x + x,0,playerGridPos.z + z }));
        }
    }

}

void WorldManager::LoadAndUnloadCollisionChunks()
{
    int loadRadius = 1;
}
