#include<bits/stdc++.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "utilityHeaders/resource_manager.h"

using namespace std;



// Instantiate static variables
map<string, Shader>       ResourceManager::Shaders;


Shader ResourceManager::LoadShader(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile, string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(string name)
{
    return Shaders[name];
}

// Texture2D ResourceManager::LoadTexture(const char *file, bool alpha, std::string name)
// {
//     Textures[name] = loadTextureFromFile(file, alpha);
//     return Textures[name];
// }

// Texture2D ResourceManager::GetTexture(std::string name)
// {
//     return Textures[name];
// }

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char *vShaderFile, const char *fShaderFile, const char *gShaderFile)
{
    // 1. retrieve the vertex/fragment source code from filePath
    string vertexCode;
    string fragmentCode;
    string geometryCode;
    try
    {
        // open files
        ifstream vertexShaderFile(vShaderFile);
        ifstream fragmentShaderFile(fShaderFile);
        stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            ifstream geometryShaderFile(gShaderFile);
            stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (exception e)
    {
        cout << "ERROR::SHADER: Failed to read shader files" << endl;
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    const char *gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    // cout << "shader compiled succesfully" << endl;
    return shader;
}

