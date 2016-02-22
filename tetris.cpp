#include "tetris.hpp"

int main()
{
    // initialize glfw, global variables, buffer objects, shaders
    init_game_globals();
    init_glfw_opengl();

    GLuint vertex_array_object;
    glGenVertexArrays(1, &vertex_array_object);
    glBindVertexArray(vertex_array_object);
    GLuint vertex_buffer_object;
    glGenBuffers(1, &vertex_buffer_object);
    float block_verts[] = {
	0.0f, 0.0f, // top left
	1.0f, 0.0f, // top right
	1.0f, 1.0f, // bottom right
	0.0f, 1.0f  // bottom left
    };
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER,
		 sizeof(block_verts), block_verts, GL_STATIC_DRAW);

    GLuint element_buffer_object;
    glGenBuffers(1, &element_buffer_object);
    GLuint elements[] = {
    	0, 1, 2,
    	2, 3, 0
    };
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
    gen_rand_colors();

    while (running)
    {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < 190; i++)
	{
	    if (grid[i] == 0)
	 	continue;
	    glm::mat4 model(1.0);
	    int row = i / 10;
	    int col = i % 10;

	    glm::vec2 position = glm::vec2(grid_left+col*block_width,
					   row*block_height);

	    model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
	    model = glm::scale(model, glm::vec3(block_width, block_height, 1.0f));
	    
	    glUniformMatrix4fv(uni_mod_loc, 1, GL_FALSE, glm::value_ptr(model));

	    glUniform3f(uni_col_loc, colors[i].r, colors[i].g, colors[i].b);
	    
	    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	glfwSwapBuffers(window);
	handle_events();
	loc_pers double prev_time = glfwGetTime();
	loc_pers uint frames;
	double this_time = glfwGetTime();
	frames++;
	if (this_time - prev_time >= 1.0f) 
	{
	    printf("Current frames: %d\n", frames);
	    frames = 0;
	    prev_time += 1.0f;
	    gen_rand_grid();
	    gen_rand_colors();
	}
    }
    glDeleteProgram(shader_program);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer_object);
    glfwTerminate();
    return 0;
}

col get_color(color_type c)
{
    col color;
    if (c < 0 || c > 11)
    {
	color.r = -1.0f;color.g = -1.0f;color.b = -1.0f;
	return color;
    }
    switch (c)
    {
	case BLACK:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
	case BACKGROUND:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
	case GRAY:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
	case FOREGROUND:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
	case WHITE:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
	case RED:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
	case ORANGE:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
	case GOLD:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
	case BLUE:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
	case BLUE_GREEN:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
	case PURPLE:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
	case LIGHT_GREEN:
	    color.r = 0.0f; color.g = 0.0f; color.b = 0.0f; 
	    break;
    }
    return color;
}

internal void
gen_rand_colors()
{
    for (int i = 0; i < 190; i++)
    {
	colors[i].r = float(rand() % 255) / 255;
	colors[i].g = float(rand() % 255) / 255;
	colors[i].b = float(rand() % 255) / 255;
    }
}

internal void
gen_rand_grid()
{
    for (int i = 0; i < 190; i++)
    {
	if (rand() % 100 >= 50)
	    grid[i] = 1;
	else
	    grid[i] = 0;
    }
}

internal void
init_game_globals()
{
    running = true;
}

internal void
handle_events()
{
    glfwPollEvents();
    if (glfwWindowShouldClose(window))
    {
	running = false;
    }   
    if (is_down(GLFW_KEY_ESCAPE)) {
	running = false;
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
    window = glfwCreateWindow(1280, 720, "tetris", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    // initialize glew
    glewExperimental = GL_TRUE;
    glewInit();
    // set glfw callback functions
    glfwSetKeyCallback(window, key_callback);
}

internal void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == -1)
    {
	// TODO(chris): diagnostics
	return;
    }
    key_status[key] = action;
}

internal int
is_up(int key)
{
    return (key_status[key] == GLFW_RELEASE);
}

internal int
is_down(int key)
{
    return (key_status[key] == GLFW_PRESS);
}

internal int
is_held(int key)
{
    return (key_status[key] == GLFW_REPEAT);
}
