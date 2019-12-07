#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <gl/glm/glm.hpp>
#include <gl/glm/gtc/type_ptr.hpp>
#include <gl/glm/gtx/transform.hpp>
#include "Mesh.h"


using namespace std;


const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, IBO, programId, modelMatLoc, viewMatLoc, projectionMatLoc, camera_positionLoc, TextureId, TextureId2;
GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);


#pragma Region Sphere
vector<vertex> sphere_vertices;
glm::vec3 Sphere_Core_vertices[4] = {
	glm::vec3(0.0, 0.0, 1.0),
	glm::vec3(0.0, 0.942809, -0.333333),
	glm::vec3(-0.816497, -0.471405, -0.333333),
	glm::vec3(0.816497, -0.471405, -0.333333)
};

void Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 normal = glm::cross((c-b),(a-b));
	sphere_vertices.push_back({ a, a });
	sphere_vertices.push_back({ b, b });
	sphere_vertices.push_back({ c, c });
}

void dividTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, int itertions)
{
	if (itertions > 0)
	{
		glm::vec3 v1 = glm::normalize(a + b);
		glm::vec3 v2 = glm::normalize(a + c);
		glm::vec3 v3 = glm::normalize(b + c);

		dividTriangle(a, v1, v2, itertions - 1);
		dividTriangle(v1, b, v3, itertions - 1);
		dividTriangle(v1, v3, v2, itertions - 1);
		dividTriangle(v2, v3, c, itertions - 1);
	}
	else
	{
		Triangle(a, b, c);
	}
}
void CreateSphere(int iterations)
{
	sphere_vertices.clear();
	dividTriangle(Sphere_Core_vertices[0], Sphere_Core_vertices[1], Sphere_Core_vertices[2], iterations);
	dividTriangle(Sphere_Core_vertices[0], Sphere_Core_vertices[3], Sphere_Core_vertices[1], iterations);
	dividTriangle(Sphere_Core_vertices[0], Sphere_Core_vertices[2], Sphere_Core_vertices[3], iterations);
	dividTriangle(Sphere_Core_vertices[3], Sphere_Core_vertices[2], Sphere_Core_vertices[1], iterations);


	//glGenBuffers(1, &VBO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sphere_vertices.size() * sizeof(vertex), sphere_vertices.data(), GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
	//glEnableVertexAttribArray(0);

	//glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
}

#pragma endregion 


void CompileShader()
{
	programId = InitShader("VS.glsl", "FS.glsl");
	glUseProgram(programId);
}

int init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "Error";
		getchar();
		return 1;
	}
	else
	{
		if (GLEW_VERSION_3_0)
			cout << "Driver support OpenGL 3.0\nDetails:\n";
	}
	cout << "\tUsing glew: " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	sf::Image img;
	if (!img.loadFromFile("C:\\Users\\khale\\OneDrive\\Pictures\\foxy.png")) {
		cout << "Error loading image" << endl;
	}
	else {
		glGenTextures(1, &TextureId);
		glBindTexture(GL_TEXTURE_2D, TextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	}

	if (!img.loadFromFile("C:\\Users\\khale\\OneDrive\\Pictures\\gabe.jpg")) {
		cout << "Error loading image" << endl;
	}
	else {
		glGenTextures(1, &TextureId2);
		glBindTexture(GL_TEXTURE_2D, TextureId2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	}

	glClearColor(0.5, 0.5, 1, 1);

	CompileShader();
	modelMatLoc = glGetUniformLocation(programId, "modelMat");
	viewMatLoc = glGetUniformLocation(programId, "viewMat");
	projectionMatLoc = glGetUniformLocation(programId, "projectionMat");
	camera_positionLoc = glGetUniformLocation(programId, "camera_position");

	glEnable(GL_DEPTH_TEST);

	return 0;
}

float theta = 1;

void Update()
{
	theta += 0.001f;
}

float* rotateY(float theta)
{
	float* matrix = new float[16];
	matrix[0] = cos(theta);
	matrix[2] = sin(theta);
	matrix[5] = 1;
	matrix[8] = -sin(theta);
	matrix[10] = cos(theta);
	matrix[15] = 1;
	return matrix;
}



void setRenderView(glm::vec3 cameraPosition, glm::vec3 cameraDirection)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 viewMat = glm::lookAt(cameraPosition, cameraPosition + cameraDirection, glm::vec3(0,1,0));
	glm::mat4 projectionMat = glm::perspectiveFov(60.0f, 5.0f, 5.0f, 0.1f, 10.0f);
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
	glUniformMatrix4fv(projectionMatLoc, 1, GL_FALSE, glm::value_ptr(projectionMat));
	glUniform3fv(camera_positionLoc, 1, glm::value_ptr(cameraPosition));


}

void RenderCube()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	GLuint thetaUniformLocation = glGetUniformLocation(programId, "theta");
	glUniform1f(thetaUniformLocation, theta);

	glUniform1f(thetaUniformLocation, theta);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT,NULL);
}

int main() {
	
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "OpenGl", sf::Style::Close, context);
	glm::vec3 position(0, 0, 5);
	glm::vec3 direction(0, 0, 1);
	sf::Vector2i oldMousePos;
	if (init()) return 1;

	CreateSphere(6);
	Mesh sphere(sphere_vertices, TextureId);

	Mesh triangle({
					{glm::vec3(-1, -1, -1), glm::vec3(0.75, 0, 0)},
					{ glm::vec3(0, -1, 1), glm::vec3(0.25, 0, 0) },
					{ glm::vec3(1,-1,-1), glm::vec3(1,0,0) }
		}, TextureId);
	triangle.scale = glm::vec3(10, 1, 10);

	Mesh cube({
		{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(1,0)},
		{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(1,1)},
		{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0,1)},
		{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 1, 0), glm::vec2(0,0)},
		{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 1)},
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 1)},
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 1)},
		{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 0, 0)}
		},
		{
			//FRONT
			0,1,2,
			0,2,3,

			//RIGHT
			3,2,5,
			3,5,4,

			//BACK
			4,5,6,
			4,6,7,

			//LEFT
			7,6,1,
			7,1,0,

			//TOP
			7,0,3,
			7,3,4,

			//BOTTOM
			2,1,6,
			2,6,5
		}, TextureId);


	Mesh cube2({
		{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(1,0)},
		{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(1,1)},
		{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0,1)},
		{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 1, 0), glm::vec2(0,0)},
		{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 1)},
		{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 1)},
		{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1, 1, 1)},
		{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 0, 0)}
		},
		{
			//FRONT
			0,1,2,
			0,2,3,

			//RIGHT
			3,2,5,
			3,5,4,

			//BACK
			4,5,6,
			4,6,7,

			//LEFT
			7,6,1,
			7,1,0,

			//TOP
			7,0,3,
			7,3,4,

			//BOTTOM
			2,1,6,
			2,6,5
		}, TextureId2);
	
	
	while (window.isOpen())
	{
		glm::vec3 side = glm::normalize(glm::cross(glm::vec3(0, 1, 0), direction));
		glm::vec4 direction4d(direction.x, direction.y, direction.z, 1);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if(event.type == sf::Event::KeyPressed){
				
				switch (event.key.code)
				{
				case sf::Keyboard::W:
					position += glm::cross(side, glm::vec3(0, 1, 0)) * 0.2f;
					break;
				case sf::Keyboard::S:
					position -= glm::cross(side, glm::vec3(0, 1, 0)) * 0.2f;
					break;
				case sf::Keyboard::A:
					position += side * 0.2f;
					break;
				case sf::Keyboard::D:
					position -= side * 0.2f;
					break;
				case sf::Keyboard::E:
					position += glm::vec3(0, 0.2, 0);
					break;
				case sf::Keyboard::Q:
					position -= glm::vec3(0, 0.2, 0);
					break;

				};


				//if(event.type == sf::Event::MouseMoved)
			}

		}
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f mouseDir = sf::Vector2f((mousePos - oldMousePos).x, (mousePos - oldMousePos).y);
			cout << mouseDir.x << " " << mouseDir.y << endl;
			direction4d = glm::rotate(-mouseDir.x, glm::vec3(0, 1, 0)) * direction4d;
			direction4d = glm::rotate(mouseDir.y, side) * direction4d;
			
			if (mousePos.x >= WIDTH) {
				sf::Mouse::setPosition(mousePos = sf::Vector2i(0, mousePos.y), window);
			}
			if (mousePos.y >= HEIGHT) {
				sf::Mouse::setPosition(mousePos = sf::Vector2i(mousePos.x, 0), window);
			}
			if (mousePos.x < 0) {
				sf::Mouse::setPosition(mousePos = sf::Vector2i(WIDTH - 1, mousePos.y), window);
			}
			if (mousePos.y < 0) {
				sf::Mouse::setPosition(mousePos = sf::Vector2i(mousePos.x, HEIGHT - 1), window);
			}
			oldMousePos = mousePos;
		}
		else {
			oldMousePos = sf::Mouse::getPosition(window);
		}

		direction.x = direction4d.x;
		if(abs(direction4d.y) < 0.7)
			direction.y = direction4d.y;
		direction.z = direction4d.z;

		//Update();
		
		setRenderView(position,direction);
		triangle.Render(modelMatLoc);
		sphere.scale = glm::vec3(0.5, 0.5, 0.5);
		//sphere.theta = 1;
		sphere.position = glm::vec3(1, 1, 2);
		sphere.Render(modelMatLoc);
		sphere.position = glm::vec3(-1, 1, 2);
		sphere.Render(modelMatLoc);
		sphere.position = glm::vec3(0, 0, 2);
		sphere.scale = glm::vec3(1, 1, 1);
		sphere.Render(modelMatLoc);

		cube.position = glm::vec3(0, 0, 10);
		cube.rotationAxis = glm::vec3(0, 1, 0);
		cube.theta = 3.14159f;
		cube.scale = glm::vec3(1, 1, 1);
		cube.Render(modelMatLoc);

		cube2.position = glm::vec3(2, 0, 5);
		cube2.rotationAxis = glm::vec3(0, 1, 0);
		cube2.theta = 3.14159f * 1.5f;
		cube2.scale = glm::vec3(1, 1, 1);
		cube2.Render(modelMatLoc);


		window.display();
	}
}