#include "tetris.hpp"

int main()
{
    // initialize glfw, global variables, buffer objects, shaders
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
    
    srand((uint)time(NULL));

    gen_rand_grid();

    const col bg = get_other_color(BACKGROUND);

    while (global_running)
    {
	handle_events();
	
	loc_pers double prev_time = glfwGetTime();
	loc_pers uint frames;
	double this_time = glfwGetTime();
	frames++;
	if (this_time - prev_time >= 1.0f) 
	{
	    //printf("Current frames: %d\n", frames);
	    frames = 0;
	    prev_time += 1.0f;
	    gen_rand_grid();
	}
	glClearColor(bg.r, bg.g, bg.b, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < 190; i++)
	{
	    if (global_grid[i] == NA)
	 	continue;
	    col col_val; //TODO(chris): research error if put farther down
	    glm::mat4 model(1.0);
	    int row = i / 10;
	    int col = i % 10;

	    glm::vec2 position = glm::vec2(grid_left+col*block_width,
					   row*block_height);
	    model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
	    model = glm::scale(model, glm::vec3(block_width, block_height, 1.0f));
	    
	    glUniformMatrix4fv(uni_mod_loc, 1, GL_FALSE, glm::value_ptr(model));
	    
	    col_val = type_to_color(global_grid[i]);
	    glUniform3f(uni_col_loc, col_val.r, col_val.g, col_val.b);
	    
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	glfwSwapBuffers(global_window);
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
reset_grid()
{
    for (int i = 0; i < 190; i++)
    {
	global_grid[i] = NA;
    }
}

internal col
type_to_color(block_type t)
{
    switch (t)
    {
	case NA:
	    return get_other_color(BACKGROUND);
	    break;
	case I:
	    return get_block_color(BLUE_GREEN);
	    break;
	case O:
	    return get_block_color(GOLD);
	    break;
	case T:
	    return get_block_color(PURPLE);
	    break;
	case Z:
	    return get_block_color(RED);
	    break;
	case S:
	    return get_block_color(LIGHT_GREEN);
	    break;
	case J:
	    return get_block_color(BLUE);
	    break;
	case L:
	    return get_block_color(RED_ORANGE);
	    break;
	default:
	    return get_block_color(block_color(-1));
	    break;
    }
}

internal col
get_block_color(block_color c)
{
    col color;
    switch (c)
    {
	case BLUE_GREEN:
	    color.r = float(91)/255; color.g = float(203)/255; color.b = float(196)/255; 
	    break;
	case GOLD:
	    color.r = float(255)/255; color.g = float(196)/255; color.b = float(0)/255; 
	    break;
	case PURPLE:
	    color.r = float(199)/255; color.g = float(146)/255; color.b = float(234)/255; 
	    break;
	case RED:
	    color.r = float(255)/255; color.g = float(81)/255; color.b = float(109)/255; 
	    break;
	case LIGHT_GREEN:
	    color.r = float(194)/255; color.g = float(233)/255; color.b = float(130)/255; 
	    break;
	case BLUE:
	    color.r = float(116)/255; color.g = float(177)/255; color.b = float(255)/255; 
	    break;
	case RED_ORANGE:
	    color.r = float(247)/255; color.g = float(118)/255; color.b = float(105)/255; 
	    break;
	default:
	    color.r = -1.0f;color.g = -1.0f;color.b = -1.0f;
	    break;
    }
    return color;
}

internal col 
get_other_color(other_color c)
{
    col color;
    switch (c)
    {
	case BLACK:
	    color.r = float(0)/255; color.g = float(0)/255; color.b = float(0)/255; 
	    break;
	case BACKGROUND:
	    color.r = float(38)/255; color.g = float(50)/255; color.b = float(56)/255; 
	    break;
	case GRAY:
	    color.r = float(84)/255; color.g = float(109)/255; color.b = float(122)/255; 
	    break;
	case FOREGROUND:
	    color.r = float(205)/255; color.g = float(211)/255; color.b = float(188)/255; 
	    break;
	case WHITE:
	    color.r = float(255)/255; color.g = float(255)/255; color.b = float(255)/255; 
	    break;
	default:
	    color.r = -1.0f;color.g = -1.0f;color.b = -1.0f;
	    break;
    }
    return color;
}

internal void
gen_rand_grid()
{
    for (int i = 0; i < 190; i++)
    {
	global_grid[i] = block_type(rand() % 8);
    }
}

internal void
init_game_globals()
{
    global_running = true;
}

internal void
handle_events()
{
    glfwPollEvents();
    if (glfwWindowShouldClose(global_window))
    {
	global_running = false;
    }   
}

internal void
update_grid()
{
    
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
	return;
    }
    bool pressed = action == GLFW_PRESS;
    switch (key)
    {
	case GLFW_KEY_ESCAPE:
	{
	    global_running = false;
	} break;
	case GLFW_KEY_SPACE:
	{
	    global_key_state.space = pressed ? true : false;
	} break;
	case GLFW_KEY_RIGHT:
	{
	    global_key_state.right= pressed ? true : false;
	} break;
	case GLFW_KEY_LEFT:
	{
	    global_key_state.left= pressed ? true : false;
	} break;
	case GLFW_KEY_DOWN:
	{
	    global_key_state.down = pressed ? true : false;
	} break;
	case GLFW_KEY_UP:
	{
	    global_key_state.up= pressed ? true : false;
	} break;
    }
}
