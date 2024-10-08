CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:= -lGL -lGLEW -lSDL2 -lSDL2_image
EXECUTABLE	:= main

OBGL_DIR := $(SRC)/ObjectGL
OBJ_DIR := $(BIN)

OBJGL_FLAGS :=  

CREATE_BIN := mkdir -p bin

OBJGL_OBJ := $(OBJ_DIR)/OGL_Instance.o $(OBJ_DIR)/OGL_Window.o $(OBJ_DIR)/OGL_BaseState.o $(OBJ_DIR)/OGL_BindableBase.o $(OBJ_DIR)/OGL_BaseFunctions.o $(OBJ_DIR)/OGL_Shader.o $(OBJ_DIR)/OGL_VertexAttributes.o $(OBJ_DIR)/OGL_UniformBuffer.o $(OBJ_DIR)/OGL_ShaderStorageBuffer.o $(OBJ_DIR)/OGL_IndexBuffer.o $(OBJ_DIR)/OGL_Texture.o $(OBJ_DIR)/OGL_Framebuffer.o $(OBJ_DIR)/OGL_ComputeShader.o
OBJGL_FIL := $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_Instance.cpp $(OBGL_DIR)/OGL_Window.cpp $(OBGL_DIR)/OGL_BaseState.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/OGL_BaseFunctions.cpp $(OBGL_DIR)/OGL_Shader.cpp $(OBGL_DIR)/OGL_VertexAttributes.cpp $(OBGL_DIR)/OGL_UniformBuffer.cpp $(OBGL_DIR)/OGL_ShaderStorageBuffer.cpp $(OBGL_DIR)/OGL_IndexBuffer.cpp $(OBGL_DIR)/OGL_Texture.cpp $(OBGL_DIR)/OGL_Framebuffer.cpp $(OBGL_DIR)/OGL_ComputeShader.cpp

all: $(BIN)/$(EXECUTABLE)

bin/main.o : $(SRC)/main.cpp $(BIN)/libObjectGL
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

libObjectGL: $(OBJGL_OBJ)
	$(CREATE_BIN)
	-ar rcs libObjectGL $(OBJGL_OBJ)

$(BIN)/libObjectGL: $(OBJGL_OBJ)
	$(CREATE_BIN)
	-ar rcs $(BIN)/libObjectGL $(OBJGL_OBJ)

$(OBJ_DIR)/OGL_Instance.o: $(OBGL_DIR)/OGL_Instance.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/OGL_Window.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_Window.o: $(OBGL_DIR)/OGL_Window.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/OGL_Instance.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_BaseFunctions.o: $(OBGL_DIR)/OGL_BaseFunctions.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/OGL_Window.cpp $(OBGL_DIR)/OGL_Instance.cpp 
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_BindableBase.o: $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_BaseState.o: $(OBGL_DIR)/OGL_BaseState.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_Shader.o: $(OBGL_DIR)/OGL_Shader.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_VertexAttributes.o: $(OBGL_DIR)/OGL_VertexAttributes.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_UniformBuffer.o: $(OBGL_DIR)/OGL_UniformBuffer.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_ShaderStorageBuffer.o: $(OBGL_DIR)/OGL_ShaderStorageBuffer.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_IndexBuffer.o: $(OBGL_DIR)/OGL_IndexBuffer.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_Texture.o: $(OBGL_DIR)/OGL_Texture.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_Framebuffer.o: $(OBGL_DIR)/OGL_Framebuffer.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

$(OBJ_DIR)/OGL_ComputeShader.o: $(OBGL_DIR)/OGL_ComputeShader.cpp $(OBGL_DIR)/OGL_BindableBase.cpp $(OBGL_DIR)/ObjectGL.hpp $(OBGL_DIR)/OGL_BaseState.cpp
	$(CREATE_BIN)
	$(CXX) -c $< -o $@ $(CXX_FLAGS) $(OBJGL_FLAGS)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): bin/main.o $(BIN)/libObjectGL
	$(CREATE_BIN)
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $(BIN)/$(EXECUTABLE) $(LIBRARIES)

clean:
	-rm $(BIN)/*
