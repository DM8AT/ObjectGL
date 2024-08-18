/**
 * @file ObjectGLGlobalState.hpp
 * @author DM8AT
 * @brief store the global state of the ObjectGL library
 * @version 0.1
 * @date 2024-08-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

//header guard
#ifndef _OBJECT_GL_GLOBAL_STATE_H_
#define _OBJECT_GL_GLOBAL_STATE_H_

//stdlib
#include <cstdint>
#include <memory>
#include <vector>

//say that an instance class will exist
class OGL_Instance;
//say that a window class will exist
class OGL_Window;

/**
 * @brief store a pointer to the current instance
 */
extern OGL_Instance* oglCurrentInstance;
/**
 * @brief store a pointer to the current window
 */
extern OGL_Window* oglCurrentWindow;
/**
 * @brief store a vector of pointers to the instances
 */
extern std::vector<OGL_Instance*> oglAllInstances;

#endif