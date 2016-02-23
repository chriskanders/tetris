#ifndef TETRIS_HPP
#define TETRIS_HPP

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

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef long long llong;
typedef unsigned long long ullong;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

#define ERROR(exp, message) {if (exp) {printf(#message);}}

#define ASSERT(expression) {if (!expression) {*(char *)0 = 0;}}

#define GLSL(src) "#version 330 core\n" #src

#define internal static
#define loc_pers static
#define glob_var static

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

const float grid_width = float(round(SCREEN_HEIGHT/1.9f));
const float grid_height = float(round(SCREEN_HEIGHT));
const float block_width = float(round(grid_width/10));
const float block_height = float(round(grid_height/19));

const float grid_left = float((SCREEN_WIDTH/2)-(grid_width/2));
const float grid_right = float((SCREEN_WIDTH/2)+(grid_width/2));

const glm::mat4 projection = glm::ortho(0.0f, 1280.0f,
				        720.0f, 0.0f);

enum block_type {NA, I, O, T, Z, S, J, L};

enum block_color {BLUE_GREEN, GOLD, PURPLE, RED,
		  LIGHT_GREEN, BLUE, RED_ORANGE};

enum other_color {BLACK, BACKGROUND, GRAY, FOREGROUND, WHITE};

typedef struct {
    float r;
    float g;
    float b;
} col;

typedef struct {
    block_type type;
    int x;
    int y;
    int dir;
    int tiles[4][4];
} block;

typedef struct {
    bool up;
    bool down;
    bool left;
    bool right;
    bool space;
} key_state;

const GLchar *vert = GLSL(
        in vec2 position;
	uniform mat4 model;
	uniform mat4 projection;
	void main()
	{
	    gl_Position = projection * model * vec4(position, 0.0, 1.0);
	}
);
const GLchar *frag = GLSL(
	out vec4 out_color;
	uniform vec3 in_color;
	void main()
	{
	    out_color = vec4(in_color, 1.0);
	}
);

// function declarations
internal void reset_grid();
internal col type_to_color(block_type);
internal col get_block_color(block_color);
internal col get_other_color(other_color);
internal void gen_rand_grid();
internal void init_game_globals();
internal void handle_events();
internal void update_grid();
internal void init_glfw_opengl();
internal void key_callback(GLFWwindow*, int, int, int, int);
internal int is_up(int);
internal int is_down(int);
internal int is_held(int);

// global variables
glob_var bool global_running;
glob_var GLFWwindow *global_window;
glob_var block_type global_active_block;
glob_var block_type global_grid[190];
glob_var key_state global_key_state;

#endif
