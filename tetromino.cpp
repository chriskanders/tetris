/* clear tetromino to empty vales */
void tetromino::
clear()
{
	type = NA;
	local_grid = NULL;
	grid_size = 0;
	orientations = 0;
	direction = 0;
	center = 0;
	grid_x = 0;
	grid_y = 0;
	drawn = false;
	bottom = false;
	locked = false;
}

/* initialize tetro to specific block type */
void tetromino::
init(block_type blo_type)
{
    if (blo_type == NA)
	return;
    type = blo_type;
    grid_size = 3;
    direction = 0;
	center = 1;
    grid_x = 4;
    grid_y = 0;
    drawn = false;
    locked = false;
    bottom = false;
	local_grid = get_new_local_grid(type, direction);
	/* initialize block type specfic members */
    switch (type)
    {
		case I:
		{
			grid_size = 4;
			orientations = 2;
			grid_y = 1;
		} break;
		case O:
		{
			grid_size = 2;
			orientations = 1;
			center = 0;
		} break;
		case T:
		{
			orientations = 4;
		} break;
		case Z:
		{
			orientations = 2;
		} break;
		case S:
		{
		} break;
		case J:
		{
			orientations = 4;
		} break;
		case L:
		{
			orientations = 4;
		} break;
    }
	if (!valid())
	{
		terminate_program();
	}
}

/* if drawn, erase tetromino from grid */
void tetromino::
erase()
{
    if (!drawn) /* return if already erased */
		return;
    int top = grid_y-center;
    int left = grid_x-center;
	/* take tetromino off grid */
    for (int i = 0; i < grid_size*grid_size; i++)
    {
		if (local_grid[i] != 0)
		{
			/* reset spot in global_grid to empty */
			global_grid[(top+i/grid_size)*10 + (left+i%grid_size)] = NA;
		}
    }
    drawn = false;
}

/* if not drawn, draw tetromino onto grid */
void tetromino::
draw()
{
    if (drawn) /* return if already drawn */
		return;
    int top = grid_y-center;
    int left = grid_x-center;
	/* draw tetromino onto grid */
    for (int i = 0; i < grid_size*grid_size; i++)
    {
		if (local_grid[i] != 0) 
		{
			/* draw local_grid[i] onto global grid */
			global_grid[(top+i/grid_size)*10 + (left+i%grid_size)] =
				(block_type)local_grid[i];
		}
    }
    drawn = true;
}

/* check if tetromino is "valid" on grid */
bool tetromino::
valid()
{
	/* FIXME(chris) possible source of wrong end game state errors? */
    if (drawn)
	   return true;
    int top = grid_y-center;
    int left = grid_x-center;
	/* loop through local grid */
    for (int i = 0; i < grid_size*grid_size; i++)
    {
		/* only check filled spots in local grid */
		if (local_grid[i] != 0)
		{
			int y = top + i / grid_size;
			int x = left + i % grid_size;
			/* check if out of global grid */
			if (x < 0 || x > 9 || y < 0 || y > 19) 
			{
				return false;
			}
			/* check if conflicts with global grid */
			if (global_grid[y * 10 + x] != 0)
			{
				return false;
			}
		}
    }
    return true;
}

/* rotate tetromino clock-wise */
void tetromino::
rotate()
{
    if (locked) /* ignore if locked */
		return;
    erase();
    const int *prev_grid = local_grid;
    int new_dir = (direction + 1) % orientations;
	local_grid = get_new_local_grid(type, new_dir);
	/* check if new rotated tetro is valid */
    if (!valid())
    {
		/* save old grid's x and y */
		int old_grid_x = grid_x;
		int old_grid_y = grid_y;
		/* check if one spot to left is valid */
		{
			grid_x -= 1;
			if (valid())
			{
				direction = new_dir;
				draw();
				return;
			}
			grid_x = old_grid_x;
		}
		/* check if one spot to right is valid */
		{
			grid_x += 1;
			if (valid())
			{
				direction = new_dir;
				draw();
				return;
			}
			grid_x = old_grid_x;
		}
		//otherwise erase and reset to prev grid
		erase();
		local_grid = prev_grid;
		draw();
		return;
    }
    bottom = false;
    direction = new_dir;
    draw();
}

/* move tetromino one space left */
void tetromino::
move_left()
{
    if (locked) /* ignore if locked */
		return;
    erase();
    grid_x--;
	/* check if not valid to move left */
    if (!valid())
    {
		grid_x++; /* move back to old spot */
		draw();
		return;
    }
    bottom = false;
    draw();
}

/* move tetromino one space right */
void tetromino::
move_right()
{
    if (locked) /* ignore if locked */
		return;
    erase();
    grid_x++;
	/* check if not valid to move right */
    if (!valid())
    {
		grid_x--; /* move back to old spot */
		draw();
		return;
    }
    bottom = false;
    draw();
}

/* move tetromino one space down */
void tetromino::
move_down()
{
    if (locked) /* ignore if locked */
		return;
    erase();
    grid_y++;
	/* check if tetro is hitting bottom */
    if (!valid())
    {
		grid_y--;
		if (bottom = true)
			locked = true;
		bottom = true;
    }
    draw();
}

/* move tetromino down until floor, lock in place */
void tetromino::
slam_down()
{
    if (locked)
		return;
    erase();
	/* move to bottom */
    while (!bottom)
    {
		move_down();
    }
    draw();
    locked = true;
}

/* find new local grid from specifc type and direction */ 
internal const int * 
get_new_local_grid(block_type type, int new_dir)
{
	if (type == NA) {return NULL;};
	switch (type)
    {
		case I:
		{
			if (new_dir == 0) {return I_0;}
			if (new_dir == 1) {return I_1;}
		} break;
		case O:
		{
			return O_0;
		} break;
		case T:
		{
			if (new_dir == 0) {return T_0;}
			if (new_dir == 1) {return T_1;}
			if (new_dir == 2) {return T_2;}
			if (new_dir == 3) {return T_3;}
		} break;
		case Z:
		{
			if (new_dir == 0) {return Z_0;}
			if (new_dir == 1) {return Z_1;}
		} break;
		case S:
		{
			if (new_dir == 0) {return S_0;}
			if (new_dir == 1) {return S_1;}
		} break;
		case J:
		{
			if (new_dir == 0) {return J_0;}
			if (new_dir == 1) {return J_1;}
			if (new_dir == 2) {return J_2;}
			if (new_dir == 3) {return J_3;}
		} break;
		case L:
		{
			if (new_dir == 0) {return L_0;}
			if (new_dir == 1) {return L_1;}
			if (new_dir == 2) {return L_2;}
			if (new_dir == 3) {return L_3;}
		} break;
    }
	return NULL;
}
