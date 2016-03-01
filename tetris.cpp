#include "tetris.hpp"
#include "tetromino.hpp"

// global variables
glob_var bool global_running;
glob_var GLFWwindow *global_window;
glob_var tetromino *global_tetromino;
glob_var block_type global_grid[200];
glob_var key_state global_key_state;
glob_var double step_time;
glob_var ullong global_points;

#include "tetromino.cpp"

int main()
{
    // initialize glfw, global variables, buffer objects, shaders
    srand((uint)time(NULL));
    init_game_globals();
    init_glfw_opengl();

    float block_verts[] = {
	0.0f, 0.0f, // top left
	1.0f, 0.0f, // top right
	1.0f, 1.0f, // bottom right
	0.0f, 1.0f  // bottom left
    };
    GLuint elements[] = {
    	0, 1, 2,
    	2, 3, 0
    };

    GLuint vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);
    GLuint vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER,
		 sizeof(block_verts), block_verts, GL_STATIC_DRAW);

    GLuint element_buffer_object;
    glGenBuffers(1, &element_buffer_object);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
     
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vert, NULL);
    glCompileShader(vertex_shader);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &frag, NULL);
    glCompileShader(fragment_shader);
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glBindFragDataLocation(shader_program, 0, "out_color");
    glLinkProgram(shader_program);
    glUseProgram(shader_program);

    GLint pos_attrib = glGetAttribLocation(shader_program, "position");
    glEnableVertexAttribArray(pos_attrib);
    glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
    GLint uni_proj_loc = glGetUniformLocation(shader_program, "projection");
    glUniformMatrix4fv(uni_proj_loc, 1, GL_FALSE, glm::value_ptr(projection));
    GLint uni_mod_loc = glGetUniformLocation(shader_program, "model");
    GLint uni_col_loc = glGetUniformLocation(shader_program, "in_color");

    reset_grid();

    global_tetromino->draw();
    
    const color bg = get_other_color(BACKGROUND);

    while (global_running)
    {
	double start_time = glfwGetTime();
	handle_events();
	loc_pers double prev_time = glfwGetTime();
	double this_time = glfwGetTime();
	if (this_time - prev_time >= step_time) 
	{
	    prev_time += step_time;
	    global_tetromino->move_down();
	    if (global_tetromino->locked)
	    {
		clear_full_lines();
		global_tetromino->init(block_type((rand() % 8) + 1));
	    }
	}
	glClearColor(bg.r, bg.g, bg.b, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 10; i < 200; i++)
	{
	    if (global_grid[i] == NA)
	 	continue;
	    
	    glm::mat4 model;
	    int row = (i - 10) / 10;
	    int col = (i - 10) % 10;
	    glm::vec2 position = glm::vec2(grid_left+col*block_width,
					   row*block_height);
	    model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
	    model = glm::scale(model, glm::vec3(block_width, block_height, 1.0f));
	    
	    glUniformMatrix4fv(uni_mod_loc, 1, GL_FALSE, glm::value_ptr(model));
	    
	    color block_col_val = type_to_color(global_grid[i]);
	    glUniform3f(uni_col_loc,
			block_col_val.r, block_col_val.g, block_col_val.b);
	    
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	for (int r = 0; r < 21; r++)
	{
	    glm::mat4 model;
	    glm::vec2 position = glm::vec2(grid_left, r*block_height);
	    model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
	    model = glm::scale(model, glm::vec3(grid_width+border_size,
						border_size, 1.0f));
	    glUniformMatrix4fv(uni_mod_loc, 1, GL_FALSE, glm::value_ptr(model));
	    color bord_col = get_other_color(GRAY);
	    glUniform3f(uni_col_loc,
			bord_col.r, bord_col.g, bord_col.b);
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	for (int c = 0; c < 11; c++)
	{
	    glm::mat4 model;
	    glm::vec2 position = glm::vec2(grid_left + c*block_width, 0);
	    model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
	    model = glm::scale(model, glm::vec3(border_size,
						grid_height+border_size, 1.0f));
	    glUniformMatrix4fv(uni_mod_loc, 1, GL_FALSE, glm::value_ptr(model));
	    color bord_col = get_other_color(GRAY);
	    glUniform3f(uni_col_loc,
			bord_col.r, bord_col.g, bord_col.b);
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	glfwSwapBuffers(global_window);
        double delta_time = glfwGetTime() - start_time;
	Sleep(int(1000.0f/15.0f - (delta_time * 1000)));
    }
    glDeleteProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer_object);
    glfwTerminate();
    return 0;
}

internal void
clear_line(int row)
{
    for (int c = 0; c < 10; c++)
    {
	global_grid[row * 10 + c] = NA;
    }
    for (int r = row - 1; r >= 0; r--)
    {
	for (int c = 0; c < 10; c++)
	{
	    global_grid[(r+1) * 10 + c] = (block_type)global_grid[r * 10 + c];
	}
    }
}

internal void
clear_full_lines()
{
    for (int r= 19; r>= 0; r--)
    {
	bool full = true;
	for (int c= 0; c < 10; c++)
	{
	    if (global_grid[r * 10 + c] == NA)
		full = false;
	}
	if (full)
	{
	    for (int c = 0; c < 10; c++)
	    {
		global_grid[r * 10 + c] = NA;
	    }
	    for (int row = r - 1; row >= 0; row--)
	    {
		for (int c = 0; c < 10; c++)
		{
		    global_grid[(row+1) * 10 + c] = (block_type)global_grid[row * 10 + c];
		}
	    }
	    r++;
	}
    }
}

internal void
reset_grid()
{
    for (int i = 10; i < 200; i++)
    {
	global_grid[i] = NA;
    }
}

internal color
type_to_color(block_type t)
{
   switch (t)
    {
	case NA:
	{
	    return get_other_color(BACKGROUND);
	} break;
	case I:
	{
	    return get_block_color(BLUE_GREEN);
	} break;
	case O:
	{
	    return get_block_color(GOLD);
	} break;
	case T:
	{
	    return get_block_color(PURPLE);
	} break;
	case Z:
	{
	    return get_block_color(RED);
	} break;
	case S:
	{
	    return get_block_color(LIGHT_GREEN);
	} break;
	case J:
	{
	    return get_block_color(BLUE);
	} break;
	case L:
	{
	    return get_block_color(RED_ORANGE);
	} break;
	default:
	{
	    return get_block_color(block_color(-1));
	} break;
	
    }
}

internal color
get_block_color(block_color c)
{
    color color_v;
    switch (c)
    {
	case BLUE_GREEN:
	{
	    color_v.r = float(91)/255;
	    color_v.g = float(203)/255;
	    color_v.b = float(196)/255; 
	} break;
	case GOLD:
	{
	    color_v.r = float(255)/255;
	    color_v.g = float(196)/255;
	    color_v.b = float(0)/255; 
	} break;
	case PURPLE:
	{
	    color_v.r = float(199)/255;
	    color_v.g = float(146)/255;
	    color_v.b = float(234)/255; 
	} break;
	case RED:
	{
	    color_v.r = float(255)/255;
	    color_v.g = float(81)/255;
	    color_v.b = float(109)/255; 
	} break;
	case LIGHT_GREEN:
	{
	    color_v.r = float(194)/255;
	    color_v.g = float(233)/255;
	    color_v.b = float(130)/255; 
	} break;
	case BLUE:
	{
	    color_v.r = float(116)/255;
	    color_v.g = float(177)/255;
	    color_v.b = float(255)/255; 
	} break;
	case RED_ORANGE:
	{
	    color_v.r = float(247)/255;
	    color_v.g = float(118)/255;
	    color_v.b = float(105)/255; 
	} break;
	default:
	{
	    color_v.r = -1.0f;
	    color_v.g = -1.0f;
	    color_v.b = -1.0f;
	} break;
    }
    return color_v;
}

internal color 
get_other_color(other_color c)
{
    color color_v;
    switch (c)
    {
	case BLACK:
	{
	    color_v.r = float(0)/255;
	    color_v.g = float(0)/255;
	    color_v.b = float(0)/255; 
	} break;
	case BACKGROUND:
	{
	    color_v.r = float(38)/255;
	    color_v.g = float(50)/255;
	    color_v.b = float(56)/255; 
	} break;
	case GRAY:
	{
	    color_v.r = float(84)/255;
	    color_v.g = float(109)/255;
	    color_v.b = float(122)/255; 
	} break;
	case FOREGROUND:
	{
	    color_v.r = float(205)/255;
	    color_v.g = float(211)/255;
	    color_v.b = float(188)/255; 
	} break;
	case WHITE:
	{
	    color_v.r = float(255)/255;
	    color_v.g = float(255)/255;
	    color_v.b = float(255)/255; 
	} break;
	default:
	{
	    color_v.r = -1.0f;
	    color_v.g = -1.0f;
	    color_v.b = -1.0f;
	} break;
    }
    return color_v;
}

internal void
init_game_globals()
{
    global_running = true;
    global_tetromino= (tetromino*)malloc(sizeof(tetromino));
    global_tetromino->init(block_type((rand() % 7) + 1));
    step_time = 1.0f;
    global_points = 0;
}

internal void
handle_events()
{
    // TODO(chris): make sure to include GLFW_PRESS in repeat incrementation
    glfwPollEvents();
    if (glfwWindowShouldClose(global_window))
    {
	global_running = false;
    }   
    if (global_key_state.up)
    {
	if (global_key_state.up == GLFW_PRESS)
	    global_tetromino->rotate();
	else if (global_key_state.repeat % 5 == 0 || global_key_state.repeat != 0)
	    global_tetromino->rotate();
    }
    if (global_key_state.down)
    {
	step_time = 0.12f;
    }
    else
    {
	step_time = 1.0f;
    }
    if (global_key_state.left)
    {
	if (global_key_state.left == GLFW_PRESS)
	    global_tetromino->move_left();
	else if (global_key_state.repeat % 3 == 0 || global_key_state.repeat != 0)
	    global_tetromino->move_left();
    }
    if (global_key_state.right)
    {
	if (global_key_state.right == GLFW_PRESS)
	    global_tetromino->move_right();
	else if (global_key_state.repeat % 3 == 0 || global_key_state.repeat != 0)
	    global_tetromino->move_right();
	
    }
    if (global_key_state.space)
    {
	if (global_key_state.space == GLFW_PRESS)
	    global_tetromino->slam_down();
	else if (global_key_state.repeat % 3 == 0 || global_key_state.repeat != 0)
	    global_tetromino->slam_down();
    }
}

internal void
init_glfw_opengl()
{
    // initialize glfw
    glfwInit();
    // give glfw window hints about OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // initialize window, make current context
    global_window = glfwCreateWindow(1280, 720, "tetris", nullptr, nullptr);
    glfwMakeContextCurrent(global_window);
    // initialize glew
    glewExperimental = GL_TRUE;
    glewInit();
    // set glfw callback functions
    glfwSetKeyCallback(global_window, key_callback);
}

internal void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_REPEAT)
    {
	global_key_state.repeat++;
	return;
    }
    if (action == GLFW_RELEASE)
    {
	global_key_state.repeat = 0;
    }
    switch (key)
    {
	case GLFW_KEY_ESCAPE:
	{
	    global_running = false;
	} break;
	case GLFW_KEY_SPACE:
	{
	    global_key_state.space = action;
	} break;
	case GLFW_KEY_RIGHT:
	{
	    global_key_state.right = action;
	} break;
	case GLFW_KEY_LEFT:
	{
	    global_key_state.left = action;
	} break;
	case GLFW_KEY_DOWN:
	{
	    global_key_state.down = action;
	} break;
	case GLFW_KEY_UP:
	{
	    global_key_state.up = action;
	} break;
    }
}
