#ifndef _SHADER_MANAGER_HPP_
#define _SHADER_MANAGER_HPP_

#include <string>

#include "OpenGL.hpp"

class ShaderManager
{
public:
	enum Shader {
		SHADER_PASS_THROUGH,
		SHADER_TILT_SHIFT,
		SHADER_COUNT // Last element
	};

	ShaderManager();
	~ShaderManager();

	sf::Shader &GetShader(Shader s);

private:
	void InitShaders(void);

	void LoadShader(Shader s);
	
	std::string FileForVertexShader(Shader s);
	std::string FileForFragmentShader(Shader s);

	sf::Shader *m_shaders;
};

extern ShaderManager eShaderManager;

#endif