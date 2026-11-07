#include "Skybox.h"
#include "../Dependencies/stb/stb_image.h"

void Skybox::LoadDataToGPU()
{
    std::vector<float> positions =
    {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);


    // Creating a buffer to store all of the vertex info
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);

    // Allocating the memory
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), &positions[0], GL_STATIC_DRAW);


    // Updating vertex attrib array
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    int texWidth, texHeight, texChannels = 0;
    unsigned char* data;
    for (int i = 0; i < cubemapTextures.size(); i++)
    {
        data = stbi_load(cubemapTextures[i].c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else
        {
            std::cout << "Failed to load texture data" << std::endl;
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Skybox::Skybox(Shader& shader)
	: Object(shader), textureId(0)
{
    cubemapTextures =
    {
        // Right
        "Textures/Skybox/sky_78_2k/sky_78_cubemap_2k/px.png",
        // Left
        "Textures/Skybox/sky_78_2k/sky_78_cubemap_2k/nx.png",
        // Top
        "Textures/Skybox/sky_78_2k/sky_78_cubemap_2k/py.png",
        // Bottom
        "Textures/Skybox/sky_78_2k/sky_78_cubemap_2k/ny.png",
        // Back
        "Textures/Skybox/sky_78_2k/sky_78_cubemap_2k/pz.png",
        // Front
        "Textures/Skybox/sky_78_2k/sky_78_cubemap_2k/nz.png",
    };
    LoadDataToGPU();
}

Skybox::Skybox(Shader& shader, const std::vector<std::string>& cubemapTextures)
    : Object(shader), textureId(0), cubemapTextures(cubemapTextures)
{
    LoadDataToGPU();
}

void Skybox::SetupInstancing(void* data, unsigned int size, unsigned int instanceCount)
{
}

void Skybox::UpdateInstanceBuffer(void* data, unsigned int size, unsigned int instanceCount)
{
}
