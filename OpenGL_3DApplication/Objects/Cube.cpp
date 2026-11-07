#include "Cube.h"


Cube::Cube()
{
}

Cube::Cube(Shader& shader)
    : Object(shader)
{
    defaultScale = { 2.0f, 2.0f, 2.0f };
    width = 0.5f;
    height = 0.5f;

    std::vector<float> positions =
    {
        //Front
        0.0f, 0.0f, 0.0f,  // 0 Bottom Left
        0.5f, 0.5f, 0.0f,  // 1 Top Right
        0.0f, 0.5f, 0.0f,  // 2 Top Left
        0.0f, 0.0f, 0.0f,  // 0 Bottom Left
        0.5f, 0.0f, 0.0f,  // 3 Bottom Right
        0.5f, 0.5f, 0.0f,  // 1 Top Right

        //Right
        0.5f, 0.0f, 0.0f,  // 3 Bottom Right
        0.5f, 0.0f, -0.5f, // 7
        0.5f, 0.5f, -0.5f, // 5
        0.5f, 0.0f, 0.0f,  // 3 Bottom Right
        0.5f, 0.5f, -0.5f, // 5
        0.5f, 0.5f, 0.0f,  // 1 Top Right

        //Left
        0.0f, 0.0f, -0.5f, // 4
        0.0f, 0.0f, 0.0f,  // 0 Bottom Left
        0.0f, 0.5f, 0.0f,  // 2 Top Left
        0.0f, 0.5f, 0.0f,  // 2 Top Left
        0.0f, 0.5f, -0.5f, // 6
        0.0f, 0.0f, -0.5f, // 4

        //Back
        0.5f, 0.0f, -0.5f, // 7
        0.0f, 0.0f, -0.5f, // 4
        0.0f, 0.5f, -0.5f, // 6
        0.0f, 0.5f, -0.5f, // 6
        0.5f, 0.5f, -0.5f, // 5
        0.5f, 0.0f, -0.5f, // 7

        //Top
        0.0f, 0.5f, 0.0f,  // 2 Top Left
        0.5f, 0.5f, 0.0f,  // 1 Top Right
        0.5f, 0.5f, -0.5f, // 5
        0.5f, 0.5f, -0.5f, // 5
        0.0f, 0.5f, -0.5f, // 6
        0.0f, 0.5f, 0.0f,  // 2 Top Left

        //Bottom
        0.0f, 0.0f, -0.5f, // 4
        0.5f, 0.0f, -0.5f, // 7
        0.5f, 0.0f, 0.0f,  // 3 Bottom Right
        0.5f, 0.0f, 0.0f,  // 3 Bottom Right
        0.0f, 0.0f, 0.0f,  // 0 Bottom Left
        0.0f, 0.0f, -0.5f, // 4

    };

#if 1
    // The numbers are based off of an unfolded cube with coordinate space x,y 0->1
    std::vector<float> uvCoords
    {
        0.3f,0.25f,
        .6f,.5f,
        .3,.5f,
        0.3f,0.25f,
        .6f,.25f,
        .6f,.5f,

        .6f,.25f, // 3
        1.0f, .25f,//7
        1.0f, .5f,// 5
        .6f,.25f, // 3
        1.0f, .5f,// 5
        .6f,.5f,  // 1

        0.0f,.25f,// 4
        .3f,.25f,//  0
        .3f, .5f,//  2
        .3f, .5f,//  2
        0.0f, .5f,// 6
        0.0f,.25f,// 4

        .6f,1.0f,//  7
        .3f,1.0f,//  4
        .3f,.75f,//  6
        .3f,.75f,//  6
        .6f,.75f,//  5
        .6f,1.0f,//  7

        .3f,.5f,//  2
        .6f,.5f,//  1
        .6f,.75f,// 5
        .6f,.75f,// 5
        .3f,.75f,// 6
        .3f,.5f,//  2

        .3f,0.0f,// 4
        .6f,0.0f,// 7
        .6f,.25f,// 3
        .6f,.25f,// 3
        .3f,.25f,// 0
        .3f,0.0f,// 4
    };
#else
    std::vector<float> uvCoords
    {
        //Front
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        ////Right
        0.0f, 0.0f,// 3
        1.0f, 0.0f,// 7
        //1.0f, 1.0f,// 5
        //0.0f, 0.0f,// 3
        //1.0f, 1.0f,// 5
        //0.0f, 1.0f,// 1

        ////Left
        //0.0f, 0.0f,// 4
        //1.0f, 0.0f,// 0
        //1.0f, 1.0f,// 2
        //1.0f, 1.0f,// 2
        //0.0f, 1.0f,// 6
        //0.0f, 0.0f,// 4

        ////Back
        //0.0f, 0.0f,// 7
        //1.0f, 0.0f,// 4
        //1.0f, 1.0f,// 6
        //1.0f, 1.0f,// 6
        //0.0f, 1.0f,// 5
        //0.0f, 0.0f,// 7

        ////Top
        //0.0f, 0.0f,// 2
        //1.0f, 0.0f,// 1
        //1.0f, 1.0f,// 5
        //1.0f, 1.0f,// 5
        //0.0f, 1.0f,// 6
        //0.0f, 0.0f,// 2

        ////Bottom
        //0.0f, 0.0f,// 4
        //1.0f, 0.0f,// 7
        //1.0f, 1.0f,// 3
        //1.0f, 1.0f,// 3
        //0.0f, 1.0f,// 0
        //0.0f, 0.0f,// 4
    };
#endif

    std::vector<Math::vector3> normals =
    {
        //Front
        Math::vector3(0.0f,0.0f, 1.0f),
        Math::vector3(0.0f,0.0f, 1.0f),
        Math::vector3(0.0f,0.0f, 1.0f),
        Math::vector3(0.0f,0.0f, 1.0f),
        Math::vector3(0.0f,0.0f, 1.0f),
        Math::vector3(0.0f,0.0f, 1.0f),

        //Right
        Math::vector3(1.0f,0.0f, 0.0f),
        Math::vector3(1.0f,0.0f, 0.0f),
        Math::vector3(1.0f,0.0f, 0.0f),
        Math::vector3(1.0f,0.0f, 0.0f),
        Math::vector3(1.0f,0.0f, 0.0f),
        Math::vector3(1.0f,0.0f, 0.0f),

        //Left
        Math::vector3(-1.0f,0.0f, 0.0f),
        Math::vector3(-1.0f,0.0f, 0.0f),
        Math::vector3(-1.0f,0.0f, 0.0f),
        Math::vector3(-1.0f,0.0f, 0.0f),
        Math::vector3(-1.0f,0.0f, 0.0f),
        Math::vector3(-1.0f,0.0f, 0.0f),

        //Back
        Math::vector3(0.0f,0.0f, -1.0f),
        Math::vector3(0.0f,0.0f, -1.0f),
        Math::vector3(0.0f,0.0f, -1.0f),
        Math::vector3(0.0f,0.0f, -1.0f),
        Math::vector3(0.0f,0.0f, -1.0f),
        Math::vector3(0.0f,0.0f, -1.0f),

        //Top
        Math::vector3(0.0f, 1.0f, 0.0f),
        Math::vector3(0.0f, 1.0f, 0.0f),
        Math::vector3(0.0f, 1.0f, 0.0f),
        Math::vector3(0.0f, 1.0f, 0.0f),
        Math::vector3(0.0f, 1.0f, 0.0f),
        Math::vector3(0.0f, 1.0f, 0.0f),

        //Bottom
        Math::vector3(0.0f, -1.0f, 0.0f),
        Math::vector3(0.0f, -1.0f, 0.0f),
        Math::vector3(0.0f, -1.0f, 0.0f),
        Math::vector3(0.0f, -1.0f, 0.0f),
        Math::vector3(0.0f, -1.0f, 0.0f),
        Math::vector3(0.0f, -1.0f, 0.0f),

    };

#if 0
    std::vector<GLuint> indicies =
    {
        //front
        0,1,2,
        0,3,1,

        //Right
        3,7,5,
        3,5,1,

        //Left
        4,0,2,
        2,6,4,

        //Back
        7,4,6,
        6,5,7,

        //Top
        2,1,5,
        5,6,2,

        //Bottom
        4,7,3,
        3,0,4,

    };
#else
    std::vector<GLuint> indicies =
    {
        //front
        0,1,2,
        3,4,5,

        //Right
        6,7,8,
        9,10,11,

        //Left
        12,13,14,
        15,16,17,

        //Back
        18,19,20,
        21,22,23,

        //Top
        24,25,26,
        27,28,29,

        //Bottom
        30,31,32,
        33,34,35,

    };
#endif

    indexSize = indicies.size();

    uint32_t offset = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Creating a buffer to store all of the vertex info
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);

    // Allocating the memory
    uint32_t bufferSize = (positions.size() * sizeof(float)) + (uvCoords.size() * sizeof(float)) + (normals.size() * sizeof(float) * 3);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);

    // Adding position data
    glBufferSubData(GL_ARRAY_BUFFER, offset, positions.size() * sizeof(float), &positions[0]);

    // Updating vertex attrib array
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // Updating the offset
    offset += positions.size() * sizeof(float);

    // Adding uv data
    glBufferSubData(GL_ARRAY_BUFFER, offset, uvCoords.size() * sizeof(float), &uvCoords[0]);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // Updating the offset
    offset += uvCoords.size() * sizeof(float);

    // Adding normal data
    glBufferSubData(GL_ARRAY_BUFFER, offset, normals.size() * sizeof(float) * 3, &normals[0]);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // Creating index buffer and allocating data
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(float), &indicies[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    mMaterial.Color({ 1,0,1,1 });
    transform.Scale(defaultScale);


    collider.SetCenterPos(Math::vector3(-GetCenterPosition().x, GetCenterPosition().y, -GetCenterPosition().z));
    collider.SetExtents(Math::vector3(.5f, .5f, .5f));
    collider.SetTransform(&transform);
}

void Cube::SetPosition(const Math::vector3& pos)
{
    transform.Position(pos);
    collider.SetCenterPos(Math::vector3(-GetCenterPosition().x, GetCenterPosition().y, -GetCenterPosition().z));
    collider.UpdateCollider(transform);
}

void Cube::SetupInstancing(void* data, unsigned int size, unsigned int instanceCount)
{
    mInstanceCount = instanceCount;

    glBindVertexArray(vao);

    // Create instance buffer
    // The instance buffer is a ssbo
    glGenBuffers(1, &instanceBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, instanceBuffer);

    // Initializing buffer
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);

    // Binding the ssbo
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, instanceBuffer);

    // Unbind
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::UpdateInstanceBuffer(void* data, unsigned int size, unsigned int instanceCount)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, instanceBuffer);

    // Overwrite buffer
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_STATIC_DRAW);

    // Binding the ssbo
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, instanceBuffer);

    // Unbind
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::OnCollisionEnter(Math::vector3 mtv, PhysicsObject* other)
{
    PhysicsObject::OnCollisionEnter(mtv, other);
    prevColor = Color();
    Color({ 1, 0, 0, 1 });
}

void Cube::OnCollision(Math::vector3 mtv, PhysicsObject* other)
{

}

void Cube::OnCollisionExit(PhysicsObject* other)
{
    PhysicsObject::OnCollisionExit(other);
    Color(prevColor);
}
