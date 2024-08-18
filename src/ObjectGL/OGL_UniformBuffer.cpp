/**
 * @file OGL_UniformBuffer.cpp
 * @author DM8AT
 * @brief implement uniform buffers
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
 * @param buffer the name of the buffer to check
 * @param error the error message to print if the buffer is not initalised
 */
#define checkExistance(buffer, error) if (buffer == 0) { std::__throw_runtime_error(error); }

OGL_UniformBuffer::OGL_UniformBuffer(void* data, size_t size)
{
    //bind the correct instance
    correctInstanceBinding()
    //create the UBO
    glGenBuffers(1, &this->ubo);
    //cast to a function
    this->updateData(data, size);
    //upload the data
    this->upload();
}

void OGL_UniformBuffer::updateData(void* data, size_t size)
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

void OGL_UniformBuffer::upload()
{
    //correct the instance
    correctInstanceBinding()
    //check if the buffer exists
    checkExistance(this->ubo, "Can't upload an unitalised buffer")
    //bind the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, this->ubo);
    //upload the data
    glBufferData(GL_UNIFORM_BUFFER, this->data.size(), this->data.data(), GL_DYNAMIC_READ);
    //unbind the buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OGL_UniformBuffer::bind(uint8_t unit)
{
    //bind the correct instance
    correctInstanceBinding()
    //check if the buffer exists
    checkExistance(this->ubo, "Can't bind an unitalised buffer")
    //simply bind the buffer
    glBindBufferBase(GL_UNIFORM_BUFFER, unit, this->ubo);
}

void OGL_UniformBuffer::unbind()
{   
    //bind the correct instance
    correctInstanceBinding()
    //unbind all buffers
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void OGL_UniformBuffer::onDestroy()
{
    //check if the buffer exists
    if (this->ubo != 0)
    {
        //bind the correct instance
        correctInstanceBinding()
        //delete the object
        glDeleteBuffers(1, &this->ubo);
    }
}