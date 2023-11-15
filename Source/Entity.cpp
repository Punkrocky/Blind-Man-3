/*******************************************************************************************************************//**
 * \file Entity.cpp
 * \author Matthew LaDouceur
 * \date 5-28-2020
 * \brief Blind Man 3
 **********************************************************************************************************************/
#include "Entity.hpp"

static unsigned int ENTITY_ID = 1;


Entity::Entity() : ID(ENTITY_ID++)
{
  // Create an empty Entity
  GraphicsComp = nullptr;
  TransformComp = nullptr;
}


Entity::Entity(TransformComponentPtr transform, GraphicsComponentPtr graphics) : ID(ENTITY_ID++)
{
  TransformComp = transform;
  GraphicsComp = graphics;
}


void Entity::Draw(float dt, const glm::mat4& viewMat) const
{
  GLuint ShaderID = GraphicsComp->shader->GetShaderLocation();
  glUseProgram(ShaderID);
  glUniform1i(glGetUniformLocation(ShaderID, "Texture"), 0);
  glBindTextureUnit(0, GraphicsComp->texture->GetTextureLocation());

  glUniformMatrix4fv(glGetUniformLocation(ShaderID, "P"), 1, GL_FALSE, &viewMat[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(ShaderID, "M"), 1, GL_FALSE, &TransformComp->GetModelMatrix()[0][0]);
  glUniform4fv(glGetUniformLocation(ShaderID, "fColor"), 1, &GraphicsComp->GetColor()[0]);
  //GraphicsComp->SetColor(GraphicsComp->Color);

  glBindVertexArray(GraphicsComp->mesh->GetVertexArray());
  glDrawElements(GL_TRIANGLES, INDEX_COUNT, GL_UNSIGNED_INT, (void*)0);
  glBindVertexArray(0);
}


TransformComponentPtr Entity::GetTransformComponent() const
{
  return TransformComp;
}


GraphicsComponentPtr Entity::GetGraphicsComponent() const
{
  return GraphicsComp;
}



//void Entity::SetTransformComponent(TransformComponentPtr transform)
//{
//  TransformComp = transform;
//}
//
//
//void Entity::SetGraphicsComponent(GraphicsComponentPtr graphics)
//{
//  GraphicsComp = graphics;
//}

/**********************************************************************************************************************\
|*************************************************| PRIVATE MEMBERS |**************************************************|
\**********************************************************************************************************************/

Entity::~Entity()
{
  delete TransformComp;
  delete GraphicsComp;
  TransformComp = nullptr;
  GraphicsComp = nullptr;
}


void Entity::InitGraphicsComponent(ShaderPtr s, TexturePtr t, MeshPtr m)
{
  GraphicsComp->shader = s;
  GraphicsComp->texture = t;
  GraphicsComp->mesh = m;
}
