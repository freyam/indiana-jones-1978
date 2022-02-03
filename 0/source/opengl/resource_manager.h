#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "glad/glad.h"
#include <bits/stdc++.h>

#include "shaders.h"
#include "textures.h"
using namespace std;

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
class ResourceManager {
public:
    // resource storage
    static map<string, Shader> Shaders;
    static map<string, Texture2D> Textures;

    // loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    static Shader LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, string name);

    // retrieves a stored sader
    static Shader GetShader(string name);

    // loads (and generates) a texture from file
    // static Texture2D LoadTexture(const char *file, bool alpha, string name);

    // // retrieves a stored texture
    // static Texture2D GetTexture(string name);

    // properly de-allocates all loaded resources
    static void Clear();

private:
    // private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    ResourceManager() {}
    // loads and generates a shader from file
    static Shader loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile = nullptr);

    // loads a single texture from file
    // static Texture2D loadTextureFromFile(const char *file, bool alpha);
};

#endif