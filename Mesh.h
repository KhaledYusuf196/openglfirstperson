#pragma once
#include <gl/glew/glew.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/type_ptr.hpp>
#include <gl/glm/gtx/transform.hpp>
#include <vector>

struct vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
};

class Mesh
{
private:
	GLuint VAO, VBO, IBO, TextureID;
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotationAxis;
	float theta;
	
	std::vector<int> triangles;
	std::vector<vertex> vertices;

	Mesh(std::vector<vertex> v, std::vector<int> t, GLuint);
	Mesh(std::vector<vertex> v, GLuint);
	void initializeGLObjects();
	glm::mat4 getTransform();
	void Render(GLuint);
	~Mesh();
};

