//Using SDL, SDL OpenGL, GLAD, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <glad/glad.h>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "SDL_opengl.h"


//DONE : Include your shader class
#include "ShaderManager.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Object.h"
#include "AudioManager.h"

#define FPS 60

//Starts up SDL, creates window, and initializes OpenGL
bool init();

//Initializes rendering program and clear color
void initGL();

//Per frame update
void update();

//Renders quad to the screen
void render();

//Frees media and shuts down SDL
void close(); 

//Random number between a and b
int randomInt(int a, int b);


//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//OpenGL context
SDL_GLContext gContext;

//Graphics data
#define NUM_OBJ 2
GLuint gVAO[NUM_OBJ];
GLuint gVBO[NUM_OBJ];

//DONE: Declare ID Texture
unsigned int StarWarsText;

//DONE: Create your shader class object
ObjectShader ShaderObj;

ObjectModel OBJ[2];
vector<float> vertices[2];
int vertexCount[2];
int ImgMulti = 1;
float angle = 0.0f, angleLight = 0.0f;
float lightY = 0.0f, lightX = 0.0f;
bool modelChanged = false;
float flowScale = 0.0f;
unsigned int lastTime = 0, currentTime, deltaTime;
float msFrame = 1 / (FPS / 1000.0f);

//Main loop flag
bool ended = false, quit = false, bright = false;

bool init()
{
	srand(time(NULL));
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		SDL_DisplayMode dm;
		if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error display mode: %s", SDL_GetError());
			return 1;
		}

		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dm.w, dm.h, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext(gWindow);
			if (gContext == NULL)
			{
				std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				// glad: load all OpenGL function pointers
				// ---------------------------------------
				if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
				{
					std::cout << "Failed to initialize GLAD" << std::endl;
					success = false;
				}

				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
				}

				glEnable(GL_DEPTH_TEST);
				//DONE : Load your Shader
				ShaderObj.init("Peach");
				initGL();
				AudioManager::getInstance();
				AudioManager::getInstance()->playFX("Assets/Musica/Peaches.mp3");

			}
		}
	}

	return success;
}

void initGL()
{
	//Success flag
	bool success = true;


	//Initialize clear color
	glClearColor(0.12f, 0.07f, 0.4f, 1.0f);

	OBJ[0].Load("Amiga-Days_MarioStar.obj");
	OBJ[1].Load("Princess.obj");
	vertices[0] = OBJ[0].GetVertexData();
	vertices[1] = OBJ[1].GetVertexData();
	vertexCount[0] = OBJ[0].GetVertexCount();
	vertexCount[1] = OBJ[1].GetVertexCount();

	for (int i = 0; i < NUM_OBJ; i++) {
		// Create VAO
		glGenVertexArrays(1, &gVAO[i]);
		glBindVertexArray(gVAO[i]);
		{
			//Create VBO
			glGenBuffers(1, &gVBO[i]);
			glBindBuffer(GL_ARRAY_BUFFER, gVBO[i]);
			glBufferData(GL_ARRAY_BUFFER, vertices[i].size() * sizeof(float), &vertices[i].at(0), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)12);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)24);
			glEnableVertexAttribArray(2);
		}
		glBindVertexArray(0);
	}
	//DONE : Generate Mipmaps. Remember to define Wraps and filter mipmaps

	ShaderObj.Use();

}

void update()
{
	//No per frame update needed
}

void render()
{
	float timeSeconds = (float)SDL_GetTicks() / 1000; 
	currentTime = SDL_GetTicks();
	deltaTime = currentTime - lastTime;

	float randX, randY;

	float ar = 500.0f / 400.0f;
	glm::mat4 projection = glm::perspective(glm::radians(50.0f), ar, 0.1f, 100.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0.0f, 3.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model = glm::mat4(1.0f);

	if (timeSeconds < 14.1f) {
		//PART 1 (WIREFRAME + APPEAR)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		model = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));
	}
	else if (timeSeconds > 14.1f && timeSeconds < 20.95f) {
		//PART 2 (NO WIREFRAME)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		model = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));
	}
	else if (timeSeconds > 20.95f && timeSeconds < 31.25f) {
		//PART 3 (STATIC LIGHT)
		lightY = 2;
		lightX = 2;
		model = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));
		if (!bright)
			bright = true;
	}
	else if (timeSeconds > 31.25f && timeSeconds < 41.75f) {
		//PART 4 (SIN TRANSLATE)
		randX = 2.0f * sin(angleLight / 25.0f);
		model = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));
		model = glm::translate(model, glm::vec3(randX, 0, 0));

	}
	else if (timeSeconds > 41.75f && timeSeconds < 46.75f) {
		//PART 5 (OBJECT ROTATE)
		model = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0, 0, 0));
	}
	else if (timeSeconds > 46.75f && timeSeconds < 52.25f) {
		//PART 6 (MOVING LIGHTS)
		lightY = 5 + 4.5f * sin(angleLight / 25.0f);
		lightX = 5 + 20 * cos(angleLight / 25.0f);
		model = glm::scale(glm::mat4(1.0f), glm::vec3(2, 2, 2));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (timeSeconds > 52.25f && timeSeconds < 62.5f) {
		//PART 7 (MODEL CHANGE & SCALED)
		lightY = 2;
		lightX = 1;

		if (!modelChanged)
			modelChanged = true;

		model = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	}
	else if (timeSeconds > 62.5f && timeSeconds < 80.5f) {
		//PART 8 (FLOW SCALE)
		if (flowScale > 0.015)
			flowScale = 0;

		flowScale += 0.000005;

		model = glm::scale(glm::mat4(1.0f), glm::vec3(flowScale, flowScale, flowScale));
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

		if(!ended)
			ended = true;
	}
	else if (ended) {
		quit = true;
	}

	angle += 0.5f;
	angleLight += 0.5f;




	//Clear color buffer
	glClearColor(0.12f, 0.07f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//DONE : Bind program shader
	ShaderObj.Use();
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj.getID(), "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj.getID(), "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(ShaderObj.getID(), "modelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(glGetUniformLocation(ShaderObj.getID(), "lightY"), lightY);
	glUniform1f(glGetUniformLocation(ShaderObj.getID(), "lightX"), lightX);

	if (!bright) {
		glUniform1f(glGetUniformLocation(ShaderObj.getID(), "bright"), 0);
	}
	else {
		glUniform1f(glGetUniformLocation(ShaderObj.getID(), "bright"), 0.5);
	}
	
	//Set VAO
	if (!modelChanged) {
		glBindVertexArray(gVAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount[0]);
	}
	else {
		glBindVertexArray(gVAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount[1]);
	}
	//Draw VAO

	//Diable VAO
	glBindVertexArray(0);
	//Unbind program
	glUseProgram(NULL);

	//Limit fps
	if (deltaTime < (int)msFrame) {
		SDL_Delay((int)msFrame - deltaTime);
	}
	lastTime = currentTime;
}

void close()
{
	//DONE : Deallocate program shader
	ShaderObj.deleteProgram();

	//Destroy data in GPU
	glDeleteVertexArrays(sizeof(gVAO), gVAO);
	glDeleteBuffers(sizeof(gVBO), gVBO);

	//Destroy window	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		std::cout << "Failed to initialize!" << std::endl;
	}
	else
	{

		//Event handler
		SDL_Event e;

		//While application is running
		while (!quit)
		{
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
						quit = true;
					}
				}
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}

			//Render quad
			render();

			//Update screen
			SDL_GL_SwapWindow(gWindow);
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}