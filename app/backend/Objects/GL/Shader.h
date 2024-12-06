#include <iostream>
#include <string>
#include <GLES3/gl3.h>
#include <GL/gl.h>


#ifndef SHADER_H
#define SHADER_H


class Shader {
	GLuint programID;
public:
	Shader();
	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
	
	void Init(const char* vertexShaderPath, const char* fragmentShaderPath);
	GLuint getProgramID();
	void useProgram();

private:
	void validateShaderCompilation(GLuint programID);
	void validateProgramLinkCompilation();
};



#endif