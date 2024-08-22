/**
 * @file main.cpp
 * @author DM8AT
 * @brief the main file for this project. It contains a simple example for the project. 
 * @version 0.1
 * @date 2024-08-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "ObjectGL/ObjectGL.hpp"
#include <iostream>
#include <cmath>

struct Vertex
{
    float x,y,z;
    float nx,ny,nz;
    float tx,ty;
};

struct mat4 {float m[4][4];};
struct vec3 {float m[3];};

void updatePos(OGL_UniformBuffer* ubo, vec3 pos)
{
    ubo->addElement<mat4>(mat4{{
        {1,0,0,pos.m[0]},
        {0,1,0,pos.m[1]},
        {0,0,1,pos.m[2]},
        {0,0,0,1}
    }});
}

void updateRot(OGL_UniformBuffer* ubo, vec3 rot)
{
    float sx = std::sin(rot.m[0]);
    float cx = std::cos(rot.m[0]);
    float sy = std::sin(rot.m[1]);
    float cy = std::cos(rot.m[1]);
    float sz = std::sin(rot.m[2]);
    float cz = std::cos(rot.m[2]);

    ubo->addElement<mat4>(mat4{{
        {cy*cz, sx*sy*cz - cx*sz, cx*sy*cz + sx*sz, 0},
        {cy*sz, sx*sy*sz + cx*cz, cx*sy*sz - sx*cz, 0},
        {-sy, sx*cy, cx*cy, 0},
        {0,0,0,1}
    }});
}

void debugCallback(GLenum source, GLenum type, GLuint, GLenum severity, GLsizei, const GLchar *message, const void *)
{
    //store the message severity
    std::string sever = "Unknown";
    //get the severity
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        sever = "GL_DEBUG_SEVERITY_HIGH";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        sever = "GL_DEBUG_SEVERITY_MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        sever = "GL_DEBUG_SEVERITY_LOW";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        sever = "GL_DEBUG_SEVERITY_NOTIFICATION";
        //don't work with notifications
        return;
        break;
    
    default:
        break;
    }

    //store the message source
    std::string src = "Unknown";
    //get the source
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        src = "GL_DEBUG_SOURCE_API";
        break;

    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        src = "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
        break;

    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        src = "GL_DEBUG_SOURCE_SHADER_COMPILER";
        break;

    case GL_DEBUG_SOURCE_THIRD_PARTY:
        src = "GL_DEBUG_SOURCE_THIRD_PARTY";
        break;

    case GL_DEBUG_SOURCE_APPLICATION:
        src = "GL_DEBUG_SOURCE_APPLICATION";
        break;

    case GL_DEBUG_SOURCE_OTHER:
        src = "GL_DEBUG_SOURCE_OTHER";
        break;
    
    default:
        break;
    }

    //store the message type
    std::string mType = "Unknown";
    //get the message type
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        mType = "GL_DEBUG_TYPE_ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        mType = "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        mType = "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        mType = "GL_DEBUG_TYPE_PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        mType = "GL_DEBUG_TYPE_PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_MARKER:
        mType = "GL_DEBUG_TYPE_MARKER";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        mType = "GL_DEBUG_TYPE_PUSH_GROUP";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        mType = "GL_DEBUG_TYPE_POP_GROUP";
        break;
    case GL_DEBUG_TYPE_OTHER:
        mType = "GL_DEBUG_TYPE_OTHER";
        break;
    
    default:
        break;
    }

    //print the message
    std::cerr << "-----------------------------------------------------------\n";
    std::cerr << "A OpenGL debug message of severity '" << sever << "' with the type '" << mType << "' was thrown by '" << src << "'\n";
    std::cerr << "Message send with call: \n" << message << "\n";
    //if the severity is high, stop the program
    if (severity == GL_DEBUG_SEVERITY_HIGH)
    {
        //throw an error
        std::__throw_runtime_error("Closed because of OpenGL debug message of high priority");
    }
}

int main()
{
    OGL_Instance instance(4.6, OGL_PROFILE_CORE);
    OGL_Window window = OGL_Window("Hello World!");
    instance.setVSync(true);
    window.setClearColor(.5,.5,.5,1);

    OGL_VertexBuffer<Vertex> VBO = OGL_VertexBuffer<Vertex>({
        //front
        Vertex{0.5,0.5,0.5,     1,0,0,  1,1},
        Vertex{-0.5,0.5,0.5,    1,0,0,  0,1},
        Vertex{0.5,-0.5,0.5,    1,0,0,  1,0},
        Vertex{-0.5,-0.5,0.5,   1,0,0,  0,0},
        //back
        Vertex{0.5,0.5,-0.5,   -1,0,0,  0,1},
        Vertex{-0.5,0.5,-0.5,  -1,0,0,  1,1},
        Vertex{0.5,-0.5,-0.5,  -1,0,0,  0,0},
        Vertex{-0.5,-0.5,-0.5, -1,0,0,  1,0},
        //right
        Vertex{0.5,0.5,0.5,     0,0,1,  0,1},
        Vertex{0.5,0.5,-0.5,    0,0,1,  1,1},
        Vertex{0.5,-0.5,0.5,    0,0,1,  0,0},
        Vertex{0.5,-0.5,-0.5,   0,0,1,  1,0},
        //left
        Vertex{-0.5,0.5,0.5,    0,0,-1, 1,1},
        Vertex{-0.5,0.5,-0.5,   0,0,-1, 0,1},
        Vertex{-0.5,-0.5,0.5,   0,0,-1, 1,0},
        Vertex{-0.5,-0.5,-0.5,  0,0,-1, 0,0},
        //up
        Vertex{-0.5,0.5,0.5,    0,1,0,  1,1},
        Vertex{-0.5,0.5,-0.5,   0,1,0,  0,1},
        Vertex{0.5,0.5,0.5,     0,1,0,  1,0},
        Vertex{0.5,0.5,-0.5,    0,1,0,  0,0},
        //down
        Vertex{-0.5,-0.5,0.5,   0,-1,0, 1,1},
        Vertex{-0.5,-0.5,-0.5,  0,-1,0, 0,1},
        Vertex{0.5,-0.5,0.5,    0,-1,0, 1,0},
        Vertex{0.5,-0.5,-0.5,   0,-1,0, 0,0}
    });

    OGL_VertexAttributes VAO = OGL_VertexAttributes({
        OGL_VertexAttribute{3, OGL_TYPE_FLOAT, false},
        OGL_VertexAttribute{3, OGL_TYPE_FLOAT, false},
        OGL_VertexAttribute{2, OGL_TYPE_FLOAT, false}
        }, sizeof(Vertex));

    OGL_IndexBuffer IBO = OGL_IndexBuffer({
        0, 1, 2,  2, 1, 3,
        5, 4, 6,  5, 6, 7,
        8, 10,9,  9,10,11,
        12,13,14, 14,13,15,
        16,18,17, 17,18,19,
        20,21,22, 22,21,23
        });

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debugCallback, 0);

    OGL_UniformBuffer UBO(0,0);
    vec3 pos = {{0,0,-2}};
    UBO.bind(0);

    OGL_UniformBuffer Proj(0,0);
    float fovh = 60 * (M_PI/360.0);
    float atfh = 1.f/std::tan(fovh);
    float far = 10.f;
    float near = 0.1;
    Proj.addElement<mat4>(mat4{{
        {atfh,0,0,0},
        {0,atfh,0,0},
        {0,0,-(far+near)/(far-near),-(2*far*near)/(far-near)},
        {0,0,-1,0}
    }});
    Proj.upload();
    Proj.bind(1);

    OGL_Texture colorTex = OGL_Texture(OGL_TEXTURE_2D, window.getWidth(), window.getHeight(), 0, GL_RGBA32F);
    colorTex.setTexParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    colorTex.setTexParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    colorTex.setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    colorTex.setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    OGL_Texture depthTex = OGL_Texture(OGL_TEXTURE_2D, window.getWidth(), window.getHeight(), 0, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT); 
    depthTex.setTexParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    depthTex.setTexParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    depthTex.setTexParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    depthTex.setTexParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    OGL_Framebuffer framebuff = OGL_Framebuffer({
        OGL_FramebufferAttachment{&colorTex, 0, OGL_COLOR_ATTACHMENT, 0},
        OGL_FramebufferAttachment{&depthTex, 0, OGL_DEPTH_ATTACHMENT, 0}
    });

    OGL_Texture texture("src/cubeTexture.png");
    texture.setTexParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    texture.setTexParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    texture.bind(0);

    OGL_Shader shader = OGL_Shader("src/vertex.vs", "src/fragment.fs");
    shader["tex"] = oglCreateUniformInfo<int>("tex", 0, OGL_TYPE_INT);
    shader["lookup"] = oglCreateUniformInfo<int>("lookup", 1, OGL_TYPE_INT);
    shader.recalculateUniforms();
    shader.bind();

    float a = 0.f;
    float b = 0.f;
    float c = 0.f;
    while (!window.isClosingRequested())
    {
        UBO.clear();
        updatePos(&UBO, pos);
        a = std::fmod(a + 0.002f, 2.0*M_PI);
        b = std::fmod(b + 0.007f, 2.0*M_PI);
        c = std::fmod(c + 0.005f, 2.0*M_PI);
        updateRot(&UBO, vec3{{a,b,c}});
        UBO.addElement<float>(window.getHeight() / (float)window.getWidth());
        UBO.upload();

        framebuff.bind();
        oglHandleAllEvents();

        window.clear();
        glDrawElements(GL_TRIANGLES, IBO.getIndexCount(), GL_UNSIGNED_INT, 0);

        framebuff.bind(GL_READ_FRAMEBUFFER);
        framebuff.unbind(GL_DRAW_FRAMEBUFFER);
        glBlitFramebuffer(0,0,window.getWidth(), window.getHeight(), 0,0,window.getWidth(), window.getHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
        window.flip();
    }
}