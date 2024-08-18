/**
 * @file OGL_IndexBuffer.cpp
 * @author DM8AT
 * @brief implement a wraper for index buffers (element array buffers)
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

OGL_IndexBuffer::OGL_IndexBuffer(std::vector<uint32_t> indices)
{
    //make sure to bind the correct instance
    correctInstanceBinding()
    //create a buffer
    glGenBuffers(1, &this->ibo);
    //store the new data
    this->updateData(indices);
    //upload the data
    this->upload();
    //bind this buffer
    this->bind();
}

void OGL_IndexBuffer::upload()
{
    //correct the instance binding
    correctInstanceBinding()
    //check if the buffer exists
    checkExistance(this->ibo, "Can't upload data to a not existing index buffer")
    //bind the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
    //upload the data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(uint32_t), this->indices.data(), GL_STATIC_DRAW);
}

void OGL_IndexBuffer::setElement(uint32_t index, size_t idx)
{
    //make sure the index is in range
    if (idx > this->indices.size())
    {
        //throw an error
        std::__throw_runtime_error("The index to access is out of bunds for the element array buffer");
        return;
    }
    //store the element
    this->indices[idx] = index;
}

uint32_t OGL_IndexBuffer::getElement(size_t idx)
{
    //make sure the index is in range
    if (idx > this->indices.size())
    {
        //throw an error
        std::__throw_runtime_error("The index to access is out of bunds for the element array buffer");
        return 0;
    }
    //return the element
    return this->indices[idx];
}

uint32_t* OGL_IndexBuffer::getElementPtr(size_t idx)
{
    //make sure the index is in range
    if (idx > this->indices.size())
    {
        //throw an error
        std::__throw_runtime_error("The index to access is out of bunds for the element array buffer");
        return 0;
    }
    //return a pointer to the element
    return &this->indices[idx];
}

uint32_t& OGL_IndexBuffer::operator[](size_t idx)
{
    //make sure the index is in range
    if (idx > this->indices.size())
    {
        //throw an error
        std::__throw_runtime_error("The index to access is out of bunds for the element array buffer");
    }
    //return a reference to the element
    return this->indices[idx];
}

void OGL_IndexBuffer::bind()
{
    //correct the instance binding
    correctInstanceBinding()
    //check if the buffer exists
    checkExistance(this->ibo, "Can't bind a non existing index buffer")
    //bind the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
}

void OGL_IndexBuffer::unbind()
{
    //correct the instance binding
    correctInstanceBinding()
    //unbind the buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OGL_IndexBuffer::onDestroy()
{
    //check if the data is intalised
    if (this->ibo != 0)
    {
        //make sure to correct the instance binding
        correctInstanceBinding()
        //delete the buffer
        glDeleteBuffers(1, &this->ibo);
    }
}