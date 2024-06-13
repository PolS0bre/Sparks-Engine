#include "ShaderManager.h"

ObjectShader::ObjectShader()
{
	mAllOK = true;
}

ObjectShader::~ObjectShader()
{
	mAllOK = false;
	mIDprogram = 0;
}

void ObjectShader::init(const GLchar* filename)
{
	//Llegir ruta i fer append amb les duex extensions:
	fstream Handle;

	string file = (string)filename;
	//string vs = "SourceFiles/" + file + ".vs";
	string vs = "Assets/shaders/" + file + ".vertex";
	//string fs = "SourceFiles/" + file + ".fs";
	string fs = "Assets/shaders/" + file + ".fragment";
	const char* vertexShader = vs.c_str();
	const char* fragmentShader = fs.c_str();

	string auxiliar, vertexContent, fragmentContent;

	//Fitxer de Vertex
	Handle.open(vertexShader, ios::in);
	
	if (!Handle.is_open()) {
		cout << "ERROR" << endl;
		mAllOK = false;
	}
	while (!Handle.eof()) {
		//Llegir fitxer
		getline(Handle, auxiliar);
		vertexContent += auxiliar + '\n';
	}
	Handle.close();
	const char* vertexFINAL = vertexContent.c_str();

	//Fitxer de Fragment
	Handle.open(fragmentShader, ios::in);
	
	if (!Handle.is_open()) {
		cout << "ERROR" << endl;
		mAllOK = false;
	}
	while (!Handle.eof()) {
		//Llegir fitxer
		getline(Handle, auxiliar);
		fragmentContent += auxiliar + '\n';
	}
	Handle.close();
	const char* fragmentFINAL = fragmentContent.c_str();


	int successShaders;
	char infoLog[512]; 
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//DONE : Create Vertex Shader, compile and check for errors
	int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexFINAL, NULL);
	glCompileShader(vertexShaderID);

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &successShaders);
	if (!successShaders) {
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::FAILED\n" << infoLog << std::endl;
		mAllOK = false;
	}

	//DONE : Create Fragment Shader, compile and check for errors
	int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentFINAL, NULL);
	glCompileShader(fragmentShaderID);

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &successShaders);
	if (!successShaders) {
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::FAILED\n" << infoLog << std::endl;
		mAllOK = false;
	}

	mIDprogram = glCreateProgram();

	glAttachShader(mIDprogram, vertexShaderID);
	glAttachShader(mIDprogram, fragmentShaderID);
	glLinkProgram(mIDprogram);

	glGetProgramiv(mIDprogram, GL_LINK_STATUS, &successShaders);
	if (!successShaders) {
		glGetProgramInfoLog(mIDprogram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::FAILED\n" << infoLog << std::endl;
		mAllOK = false;
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

void ObjectShader::Use()
{
	if (mAllOK) {
		glUseProgram(mIDprogram);
	}
}

void ObjectShader::deleteProgram()
{
	if (mAllOK) {
		glDeleteProgram(mIDprogram);
	}
}
