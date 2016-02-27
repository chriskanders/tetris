void tetromino::
init(block_type blo_type)
{
    if (blo_type == NA)
	return;
    type = blo_type;
    grid_size = 3;
    direction = 0;
    cen_x = 1; 
    cen_y = 1;
    grid_x = 4;
    grid_y = 0;
    drawn = false;
    locked = false;
    bottom = false;
    switch (type)
    {
	case I:
	{
	    local_grid = (I_0);
	    grid_size = 4;
	    orientations = 2;
	} break;
	case O:
	{
	    local_grid = (O_0);
	    grid_size = 2;
	    orientations = 1;
	    cen_x = 0;
	    cen_y = 0;
	} break;
	case T:
	{
	    local_grid = (T_0);
	    orientations = 4;
	} break;
	case Z:
	{
	    local_grid = (Z_0);
	    orientations = 2;
	} break;
	case S:
	{
	    local_grid = (S_0);
	    orientations = 2;
	} break;
	case J:
	{
	    local_grid = (J_0);
	    orientations = 4;
	} break;
	case L:
	{
	    local_grid = (L_0);
	    orientations = 4;
	} break;
    }
}

void tetromino::
erase()
{
    if (!drawn)
	return;
    int top = grid_y-cen_y;
    int left = grid_x-cen_x;
    for (int i = 0; i < grid_size*grid_size; i++)
    {
	if (local_grid[i] != 0)
	{
	    global_grid[(top+i/grid_size)*10 + (left+i%grid_size)] = NA;
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
    for (int i = 0; i < grid_size*grid_size; i++)
    {
	if (local_grid[i] != 0)
	{
	    global_grid[(top+i/grid_size)*10 + (left+i%grid_size)] =
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
    for (int i = 0; i < grid_size*grid_size; i++)
    {
	if (local_grid[i] != 0)
	{
	    int y = top + i / grid_size;
	    int x = left + i % grid_size;
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
    const int *prev_grid = local_grid;
    int new_dir = (direction + 1) % orientations;
    switch (type)
    {
	case I:
	{
	    if (new_dir == 0) {local_grid = I_0;}
	    if (new_dir == 1) {local_grid = I_1;}
	} break;
	case O:
	{
	} break;
	case T:
	{
	    if (new_dir == 0) {local_grid = T_0;}
	    if (new_dir == 1) {local_grid = T_1;}
	    if (new_dir == 2) {local_grid = T_2;}
	    if (new_dir == 3) {local_grid = T_3;}
	    
	} break;
	case Z:
	{
	    if (new_dir == 0) {local_grid = Z_0;}
	    if (new_dir == 1) {local_grid = Z_1;}
	} break;
	case S:
	{
	    if (new_dir == 0) {local_grid = S_0;}
	    if (new_dir == 1) {local_grid = S_1;}
	} break;
	case J:
	{
	    if (new_dir == 0) {local_grid = J_0;}
	    if (new_dir == 1) {local_grid = J_1;}
	    if (new_dir == 2) {local_grid = J_2;}
	    if (new_dir == 3) {local_grid = J_3;}
	} break;
	case L:
	{
	    if (new_dir == 0) {local_grid = L_0;}
	    if (new_dir == 1) {local_grid = L_1;}
	    if (new_dir == 2) {local_grid = L_2;}
	    if (new_dir == 3) {local_grid = L_3;}
	} break;
    }
    if (!valid())
    {
	int old_grid_x = grid_x;
	int old_grid_y = grid_y;
	// for (int i = 0; i < 9; i++)
	// {
	//     grid_x += (i % 3) - 1;
	//     grid_y += (i / 3) - 1;
	//     if (valid())
	//     {
	// 	direction = new_dir;
	// 	draw();
	// 	return;
	//     }
	//     grid_x = old_grid_x;
	//     grid_y = old_grid_y;
	// }
	for (int i = 0; i < grid_size; i++)
	{
	    grid_x += i - (grid_size == 3 ? 1 : 2);
	    if (valid())
	    {
		direction = new_dir;
		draw();
		return;
	    }
	    grid_x = old_grid_x;
	}
	erase();
	local_grid = prev_grid;
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
