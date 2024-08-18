/**
 * @file OGL_BaseState.cpp
 * @author DM8AT
 * @brief handle the definition of the base state
 * @version 0.1
 * @date 2024-08-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

//include the global state
#include "ObjectGLGlobalState.hpp"

//say that an instance class will exist
class OGL_Instance;
/**
 * @brief store a weak pointer to the current instance
 */
OGL_Instance* oglCurrentInstance = 0;
/**
 * @brief store a pointer to the current window
 */
OGL_Window* oglCurrentWindow = 0;
/**
 * @brief store a vector of pointers to the instances
 */
std::vector<OGL_Instance*> oglAllInstances = {};