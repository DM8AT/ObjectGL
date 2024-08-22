/**
 * @file OGL_Framebuffer.cpp
 * @author DM8AT
 * @brief implement the handeling of framebuffers
 * @version 0.1
 * @date 2024-08-20
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

OGL_Framebuffer::OGL_Framebuffer(std::vector<OGL_FramebufferAttachment> attachments)
{
    //create the framebuffer
    glGenFramebuffers(1, &this->framebuffer);
    //store the attachments
    this->attachments = attachments;
    //apply the attachments
    this->updateAttachmentBindings();
}

void OGL_Framebuffer::updateAttachmentBindings()
{
    //bind the framebuffer
    this->bind(GL_FRAMEBUFFER);
    //loop over all attachments
    for (size_t i = 0; i < this->attachments.size(); ++i)
    {
        //quick safty check that the pointer is valid
        if (!this->attachments[i].texture)
        {
            //throw an error
            std::__throw_runtime_error("One or more of the color attachments is a nullpointer");
        }
        //store the attachment
        GLenum attachment = GL_COLOR_ATTACHMENT0;
        //get the type of identifyer to use
        switch (this->attachments[i].type)
        {
        case OGL_COLOR_ATTACHMENT:
            //store the correct color attachment
            attachment += this->attachments[i].attachmentID;
            break;
        case OGL_DEPTH_ATTACHMENT:
            //store that this is a depth attachment
            attachment = GL_DEPTH_ATTACHMENT;
            break;
        case OGL_STENCIL_ATTACHMENT:
            //store that this is a stencil attachment
            attachment = GL_STENCIL_ATTACHMENT;
            break;
        case OGL_DEPTH_STENCIL_ATTACHMENT:
            //store that this is a depth and stencil attachment
            attachment = GL_DEPTH_STENCIL_ATTACHMENT;
            break;
        
        default:
            break;
        }
        //identify the texture type
        switch (this->attachments[i].texture->getType())
        {
        case OGL_TEXTURE_2D:
            //attach a simple 2D texture
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, this->attachments[i].texture->getTexture(), 0);
            break;

        case OGL_TEXTURE_CUBEMAP:
            //attach a cubemap face
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_CUBE_MAP_POSITIVE_X + this->attachments[i].layerSelect, this->attachments[i].texture->getTexture(), 0);
            break;
        
        case OGL_TEXTURE_ARRAY_2D:
            //attach a specific slice of the array
            glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, this->attachments[i].texture->getTexture(), 0, this->attachments[i].layerSelect);
            break;

        default:
            break;
        }
    }
    //store if the framebuffer is compleate
    GLenum compleate = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    //check if the framebuffer is compleate
    if (compleate != GL_FRAMEBUFFER_COMPLETE)
    {
        //throw an error
        std::__throw_runtime_error((std::string("The framebuffer finished binding the attachments but is still incompleate. Error ID: ") + std::to_string(compleate)).c_str());
    }
}

void OGL_Framebuffer::bind(GLenum target)
{
    //make sure to bind the correct instance
    correctInstanceBinding()
    //bind to the requested buffer
    glBindFramebuffer(target, this->framebuffer);
}

void OGL_Framebuffer::unbind(GLenum target)
{
    //make sure to bind the correct instance
    correctInstanceBinding()
    //unbind the requested target
    glBindFramebuffer(target, 0);
}

void OGL_Framebuffer::onDestroy()
{
    //bind the correct instance
    correctInstanceBinding()
    //delete the framebuffer
    glDeleteFramebuffers(1, &this->framebuffer);
    //clear the attachments
    this->attachments.clear();
}