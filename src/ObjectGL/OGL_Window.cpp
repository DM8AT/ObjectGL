/**
 * @file OGL_Window.cpp
 * @author DM8AT
 * @brief implement the window class to handle multiple windows
 * @version 0.1
 * @date 2024-08-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

//ask for access to the background library
#define OGL_KEEP_BG_ACCESS
//include the main header
#include "ObjectGL.hpp"

//a macro to ensure that the correct window is bound
#define correctWindowBinding() if (oglCurrentWindow != this) {this->makeCurrent();}

//define the constructor of the window
OGL_Window::OGL_Window(const char* name, uint32_t width, uint32_t height, uint32_t x, uint32_t y)
{
    //store the name
    this->name = name;
    //store the width
    this->width = width;
    //store the height
    this->height = height;
    //store the x position
    this->posX = x;
    //store the y position
    this->posY = y;
    //check if a current instance exists
    if (oglAllInstances.size() == 0)
    {
        //throw an error
        std::__throw_runtime_error("Can't create a window without creating an instance first");
    }
    //check if an instance is bound
    if (oglCurrentInstance)
    {
        //store the bound instance
        this->instance = oglCurrentInstance;
    }
    else
    {
        //else, use the last created instance
        this->instance = oglAllInstances[oglAllInstances.size()-1];
    }
    //create a window
    this->window = SDL_CreateWindow(name, x,y, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    //register this class to the instance
    this->instance->registerWindow(this);

}

//define the destroctor of the windwo
OGL_Window::~OGL_Window()
{
    //check if this is the current window
    if (oglCurrentWindow == this)
    {
        //set the current window to 0
        oglCurrentWindow = 0;
    }
    //de-register the window
    this->instance->removeWindow(this);
    //close the window
    SDL_DestroyWindow(this->window);
}

//define the funciton to bind this window and the own context
void OGL_Window::makeCurrent()
{
    //bind the OpenGL context to this window
    SDL_GL_MakeCurrent(this->window, this->instance->getContext());
    //store this as the current window
    oglCurrentWindow = this;
}

//define the function to handle events
bool OGL_Window::handleEvent(SDL_Event event)
{
    //check if the event window id belongs to this window. Return false if not
    if (event.window.windowID != SDL_GetWindowID(this->window)) {return false;}

    //handle the event
    switch (event.window.event)
    {
    //check for the closing event
    case SDL_WINDOWEVENT_CLOSE:
        //store that the window should close
        this->shouldClose = true;
        break;
    
    //check if the window was resized
    case SDL_WINDOWEVENT_RESIZED:
        //store the new width
        this->width = event.window.data1;
        //store the new height
        this->height = event.window.data2;
        //make this the current window
        correctWindowBinding()
        //set the viewport
        glViewport(0,0, this->width, this->height);
        break;

    default:
        break;
    }

    //return that the event was handled
    return true;
}

//define the function to set the clear color
void OGL_Window::setClearColor(float r, float g, float b, float a)
{
    //store the red component
    this->clearR = r;
    //store the green component
    this->clearG = g;
    //store the blue component
    this->clearB = b;
    //store the alpha component
    this->clearA = a;
}

void OGL_Window::setSize(uint32_t w, uint32_t h)
{
    //make sure to bind the correct window
    correctWindowBinding()
    //set the window size
    SDL_SetWindowSize(this->window, w,h);
    //update the viewport
    glViewport(0,0,w,h);
    //store the width
    this->width = w;
    //store the height
    this->height = h;
}

//define the funciton to clear this window
void OGL_Window::clear(bool clearColor, bool clearDepth, bool clearStencil)
{
    //make sure to bind the correct window
    correctWindowBinding()
    //set the clear color
    glClearColor(this->clearR,this->clearG,this->clearB,this->clearA);
    //make the clear OpenGL call
    glClear((GL_COLOR_BUFFER_BIT * clearColor) | (GL_DEPTH_BUFFER_BIT * clearDepth) | (GL_STENCIL_BUFFER_BIT * clearStencil));
}

//define the function to flip the window's surface
void OGL_Window::flip()
{
    //flip the window
    SDL_GL_SwapWindow(this->window);
}