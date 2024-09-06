/**
 * @file OGL_Shader.cpp
 * @author DM8AT
 * @brief 
 * @version 0.1
 * @date 2024-08-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

//ask for access to the background library
#define OGL_KEEP_BG_ACCESS
//include the main header
#include "ObjectGL.hpp"

//stdlib
#include <fstream>

//a macro to ensure that the correct window is bound
#define correctInstanceBinding() if (oglGetCurrentInstance() != this->instance) {this->instance->makeCurrent();}

/**
 * @brief attcg a shader to an existing program
 * 
 * @param program the program to link to
 * @param source the source code of the shader
 * @param type the type of the shader to compile
 * 
 * @return int the compiled shader
 */
static int attachShader(GLint program, std::string source, GLenum type)
{
    //store the success tatus
    GLint status = 0;
    //create the vertex shader
    GLuint s = glCreateShader(type);
    //the program created correctly, so just guess that the shader worked too. 

    //store the source code
    const char* src = source.c_str();
    //store the length of the source
    int len = (int)source.length();
    //attach the shader source code
    glShaderSource(s, 1, &src, &len);
    //compile the shader
    glCompileShader(s);
    
    //get the compile status
    glGetShaderiv(s, GL_COMPILE_STATUS, &status);
    //check the status
    if (status == GL_FALSE)
    {
        //store the size of the error log
        GLint logSize = 0;
        //store the error message
        glGetShaderiv(s, GL_INFO_LOG_LENGTH, &logSize);
        //create the error log
        GLchar* buff = new GLchar[logSize];
        //get the error
        glGetShaderInfoLog(s, logSize, &logSize, buff);
        //print the error
        std::__throw_runtime_error((std::string("Encounterd a compile error while compiling a shader. Compile Error: \n") + buff).c_str());
        //free the buffer
        delete[] buff;
        //delete the shader
        glDeleteShader(s);
        //stop the function
        return 0;
    }
    
    //attach the shader
    glAttachShader(program, s);
    //return the shader
    return s;
}

/**
 * @brief compile a shader to an OpenGL shader on the GPU
 * 
 * @param vss the vertex shader source code
 * @param fss the fragment shader source code
 * @return GLuint the compiled shader on the GPU
 */
static GLuint compileShader(std::string vss, std::string fss)
{
    //create the program
    GLuint prog = glCreateProgram();
    //check if the program could be created
    if (prog == 0)
    {
        //if not, throw an error
        std::__throw_runtime_error("Failed to create shader program");
    }

    //link the vertex shader and store it
    GLuint vsg = attachShader(prog, vss, GL_VERTEX_SHADER);
    //link the fragment shader and store it
    GLuint fsg = attachShader(prog, fss, GL_FRAGMENT_SHADER);

    //link the whole program
    glLinkProgram(prog);

    //store if the program is linked
    GLint status = 0;
    //get if the program is linked
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    //check if the shader is linked
    if (status == GL_FALSE)
    {
        //store the size of the error log
        GLint logSize = 0;
        //store the error message
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logSize);
        //create the error log
        GLchar* buff = new GLchar[logSize];
        //get the error
        glGetProgramInfoLog(prog, logSize, &logSize, buff);
        //print the error
        std::__throw_runtime_error((std::string("Encounterd a linknig error while linking a shader. Linker error: \n") + buff).c_str());
        //free the buffer
        delete[] buff;
        //delete the shader
        glDeleteProgram(prog);
        //stop the function
        return 0;
    }

    //clean the vertex shader
    glDeleteShader(vsg);
    //clean the fragment shader
    glDeleteShader(fsg);

    //on success, return the shader
    return prog;
}

/**
 * @brief read a file into a string
 * 
 * @param file the file to read
 * @return std::string the read file
 */
static std::string readFile(std::string file)
{
    //open the file
    std::ifstream f(file.c_str());
    //check if the file is opend
    if (!f.is_open())
    {
        //else, print an error
        std::__throw_runtime_error((std::string("Failed to open file ") + file).c_str());
        return "";
    }
    //store the file output
    std::string out;
    //store a line
    std::string line;
    //read the file
    while (getline(f, line))
    {
        //store the line
        out += (line + "\n");
    }
    //close the file
    f.close();
    //return the string
    return out;
}

//define the constructor
OGL_Shader::OGL_Shader(std::string vs, std::string fs, OGL_ShaderInput type)
{
    //just re-compile the shader
    this->recompileShader(vs, fs, type);
}

//define the function to recompile a shader
void OGL_Shader::recompileShader(std::string vs, std::string fs, OGL_ShaderInput type)
{
    //make sure the correct instance is bound
    correctInstanceBinding()
    //switch over the types
    switch (type)
    {
    //check if the data is in a file
    case OGL_SHADER_INPUT_FILE:
    {
        //read the vertex shader
        std::string vss = readFile(vs);
        //read the fragment shader
        std::string fss = readFile(fs);
        //compile the shader
        this->shader = compileShader(vss, fss);
        break;
    }

    //check if this is the raw data
    case OGL_SHADER_INPUT_SOURCE_GLSL:
    {
        //just compile the data
        this->shader = compileShader(vs,fs);
        break;
    }
    
    default:
        break;
    }
}

void OGL_Shader::bind()
{
    //make sure to bind the correct instace
    correctInstanceBinding()

    //bind the shader
    glUseProgram(this->shader);
    //loop over all uniforms
    for (OGL_UniformInfo& ui : this->uniforms)
    {
        //switch over the type
        switch (ui.type)
        {
        case OGL_TYPE_FLOAT:
            //set the float
            glUniform1fv(ui.location, 1, (float*)((void*)ui.data));
            break;
        
        case OGL_TYPE_INT:
            //set the int
            glUniform1iv(ui.location, 1, (int*)((void*)ui.data));
            break;

        case OGL_TYPE_UINT:
            //set the unsigned int
            glUniform1uiv(ui.location, 1, (unsigned int*)((void*)ui.data));
            break;

        default:
            //throw an error
            std::__throw_runtime_error("The requested type is not a valid type for an uniform");
            break;
        }
    }
}

void OGL_Shader::unbind()
{
    //make sure to bind the correct instace
    correctInstanceBinding()
    //unbind the shader
    glUseProgram(0);
}

OGL_UniformInfo& OGL_Shader::operator[](std::string name)
{
    //store the element index
    size_t idx = -1;
    //iterate over all uniforms
    for (size_t i = 0; i < this->uniforms.size(); ++i)
    {
        //check if this is the correct element
        if (this->uniforms[i].name == name)
        {
            //if it is, store the index
            idx = i;
            //stop the loop
            break;
        }
    }
    //check if the element was found
    if (idx != (size_t)-1)
    {
        //return a reference to the element
        return this->uniforms[idx];
    }
    //store the index of the new element
    idx = this->uniforms.size();
    //create a new element
    this->uniforms.push_back(OGL_UniformInfo());
    //set the name to the requested one
    this->uniforms[idx].name = name;
    //return a reference to the requested element
    return this->uniforms[idx];
}

OGL_UniformInfo& OGL_Shader::operator[](size_t index)
{
    //check if the element is in range
    if (index < this->uniforms.size())
    {
        //if it is, return a reference to it
        return this->uniforms[index];
    }
    //else, throw an error
    std::__throw_runtime_error("The index is out of bounds for an uniform in the requested shader");
}

void OGL_Shader::recalculateUniforms()
{
    //loop over all uniforms
    for (OGL_UniformInfo& ui : this->uniforms)
    {
        //get the location
        ui.location = glGetUniformLocation(this->shader, ui.name.c_str());
    }
}

void OGL_Shader::onDestroy()
{
    //make sure to bind the correct instance
    correctInstanceBinding()
    //delete the program
    glDeleteProgram(this->shader);
}