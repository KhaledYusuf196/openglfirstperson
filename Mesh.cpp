#include "Mesh.h"



Mesh::Mesh(std::vector<vertex> v, std::vector<int> t, GLuint texture)
{
	TextureID = texture;
	triangles = t;
	vertices = v;
	initializeGLObjects();
}


Mesh::Mesh(std::vector<vertex> v, GLuint texture)
{
	TextureID = texture;
	vertices = v;
	for (int i = 0; i < vertices.size(); i++) {
		triangles.push_back(i);
	}
	initializeGLObjects();
}

void Mesh::initializeGLObjects()
{
	rotationAxis = glm::vec3(0, 1, 0);
	scale = glm::vec3(1, 1, 1);
	theta = 0;
	position = glm::vec3(0, 0, 0);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//create VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), vertices.data(), GL_STATIC_DRAW);

	//create IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(int), triangles.data(), GL_STATIC_DRAW);

	//shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), ((char*)(sizeof(glm::vec3))));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(vertex), ((char*)(2 * sizeof(glm::vec3))));
	glEnableVertexAttribArray(2);
}

glm::mat4 Mesh::getTransform()
{
	return glm::translate(position) * glm::rotate((theta * 180 / 3.14159f), rotationAxis) * glm::scale(scale);
}

void Mesh::Render(GLuint modelMatLoc)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(getTransform()));
	glDrawElements(GL_TRIANGLES, triangles.size(), GL_UNSIGNED_INT, NULL);
}

Mesh::~Mesh()
{
}
