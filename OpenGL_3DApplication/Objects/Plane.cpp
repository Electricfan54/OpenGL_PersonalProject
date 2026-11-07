#include "Plane.h"

Plane::Plane(Shader& shader)
    : Object(shader)
{
    float groundVerts[] =
    {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
    };

    unsigned int groundIndicies[] =
    {
        0,1,2,
        2,3,0,
    };

    float groundUV[] =
    {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
    };

    float normals[] =
    {
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);


    // Allocating the memory
    uint32_t bufferSize = (12 * sizeof(float)) + (8 * sizeof(float)) + (12 * sizeof(float) * 3);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);

    uint32_t offset = 0;

    // Adding position data
    glBufferSubData(GL_ARRAY_BUFFER, offset, 12 * sizeof(float), &groundVerts[0]);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // Updating the offset
    offset += 12 * sizeof(float);

    // Adding uv data
    glBufferSubData(GL_ARRAY_BUFFER, offset, 8 * sizeof(float), &groundUV);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // Updating the offset
    offset += 8 * sizeof(float);

    // Adding normal data
    glBufferSubData(GL_ARRAY_BUFFER, offset, 12 * sizeof(float) * 3, &normals[0]);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // Creating index buffer and assigning data
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), &groundIndicies, GL_STATIC_DRAW);
    indexSize = 6;

    glBindVertexArray(0);

    mMaterial.Color(Math::vector4(0,.5,1,1));
}

void Plane::SetupInstancing(void* data, unsigned int size, unsigned int instanceCount)
{
}

void Plane::UpdateInstanceBuffer(void* data, unsigned int size, unsigned int instanceCount)
{
}
