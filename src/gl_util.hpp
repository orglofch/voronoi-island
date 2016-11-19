#ifndef _GL_UTIL_HPP_
#define _GL_UTIL_HPP_

#include <iostream>

#include <GL\glew.h>

#include "util.hpp"

typedef GLuint Texture;

#define GL_CHECK() PrintGLError(__FILE__, __LINE__)

inline
int PrintGLError(char *file, int line) {

	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	if (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s\n",
			file, line, gluErrorString(glErr));
		retCode = 1;
	}
	return retCode;
}

// TODO(orglofch): Use vbo
inline
void glDrawRect(float left, float right, float bottom, float top, float depth) {
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(left, bottom, depth);
	glTexCoord2f(1, 0); glVertex3f(right, bottom, depth);
	glTexCoord2f(1, 1); glVertex3f(right, top, depth);
	glTexCoord2f(0, 1); glVertex3f(left, top, depth);
	glEnd();
}

inline
void glCreateTexture2D(GLuint *texture, int width, int height, int channels, void *data) {
	assert(data);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, texture);
	if (*texture < 1) {
		LOG("Failed to load texture\n");
		return;
	}

	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, channels == 4 ? GL_RGBA32F : GL_RGB32F,
		width, height, 0, channels == 4 ? GL_RGBA : GL_RGB,
		GL_FLOAT, data);
	glBindTexture(GL_TEXTURE_2D, 0);
}

inline
void glCreateTexture3D(GLuint *texture, int width, int height, int depth, int channels, void *data) {
	assert(data);

	glEnable(GL_TEXTURE_3D);
	glGenTextures(1, texture);
	if (*texture < 1) {
		LOG("Failed to load texture\n");
		return;
	}

	glBindTexture(GL_TEXTURE_3D, *texture);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage3D(GL_TEXTURE_3D, 0, channels == 4 ? GL_RGBA32F : GL_RGB32F,
		width, height, depth, 0, channels == 4 ? GL_RGBA : GL_RGB,
		GL_FLOAT, data);
	glBindTexture(GL_TEXTURE_3D, 0);
}

inline
void glSetPerspectiveProjection(const size_t width,
	const size_t height,
	const GLdouble fov,
	const GLdouble n,
	const GLdouble f) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float aspect = 1.0f * width / height;
	gluPerspective(fov, 1.0 * width / height, n, f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

inline
void glSetOrthographicProjection(const GLdouble left,
	const GLdouble right,
	const GLdouble bottom,
	const GLdouble top,
	const GLdouble n,
	const GLdouble f) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(left, right, bottom, top, n, f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

inline
void glPrintShaderInfo(GLint shader) {
	int info_log_len = 0;
	int char_written = 0;
	GLchar *info_log;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_len);

	if (info_log_len > 0) {
		info_log = new GLchar[info_log_len];
		glGetShaderInfoLog(shader, info_log_len, &char_written, info_log);
		printf("Info: %s\n", info_log);
		delete[] info_log;
	}
}

struct Shader
{
	GLuint main;
	GLuint vertex; // TODO(orglofch): See if we can remove this
	GLuint fragment; // TODO(orglofch): See if we can remove this
};

inline
bool glLoadShader(const char *vs_filename,
	const char *fs_filename,
	Shader *shader) {
	*shader = {};

	std::string vertex_shader_string = ReadFile(vs_filename);
	std::string fragment_shader_string = ReadFile(fs_filename);
	int vlen = vertex_shader_string.length();
	int flen = fragment_shader_string.length();

	if (vertex_shader_string.empty())
		return false;
	if (fragment_shader_string.empty())
		return false;

	shader->vertex = glCreateShader(GL_VERTEX_SHADER);
	shader->fragment = glCreateShader(GL_FRAGMENT_SHADER);

	const char *vertex_shader_cstr = vertex_shader_string.c_str();
	const char *fragment_shader_cstr = fragment_shader_string.c_str();
	glShaderSource(shader->vertex, 1, (const GLchar **)&vertex_shader_cstr, &vlen);
	glShaderSource(shader->fragment, 1, (const GLchar **)&fragment_shader_cstr, &flen);

	GLint compiled;

	glCompileShader(shader->vertex);
	glGetShaderiv(shader->vertex, GL_COMPILE_STATUS, &compiled);
	if (compiled == false) {
		printf("Vertex shader not compliled\n");
		glPrintShaderInfo(shader->vertex);

		glDeleteShader(shader->vertex);
		shader->vertex = 0;
		glDeleteShader(shader->fragment);
		shader->fragment = 0;

		return false;
	}

	glCompileShader(shader->fragment);
	glGetShaderiv(shader->fragment, GL_COMPILE_STATUS, &compiled);
	if (compiled == false) {
		printf("Fragment shader not compiled\n");
		glPrintShaderInfo(shader->fragment);

		glDeleteShader(shader->vertex);
		shader->vertex = 0;
		glDeleteShader(shader->fragment);
		shader->fragment = 0;

		return false;
	}

	shader->main = glCreateProgram();

	glAttachShader(shader->main, shader->vertex);
	glAttachShader(shader->main, shader->fragment);

	glLinkProgram(shader->main);

	GLint is_linked;
	glGetProgramiv(shader->main, GL_LINK_STATUS, (GLint *)&is_linked);
	if (is_linked == false) {
		printf("Failed to link shader\n");

		GLint max_length;
		glGetProgramiv(shader->main, GL_INFO_LOG_LENGTH, &max_length);
		if (max_length > 0) {
			char *p_link_info_log = new char[max_length];
			glGetProgramInfoLog(shader->main, max_length, &max_length, p_link_info_log);
			printf("%s\n", p_link_info_log);
			delete[] p_link_info_log;
		}

		glDetachShader(shader->main, shader->vertex);
		glDetachShader(shader->main, shader->fragment);
		glDeleteShader(shader->vertex);
		shader->vertex = 0;
		glDeleteShader(shader->fragment);
		shader->fragment = 0;
		glDeleteProgram(shader->main);
		shader->main = 0;

		return false;
	}
	return true;
}

#endif