#include <stdlib.h>
#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <stdio.h>
#include <iostream>
#include <vector>
#include <GL/freeglut.h>

#include "shader.h"
#include "ParticleSystem.h"
#include "vec2.h"

#define kWindowWidth 640
#define kWindowHeight 480

#define kSubSteps 7

void render();
void update();
void glInit();

unsigned int vao;
unsigned int vbo;
unsigned int shader_programme;

ParticleSystem* simulation;

const float kViewScale =  2.0f;

GLuint programID = 0;
std::vector<vec2> pointsToDraw;

int main (int argc, char** argv) {
	simulation = new ParticleSystem();

	glutInitWindowSize(kWindowWidth, kWindowHeight);
	glutInit(&argc, argv);
	glutInitDisplayString("samples stencil>=3 rgb double depth");
	glutCreateWindow("SPH");
	glutDisplayFunc(render);
	glutIdleFunc(update);
               
  // start GLEW extension handler
  glewExperimental = GL_TRUE;
  glewInit ();

  // get version info
  const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString (GL_VERSION); // version as a string
  printf ("Renderer: %s\n", renderer);
  printf ("OpenGL version supported %s\n", version);



	glInit();

	glutMainLoop();

	return 0;
}

void glInit()
{
    programID = LoadShader( "src/default.vert", "src/flat.frag" );
	vbo = 0;
	glGenBuffers (1, &vbo);
	vao = 0;
	glGenVertexArrays (1, &vao);
}


void render()
{
    glClearColor(0.05f, 0.05f, 0.05f, 1);
	glClear (GL_COLOR_BUFFER_BIT);
	glUseProgram (programID);
    
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	pointsToDraw = simulation->getCoordinateVector();
	glBufferData (GL_ARRAY_BUFFER, (pointsToDraw.size()) * sizeof(vec2), &pointsToDraw[0], GL_STATIC_DRAW);
	glBindVertexArray (vao);
    //glBindVertexArray (vao);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    
 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 640, 0, 480, 0, 1);
 
	//Draw points as smooth balls (with AA)
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPointSize(5.0f);

    // draw points from the currently bound VAO with current in-use shader
    glDrawArrays (GL_POINTS, 0, pointsToDraw.size());
	//glDisableVertexAttribArray(0);

	glutSwapBuffers();
}

void update()
{
	for(int i = 0; i < kSubSteps; ++i)
	{
		simulation->advance();

	}
	glutPostRedisplay();
}