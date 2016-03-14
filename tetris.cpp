#include "tetris.h"
#include "colors.h"
#include "tetromino.h"

/* global variables */
global_variable bool global_running; /* game loop condition */
global_variable GLFWwindow *global_window; /* GLFW window context */
global_variable tetromino *global_this_tetro; /* tetromino on board */
global_variable tetromino *global_next_tetro; /* tetromino to be played */
global_variable tetromino *global_ghost_tetro; /* where tetromino would be placed */
global_variable block_type global_grid[200]; /* tetris grid (20 * 10) */
global_variable key_state global_key_state; /* key state for keyboard input  */
global_variable double global_step_time; /* period between normal grid updates (sec) */
global_variable ullong global_points; /* rows cleared throughout game */
global_variable opengl_objects global_objects; /* buffer objects, vertex arrays*/
global_variable opengl_shaders global_shaders; /* shaders/program */
global_variable opengl_var_locs global_var_locs; /* shader variables */

#include "tetromino.cpp"

int main()
{
	/* initialize glfw, glew, opengl, game */
    init_contexts();
	init_opengl_objects();
	init_opengl_shaders();
	init_opengl_var_locs();
    init_game_globals();

    const color bg = get_other_color(BACKGROUND); /* background color */

	/* main loop */
    while (global_running)
    {
		double start_time = glfwGetTime(); /* frame start time */
		handle_events(); 
		local_persist double prev_time = glfwGetTime(); /* time when stepped last */
		double this_time = glfwGetTime();  /* difference from prev_time */
		if (this_time - prev_time >= global_step_time) /* check whether to step */
		{
			prev_time += global_step_time; /* inc prev time by step time */
			global_this_tetro->move_down(); /* move tetromino down */
			if (global_this_tetro->locked) /* check if done w/ current tetro */
			{
				clear_full_lines(); /* clear any and all full rows */
				global_this_tetro->clear(); /* reset current tetro */
				global_this_tetro->init(block_type((rand() % 8) + 1)); /* new tetro */
				global_this_tetro->draw(); /* draw tetro to grid */
			}
		}
		glClearColor(bg.r, bg.g, bg.b, 0.0f); /* clear screen w/background color */
		glClear(GL_COLOR_BUFFER_BIT); /* clear gl color buffer bit */
		for (int i = 10; i < 200; i++) /* draw all blocks in grid */
		{
			if (global_grid[i] == NA) /* no block in spot i */
				continue;

			glm::mat4 model; /* model matrix */
			int row = (i - 10) / 10; /* row of i */
			int col = (i - 10) % 10; /* column of i */
			glm::vec2 position = glm::vec2(grid_left+col*block_width,
						   row*block_height); /* calculate block position */

			/* move model to position */
			model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
			/* scale model by block size */
			model = glm::scale(model, glm::vec3(block_width, block_height, 1.0f));

			/* send model matrix to shader */
			glUniformMatrix4fv(global_var_locs.model,
				1, GL_FALSE, glm::value_ptr(model));

			/* find color of block */
			color block_col_val = type_to_color(global_grid[i]);
			/* send block color to shader */
			glUniform3f(global_var_locs.color,
				block_col_val.r, block_col_val.g, block_col_val.b);

			/* draw block to screen */
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		for (int r = 0; r < 20; r++) //drawing row border lines
		{
			glm::mat4 model; /* model matrix */

			/* calculate position of row border */
			glm::vec2 position = glm::vec2(grid_left, r*block_height);
			/* move model to position */
			model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
			/* scale model by border size, */
			model = glm::scale(model, glm::vec3(grid_width+border_size,
							border_size, 1.0f));
			/* send model data to shader */
			glUniformMatrix4fv(global_var_locs.model,
				1, GL_FALSE, glm::value_ptr(model));

			/* get color of border */
			color border_col = get_other_color(GRAY);
			/* send color data to shader */
			glUniform3f(global_var_locs.color,
				border_col.r, border_col.g, border_col.b);

			/* draw horizontal border to screen */
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		for (int c = 0; c < 11; c++) // drawing column border lines
		{
			/* same as row border lines, just vertical */
			glm::mat4 model;
			glm::vec2 position = glm::vec2(grid_left + c*block_width, 0);

			model = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
			model = glm::scale(model, glm::vec3(border_size,
							grid_height+border_size, 1.0f));

			glUniformMatrix4fv(global_var_locs.model,
				1, GL_FALSE, glm::value_ptr(model));

			color bor_col = get_other_color(GRAY);
			glUniform3f(global_var_locs.color,
				bor_col.r, bor_col.g, bor_col.b);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		/* draw buffer to window */
		glfwSwapBuffers(global_window);
		/* get time between start and end frame */
       	double delta_time = glfwGetTime() - start_time;
		/* sleep for remaining frame time length */
		Sleep(int(1000.0f/24.0f - (delta_time * 1000)));
    }
	terminate_program();
    return 0;
}

/* end program */
internal void 
terminate_program() 
{
	/* clean up program (shaders, objects, glfw) */
	free(global_this_tetro); global_this_tetro = NULL;
    glDeleteProgram(global_shaders.program);
    glDeleteShader(global_shaders.vertex);
    glDeleteShader(global_shaders.fragment);
    glDeleteVertexArrays(1, &global_objects.vao);
    glDeleteBuffers(1, &global_objects.vbo);
    glfwTerminate();
	printf("final points: %llu\n", global_points);
}

/* clear individual row of grid */
internal void
clear_line(int row)
{
	// clear row to NA`
    for (int c = 0; c < 10; c++)
    {
		global_grid[row * 10 + c] = NA;
	}
	// move previous rows down
	for (int r = row - 1; r >= 0; r--)
	{
		for (int c = 0; c < 10; c++)
		{
			global_grid[(r+1) * 10 + c] = (block_type)global_grid[r * 10 + c];
		}
    }
}

/* clear all full lines in grid */
internal void
clear_full_lines()
{
	/* check for full lines */
    for (int r = 19; r > 0; r--)
    {
		bool full = true;
		/* check for empty block in row */
		for (int c = 0; c < 10; c++)
		{
			if (global_grid[r * 10 + c] == NA)
			full = false;
		}
		/* clear line if full */
		if (full)
		{
			clear_line(r);
			r++;
			global_points++;
		}
    }
}

/* clear entire grid's values to 0 */
internal void
clear_grid()
{
	/* clear entire grid to NA */
    for (int i = 10; i < 200; i++)
    {
		global_grid[i] = NA;
    }
}

/* initialize game global values */
internal void
init_game_globals()
{
    srand((uint)time(NULL)); /* set psuedo-random number gen seed */
    global_running = true;
    global_step_time = 1.0f; /* 1 second real time */
    global_points = 0;
	/* allocate memory for tetro */
    global_this_tetro = (tetromino*)malloc(sizeof(global_this_tetro));
    global_this_tetro->init(block_type((rand() % 7) + 1));
	clear_grid();
   	global_this_tetro->draw();
}

/* initialize opengl buffer objects, vertex arrays */
internal void
init_opengl_objects()
{
    glGenVertexArrays(1, &global_objects.vao);
    glBindVertexArray(global_objects.vao);
    glGenBuffers(1, &global_objects.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, global_objects.vbo);
	/* send vertices data to vertex array */
    glBufferData(GL_ARRAY_BUFFER,
		 sizeof(vertices), vertices, GL_STATIC_DRAW);
    glGenBuffers(1, &global_objects.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, global_objects.ebo);
	/* send element order data to element array */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

/* initialize opengl GLSL shaders */
internal void
init_opengl_shaders()
{
	/* create vertex shader */
    global_shaders.vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(global_shaders.vertex, 1, &vert_source, NULL);
    glCompileShader(global_shaders.vertex);
	/* create fragment shader */
    global_shaders.fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(global_shaders.fragment, 1, &frag_source, NULL);
    glCompileShader(global_shaders.fragment);
    global_shaders.program = glCreateProgram();
	/* attach shaders to program */
    glAttachShader(global_shaders.program, global_shaders.vertex);
    glAttachShader(global_shaders.program, global_shaders.fragment);
    //glBindFragDataLocation(global_shaders.program, 0, "out_color");
    glLinkProgram(global_shaders.program);
    glUseProgram(global_shaders.program);
}

/* initialize opengl shader variable locations */
internal void
init_opengl_var_locs()
{
	/* get shader location of "position" */
    global_var_locs.position = glGetAttribLocation(global_shaders.program, "position");
    glEnableVertexAttribArray(global_var_locs.position);
	/* point position data to vertices data */
    glVertexAttribPointer(global_var_locs.position,
		2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	/* get shader location of "projection" */ 
    global_var_locs.projection =
		glGetUniformLocation(global_shaders.program, "projection");
	/* send projection matrix to shader */
    glUniformMatrix4fv(global_var_locs.projection,
		1, GL_FALSE, glm::value_ptr(projection));
	/* get shader location of "model" */
    global_var_locs.model = glGetUniformLocation(global_shaders.program, "model");
	/* get shader location of "in_color" */
    global_var_locs.color = glGetUniformLocation(global_shaders.program, "in_color");
}

/* handle global key state */
internal void
handle_events()
{
    // TODO(chris): make sure to include GLFW_PRESS in repeat incrementation
    glfwPollEvents(); /* get any new keyboard events */
    if (glfwWindowShouldClose(global_window))
    {
		global_running = false;
    }
    if (global_key_state.up)
    {
		/* execute action if pressed but not held */
		if (!global_key_state.up_pressed)
		{
	    	global_this_tetro->rotate();
	    	global_key_state.up_pressed = true;
        }
    }
    if (global_key_state.down)
    {
        global_step_time = 0.12f; /* faster step time */
    }
    else
    {
        global_step_time = 1.0f;
    }
    if (global_key_state.left)
    {
	    global_this_tetro->move_left();
    }
    if (global_key_state.right)
    {
	    global_this_tetro->move_right();
    }
    if (global_key_state.space)
    {
		/* execute action if pressed but not held */
		if (!global_key_state.down_pressed)
		{
	    	global_this_tetro->slam_down();
	    	global_key_state.down_pressed = true;
		}
    }
}

/* initialize glfw, glew, opengl contexts */
internal void
init_contexts()
{
    /* initialize glfw */
    glfwInit();
    /* give glfw window hints about OpenGL */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    /* initialize window, make current context */
    global_window = glfwCreateWindow(1280, 720, "tetris", nullptr, nullptr);
    glfwMakeContextCurrent(global_window);
    /* initialize glew */
    glewExperimental = GL_TRUE;
    glewInit();
    /* set glfw callback functions */
    glfwSetKeyCallback(global_window, key_callback);
}

/* key callback function */
internal void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	/* modify global_key_state */

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
			if (action == GLFW_RELEASE) {global_key_state.down_pressed = false;}
			global_key_state.down = action;
		} break;
		case GLFW_KEY_UP:
		{
			if (action == GLFW_RELEASE) {global_key_state.up_pressed = false;}
			global_key_state.up = action;
		} break;
    }
}
