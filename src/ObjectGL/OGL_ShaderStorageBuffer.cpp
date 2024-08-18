/**
 * @file OGL_ShaderStorageBuffer.cpp
 * @author DM8AT
 * @brief implement the non-template functions for shader storage buffers
 * @version 0.1
 * @date 2024-08-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

//ask for access to the background library
#define OGL_KEEP_BG_ACCESS
//include the main header
#include "ObjectGL.hpp"

//a macro to ensure that the correct window is bound
#define correctInstanceBinding() if (oglGetCurrentInstance() != this->instance) {this->instance->makeCurrent();}
/**
 * @brief a mactor to check if the buffer is inalised
 * 
 * @param error the error message to print if the buffer is not initalised
 */
#define checkExistance(buffer, error) if (buffer == 0) { std::__throw_runtime_error(error); }

OGL_ShaderStorageBuffer::OGL_ShaderStorageBuffer(void* data, size_t size)
{
    //bind the correct instance
    correctInstanceBinding()
    //create the SSBO
    glGenBuffers(1, &this->ssbo);
    //cast to a function
    this->updateData(data, size);
    //upload the data
    this->upload();
}

void OGL_ShaderStorageBuffer::updateData(void* data, size_t size)
{
    //clear the own vector
    this->data.clear();
    //check if the data is correct
    if (data && size!=0)
    {
        //set the size of the data
        this->data.resize(size);
        //copy the data
        memcpy(this->data.data(), data, size);
    }
}

void OGL_ShaderStorageBuffer::upload()
{
    //correct the instance
    correctInstanceBinding()
    //check if the buffer exists
    checkExistance(this->ssbo, "Can't upload an unitalised buffer")
    //bind the buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
    //upload the data
    glBufferData(GL_SHADER_STORAGE_BUFFER, this->data.size(), this->data.data(), GL_DYNAMIC_READ);
    //unbind the buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void OGL_ShaderStorageBuffer::download()
{
    //correct the instance
    correctInstanceBinding()
    //bind the buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
    //download the data
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, this->data.size(), this->data.data());
}

void OGL_ShaderStorageBuffer::bind(uint8_t unit)
{
    //bind the correct instance
    correctInstanceBinding()
    //check if the buffer exists
    checkExistance(this->ssbo, "Can't bind an unitalised buffer")
    //simply bind the buffer
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, unit, this->ssbo);
}

void OGL_ShaderStorageBuffer::unbind()
{   
    //bind the correct instance
    correctInstanceBinding()
    //unbind all buffers
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void OGL_ShaderStorageBuffer::onDestroy()
{
    //check if the buffer exists
    if (this->ssbo != 0)
    {
        //bind the correct instance
        correctInstanceBinding()
        //delete the object
        glDeleteBuffers(1, &this->ssbo);
    }
}