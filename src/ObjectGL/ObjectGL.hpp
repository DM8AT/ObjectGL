/**
 * @file ObjectGL.hpp
 * @author DM8AT
 * @brief the main header for the object oriented OpenGL approche
 * @version 0.1
 * @date 2024-08-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

//header guard
#ifndef _OGL_OGL_H_
#define _OGL_OGL_H_

//stdlib
#include <vector>
#include <memory>
#include <string>

//include SDL2
#include <SDL2/SDL.h>
//include SDL_image
#include <SDL2/SDL_image.h>
//include GLEW. GLEW is a required library for this lib
#include <GL/glew.h>

//check if the file this is included into should get access to the background dependencys
#ifdef OGL_KEEP_BG_ACCESS

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

/**
 * @brief handle all event inputs and pass them to they're correct locations
 */
void oglHandleAllEvents();

//say that instances will exist
class OGL_Instance;
//say that a window class will exist
class OGL_Window;
//say that a base class will exist
class OGL_BindableBase;

/**
 * @brief get the currently active instance
 * 
 * @return OGL_Instance* a pointer to the active instance
 */
OGL_Instance* oglGetCurrentInstance();

/**
 * @brief specify the type of profile to use
 */
enum OGL_ProfileType {
    /**
     * @brief use the core verion of OpenGL. This version is optimised for desktop applications
     */
    OGL_PROFILE_CORE, 
    /**
     * @brief use the ES version of OpenGL. This version is optimised for mobile applications
     */
    OGL_PROFILE_ES,
    /**
     * @brief use some sort of compatible profile for both platforms
     */
    OGL_PROFILE_COMPATIBLE
    };

/**
 * @brief specify the base type used
 */
enum OGL_Type {
    /**
     * @brief a 32-Bit floating point number
     */
    OGL_TYPE_FLOAT, 
    /**
     * @brief a 32-Bit whole number with two's complement
     */
    OGL_TYPE_INT,
    /**
     * @brief an unsigned 32-Bit whole number without two's complement
     */
    OGL_TYPE_UINT
};

/**
 * @brief specify the meaning of the input for the shader class
 */
enum OGL_ShaderInput {
    /**
     * @brief specify that the input corresponds to files
     */
    OGL_SHADER_INPUT_FILE,
    /**
     * @brief specify that the input corresponds to the GLSL source code for shaders
     */
    OGL_SHADER_INPUT_SOURCE_GLSL
};

/**
 * @brief specify the type of a texture if it is not clear throught the constructor
 */
enum OGL_TextureType {
    /**
     * @brief a simple 2D texture
     */
    OGL_TEXTURE_2D, 
    /**
     * @brief a array of 2D textures of the same size
     */
    OGL_TEXTURE_ARRAY_2D,
    /**
     * @brief a cubemap texture
     */
    OGL_TEXTURE_CUBEMAP
};

/**
 * @brief store the type of attachment the seleced one is
 */
enum OGL_FramebufferAttachmentType {
    /**
     * @brief say that the texture should be used as a color attachment
     */
    OGL_COLOR_ATTACHMENT, 
    /**
     * @brief say that the texture should be used as a depth attachment
     */
    OGL_DEPTH_ATTACHMENT,
    /**
     * @brief say that the texture should be used as a stencil attachment
     */
    OGL_STENCIL_ATTACHMENT,
    /**
     * @brief a combination of depth and stencil attachment
     */
    OGL_DEPTH_STENCIL_ATTACHMENT
};

/**
 * @brief store an OpenGL instance and handle its creation and destruction
 */
class OGL_Instance
{
public:
    /**
     * @brief Construct a new ogl instance
     * 
     * @param version the version of OpenGL to use as a float
     * @param profile the type of profile to use
     */
    OGL_Instance(float version = 4.5, OGL_ProfileType profile = OGL_PROFILE_COMPATIBLE);

    /**
     * @brief Destroy the ogl instance
     */
    ~OGL_Instance();

//check if behind-the-scenes access is enabled
#ifdef OGL_KEEP_BG_ACCESS

    /**
     * @brief register a window to the instance. this function is called in the window's constructor
     * @warning this function is only usable behind the scenes
     * 
     * @param window a raw pointer to the window
     */
    void registerWindow(OGL_Window* window);

    /**
     * @brief remove a window from the instance. this function is called in the window's destructor, which is automaticaly called when an instance with bound windows is destroyed
     * @warning this function is only usable behind the scens
     * 
     * @param window a raw pointer to the window
     */
    void removeWindow(OGL_Window* window);

    /**
     * @brief remove a bindable from the instance. this function is called in the bindable's destructor, which is automaticaly called when an instance with bound windows is destroyed
     * @warning this function is only usable behind the scens
     * 
     * @param bindable a raw pointer to the bindable
     */
    void registerBindable(OGL_BindableBase* bindable);

    /**
     * @brief register a bindable to the instance. this function is called in the bindable's constructor
     * @warning this function is only usable behind the scenes
     * 
     * @param bindable a raw pointer to the bindable
     */
    void removeBindable(OGL_BindableBase* bindable);

    /**
     * @brief Get the OpenGL context of this context, or 0 if none exists
     * 
     * @return SDL_GLContext the OpenGL context
     */
    inline SDL_GLContext getContext() {return this->context;}

    /**
     * @brief handle an SDL event and pass it to the correct internal window
     * 
     * @param event the event to handle
     * 
     * @return true : the event was for a window registed to this instance
     * @return false : the event was for another instance
     */
    bool handleEvent(SDL_Event event);

#endif

    /**
     * @brief make this the current instance
     * @warning this won't bind the OpenGL-Context or any window, it will just set the internal current index to this
     */
    void makeCurrent();

    /**
     * @brief check if this is the current instance
     * 
     * @return true : this is the current instance
     * @return false : this is not the current instance
     */
    bool isCurrent();

    /**
     * @brief set the mode of VSync
     * 
     * @param useVSync say if VSync should be used
     */
    void setVSync(bool useVSync);

    /**
     * @brief get if vsync is used
     * 
     * @return true : vsync is enabled | 
     * @return false : vsync is disabled
     */
    inline bool usingVSync() {return this->vsync;}

private:
    /**
     * @brief store a vector of pointers to the windows
     */
    std::vector<OGL_Window*> windows = {};
    /**
     * @brief store a vector of pointers to the bindables
     */
    std::vector<OGL_BindableBase*> bindables = {};
    /**
     * @brief store the OpenGL instance
     */
    SDL_GLContext context = 0;
    /**
     * @brief store if VSync is enabled
     */
    bool vsync = false;
};

/**
 * @brief a base class which is used to handle the binding of the OpenGL classes to the instance. The binding is done automaticaly behind the scenes, as well as the removal. This is to ensure that all objects belong to the context and can't be used any more after the context is destroyed, to insure memory safty. 
 */
class OGL_BindableBase
{
public:
    /**
     * @brief Constructor for a bindalbe base. This registers the object to the instance, if no instance exists it will throw a runtime error
     */
    OGL_BindableBase(); 

    /**
     * @brief Destructor for a bindable base. This will remove the object from the window. 
     * 
     */
    virtual ~OGL_BindableBase();

protected:
    /**
     * @brief this function should be used as the destructor of the child class to ensure memory safty and correct object destruction
     */
    virtual void onDestroy() {};
    /**
     * @brief store the instance the object is bound to
     */
    OGL_Instance* instance = 0;
};

/**
 * @brief store a window which can render with OpenGL. The window dose not contain the context
 */
class OGL_Window
{
public:
    /**
     * @brief Construct a new ogl window
     */
    OGL_Window() = default;

    /**
     * @brief Construct a new ogl window
     * 
     * @param name the name of the window
     * @param width the width of the winow in pixels, default is 1000
     * @param height the height of the window in pixels, default is 1000
     * @param x the x position of the window relative to the top left corner in pixels, default is 0
     * @param y the y position of the window relative to the top left corner in pixels, default is 0
     */
    OGL_Window(const char* name, uint32_t width = 1000, uint32_t height = 1000, uint32_t x = 0, uint32_t y = 0);

    /**
     * @brief Destroy the ogl window
     */
    ~OGL_Window();

    /**
     * @brief make this window with it's registerd instance the current window
     */
    void makeCurrent();

//check if behind-the-scenes access is enabled
#ifdef OGL_KEEP_BG_ACCESS

    /**
     * @brief Get the SDL_Window pointer from the window
     * 
     * @return SDL_Window* a pointer to the SDL window
     */
    inline SDL_Window* getWindow() {return this->window;}

    /**
     * @brief handle an SDL event
     * 
     * @param event the event to handle
     * 
     * @return true : the event was for this window
     * @return false : the event was for another window
     */
    bool handleEvent(SDL_Event event);

#endif

    /**
     * @brief get if the window should close
     * 
     * @return true : the window should close | 
     * @return false : the window can stay open
     */
    inline bool isClosingRequested() {return this->shouldClose;}

    /**
     * @brief Set the Clear Color for the window
     * 
     * @param r the red component of the clear color. Like OpenGL in range from 0.f to 1.f
     * @param g the green component of the clear color. Like OpenGL in range from 0.f to 1.f
     * @param b the blue component of the clear color. Like OpenGL in range from 0.f to 1.f
     * @param a the alpha component of the clear color. Like OpenGL in range from 0.f to 1.f
     */
    void setClearColor(float r, float g, float b, float a);

    /**
     * @brief Get the red component of the clear color
     * 
     * @return float the red component of the clear color
     */
    inline float getClearR() {return this->clearR;}

    /**
     * @brief Get the green component of the clear color
     * 
     * @return float the green component of the clear color
     */
    inline float getClearG() {return this->clearG;}

    /**
     * @brief Get the blue component of the clear color
     * 
     * @return float the blue component of the clear color
     */
    inline float getClearB() {return this->clearB;}

    /**
     * @brief Get the alpha component of the clear color
     * 
     * @return float the alpha component of the clear color
     */
    inline float getClearA() {return this->clearA;}

    /**
     * @brief Set the size of the window
     * 
     * @param width the width of the window in pixels
     * @param height the height of the window in pixels
     */
    void setSize(uint32_t width, uint32_t height);

    /**
     * @brief Get the width of the window in pixels
     * 
     * @return uint32_t the window of the window in pixels
     */
    inline uint32_t getWidth() {return this->width;}

    /**
     * @brief Get the height of the window in pixels
     * 
     * @return uint32_t the height of the window in pixels
     */
    inline uint32_t getHeight() {return this->height;}

    /**
     * @brief clear the window. By default, all masks are set. 
     * 
     * @param clearColor if this is true, the color buffer of the window will be cleared with the clear color. 
     * @param clearDepth if this is true, the depth buffer will be cleard. 
     * @param clearStencil if this is true, the stencil buffer of the window will be cleard.
     */
    void clear(bool clearColor = true, bool clearDepth = true, bool clearStencil = true);

    /**
     * @brief update the window
     */
    void flip();

    /**
     * @brief Set a function to call if the window is resized
     * 
     * @param func the function to call
     * @param userData some abituary data that can be passed into the function
     */
    void setOnResizeHook(void (*func)(uint32_t, uint32_t, void*), void* userData = 0);

    /**
     * @brief Get the function the window calls if the window is resized
     */
    inline void (*getOnResizeHook())(uint32_t,uint32_t, void*) {return this->resizeFunc;}

private:
    /**
     * @brief store a pointer to the instance
     */
    OGL_Instance* instance = 0;
    /**
     * @brief store the window pointer
     */
    SDL_Window* window = 0;
    /**
     * @brief store if the window should close
     */
    bool shouldClose = false;
    /**
     * @brief store the clear color's r component
     */
    float clearR = 0.f;
    /**
     * @brief store the clear color's g component
     */
    float clearG = 0.f;
    /**
     * @brief store the clear color's b component
     */
    float clearB = 0.f;
    /**
     * @brief store the clear color's a component
     */
    float clearA = 0.f;

    /**
     * @brief store the x-position of the window in pixels relative to the top left corner of the screen
     */
    uint32_t posX = 0;
    /**
     * @brief store the y-position of the window in pixels relative to the top left corner of the screen
     */
    uint32_t posY = 0;

    /**
     * @brief store the widht of the window in pixels
     */
    uint32_t width = 0;
    /**
     * @brief store the height of the window in pixels
     */
    uint32_t height = 0;
    /**
     * @brief store the name of the window
     */
    const char* name;
    /**
     * @brief store a function that should be called if the window is resized
     */
    void (*resizeFunc)(uint32_t, uint32_t, void*) = 0;
    /**
     * @brief store some abituary data for the user that can be passed into the resize function
     */
    void* resizeFuncUserData = 0;
};

//a macro to ensure that the correct window is bound
#define correctInstanceBinding() if (oglGetCurrentInstance() != this->instance) {this->instance->makeCurrent();}

/**
 * @brief handle vertex buffers in a class-wraped fassion
 * 
 * @tparam T the type of a vertex, can be anything. 
 */
template<typename T> class OGL_VertexBuffer : OGL_BindableBase
{
public:
    /**
     * @brief Construct a new ogl vertex buffer
     */
    OGL_VertexBuffer() = default;

    /**
     * @brief Construct a new ogl vertex buffer
     * 
     * @param vertices the vertices 
     */
    OGL_VertexBuffer(std::vector<T> vertices)
    {
        //store the vertices
        this->vertices = vertices;
        //create the VB
        glGenBuffers(1, &this->buffer);
        //upload to the GPU
        this->uploadToGPU();
    }

    /**
     * @brief add a vertex ad the end of the vertices
     * 
     * @param vertex the vertex to add
     */
    void addVertex(T vertex)
    {
        //add a vertex
        this->vertices.push_back(vertex);
    }

    /**
     * @brief Set the Vertex at the specified position
     * @warning indexing out of bounds resoults in undefined behaviour. This dose not add a vertex, but changes an existing one. 
     * 
     * @param index the index to set the vertex to
     * @param vertex the vertex to set
     */
    void setVertex(size_t index, T vertex)
    {
        //update the vertex
        this->vertices[index] = vertex;
    }

    /**
     * @brief Get the vertex at the specified index
     * @warning indexing out of bounds resoults in undefined behaviour. This dose not add a vertex, but changes an existing one. 
     * 
     * @param index the index to get the vertex from
     * @return T the vertex at the specified index
     */
    T getVertex(size_t index)
    {
        //return the vertex at the specified location
        return this->vertices[index];
    }

    /**
     * @brief remove the vertex at the specified position
     * @warning indexing out of bounds resoults in undefined behaviour. This dose not add a vertex, but changes an existing one. 
     * 
     * @param index the index to delete the vertex from
     */
    void deleteVertex(size_t index)
    {
        //remove the index at the specified position
        this->vertices.erase(this->vertices.begin() + index);
    }

    /**
     * @brief access the element at the specified index
     * @warning indexing out of bounds resoults in undefined behaviour. This dose not add a vertex, but changes an existing one. 
     * 
     * @param index the index to access the element at
     * @return T& a reference to the element
     */
    T& operator[](size_t index)
    {
        //return a reference to the element
        return this->vertices[index];
    }

    /**
     * @brief Get a pointer to all the vertices
     * 
     * @return std::vector<T>* a pointer to the vertices
     */
    std::vector<T>* getVertices()
    {
        //return a pointer to the vertices
        return &this->vertices;
    }

    /**
     * @brief bind the vertex buffer object
     */
    void bind()
    {
        //bind the correct window / instance
        correctInstanceBinding()

        //bind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
    }

    /**
     * @brief unbind any vertex buffer
     */
    void bindNone()
    {
        //bind the correct window / instance
        correctInstanceBinding()
        //bind buffer 0, which means none
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /**
     * @brief upload the data of the element to the GPU
     */
    void uploadToGPU()
    {
        //bind the correct window / instance
        correctInstanceBinding()
        //bind the buffer
        glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
        //buffer the whole vertices
        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * this->vertices.size(), this->vertices.data(), GL_STATIC_DRAW);
    }

private:
    /**
     * @brief this function is responsible for deleting the object
     */
    virtual void onDestroy() override
    {
        //delete the vertex buffer
        glDeleteBuffers(1, &this->buffer);
        //set the buffer to 0
        this->buffer = 0;
        //free the vertices
        this->vertices.clear();
        //bind no buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /**
     * @brief store the vertices
     */
    std::vector<T> vertices;
    /**
     * @brief store the OpenGL buffer
     */
    GLuint buffer = 0;
};

/**
 * @brief store the information about a part of an vertex
 */
struct OGL_VertexAttribute
{
    //store the vertex size
    uint8_t size = 4;
    //store the type of the data
    OGL_Type type = OGL_TYPE_FLOAT;
    //store if the data should be normalized
    bool normalize = false;
};

/**
 * @brief handle the definition of how a vertex looks
 */
class OGL_VertexAttributes : OGL_BindableBase
{
public:

    /**
     * @brief Construct a new structure to describe the structure of the vertices
     */
    OGL_VertexAttributes() = default;

    /**
     * @brief Construct a new structure to describe the structure of the vertices
     * 
     * @param attributes the attributes of the vertices
     * @param vertexSize the size of a single vertex
     */
    OGL_VertexAttributes(std::vector<OGL_VertexAttribute> attributes, size_t vertexSize);

    /**
     * @brief change the structure of the vertices
     * 
     * @param attributes the new attributes for the structure of the vertices
     * @param vertexSize the size of a single vertex
     */
    void updateStructure(std::vector<OGL_VertexAttribute> attributes, size_t vertexSize);

    /**
     * @brief only re-upload the structure of the VAO, don't set a new structure
     */
    void update();

    /**
     * @brief change and upload the structure of a single attribute of the vertex
     * 
     * @param idx the index to change the data at. If the index is out of range, it will be put to the end of the list
     * @return OGL_VertexAttribute& a reference to the requested attribute. If it changes, the data will be re-uploaded. 
     */
    OGL_VertexAttribute& operator[](size_t idx);

    /**
     * @brief bind this as the structure for the current vertices
     */
    void bind();

private:
    /**
     * @brief clean up the object
     */
    virtual void onDestroy() override;

    /**
     * @brief store the attributes for the vertices
     */
    std::vector<OGL_VertexAttribute> attributes;
    /**
     * @brief store the size of a vertex
     */
    size_t vertSize = 0;
    /**
     * @brief store the VAO
     */
    GLuint vao = 0;
};

/**
 * @brief handle an element array buffer, also called index buffer, for OpenGL
 */
class OGL_IndexBuffer : OGL_BindableBase
{
public:
    /**
     * @brief Construct a new index buffer
     */
    OGL_IndexBuffer() = default;

    /**
     * @brief Construct a new index buffer
     * 
     * @param indices the indices for the buffer
     */
    OGL_IndexBuffer(std::vector<uint32_t> indices);

    /**
     * @brief change the data stored in the buffer
     * 
     * @param indices the new indices for the buffer
     */
    inline void updateData(std::vector<uint32_t> indices) {this->indices = indices;}

    /**
     * @brief upload the buffer to the index buffer on the GPU
     */
    void upload();

    /**
     * @brief add an element to the buffer
     * 
     * @param index the index to add
     */
    inline void addElement(uint32_t index) {this->indices.push_back(index);}

    /**
     * @brief Set an element in the buffer to a new value
     * 
     * @param index the new value
     * @param idx the index to change
     */
    void setElement(uint32_t index, size_t idx);

    /**
     * @brief Get a specific element from the buffer
     * 
     * @param idx the index to access
     * @return uint32_t the value of the element in the buffer
     */
    uint32_t getElement(size_t idx);

    /**
     * @brief Get a pointer to an specific element from the buffer
     * 
     * @param idx the index to access
     * @return uint32_t* a pointer to the element
     */
    uint32_t* getElementPtr(size_t idx);

    /**
     * @brief access a reference of a specific element with the index operator
     * 
     * @param idx the index to access
     * @return uint32_t& a reference to the element (a REFERENCE, not a POINTER)
     */
    uint32_t& operator[](size_t idx);

    /**
     * @brief Get the values of all indices
     * 
     * @return std::vector<uint32_t> a vector of all values
     */
    inline std::vector<uint32_t> getIndices() {return this->indices;}

    /**
     * @brief Get a pointer to all the indices
     * 
     * @return std::vector<uint32_t>* a pointer to the vector containing the indices
     */
    inline std::vector<uint32_t>* getIndicesPtr() {return &this->indices;}

    /**
     * @brief bind this as the current element array buffer
     */
    void bind();

    /**
     * @brief bind this as the current element array buffer
     */
    void unbind();

    /**
     * @brief Get the amount of stored indices
     * 
     * @return size_t the amount of indices
     */
    inline size_t getIndexCount() {return this->indices.size();}

private:
    /**
     * @brief handle the destruction of the buffer
     */
    virtual void onDestroy() override;

    /**
     * @brief store the index data for the buffer
     */
    std::vector<uint32_t> indices;
    /**
     * @brief store the index buffer
     */
    GLuint ibo = 0;
};

/**
 * @brief store the information about a single uniform
 */
class OGL_UniformInfo
{
public:

    /**
     * @brief Construct a new uniform information structure
     */
    OGL_UniformInfo() = default;

    /**
     * @brief set the data contained in the info to a specific value
     * 
     * @tparam T the type of the data
     * @param value the value to set to
     */
    template<typename T> void set(T value)
    {
        //check if the type fits
        if (sizeof(T) > 64)
        {
            //if not, throw an error
            std::__throw_runtime_error("The type of the value tried to set as uniform is bigger as 64, wich is the maximum size of a uniform");
            return;
        }
        //if not, copy the data
        memcpy(data, &value, sizeof(T));
    }

    /**
     * @brief get the data stored in the uniform info
     * 
     * @tparam T the type of the data
     * @return T the stored data
     */
    template<typename T> inline T get()
    {
        //return the re-interpreted data
        return *((T*)((void*)data));
    }

    template<typename T> inline T& getRef()
    {
        //retuern a reference to the re-interpreted data
        return (*((T*)((void*)data)));
    }

    /**
     * @brief store the name of the uniform
     */
    std::string name;
    /**
     * @brief store the type
     */
    OGL_Type type;
    /**
     * @brief store the raw data of the contained value
     */
    uint8_t data[sizeof(float)*16] = {};
    /**
     * @brief store the location of the uniform
     */
    GLint location = -1;
};

/**
 * @brief utility function to create an uniform info and initalise it
 * 
 * @tparam T the type for the value
 * @param name the name of the uniform
 * @param value the value of the uniform
 * @param type the base type for OpenGL
 * @return OGL_UniformInfo the finished uniform information
 */
template<typename T> OGL_UniformInfo oglCreateUniformInfo(std::string name, T value, OGL_Type type)
{
    //crate a new info
    OGL_UniformInfo info;
    //store the name
    info.name = name;
    //store the type identifyer
    info.type = type;
    //set the data
    info.set<T>(value);
    //return the info
    return info;
}

/**
 * @brief handle all functions neceserry for shaders
 */
class OGL_Shader : OGL_BindableBase
{
public:

    /**
     * @brief Construct a new shader
     */
    OGL_Shader() = default;

    /**
     * @brief Construct a new shader
     * 
     * @param vs the vertex shader information, in the format specified by type
     * @param fs the fragment shader information, in the format specified by type
     * @param type the type the data is given in, can be inputed as file, source or binary
     */
    OGL_Shader(std::string vs, std::string fs, OGL_ShaderInput type = OGL_SHADER_INPUT_FILE);

    /**
     * @brief change this shader to a new shader
     * 
     * @param vs the vertex shader information, in the format specified by type
     * @param fs the fragment shader information, in the format specified by type
     * @param type the type the data is given in, can be inputed as file, source or binary
     */
    void recompileShader(std::string vs, std::string fs, OGL_ShaderInput type = OGL_SHADER_INPUT_FILE);

    /**
     * @brief Get the Compiled Shader on the GPU
     * 
     * @return uint32_t the compiled OpenGL shader on the GPU
     */
    inline uint32_t getCompiledShader() {return this->shader;}

    /**
     * @brief bind the shader
     */
    void bind();

    /**
     * @brief unbind the shader
     */
    void unbind();

    /**
     * @brief Set the uniforms for the buffer
     * 
     * @param uniforms the uniforms for the shader
     */
    inline void setUniforms(std::vector<OGL_UniformInfo> uniforms) {this->uniforms = uniforms;}

    /**
     * @brief Get a pointer to all stored uniforms
     * 
     * @return std::vector<OGL_UniformInfo>* a pointer to the internal vector of uniforms
     */
    inline std::vector<OGL_UniformInfo>* getUniformPtr() {return &this->uniforms;}

    /**
     * @brief Get the amount of stored uniforms
     * 
     * @return size_t the amount of uniforms
     */
    inline size_t getUniformCount() {return this->uniforms.size();}

    /**
     * @brief get a specific uniform from the shader. If it dosn't exist, create it
     * 
     * @param name the name of the uniform
     * @return OGL_UniformInfo& a reference to the uniform
     */
    OGL_UniformInfo& operator[](std::string name);

    /**
     * @brief get a specific uniform from the shader. If it dosn't exist, throw an error
     * 
     * @param index the index of the 
     * @return OGL_UniformInfo& 
     */
    OGL_UniformInfo& operator[](size_t index);

    /**
     * @brief update the positions of all uniforms
     */
    void recalculateUniforms();

private:
    /**
     * @brief clean up the object after destruction
     */
    virtual void onDestroy() override;

    /**
     * @brief store the OpenGL shader
     */
    GLuint shader = 0;
    /**
     * @brief store information about all the uniforms
     */
    std::vector<OGL_UniformInfo> uniforms;
};

/**
 * @brief handle uniform buffers to transfear read-only data to a shader
 */
class OGL_UniformBuffer : OGL_BindableBase
{
public:

    /**
     * @brief Construct a new uniform buffer
     */
    OGL_UniformBuffer() = default;

    /**
     * @brief Construct a new uniform buffer
     * 
     * @param data the data for the buffer, or 0 to clear the data
     * @param size the size of the data in bytes, or 0 to clear the data
     * @warning if the size of the data is bigger than the actual data this function can throw a segmentation fault
     */
    OGL_UniformBuffer(void* data, size_t size);

    /**
     * @brief change the data that is stored in the buffer
     * 
     * @param data the data for the buffer, or 0 to clear the data
     * @param size the size of the data in bytes, or 0 to clear the data
     * @warning if the size of the data is bigger than the actual data this function can throw a segmentation fault 
     */
    void updateData(void* data, size_t size);

    /**
     * @brief add an element to the buffer
     * 
     * @tparam T the type of the new element
     * @param element the element to add
     */
    template<typename T> void addElement(T element)
    {
        //store the element index of the data
        size_t i = this->data.size();
        //add the size of T elements
        for (size_t i = 0; i < sizeof(T); ++i) {this->data.push_back(0);}
        //copy the data
        memcpy(this->data.data() + i, &element, sizeof(T));
    }

    /**
     * @brief set an element in the element data
     * 
     * @tparam T the type of the element
     * @param element the element to store
     * @param index the index of the element in bytes
     * @warning if the index + the elment size is smaller than the size of the data, a runtime error is thrown
     */
    template<typename T> void setElement(T element, size_t index)
    {
        //check if the elment is in bounds
        if ((index + sizeof(T)) > this->data.size())
        {
            //if not, throw an error
            std::__throw_runtime_error("The requested element is not in bounds of the uniform buffer");
            return;
        }
        //modify the element at the requested index
        memcpy(this->data.data() + index, &element, sizeof(T));
    }

    /**
     * @brief Get the Element at an specific index
     * 
     * @tparam T the type of the elment
     * @param index the index to get the element from the data in bytes
     * @return T the requested element
     * @warning if the index + the elment size is smaller than the size of the data, a runtime error is thrown
     */
    template<typename T> T getElement(size_t index)
    {
        //check if the elment is in bounds
        if ((index + sizeof(T)) > this->data.size())
        {
            //if not, throw an error
            std::__throw_runtime_error("The requested element is not in bounds of the uniform buffer");
            return 0;
        }
        //return the requested data
        return *((T*)((void*)this->data.data()));
    }

    /**
     * @brief Get a pointer to an specific element
     * 
     * @tparam T the type of the object to get
     * @param idx the index of the requested element in bytes
     * @return T* a pointer to the requested element or NULL if the elment is out of bounds
     * @warning if the index + the elment size is smaller than the size of the data, a runtime error is thrown
     */
    template<typename T> T* getRefElement(size_t idx)
    {
        //check if the elment is in bounds
        if ((idx+sizeof(T)) > this->data.size())
        {
            //if not, throw an error
            std::__throw_runtime_error("The requested element is not in bounds of the uniform buffer");
            return 0;
        }

        //return the requested data
        return ((T*)(this->data.data() + idx));
    }

    /**
     * @brief upload the stored data to the GPU
     */
    void upload();

    /**
     * @brief bind this buffer for use in a shader
     * 
     * @param unit the unit to bind to
     */
    void bind(uint8_t unit);

    /**
     * @brief unbind all buffers
     */
    void unbind();

    /**
     * @brief clear all data stored in the buffer
     */
    inline void clear() {this->data.clear();}

    /**
     * @brief Get a reference to the own data
     * 
     * @return std::vector<uint8_t>* a pointer to the stored data
     */
    inline std::vector<uint8_t>* getDataRef() {return &this->data;}

    /**
     * @brief Get the data stored in the buffer
     * 
     * @return std::vector<uint8_t> a vector of the data, structured in bytes
     */
    inline std::vector<uint8_t> getData() {return this->data;}

private:
    /**
     * @brief handle the destruction of the object
     */
    virtual void onDestroy() override;

    /**
     * @brief store the data of the buffer
     */
    std::vector<uint8_t> data;
    /**
     * @brief store the buffer
     */
    GLuint ubo = 0;

};

/**
 * @brief handle shader storage buffers to transfear read / write data to the GPU and read it back
 */
class OGL_ShaderStorageBuffer : OGL_BindableBase
{
public:

    /**
     * @brief Construct a new shader storage buffer
     * 
     */
    OGL_ShaderStorageBuffer() = default;

    /**
     * @brief Construct a new shader storage buffer
     * 
     * @param data the data for the buffer, or 0 to clear the data
     * @param size the size of the data in bytes, or 0 to clear the data
     * @warning if the size of the data is bigger than the actual data this function can throw a segmentation fault 
     */
    OGL_ShaderStorageBuffer(void* data, size_t size);

    /**
     * @brief change the data that is stored in the buffer
     * 
     * @param data the data for the buffer, or 0 to clear the data
     * @param size the size of the data in bytes, or 0 to clear the data
     * @warning if the size of the data is bigger than the actual data this function can throw a segmentation fault 
     */
    void updateData(void* data, size_t size);

    /**
     * @brief clear all data stored in the buffer
     */
    inline void clear() {this->data.clear();}

    /**
     * @brief add an element to the buffer
     * 
     * @tparam T the type of the new element
     * @param element the element to add
     */
    template<typename T> void addElement(T element)
    {
        //store the element index of the data
        size_t i = this->data.size();
        //add the size of T elements
        for (size_t i = 0; i < sizeof(T); ++i) {this->data.push_back(0);}
        //copy the data
        memcpy(this->data.data() + i, &element, sizeof(T));
    }

    /**
     * @brief set an element in the element data
     * 
     * @tparam T the type of the element
     * @param element the element to store
     * @param index the index of the element in bytes
     * @warning if the index + the elment size is smaller than the size of the data, a runtime error is thrown
     */
    template<typename T> void setElement(T element, size_t index)
    {
        //check if the elment is in bounds
        if ((index + sizeof(T)) > this->data.size())
        {
            //if not, throw an error
            std::__throw_runtime_error("The requested element is not in bounds of the uniform buffer");
            return;
        }
        //modify the element at the requested index
        memcpy(this->data.data() + index, &element, sizeof(T));
    }

    /**
     * @brief Get the Element at an specific index
     * 
     * @tparam T the type of the elment
     * @param index the index to get the element from the data in bytes
     * @return T the requested element
     * @warning if the index + the elment size is smaller than the size of the data, a runtime error is thrown
     */
    template<typename T> T getElement(size_t index)
    {
        //check if the elment is in bounds
        if ((index + sizeof(T)) > this->data.size())
        {
            //if not, throw an error
            std::__throw_runtime_error("The requested element is not in bounds of the uniform buffer");
            return 0;
        }
        //return the requested data
        return *((T*)((void*)this->data.data()));
    }

    /**
     * @brief Get a pointer to an specific element
     * 
     * @tparam T the type of the object to get
     * @param idx the index of the requested element in bytes
     * @return T* a pointer to the requested element or NULL if the elment is out of bounds
     * @warning if the index + the elment size is smaller than the size of the data, a runtime error is thrown
     */
    template<typename T> T* getRefElement(size_t idx)
    {
        //check if the elment is in bounds
        if ((idx+sizeof(T)) > this->data.size())
        {
            //if not, throw an error
            std::__throw_runtime_error("The requested element is not in bounds of the uniform buffer");
            return 0;
        }

        //return the requested data
        return ((T*)(this->data.data() + idx));
    }

    /**
     * @brief upload the stored data to the GPU
     */
    void upload();

    /**
     * @brief donalod the data from the GPU and store it
     */
    void download();

    /**
     * @brief bind this buffer for use in a shader
     * 
     * @param unit the unit to bind to
     */
    void bind(uint8_t unit);

    /**
     * @brief unbind all buffers
     */
    void unbind();

private:

    /**
     * @brief handle the destruction of the object
     */
    virtual void onDestroy() override;

    /**
     * @brief store the data for the buffer
     */
    std::vector<uint8_t> data;
    /**
     * @brief store the shader storage buffer object
     */
    GLuint ssbo = 0;
};

/**
 * @brief handle textures for OpenGL
 */
class OGL_Texture : OGL_BindableBase
{
public:

    /**
     * @brief Construct a new texture
     */
    OGL_Texture() = default;

    /**
     * @brief Construct a new texture
     * 
     * @param type the type of the texture
     * @param width the width of the texture
     * @param height the height of the texture
     * @param layers the amount of layers for 3D textures
     * @param internalFormat the internal format of the texture
     */
    OGL_Texture(OGL_TextureType type, uint32_t width, uint32_t height, uint32_t layers, GLenum internalFormat = GL_RGBA32F, GLenum format = GL_RGBA);

    /**
     * @brief Construct a new texture
     * 
     * @param texFile the path to the texture
     * @param internalFormat the internal format for the texture
     */
    OGL_Texture(const char* texFile, GLenum internalFormat = GL_RGBA32F);

    /**
     * @brief Construct a new texture
     * 
     * @param texFiles the texture files for the textures
     * @param type the type of the texture
     * @param internalFormat the internal format for the texture
     */
    OGL_Texture(std::vector<const char*> texFiles, OGL_TextureType type, GLenum internalFormat = GL_RGBA32F);

    /**
     * @brief Construct a new texture
     * 
     * @param data the data for the texture
     * @param width the width of the texture
     * @param height the height of the texture
     * @param format the format of the data
     * @param type the type of the data
     * @param internalFormat the internal format for the texture
     */
    OGL_Texture(void* data, uint32_t width, uint32_t height, GLenum format, GLenum type, GLenum internalFormat = GL_RGBA32F);

    /**
     * @brief update the texture
     * 
     * @param texFile the texture file to read
     * @param internalFormat the internal format of the texture
     */
    void setTexture(const char* texFile, GLenum internalFormat = GL_RGBA32F);

    /**
     * @brief update the texture
     * 
     * @param texFiles the texture files to read from
     * @param type the type of the texture
     * @param internalFormat the internal format of the texture
     */
    void setTexture(std::vector<const char*> texFiles, OGL_TextureType type, GLenum internalFormat = GL_RGBA32F);

    /**
     * @brief update the texture
     * 
     * @param data the data for the texture
     * @param width the width of the texture
     * @param height the height of the texture
     * @param format the format of the data
     * @param type the type of the data
     * @param internalFormat the internal format for the texture
     */
    void setTexture(void* data, uint32_t width, uint32_t height, GLenum format, GLenum type, GLenum internalFormat = GL_RGBA32F);

    /**
     * @brief bind the texture to a specific unit
     * 
     * @param unit the unit to unbind from
     */
    void bind(uint8_t unit);

    /**
     * @brief unbind the texture from a specific unit
     * 
     * @param unit the unit to unbind from
     */
    void unbind(uint8_t unit);

    /**
     * @brief Set any OpenGL texture parameter for the texture
     * 
     * @param parameter the parameter to set
     * @param value the value for the parameter
     */
    void setTexParameter(GLenum parameter, GLenum value);

    /**
     * @brief Create a mipmap for the texture
     */
    void createMipmap();

    /**
     * @brief Get the type of the texture
     * 
     * @return OGL_TextureType the texture type
     */
    inline OGL_TextureType getType() {return this->type;}

    /**
     * @brief Get the OpenGL texture
     * 
     * @return GLuint the OpenGL texture
     */
    inline GLuint getTexture() {return this->texture;}

private:
    /**
     * @brief delete the object
     */
    virtual void onDestroy() override;
    /**
     * @brief store the type of the texture
     */
    OGL_TextureType type;
    /**
     * @brief store the internal format depending on type
     */
    GLenum format = 0;
    /**
     * @brief store the OpenGL texture
     */
    GLuint texture = 0;
    /**
     * @brief store the width of the texture in pixels
     */
    uint32_t width = 0;
    /**
     * @brief store the height of the texture in pixels
     */
    uint32_t height = 0;
    /**
     * @brief store the amount of layers in the texture
     */
    uint32_t layers = 0;
};

/**
 * @brief store the information about a single color attachment
 */
struct OGL_FramebufferAttachment
{
    /**
     * @brief store a pointer to the texture to use
     */
    OGL_Texture* texture = 0;
    /**
     * @brief store the id of the attachment unit
     */
    uint8_t attachmentID = 0;
    /**
     * @brief store the type of attachment
     */
    OGL_FramebufferAttachmentType type = OGL_COLOR_ATTACHMENT;
    /**
     * @brief for layers and cube maps, selecte the layer to use
     */
    uint8_t layerSelect = 0;
};

/**
 * @brief handle multiple framebuffers
 */
class OGL_Framebuffer : OGL_BindableBase
{
public:

    /**
     * @brief Construct a new framebuffer
     */
    OGL_Framebuffer() = default;

    /**
     * @brief Construct a new framebuffer
     * 
     * @param attachments the attachments for the framebuffer
     */
    OGL_Framebuffer(std::vector<OGL_FramebufferAttachment> attachments);

    /**
     * @brief update all the attachments bound to the framebuffer
     */
    void updateAttachmentBindings();

    /**
     * @brief access a specific framebuffer attachment
     * @warning this operator dose not check if the element belongs to the framebuffer. If the index is out of range, this may lead to undefined behaviour
     * 
     * @param index the index of that attachment
     * @return OGL_FramebufferAttachment& a reference of the attachment
     */
    inline OGL_FramebufferAttachment& operator[](size_t index) {return this->attachments[index];}

    /**
     * @brief bind the framebuffer
     * 
     * @param target the framebuffer type to bind to
     */
    void bind(GLenum target = GL_FRAMEBUFFER);

    /**
     * @brief unbind the framebuffer
     * 
     * @param target the framebuffer type to unbind from
     */
    void unbind(GLenum target = GL_FRAMEBUFFER);

private:
    virtual void onDestroy() override;

    GLuint framebuffer;

    std::vector<OGL_FramebufferAttachment> attachments;
};

//undefine the helper macros
#undef correctInstanceBinding

#endif