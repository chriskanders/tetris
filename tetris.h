#ifndef TETRIS_H
#define TETRIS_H

#include <windows.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

/* standard useful typedefs for common types */

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef long long llong;
typedef unsigned long long ullong;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float r32;
typedef double r64;

/* if expression not true, write to null ptr (will halt program) */
#define ASSERT(expression) {if (!expression) {*(char *)0 = 0;}}

/* C macro for inline GLSL shaders */
#define GLSL(src) "#version 330 core\n" #src

/* differentiate three unique uses of static */
#define internal static /* used for functions with single-file scope */
#define local_persist static /* variables that persist between function calls  */
#define global_variable static /* global variables */

/* program window width, height */
#define SCREEN_WIDTH 1280.0f
#define SCREEN_HEIGHT 720.0f

/* grid width/height, block width/height in pixels */
const float grid_width = float(round(SCREEN_HEIGHT/1.9f));
const float grid_height = float(round(SCREEN_HEIGHT));
const float block_width = float(round(grid_width/10));
const float block_height = float(round(grid_height/19));

/* 
	grid left and right boundaries on screen
    in pixels with (0,0) being top left corner
 */	
const float grid_left = float((SCREEN_WIDTH/2)-(grid_width/2));
const float grid_right = float((SCREEN_WIDTH/2)+(grid_width/2));

/* border thickness in pixels */
const float border_size = 6.0f;

/*
	constant projection matrix, to calculate between
	typical OpenGL coordinates and screen pixel coordinates
 */
const glm::mat4 projection = glm::ortho(0.0f, 1280.0f,
				        720.0f, 0.0f);

/* OpenGL vertex array, vertex buffer, element buffer */
struct opengl_objects {
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
};

/* OpenGL shaders + shader program */
struct opengl_shaders {
	GLuint vertex;
	GLuint fragment;
	GLuint program;
};

/* OpenGL GLSL variable locations */
struct opengl_var_locs {
	GLint position;
	GLint projection;
	GLint model;
	GLint color;
};

/* keyboard state */
struct key_state {
    int up;
    bool up_pressed;
    int down;
    bool down_pressed;
    int left;
    int right;
    int space;
};

/* GLSL vertex shader */
const GLchar *vert_source = GLSL(
    in vec2 position;
	uniform mat4 model;
	uniform mat4 projection;
	void main()
	{
	    gl_Position = projection * model * vec4(position, 0.0, 1.0);
	}
);
/* GLSL fragment shader */
const GLchar *frag_source = GLSL(
	out vec4 out_color; /* final color of vertex */
	uniform vec3 in_color;
	void main()
	{
	    out_color = vec4(in_color, 1.0);
	}
);

/* vertex data */
const GLfloat vertices[] = {
		0.0f, 0.0f, /* top left */
		1.0f, 0.0f, /* top right */
		1.0f, 1.0f, /* bottom right */
		0.0f, 1.0f  /* bottom left */
};

/* element order data */
const GLuint elements[] = {
    	0, 1, 2,
    	2, 3, 0
};

/* function declarations */
internal void terminate_program();
internal void clear_line(int);
internal void clear_full_lines();
internal void clear_grid();
internal void init_contexts();
internal void init_game_globals();
internal void init_opengl_objects();
internal void init_opengl_shaders();
internal void init_opengl_var_locs();
internal void handle_events();
internal void update_grid();
internal void key_callback(GLFWwindow*, int, int, int, int);

#endif
