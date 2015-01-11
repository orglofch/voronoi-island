#ifndef _TEXTURE_MANAGER_HPP_
#define _TEXTURE_MANAGER_HPP_

#include <unordered_map>
#include <string>

#include "OpenGL.hpp"

struct Texture
{

};

class TextureManager
{
public:
	void LoadPng(const std::string &filename);

private:
	std::unordered_map<std::string, Texture> m_textures;
};

extern TextureManager eTextureManager;

#endif