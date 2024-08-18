This file describes the guidelines to follow when you want to contribute to this project. If you do not follow this guidelines, your changes will not be approved.
# Coding style
The coding style applies to all code-related files, like headers and source files. 
## File structure
### General file structure
All files related to the library need to be stored in the folder "src/ObjectGL". Any test-code or assets are stored in "src" directly. You should not create sub-directorys in the directory "src/ObjectGL". 
### Adding functionality and source files
If you add a source file to the library, you need to add the line to compile it into the library in the makefile. You can simply copy an allready existing line and change the parameters to the needs of your file. You should compile all source files into seperate object files, before they are all linked together to a static compiled libaray, which is linked to all files working with the library. 
If you want to add a new class, struct or enum, use the allready existing headers. The main header file is called "ObjectGL.hpp", the behind-the-scenes header "ObjectGLGlobalState.hpp". It's purpos is to manage the behind-the-scene state of the library. This structure is due to the OpenGL-Structure, which is a state machine. If you need a structure that is only library-intern, declare it there. 
### Code seperation
You should define the structure of structs and classes in the header files and implement theyre functionality in source files. This rule does not apply for inline methodes, which usualy are set and get methodes, and templates for all types, which can not be pre-compiled and need to be written in the header. 
### File naming convention
If you add a soure file to the library, it should start with "OGL_". This simbolises that it is a file which belongs to the library. After that, it should be followed by the class name it implements. The file ending for source files is allways ".cpp". 
## Comments
You need to comment your code. Code without comments can not be understood by other people and thus can not be maintaind, so it won't be approved. You do not need to comment every line of code, as that can lead to a lot of redundent comments, but you should comment at leas the major funktional paragraphs and describe what you are doing. Every function, besides inline set and get functions, needs comments. Single-Line comments start with two slashes and then are directly followed by the comment. No space between the slashes and the text so the comment style is the same over all files. An example could look like this: 
```cpp
int foo(int a)
{
    //add one to the integer and return
    return a+1;
}
```
Every funktion, class and structure needs to be commented in the doxygen comment style. All parameters must be explained, as well as the return value and what the function is doing in brief words. If the function dose not check memory, it is recomended to warn about that. An example function could look like this:
```cpp
/**
 * @brief add one to the inputed integer pointer
 * @warning the function dose not check if a is valid
 * 
 * @param a a pointer to the integer to increment
 */
void foo(int* a)
{
    //add one to the integer pointer
    (*a)+=1;
}
```
## Use of pointers
The use of pointers is discurraged as much as possible, but it is some times nessesery. If you need to use a pointer, only use raw pointers. The implicit destruction of other pointer types can make the debugging of the library dificult. If a pointer is deleted, the storing value should be set to 0 to ensure that it is clear that the memory was freed. If the pointer it self gets deleted (like in a destructor of an class) it is allowed to just ignore it. But if possible, always prefer to use no pointers and allocate on the stack whenever possible. The functionality of std::vector's is encouraged when you need an resizable array. 
## Lifetime
All objects should be destroyed when the instance contining them gets destroyed. There is allready a class implemented to make sure that the element is destroyed when the reference is deleted. It is called "OGL_BindableBase". To use it, simply inherit from the class. More information is given in [this chapter](#adding-your-own-class).
If you want to manage lifetime your self, just make sure that it is deleted if the instance is deleted. To manualy add this, you need to add your on register and remove functions to the instance. THIS WAY IS HARDLY DISCURRAGED!
# Adding features
This chapter is about adding simple features like functions, defines or structs. If you want to learn about classes, you should see [this chapter](#adding-your-own-class). 
Defines and structs should always be declared at the top of the main header file, functions at the top-most position where all dependend classes exist. Defines should be declared aboe structs or enums. The struct should be defined above a class, if it is used to manage information about the class, like uniforms for shaders. Structs also should be handled like C-Structs, so do not add functions to structs. The only exeptions are constructors and destructors, which are allowed to simplify the creation of struct instances. 
# Adding your own class
To add your own class, go into the main header, navigate to the bottom (but still above the endif directive) and add your class there. This way you get access to all other classes in the file. If you want to manage some OpenGL object, you should inherite from the "OGL_BindableBase" class to automaticaly manage the lifetime corresponding to the lifetime of the OpenGL context and to automaticaly get the correct context. You should inherite privatly if you do inherite. To correctly destroy the object, override the "onDestroy" methode of the base class. An example can be seen below. 
The class declaration should always start with the public part, which contains all public mehtodes. That is followed with the protected part, which contains all protected functions and variables. The last part is the private part. The use of public variables is heavely discouraged and should be mitigated whenever possible. 
The class needs at least one constructor with no parameters. If you do nothing within and simply set variable defaults, define the defaults an use the default keyword after the constructor. A class also needs to contain an destructor, it is ok if it dose nothing, but it must exist. 
The name of your class should always start with "OGL_" to symbolise that it is a class belonging to the library. The library uses name prefixing instead of namspaces to keep it more inline with SDL2 and OpenGL, so please don't add namespaces. 
An example class could look like this:
```cpp
/**
 * @brief an example class to show the class structur
 */
class OGL_Foo
{
public:
    /**
     * @brief create a new instance of foo
     */
    OGL_Foo() = default;

    /**
     * @brief create a new instance of foo and set the a variable
     * 
     * @param a the new value for a
     */
    OGL_Foo(int a);

    /**
     * @brief destroy this instance of foo
     */
    ~OGL_Foo();

    /**
     * @brief set a new value for a and update the old a value
     * 
     * @param a the new value for a
     */
    void setA(int a);

    /**
     * @brief get the current value of a
     * 
     * @return int the current value of a
     */
    inline int getA() {return this->a;}

    /**
     * @brief get the old value of a
     * 
     * @return int the old value of a
     */
    inline int getOldA() {return this->oldA;}

protected:
    /**
     * @brief store the current value of a. It is inherited classes allowed to change this. 
     */
    int a = 0;
private:
    /**
     * @brief store the old value of a. Inherited classes can not change this value. 
     */
    int oldA = 0;
}
```
# Adding librarys
If you do realy nececerry need a new library, you need to write an issue and describe in detail what the library dose and why a this can't be done by allready included librarys. Please do not add a tone of librarys and try to use the once allready needed for the library. The same goes for changing the C++ version used. 
If you do include a new library from the C++ default library (stdlib) or the C default library, that is ok and dose not need to be specified because the C++ and C default librarys are library collections allready in use of the main library and these should always exist. 