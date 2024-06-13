#pragma once
#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class ObjectShader
{
public:
	ObjectShader();
	~ObjectShader();

	void init(const GLchar* filename);
	GLuint getID() { return mIDprogram; };
	void Use();
	void deleteProgram();


private:
	GLuint mIDprogram;
	bool mAllOK;
};

