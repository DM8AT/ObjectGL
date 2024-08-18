CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:= -lGL -lGLEW -lSDL2 -lglfw -lSDL2_image
EXECUTABLE	:= main

OBGL_DIR := $(SRC)/ObjectGL
OBJ_DIR := $(BIN)

OBJGL_FLAGS := 

OBJGL_OBJ := $(OBJ_DIR)/OGL_Instance.o $(OBJ_DIR)/OGL_Window.o $(OBJ_DIR)/OGL_BaseState.o $(OBJ_DIR)/OGL_BindableBase.o $(OBJ_DIR)/OGL_BaseFunctions.o $(OBJ_DIR)/OGL_Shader.o $(OBJ_DIR)/OGL_VertexAttributes.o $(OBJ_DIR)/OGL_UniformBuffer.o $(OBJ_DIR)/OGL_ShaderStorageBuffer.o $(OBJ_DIR)/OGL_IndexBuffer.o $(OBJ_DIR)/OGL_Texture.o
OBJGL_FIL := $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/ObjectGLGlobalState.hpp $(OBGL_DIR)/OGL_Instance.cpp $(OBGL_DIR)/OGL_Window.cpp $(OBGL_DIR)/OGL_BaseState.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/OGL_BaseFunctions.cpp $(OBGL_DIR)/OGL_Shader.cpp $(OBGL_DIR)/OGL_VertexAttributes.cpp $(OBGL_DIR)/OGL_UniformBuffer.cpp $(OBGL_DIR)/OGL_ShaderStorageBuffer.cpp $(OBGL_DIR)/OGL_IndexBuffer.cpp $(OBGL_DIR)/OGL_Texture.cpp

all: $(BIN)/$(EXECUTABLE)

bin/main.o : $(SRC)/main.cpp $(BIN)/libObjectGL
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

libObjectGL: $(OBJGL_OBJ)
	-ar rcs libObjectGL $(OBJGL_OBJ)

$(BIN)/libObjectGL: $(OBJGL_OBJ)
	-ar rcs $(BIN)/libObjectGL $(OBJGL_OBJ)

$(OBJ_DIR)/OGL_Instance.o: $(OBGL_DIR)/OGL_Instance.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/ObjectGLGlobalState.hpp $(OBGL_DIR)/OGL_BaseState.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/OGL_Window.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_Window.o: $(OBGL_DIR)/OGL_Window.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/ObjectGLGlobalState.hpp $(OBGL_DIR)/OGL_BaseState.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/OGL_Instance.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_BaseFunctions.o: $(OBGL_DIR)/OGL_BaseFunctions.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/ObjectGLGlobalState.hpp $(OBGL_DIR)/OGL_BaseState.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/OGL_Window.cpp $(OBGL_DIR)/OGL_Instance.cpp 
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_BindableBase.o: $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/ObjectGLGlobalState.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_BaseState.o: $(OBGL_DIR)/OGL_BaseState.cpp $(OBGL_DIR)/ObjectGLGlobalState.hpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_Shader.o: $(OBGL_DIR)/OGL_Shader.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/ObjectGLGlobalState.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_VertexAttributes.o: $(OBGL_DIR)/OGL_VertexAttributes.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/ObjectGLGlobalState.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_UniformBuffer.o: $(OBGL_DIR)/OGL_UniformBuffer.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/ObjectGLGlobalState.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_ShaderStorageBuffer.o: $(OBGL_DIR)/OGL_ShaderStorageBuffer.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/ObjectGLGlobalState.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_IndexBuffer.o: $(OBGL_DIR)/OGL_IndexBuffer.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/ObjectGLGlobalState.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_Texture.o: $(OBGL_DIR)/OGL_Texture.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/ObjectGLGlobalState.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): bin/main.o $(BIN)/libObjectGL
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $(BIN)/$(EXECUTABLE) $(LIBRARIES)

clean:
	-rm $(BIN)/*
