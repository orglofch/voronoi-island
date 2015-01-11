#include "ShaderManager.hpp"

#include <iostream>

#include "Assertion.hpp"

using std::string;
using std::cout;
using std::endl;

ShaderManager eShaderManager;

ShaderManager::ShaderManager() :
	m_shaders(new sf::Shader[SHADER_COUNT])
{
	if (sf::Shader::isAvailable()) {
		InitShaders();
	} else {
		cout << "Shaders aren't available" << endl;
	}
}

ShaderManager::~ShaderManager()
{
	delete[] m_shaders;
}

sf::Shader &ShaderManager::GetShader(Shader s) 
{
	return m_shaders[s];
}

void ShaderManager::InitShaders(void)
{
	for (int i = 0; i < SHADER_COUNT; ++i) {
		LoadShader((Shader)i);
	}
}

void ShaderManager::LoadShader(Shader s)
{
	if (!m_shaders[s].loadFromFile(FileForVertexShader(s), FileForFragmentShader(s))) {
		cout << "Failed to load shader " << s << endl;
	}
}

string ShaderManager::FileForVertexShader(Shader s)
{
	switch (s) {
		case SHADER_PASS_THROUGH:
		case SHADER_TILT_SHIFT:
			return "pass_through.vert";
			break;
	}
	NOTREACHED();
	return "???";
}

string ShaderManager::FileForFragmentShader(Shader s)
{
	switch (s) 
	{
		case SHADER_PASS_THROUGH:
			return "pass_through.frag";
			break;
		case SHADER_TILT_SHIFT:
			return "tilt_shift.frag";
			break;
	}
	NOTREACHED();
	return "???";
}