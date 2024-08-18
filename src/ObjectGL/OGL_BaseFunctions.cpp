/**
 * @file OGL_BaseFunctions.cpp
 * @author DM8AT
 * @brief implement all base functions like the handeling of events
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

//define how the event creation is handled
void oglHandleAllEvents()
{
    //check if instances exist
    if (oglAllInstances.size() == 0)
    {
        //throw an error
        std::__throw_runtime_error("Can't handle events if no instances exist");
        return;
    }

    //store the current event
    SDL_Event e;
    //loop over all events
    while (SDL_PollEvent(&e))
    {
        //loop over all instances
        for (OGL_Instance* instance : oglAllInstances)
        {
            //pass the event, if it belongs to this, continue to the next event
            if (instance->handleEvent(e)) { continue; }
        }
    }
}

//define the function to get the current instance
OGL_Instance* oglGetCurrentInstance()
{
    //return the current instance
    return oglCurrentInstance;
}