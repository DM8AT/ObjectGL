/**
 * @file OGL_Instance.cpp
 * @author DM8AT
 * @brief implement the basic OpenGL instance handeling
 * @version 0.1
 * @date 2024-08-10
 * 
 * @copyright Copyright (c) 2024
 */

//ask for access to the background library
#define OGL_KEEP_BG_ACCESS
//include the main header
#include "ObjectGL.hpp"

//stdlib includes
#include <stdexcept>
#include <iostream>

/**
 * @brief set the profile to use as well as the version
 * 
 * @param version the version to use
 * @param profile the type of profile to use
 */
static void specifyOGLVersion(float version, OGL_ProfileType profile)
{
    //extract the major version
    int major = floor(version);
    //extract the minor version
    int minor = ceil((version - major)*10.f);
    //store the profile mask
    int sdlProf = SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
    //which profile to use
    switch (profile)
    {
    //check for the core profile
    case OGL_PROFILE_CORE:
        sdlProf = SDL_GL_CONTEXT_PROFILE_CORE;
        break;

    //check for the es profile
    case OGL_PROFILE_ES:
        sdlProf = SDL_GL_CONTEXT_PROFILE_ES;
        break;
    
    //in this case, run with the compatibility profile, it works for both
    default:
        break;
    }
    //set the major version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
    //set the minor version
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
    //set the profile type
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, sdlProf);
}

//define the constructor
OGL_Instance::OGL_Instance(float version, OGL_ProfileType profile)
{
    //check if the background library is intalised
    if (oglAllInstances.size() == 0)
    {
        //initalise the background library
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            //throw an error
            throw std::runtime_error("Failed to initalise SDL2");
        }
        //store what to init
        int32_t flags = IMG_INIT_PNG;
        //initalise SDL2 image
        if (!(IMG_Init(flags) & flags))
        {
            //throw an error
            throw std::runtime_error("Failed to initalise SDL2 image");
        }
        //set the profile version
        specifyOGLVersion(version, profile);
    }
    //store the instance in the vector of instances
    oglAllInstances.push_back(this);
}

//define the destructor
OGL_Instance::~OGL_Instance()
{
    //loop over all bound bindables
    for (size_t i = 0; i < this->bindables.size(); ++i)
    {
        //delete the bindable
        delete this->bindables[i];
    }

    //loop over all bound windows
    for (size_t i = 0; i < this->windows.size(); ++i)
    {
        //delete the window
        delete this->windows[i];
    }
    //store the ID of the instance
    size_t id = -1;
    //loop over all instances
    for (size_t i = 0; i < oglAllInstances.size(); ++i)
    {
        //check if this is the own instance
        if (this == oglAllInstances[i])
        {
            //store the id
            id = i;
            //stop the loop
            break;
        }
    }
    //check if the element was found
    if (id == (size_t)-1)
    {
        //throw a runtime error
        std::__throw_runtime_error("Failed to find the instance queued for deletion in the pool of instances");
        //imediatly stop the function
        return;
    }
    //else, remove the instance
    oglAllInstances.erase(oglAllInstances.begin() + id);
    //check if this is the current instance
    if (this == oglCurrentInstance)
    {
        //set the current instance to 0
        oglCurrentInstance = 0;
    }
    //check if instances remain
    if (oglAllInstances.size() == 0)
    {
        //close the background library
        SDL_Quit();
    }
}

//define the binding of the instance
void OGL_Instance::makeCurrent()
{
    //store the own shared pointer as the current instance
    oglCurrentInstance = this;
    //check that windows exist
    if (this->windows.size() == 0)
    {
        //throw an error
        std::__throw_runtime_error("Can't make an instance current wich has no window");
    }
    //make this the current instance with the first window
    SDL_GL_MakeCurrent(this->windows[0]->getWindow(),this->context);
}

//define the getting if this is the current context
bool OGL_Instance::isCurrent()
{
    //return if the current weak pointer is the own pointer, if it dosn't exist, return false
    return (oglCurrentInstance == this);
}

//define the function to add a window to the instance
void OGL_Instance::registerWindow(OGL_Window* window)
{
    //store the pointer
    this->windows.push_back(window);
    //check if no context exists
    if (this->context == 0)
    {
        //create a new context
        this->context = SDL_GL_CreateContext(window->getWindow());
        //instantly make this the current context
        SDL_GL_MakeCurrent(window->getWindow(), this->context);
        //initalise GLEW
        if (glewInit() != GLEW_OK)
        {
            //throw an error
            std::__throw_runtime_error("Failed to initalise GLEW");
        }
        //set the correct swap interval state
        SDL_GL_SetSwapInterval(0);
        //make this the current instance
        this->makeCurrent();
    }
}

//define the function to remove a window from the instance
void OGL_Instance::removeWindow(OGL_Window* window)
{
    //store the index of the window
    size_t idx = -1;
    //loop over the windos
    for (size_t i = 0; i < this->windows.size(); ++i)
    {
        //check if this is the requested window
        if (this->windows[i] == window)
        {
            //store the index
            idx = i;
            //stop the loop
            break;
        }
    }
    //check if the window was found, if not, stop this
    if (idx == (size_t)-1) { return; }

    //else, erase the window from the stack
    this->windows.erase(this->windows.begin() + idx);
}

//define the function to add a window to the instance
void OGL_Instance::registerBindable(OGL_BindableBase* bindalbe)
{
    //store the pointer
    this->bindables.push_back(bindalbe);
}

//define the function to remove a window from the instance
void OGL_Instance::removeBindable(OGL_BindableBase* bindable)
{
    //store the index of the window
    size_t idx = -1;
    //loop over the windos
    for (size_t i = 0; i < this->bindables.size(); ++i)
    {
        //check if this is the requested window
        if (this->bindables[i] == bindable)
        {
            //store the index
            idx = i;
            //stop the loop
            break;
        }
    }
    //check if the window was found, if not, stop this
    if (idx == (size_t)-1) { return; }

    //else, erase the window from the stack
    this->bindables.erase(this->bindables.begin() + idx);
}

//implement the function to handle events
bool OGL_Instance::handleEvent(SDL_Event event)
{
    //store if a window was found
    bool found = false;
    //loop over all windows
    for (OGL_Window* window : this->windows)
    {
        //pass the event. If it belongs to the window, store it and return
        if (window->handleEvent(event)) { found = true; break; }
    }
    //return if a window was found
    return found;
}

//implement the function to set VSync
void OGL_Instance::setVSync(bool useVsync)
{
    //check if the correct instance is selected
    if (oglCurrentInstance != this)
    {
        //if not, select this instance and bind to window 0
        this->windows[0]->makeCurrent();
    }
    //set the vsync mode
    SDL_GL_SetSwapInterval(useVsync);
    //store if vsync is eanbled
    this->vsync = useVsync;
}