/**
 * @file OGL_VertexAttributes.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-08-14
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

OGL_VertexAttributes::OGL_VertexAttributes(std::vector<OGL_VertexAttribute> attributes, size_t vertexSize)
{
    //update the structure
    this->updateStructure(attributes, vertexSize);
}

void OGL_VertexAttributes::updateStructure(std::vector<OGL_VertexAttribute> attributes, size_t vertexSize)
{
    //bind the correct instance
    correctInstanceBinding()

    //check if a VAO exists
    if (this->vao == 0)
    {
        //if not, create one
        glGenVertexArrays(1, &this->vao);
    }
    //store the size of a single vertex
    this->vertSize = vertexSize;
    //store the attributes
    this->attributes = attributes;

    //re-upload to GPU
    this->update();
}

void OGL_VertexAttributes::update()
{
    //bind the correct instance
    correctInstanceBinding()

    //store if a VBO is bound
    GLint VBO = 0;
    //get the bound VBO
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &VBO);
    //if no VBO is bound, throw an error
    if (VBO == 0)
    {
        //throw an error
        std::__throw_runtime_error("Can't bind a VAO if no VBO is bound");
        return;
    }
    //bind the own VAO
    glBindVertexArray(this->vao);

    //store the offset
    size_t offset = 0;
    //loop over all attributes
    for (size_t i = 0; i < this->attributes.size(); ++i)
    {
        //enable the vertex attribute
        glEnableVertexAttribArray(i);
        //switch over the type
        switch (this->attributes[i].type)
        {
        //check for a float
        case OGL_TYPE_FLOAT:
            //pass the vertex data
            glVertexAttribPointer(i, this->attributes[i].size, GL_FLOAT, this->attributes[i].normalize, this->vertSize, (void*)offset);
            //increase the offset
            offset += this->attributes[i].size * sizeof(float);
            break;

        //check for an int
        case OGL_TYPE_INT:
            //pass the vertex data
            glVertexAttribIPointer(i, this->attributes[i].size, GL_INT, this->attributes[i].normalize, (void*)offset);
            //increase the offset
            offset += this->attributes[i].size * sizeof(int);
            break;

        //check for an unsigned integer
        case OGL_TYPE_UINT:
            //pass the vertex data
            glVertexAttribIPointer(i, this->attributes[i].size, GL_UNSIGNED_INT, this->attributes[i].normalize, (void*)offset);
            //increase the offset
            offset += this->attributes[i].size * sizeof(unsigned int);
            break;
        
        default:
            break;
        }
    }
}

OGL_VertexAttribute& OGL_VertexAttributes::operator[](size_t idx)
{
    //check if the index is in bounds
    if (idx >= this->attributes.size())
    {
        //if not, set the index to the size
        idx = (int)this->attributes.size();
        //create a new element
        this->attributes.push_back(OGL_VertexAttribute{4, OGL_TYPE_FLOAT, false});
    }
    //return access to the requested arguments
    return this->attributes[idx];
}

void OGL_VertexAttributes::bind()
{
    //bind the own vao
    glBindVertexArray(this->vao);
}

void OGL_VertexAttributes::onDestroy()
{

}