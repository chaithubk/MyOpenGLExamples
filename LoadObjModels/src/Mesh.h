//-----------------------------------------------------------------------------
// Mesh.h by Steve Jones 
// Copyright (c) 2015-2016 Game Institute. All Rights Reserved.
//
// Basic Mesh class
//-----------------------------------------------------------------------------
#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>

//GLEW
#define GLEW_STATIC
#include<GL\glew.h>

//GLFW
#include<GLFW\glfw3.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"


struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
};

class Mesh
{
public:

	 Mesh();
	~Mesh();

	bool loadOBJ(const std::string& filename);
	void draw();

private:

	void initBuffers();

	bool mLoaded;
	std::vector<Vertex> mVertices;
	GLuint mVBO, mVAO;
};
#endif //MESH_H
