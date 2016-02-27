void tetromino::
init(block_type blo_type)
{
    ASSERT(blo_type != NA);
    type = blo_type;
    direction = 0;
    cen_x = 2;
    cen_y = 2;
    grid_x = 4;
    grid_y = 1;
    drawn = false;
    locked = false;
    bottom = false;
    switch (type)
    {
	case I:
	{
	    set_grid(I_0);
	    orientations = 2;
	} break;
	case O:
	{
	    set_grid(O_0);
	    orientations = 1;
	} break;
	case T:
	{
	    set_grid(T_0);
	    orientations = 4;
	} break;
	case Z:
	{
	    set_grid(Z_0);
	    orientations = 2;
	} break;
	case S:
	{
	    set_grid(S_0);
	    orientations = 2;
	} break;
	case J:
	{
	    set_grid(J_0);
	    orientations = 4;
	} break;
	case L:
	{
	    set_grid(L_0);
	    orientations = 4;
	} break;
    }
}

void tetromino::
set_grid(const int *array)
{
    for (int i = 0; i < 25; i++)
    {
	local_grid[i] = array[i];
    }
}

void tetromino::
erase()
{
    if (!drawn)
	return;
    int top = grid_y-cen_y;
    int left = grid_x-cen_x;
    for (int i = 0; i < 25; i++)
    {
	if (local_grid[i] != 0)
	{
	    global_grid[(top+i/5)*10 + (left+i%5)] = NA;
	}
    }
    drawn = false;
}

void tetromino::
draw()
{
    if (drawn)
	return;
    int top = grid_y-cen_y;
    int left = grid_x-cen_x;
    for (int i = 0; i < 25; i++)
    {
	if (local_grid[i] != 0)
	{
	    global_grid[(top+i/5)*10 + (left+i%5)] =
		(block_type)local_grid[i];
	}
    }
    drawn = true;
}

bool tetromino::
valid()
{
    ASSERT(!drawn);
    int top = grid_y-cen_y;
    int left = grid_x-cen_x;
    for (int i = 0; i < 25; i++)
    {
	if (local_grid[i] != 0)
	{
	    int y = top + i / 5;
	    int x = left + i % 5;
	    if (x < 0 || x > 9 || y < 0 || y > 19) 
	    {
		return false;
	    }
	    if (global_grid[y * 10 + x] != 0)
	    {
		return false;
	    }
	}
    }
    return true;
}

void tetromino::
rotate()
{
    if (locked)
	return;
    erase();
    int prev_grid [25] = ARRAY_LIST(local_grid);
    int new_dir = (direction + 1) % orientations;
    switch (type)
    {
	case I:
	{
	    if (new_dir == 0) {set_grid(I_0);}
	    if (new_dir == 1) {set_grid(I_1);}
	} break;
	case O:
	{
	    if (new_dir == 0) {set_grid(O_0);}
	} break;
	case T:
	{
	    if (new_dir == 0) {set_grid(T_0);}
	    if (new_dir == 1) {set_grid(T_1);}
	    if (new_dir == 2) {set_grid(T_2);}
	    if (new_dir == 3) {set_grid(T_3);}
	    
	} break;
	case Z:
	{
	    if (new_dir == 0) {set_grid(Z_0);}
	    if (new_dir == 1) {set_grid(Z_1);}
	} break;
	case S:
	{
	    if (new_dir == 0) {set_grid(S_0);}
	    if (new_dir == 1) {set_grid(S_1);}
	} break;
	case J:
	{
	    if (new_dir == 0) {set_grid(J_0);}
	    if (new_dir == 1) {set_grid(J_1);}
	    if (new_dir == 2) {set_grid(J_2);}
	    if (new_dir == 3) {set_grid(J_3);}
	} break;
	case L:
	{
	    if (new_dir == 0) {set_grid(L_0);}
	    if (new_dir == 1) {set_grid(L_1);}
	    if (new_dir == 2) {set_grid(L_2);}
	    if (new_dir == 3) {set_grid(L_3);}
	} break;
    }
    if (!valid())
    {
	erase();
	set_grid(prev_grid);
	draw();
	return;
    }
    direction = new_dir;
    draw();
}

void tetromino::
move_left()
{
    if (locked)
	return;
    erase();
    grid_x--;
    if (!valid())
    {
	grid_x++;
	draw();
	return;
    }
    draw();
}

void tetromino::
move_right()
{
    if (locked)
	return;
    erase();
    grid_x++;
    if (!valid())
    {
	grid_x--;
	draw();
	return;
    }
    draw();
}

void tetromino::
move_down()
{
    erase();
    grid_y++;
    if (!valid())
    {
	grid_y--;
	if (bottom = true)
	    locked = true;
	bottom = true;
    }
    draw();
}

void tetromino::
slam_down()
{
    erase();
    grid_y++;
    while (valid())
    {
	grid_y++;
    }
    grid_y--;
    draw();
    locked = true;
}
