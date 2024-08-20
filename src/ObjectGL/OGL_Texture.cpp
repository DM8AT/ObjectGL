/**
 * @file OGL_Texture.cpp
 * @author your name (you@domain.com)
 * @brief 
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

/**
 * @brief add a side to a cubemap texture
 * 
 * @param side the side to add
 * @param texFile path to the texture file
 */
void addTextureCubemap(GLenum side, const char* texFile, uint32_t* width, uint32_t* height, GLenum internalFormat)
{
    //read the texture file
    SDL_Surface* surf = IMG_Load(texFile);
    //check if the surface could load
    if (!surf)
    {
        //if not, print an error
        std::__throw_runtime_error((std::string("Failed to open file ") + texFile).c_str());
    }

    //store a formated surface
    SDL_Surface* formated = SDL_CreateRGBSurface(0,surf->w,surf->h,32,0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
    //blit the texture
    SDL_BlitSurface(surf, 0, formated, 0);

    //store the width
    *width = surf->w;
    //store the height
    *height = surf->h;

    //upload the texture data
    glTexImage2D(side, 0, internalFormat, surf->w,surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, formated->pixels);

    //free the surfaces
    SDL_FreeSurface(surf);
    SDL_FreeSurface(formated);
}

OGL_Texture::OGL_Texture(OGL_TextureType type, uint32_t width, uint32_t height, uint32_t layers, GLenum internalFormat)
{
    //create the texture
    glGenTextures(1, &this->texture);
    //store the type
    this->type = type;

    //switch over the type
    switch (type)
    {
    case OGL_TEXTURE_2D:
        //store the type
        this->format = GL_TEXTURE_2D;
        //create a clear 2D texture
        glTexStorage2D(this->texture, layers, internalFormat, width, height);
        break;

    case OGL_TEXTURE_CUBEMAP:
        //store the type
        this->format = GL_TEXTURE_CUBE_MAP;
        //bind the texture
        this->bind(0);
        //loop over all 6 faces
        for (int i = 0; i < 6; ++i)
        {
            //store a clear 2D texture
            glTextureStorage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, layers, internalFormat, width, height);
        }
        break;

    case OGL_TEXTURE_ARRAY_2D:
        //store the type
        this->format = GL_TEXTURE_2D_ARRAY;
        //make the format ready
        glTexStorage3D(this->texture, 1, internalFormat, width, height, layers);
        break;
    
    default:
        break;
    }
}

OGL_Texture::OGL_Texture(const char* texFile, GLenum internalFormat)
{
    //create the texture
    glGenTextures(1, &this->texture);
    //set the texture
    this->setTexture(texFile, internalFormat);
}

OGL_Texture::OGL_Texture(std::vector<const char*> texFiles, OGL_TextureType type, GLenum internalFormat)
{
    //create the texture
    glGenTextures(1, &this->texture);
    //set the texture
    this->setTexture(texFiles, type, internalFormat);
}

OGL_Texture::OGL_Texture(void* data, uint32_t width, uint32_t height, GLenum format, GLenum type, GLenum internalFormat)
{
    //create the texture
    glGenTextures(1, &this->texture);
    //set the texture
    this->setTexture(data, width, height, format, type, internalFormat);
}

void OGL_Texture::setTexture(const char* texFile, GLenum internalFormat)
{
    //open the surface
    SDL_Surface* surf = IMG_Load(texFile);
    //check if the surface could load
    if (!surf)
    {
        //if not, print an error
        std::__throw_runtime_error((std::string("Failed to open file ") + texFile).c_str());
    }

    //store a formated surface
    SDL_Surface* formated = SDL_CreateRGBSurface(0,surf->w,surf->h,32,0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
    //blit the texture
    SDL_BlitSurface(surf, 0, formated, 0);

    //store the width
    this->width = surf->w;
    //store the height
    this->height = surf->h;
    //set the amount of layers to 0
    this->layers = 0;
    //store the format as 2D texture
    this->type = OGL_TEXTURE_2D;
    //store the internal format
    this->format = GL_TEXTURE_2D;
    //bind to unit 0
    this->bind(0);
    //upload the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, surf->w,surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, formated->pixels);
    //unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    //free the surfaces
    SDL_FreeSurface(surf);
    SDL_FreeSurface(formated);
}

void OGL_Texture::setTexture(std::vector<const char*> texFiles, OGL_TextureType type, GLenum internalFormat)
{
    //switch over the type
    switch (type)
    {
    //check for the 2D texture
    case OGL_TEXTURE_2D:
        //check if the amount of layers is correct
        if (texFiles.size() != 1)
        {
            //throw an error
            std::__throw_runtime_error("The amount of files is not correct to create a 2D texture");
            return;
        }
        //else, pass to another function
        this->setTexture(texFiles[0], internalFormat);
        break;

    case OGL_TEXTURE_CUBEMAP:
        //check if the amount of layers is correct
        if (texFiles.size() != 6)
        {
            //throw an error
            std::__throw_runtime_error("The amount of files is not correct to create a cubemap texture");
            return;
        }
        //set the inetrnal format
        this->type = OGL_TEXTURE_CUBEMAP;
        //set the format
        this->format = GL_TEXTURE_CUBE_MAP;
        //set the amount of layers to 0
        this->layers = 0;
        //bind the texture
        this->bind(0);
        //loop over all 6 sides
        for (int i = 0; i < 6; ++i)
        {
            //add the face
            addTextureCubemap(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i, texFiles[i], &this->width, &this->height, internalFormat);
        }
        //unbind the texture
        this->unbind(0);
        break;
    
    case OGL_TEXTURE_ARRAY_2D:
        //no size-check needed, every size is good
        //store the internal format
        this->type = OGL_TEXTURE_ARRAY_2D;
        //store the format
        this->format = GL_TEXTURE_2D_ARRAY;
        //bind the texture
        this->bind(0);
        //loop over all layers
        for (size_t i = 0; i < texFiles.size(); ++i)
        {
            //read the texture file
            SDL_Surface* surf = IMG_Load(texFiles[i]);
            //check if the surface could load
            if (!surf)
            {
                //if not, print an error
                std::__throw_runtime_error((std::string("Failed to open file ") + texFiles[i]).c_str());
            }

            //store a formated surface
            SDL_Surface* formated = SDL_CreateRGBSurface(0,surf->w,surf->h,32,0x000000FF,0x0000FF00,0x00FF0000,0xFF000000);
            //blit the texture
            SDL_BlitSurface(surf, 0, formated, 0);

            //check if this is the first image
            if (i == 0)
            {
                //store the width
                this->width = surf->w;
                //store the height
                this->height = surf->h;
                //store the layers
                this->layers = texFiles.size();
                //prepare the storage
                glTexStorage3D(this->format, 1, internalFormat, this->width, this->height, this->layers);
            }

            //upload the texture data
            glTexSubImage3D(this->format, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, formated->pixels);

            //free the surfaces
            SDL_FreeSurface(surf);
            SDL_FreeSurface(formated);
        }

    default:
        break;
    }
}

void OGL_Texture::setTexture(void* data, uint32_t width, uint32_t height, GLenum format, GLenum type, GLenum internalFormat)
{
    //store the width
    this->width = width;
    //store the height
    this->height = height;
    //store the layers
    this->layers = 0;
    //store the type
    this->type = OGL_TEXTURE_2D;
    //store the format
    this->format = GL_TEXTURE_2D;
    //bind to unit 0
    this->bind(0);
    //store the inputed user data
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
}

void OGL_Texture::bind(uint8_t unit)
{
    //make sure to activate the instance
    correctInstanceBinding()
    //activate the requested unit
    glActiveTexture(GL_TEXTURE0 + unit);
    //bind the texture
    glBindTexture(this->format, this->texture);
}

void OGL_Texture::unbind(uint8_t unit)
{
    //make sure to activate the instance
    correctInstanceBinding()
    //activate the requested unit
    glActiveTexture(GL_TEXTURE0 + unit);
    //unbind the texture
    glBindTexture(this->format, 0);
}

void OGL_Texture::setTexParameter(GLenum parameter, GLenum value)
{
    //make sure to activate the instance
    correctInstanceBinding()
    //bind to unit 0
    this->bind(0);
    //set the texture parameter
    glTexParameteri(this->format, parameter, value);
}

void OGL_Texture::createMipmap()
{
    //make sure to activate the instance
    correctInstanceBinding()
    //bind to unit 0
    this->bind(0);
    //create the mip chain
    glGenerateMipmap(this->format);
}

void OGL_Texture::onDestroy()
{
    //make sure to bind the correct instance
    correctInstanceBinding()
    //delete the texture
    glDeleteTextures(1, &this->texture);
    //set the texture to 0
    this->texture = 0;
}