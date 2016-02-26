void tetromino::
init(block_type blo_type)
{
    ASSERT(blo_type != NA);
    type = blo_type;
    direction = 0;
    cen_x = 2;
    cen_y = 2;
    grid_x = 4;
    grid_y = 0;
    bottom = false;
    switch (type)
    {
	case I:
	{
	    copy_grid(I_0);
	    orientations = 2;
	} break;
	case O:
	{
	    copy_grid(O_0);
	    orientations = 1;
	} break;
	case T:
	{
	    copy_grid(T_0);
	    orientations = 4;
	} break;
	case Z:
	{
	    copy_grid(Z_0);
	    orientations = 2;
	} break;
	case S:
	{
	    copy_grid(S_0);
	    orientations = 2;
	} break;
	case J:
	{
	    copy_grid(J_0);
	    orientations = 4;
	} break;
	case L:
	{
	    copy_grid(L_0);
	    orientations = 4;
	} break;
    }
}

void tetromino::
copy_grid(const int *array)
{
    for (int i = 0; i < 25; i++)
    {
	local_grid[i] = array[i];
    }
}

void tetromino::
draw_on_grid()
{
    int top = grid_y-cen_y;
    int left = grid_x-cen_x;
    for (int i = 0; i < 25; i++)
    {
	if (local_grid[i] != 0)
	{
	    // printf("global_grid[(%d-%d) * 10 + (%d-%d)] = local_grid[%d]: %d\n",
	    // 	   grid_y, cen_y, grid_x, cen_x, i, local_grid[i]);
	    global_grid[(top+i/5)*10 + (left+i%5)] =
		(block_type)local_grid[i];
	}
    }
}

void tetromino::
rotate()
{
    
}

void tetromino::
move_left()
{
    
}

void tetromino::
move_right()
{
    
}

void tetromino::
slam_down()
{
	
}
