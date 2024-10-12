#include "Shader.h"
#include <fstream>
#include <sstream>

Shader::Shader() {
	programID = 0;
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath) {
	//Shader();
	Init(vertexShaderPath, fragmentShaderPath);
}

void Shader::Init(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string string_vertexShaderCode;
	std::string string_fragmentShaderCode;

	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	
	try {
		vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);

		std::stringstream vertexShaderStream;
		std::stringstream fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		string_vertexShaderCode = vertexShaderStream.str();
		string_fragmentShaderCode = fragmentShaderStream.str();
	}
	catch (std::ifstream::failure &e) {
		std::cout <<  "Failed to open filestream: " << e.what() << std::endl;
	}

	
	const char* vertexShaderCode = string_vertexShaderCode.c_str();
	const char* fragmentShaderCode = string_fragmentShaderCode.c_str();

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);

	validateShaderCompilation(vertexShader);

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);

	validateShaderCompilation(fragmentShader);

	programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	validateProgramLinkCompilation();

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::validateShaderCompilation(GLuint shaderID) {
	int success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		glGetShaderInfoLog(shaderID, sizeof(infoLog), NULL, infoLog);
		std::cout << "FAILED TO LOAD A SHADER: " << infoLog << std::endl;
	}
}

void Shader::validateProgramLinkCompilation() {
	int success;
	char infoLog[512];
	glGetProgramiv(programID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		glGetProgramInfoLog(programID, sizeof(infoLog), NULL, infoLog);
		std::cout << "FAILED TO LINK A SHADER PROGRAM: " << infoLog << std::endl;
	}
}

GLuint Shader::getProgramID() {
	return programID;
}

void Shader::useProgram() {
	glUseProgram(programID);
}