/**
 * @file OGL_BindableBase.cpp
 * @author DM8AT
 * @brief implement the bindable base class which is used to handle the binding of OpenGL objects to they're correct contexts
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

//define the constructor for the bindable base
OGL_BindableBase::OGL_BindableBase()
{
    //check if an instance is bound to the object. If it is, bind it
    if (this->instance != 0) {this->instance->makeCurrent();return;}
    //check if a current instance exists
    if (!oglCurrentInstance)
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
    //register this class to the instance
    this->instance->registerBindable(this);
}

//define the destructor for the bindable base
OGL_BindableBase::~OGL_BindableBase()
{
    //call the destroy function
    this->onDestroy();
    //remove this from the instance
    this->instance->removeBindable(this);
    //set the instance to 0
    this->instance = 0;
}