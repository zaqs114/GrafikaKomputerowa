#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"

GLuint texture;
GLuint programColor;
GLuint programTexture;
GLuint texture_back;
GLuint texture_uboot;
GLuint texture_mine;
GLuint texture_fish1;
GLuint texture_moon;
GLuint texture_fish2;
GLuint texture_fish3;
GLuint texture_fish4;
GLuint texture_shark;

Core::Shader_Loader shaderLoader;


obj::Model ubootModel;
obj::Model fishModel;
obj::Model fish2Model;
obj::Model sphereModel;
obj::Model mineModel;
obj::Model sharkModel;
obj::Model sharkTailModel;

float cameraAngle = 0;
glm::vec3 cameraPos = glm::vec3(-8, 0, 0);
glm::vec3 cameraDir;
//(x,y,z) x-przod/tyl, y-gora/dol, z-prawo/lewo
glm::vec3 mineVEC = glm::vec3(-1,0,0);
glm::vec3 mine2VEC = glm::vec3(-2,0,4);
glm::vec3 mine3VEC = glm::vec3(-2,-2,-2);
glm::vec3 mine4VEC = glm::vec3(3,2,0);
glm::vec3 mine5VEC = glm::vec3(3,0,3);
glm::vec3 mine6VEC = glm::vec3(3,0,-3);
glm::vec3 mine7VEC = glm::vec3(0,3,0);
glm::vec3 mine8VEC = glm::vec3(0,3,3);
glm::vec3 mine9VEC = glm::vec3(0,1,-3);

glm::vec3 fish1VEC;
glm::vec3 fish2VEC;
glm::vec3 fish3VEC;
glm::vec3 fish4VEC;
glm::vec3 fish5VEC;
glm::vec3 fish6VEC;
glm::vec3 fish7VEC;
glm::vec3 fish8VEC;
glm::vec3 fish9VEC;
glm::vec3 fish10VEC;
glm::vec3 fish11VEC;
glm::vec3 fish12VEC;

glm::vec3 sharkVEC= glm::vec3(-3,-3,3);
glm::vec3 sharkTailVEC;
glm::vec3 shark2VEC;
glm::vec3 shark3VEC;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));

void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.1f;
	float moveSpeed = 0.1f;
	switch(key)
	{
	case 'q': cameraAngle -= angleSpeed; break;
	case 'e': cameraAngle += angleSpeed; break;
	case 'w': cameraPos += cameraDir * moveSpeed; break;
	case 's': cameraPos -= cameraDir * moveSpeed; break;
	case 'd': cameraPos += glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
	case 'a': cameraPos -= glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
	case 'z': cameraPos += glm::cross(cameraDir, glm::vec3(0, 0, 1)) * moveSpeed; break;
	case 'x': cameraPos -= glm::cross(cameraDir, glm::vec3(0, 0, 1)) * moveSpeed; break;
	}
}


glm::mat4 createCameraMatrix()
{
	// Obliczanie kierunku patrzenia kamery (w plaszczyznie x-z) przy uzyciu zmiennej cameraAngle kontrolowanej przez klawisze.
	cameraDir = glm::vec3(cosf(cameraAngle), 0.0f, sinf(cameraAngle));
	glm::vec3 up = glm::vec3(0,1,0);

	return Core::createViewMatrix(cameraPos, cameraDir, up);
}

void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint text)
{
	GLuint program = programTexture;

	glUseProgram(program);

	Core::SetActiveTexture(text, "sampler", program, 0);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void renderScene()
{

	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	// Aktualizacja macierzy widoku i rzutowania. Macierze sa przechowywane w zmiennych globalnych, bo uzywa ich funkcja drawObject.
	// (Bardziej elegancko byloby przekazac je jako argumenty do funkcji, ale robimy tak dla uproszczenia kodu.
	//  Jest to mozliwe dzieki temu, ze macierze widoku i rzutowania sa takie same dla wszystkich obiektow!)*+-
	float time = glutGet(GLUT_ELAPSED_TIME) / 5000.0f; //czas od zainicjowania programu

	fish1VEC = glm::vec3(cos(time * 2) * 1 , 0, sin(time * 2) * 1); //cos(kąt * szybkość obrotu)*promień, 0, sin((kąt * szybkość obrotu)*promień
	fish2VEC = glm::vec3(cos(time * -3) * 2, 0, sin(time * -3) * 2);
	fish3VEC = glm::vec3(cos(time * 6) * 2, 0, sin(time * 6) * 2);
	fish4VEC = glm::vec3(cos(time * -3) * 6, 0, sin(time * -3) * 6);
	fish5VEC = glm::vec3(cos(time * 1) * 4, 0, sin(time * 1) * 4);
	fish6VEC = glm::vec3(cos(time * -4) * 6, 0, sin(time * -4) * 6);
	fish7VEC = glm::vec3(cos(time * 2) * 6, 0, sin(time * 2) * 6);
	fish8VEC = glm::vec3(cos(time * -7) * 4, 0, sin(time * -7) * 4);
	fish9VEC = glm::vec3(cos(time * 2) * 5, 0, sin(time * 2) * 5);
	fish10VEC = glm::vec3(cos(time * -3) * 15, 0, sin(time * -3) * 15);
	fish11VEC = glm::vec3(cos(time * 4) * 15, 0, sin(time * 4) * 15);
	fish12VEC = glm::vec3(cos(time * -5) * 15, 0, sin(time * -5) * 15);

	shark2VEC = glm::vec3(cos(time * 1) * 2, 0, sin(time * 1) * 2);
	shark3VEC = glm::vec3(cos(time * 5) * 0.2, 0, sin(time * 5) * 0.2);
	

	// Macierz statku "przyczepia" go do kamery. Warto przeanalizowac te linijke i zrozumiec jak to dziala.
	glm::mat4 ubootModelMatrix = glm::translate(cameraPos + cameraDir * 0.7f + glm::vec3(0,-0.25f,0)) * glm::rotate(-cameraAngle + glm::radians(90.0f), glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.05f));
	
	glm::mat4 BackModelMatrix = glm::scale(glm::vec3(100.0f));
	
	//miny
	glm::mat4 mineModelMatrix = glm::translate(mineVEC) * glm::scale(glm::vec3(0.005f)) * glm::rotate(time * 1, glm::vec3(0, -1, 0));
	glm::mat4 mineModelMatrix2 = glm::translate(mine2VEC) * glm::scale(glm::vec3(0.005f)) * glm::rotate(time * 1, glm::vec3(0, 1, 0));
	glm::mat4 mineModelMatrix3 = glm::translate(mine3VEC) * glm::scale(glm::vec3(0.005f)) * glm::rotate(time * 1, glm::vec3(0, -1, 0));
	glm::mat4 mineModelMatrix4 = glm::translate(mine4VEC) * glm::scale(glm::vec3(0.005f)) * glm::rotate(time * 1, glm::vec3(0, 1, 0));
	glm::mat4 mineModelMatrix5 = glm::translate(mine5VEC) * glm::scale(glm::vec3(0.005f)) * glm::rotate(time * 1, glm::vec3(0, -1, 0));
	glm::mat4 mineModelMatrix6 = glm::translate(mine6VEC) * glm::scale(glm::vec3(0.005f)) * glm::rotate(time * 1, glm::vec3(0, 1, 0));
	glm::mat4 mineModelMatrix7 = glm::translate(mine7VEC) * glm::scale(glm::vec3(0.005f)) * glm::rotate(time * 1, glm::vec3(0, -1, 0));
	glm::mat4 mineModelMatrix8 = glm::translate(mine8VEC) * glm::scale(glm::vec3(0.005f)) * glm::rotate(time * 1, glm::vec3(0, 1, 0));
	glm::mat4 mineModelMatrix9 = glm::translate(mine9VEC) * glm::scale(glm::vec3(0.005f)) * glm::rotate(time * 1, glm::vec3(0, -1, 0));

	// rybki
	glm::mat4 fish1ModelMatrix = glm::translate(fish1VEC) * glm::translate(mine2VEC) * glm::rotate(time * 2, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f));
	//		*2 zeby sie synchronizowalo z vectorem obrotu, inaczej bedzie sie obracac szybciej/wolniej			-1 zeby ryba byla pyskiem do przodu
	glm::mat4 fish2ModelMatrix = glm::translate(fish2VEC) * glm::translate(mineVEC) * glm::rotate(time * -3, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f));
	glm::mat4 fish3ModelMatrix = glm::translate(fish3VEC) * glm::translate(mine3VEC) * glm::rotate(time * 6, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f))* glm::translate(glm::vec3(0,2,0));
	glm::mat4 fish4ModelMatrix = glm::translate(fish4VEC) * glm::translate(mine4VEC) * glm::rotate(time * -3, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f))* glm::translate(glm::vec3(0,-2,0));
	glm::mat4 fish5ModelMatrix = glm::translate(fish5VEC) * glm::translate(mine5VEC) * glm::rotate(time * 1, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f))* glm::translate(glm::vec3(0,2,0));
	glm::mat4 fish6ModelMatrix = glm::translate(fish6VEC) * glm::translate(mine6VEC) * glm::rotate(time * -4, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f))* glm::translate(glm::vec3(0,-2,0));
	glm::mat4 fish7ModelMatrix = glm::translate(fish7VEC) * glm::translate(mine7VEC) * glm::rotate(time * 2, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f))* glm::translate(glm::vec3(0,2,0));
	glm::mat4 fish8ModelMatrix = glm::translate(fish8VEC) * glm::translate(mine8VEC) * glm::rotate(time * -7, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f))* glm::translate(glm::vec3(0,-2,0));
	glm::mat4 fish9ModelMatrix = glm::translate(fish9VEC) * glm::translate(mine9VEC) * glm::rotate(time * 2, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f))* glm::translate(glm::vec3(0,2,0));
	glm::mat4 fish10ModelMatrix = glm::translate(fish10VEC) * glm::translate(glm::vec3(0,0,0)) * glm::rotate(time * -3, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f))* glm::translate(glm::vec3(0,-3,0));
	glm::mat4 fish11ModelMatrix = glm::translate(fish11VEC) * glm::translate(glm::vec3(0,0,0)) * glm::rotate(time * 4, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f))* glm::translate(glm::vec3(0,0,0));
	glm::mat4 fish12ModelMatrix = glm::translate(fish12VEC) * glm::translate(glm::vec3(0,0,0)) * glm::rotate(time * -5, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.6f))* glm::translate(glm::vec3(0,-2,0));
	
	//rekiny
	glm::mat4 sharkModelMatrix = glm::translate(sharkVEC)  * glm::scale(glm::vec3(0.05f));
	glm::mat4 sharkTailModelMatrix = glm::translate(sharkTailVEC) * sharkModelMatrix * glm::scale(glm::vec3(1.0f)) *glm::rotate(float(sin(time*10)*0.05), glm::vec3(0, 1, 0));
	
	glm::mat4 shark2ModelMatrix = glm::translate(shark2VEC) * glm::translate(glm::vec3(-5,0,0)) * glm::rotate(time * 1, glm::vec3(0, -1, 0)) * glm::scale(glm::vec3(0.05f));
	glm::mat4 shark2TailModelMatrix = glm::translate(sharkTailVEC) * shark2ModelMatrix * glm::scale(glm::vec3(1.0f)) *glm::rotate(float(sin(time*10)*0.05), glm::vec3(0, 1, 0));

	glm::mat4 shark3ModelMatrix = glm::translate(shark3VEC)*glm::translate(glm::vec3(-3,-3,-3)) * glm::scale(glm::vec3(0.05f));
	glm::mat4 shark3TailModelMatrix = (&sharkTailModel, shark3ModelMatrix * glm::translate(glm::vec3(0.0f, 1.0f, -4.1f)) *
														   glm::rotate(glm::radians(float(sin(-time * 3) * 5.0f)), glm::vec3(0, 1, 0)) *
														   glm::scale(glm::vec3(0.6f, 0.7f, 0.6f)));

	drawObjectTexture(&ubootModel, ubootModelMatrix, texture_uboot);

	drawObjectTexture(&sphereModel, BackModelMatrix, texture_back);

	drawObjectTexture(&mineModel, mineModelMatrix, texture_mine);
	drawObjectTexture(&mineModel, mineModelMatrix2, texture_mine);
	drawObjectTexture(&mineModel, mineModelMatrix3, texture_mine);
	drawObjectTexture(&mineModel, mineModelMatrix4, texture_mine);
	drawObjectTexture(&mineModel, mineModelMatrix5, texture_mine);
	drawObjectTexture(&mineModel, mineModelMatrix6, texture_mine);
	drawObjectTexture(&mineModel, mineModelMatrix7, texture_mine);
	drawObjectTexture(&mineModel, mineModelMatrix8, texture_mine);
	drawObjectTexture(&mineModel, mineModelMatrix9, texture_mine);

	drawObjectTexture(&fishModel, fish1ModelMatrix, texture_fish1);
	drawObjectTexture(&fish2Model, fish2ModelMatrix, texture_fish2);
	drawObjectTexture(&fishModel, fish3ModelMatrix, texture_fish3);
	drawObjectTexture(&fish2Model, fish4ModelMatrix, texture_fish4);
	drawObjectTexture(&fishModel, fish5ModelMatrix, texture_fish1);
	drawObjectTexture(&fish2Model, fish6ModelMatrix, texture_fish2);
	drawObjectTexture(&fishModel, fish7ModelMatrix, texture_fish3);
	drawObjectTexture(&fish2Model, fish8ModelMatrix, texture_fish4);
	drawObjectTexture(&fishModel, fish9ModelMatrix, texture_fish1);
	drawObjectTexture(&fish2Model, fish10ModelMatrix, texture_fish2);
	drawObjectTexture(&fishModel, fish11ModelMatrix, texture_fish3);
	drawObjectTexture(&fish2Model, fish12ModelMatrix, texture_fish4);
	
	drawObjectTexture(&sharkModel, sharkModelMatrix, texture_shark);
	drawObjectTexture(&sharkTailModel, sharkTailModelMatrix, texture_shark);

	drawObjectTexture(&sharkModel, shark2ModelMatrix, texture_shark);
	drawObjectTexture(&sharkTailModel, shark2TailModelMatrix, texture_shark);

	drawObjectTexture(&sharkModel, shark3ModelMatrix, texture_shark);
	drawObjectTexture(&sharkTailModel, shark3TailModelMatrix, texture_shark);

	glutSwapBuffers();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	mineModel = obj::loadModelFromFile("models/ball.obj");
	ubootModel = obj::loadModelFromFile("models/dasboot.obj");
	fishModel = obj::loadModelFromFile("models/fish.obj");
	fish2Model = obj::loadModelFromFile("models/fish2.obj");
	sharkModel = obj::loadModelFromFile("models/sharkFront.obj");
	sharkTailModel = obj::loadModelFromFile("models/sharkTail.obj");

	texture= Core::LoadTexture("textures/grid.png");
	
	
	texture_back = Core::LoadTexture("textures/back.png");
	texture_uboot = Core::LoadTexture("textures/uboot.png");

	texture_mine = Core::LoadTexture("textures/mine.png");

	texture_fish1 = Core::LoadTexture("textures/scale.png");
	texture_fish2 = Core::LoadTexture("textures/scale2.png");
	texture_fish3 = Core::LoadTexture("textures/scale3.png");
	texture_fish4 = Core::LoadTexture("textures/scale4.png");

	texture_shark = Core::LoadTexture("textures/shark.png");


}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1200, 1000);
	glutCreateWindow("Projekt Grafika");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
