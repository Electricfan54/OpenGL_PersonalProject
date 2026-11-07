#include "Renderer.h"

Renderer::Renderer()
    : width(0),height(0)
{
}

void Renderer::SetViewMatrix(const Math::mat4f& matrix)
{
    viewMatrix = matrix;
}

void Renderer::SetWindowSize(int w, int h)
{
    width = w;
    height = h;
    projMatrix = Math::ProjectionMatrix(100, .5f, 45.0f * (3.14 / 180), (float)width / height);
}

void Renderer::RenderObject(Object& object)
{
    object.Bind();
    object.GetShader().SetUniform4f("u_Color", object.Color().x, object.Color().y, object.Color().z, object.Color().w);
    object.GetShader().SetUniformMat4f("projection", projMatrix, GL_FALSE);

    Math::mat4f model = object.GetModelMatrix();
    Math::mat4f mvp = projMatrix.Transposed() * viewMatrix * model;
    Math::mat4f normMat = Math::mat4f(viewMatrix * model).Inversed().Transposed();

    object.GetShader().SetUniformMat4f("model", model, GL_TRUE);
    object.GetShader().SetUniformMat4f("view", viewMatrix, GL_TRUE);
    object.GetShader().SetUniformMat4f("mvp", mvp, GL_TRUE);
    object.GetShader().SetUniformMat4f("normalMat", normMat, GL_TRUE);

    //Lighting
    object.GetShader().SetUniform4f("uLightPos", Math::vector4( lightPos.x, lightPos.y, lightPos.z, 1) * viewMatrix);
    object.GetShader().SetUniform4f("lightColor", lightColor);
    object.GetShader().SetUniform1f("ambientLight", object.Material().AmbientStrength());
    object.GetShader().SetUniform1f("specularLight", object.Material().SpecularStrength());
    object.GetShader().SetUniform1i("specularPow", object.Material().SpecularPow());


    glDrawElements(GL_TRIANGLES, object.IndexSize(), GL_UNSIGNED_INT, 0);
}

void Renderer::RenderObjectInstanced(Object& object)
{
    object.Bind();
    object.GetShader().SetUniform4f("u_Color", object.Color().x, object.Color().y, object.Color().z, object.Color().w);
    object.GetShader().SetUniformMat4f("projection", projMatrix, GL_FALSE);

    //object.GetShader().SetUniformMat4f("model", model, GL_TRUE); Using an instanced array for model matrix
    object.GetShader().SetUniformMat4f("view", viewMatrix, GL_TRUE);
    object.GetShader().SetUniformMat4f("vp", projMatrix.Transposed() * viewMatrix, GL_TRUE);

    //Lighting
    object.GetShader().SetUniform4f("uLightPos", Math::vector4(lightPos.x, lightPos.y, lightPos.z, 1) * viewMatrix);
    object.GetShader().SetUniform4f("lightColor", lightColor);
    object.GetShader().SetUniform1f("ambientLight", object.Material().AmbientStrength());
    object.GetShader().SetUniform1f("specularLight", object.Material().SpecularStrength());
    object.GetShader().SetUniform1i("specularPow", object.Material().SpecularPow());


    glDrawElementsInstanced(GL_TRIANGLES, object.IndexSize(), GL_UNSIGNED_INT, 0, object.GetInstanceCount());
}

void Renderer::RenderSkybox(Object& object, GLuint& textureid)
{
    object.Bind();
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureid);

    object.GetShader().SetUniformMat4f("view", viewMatrix, GL_TRUE);
    object.GetShader().SetUniformMat4f("projection", projMatrix, GL_FALSE);
    object.GetShader().SetUniform1i("skybox", 0);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Renderer::SetLightPos(const Math::vector3& pos)
{
    lightPos = pos;
}

void Renderer::SetLightColor(const Math::vector4& color)
{
    lightColor = color;
}
