#ifndef TETROMINO_HPP
#define TETROMINO_HPP

#define ARRAY_LIST(arr){ \
	arr[0], arr[1], arr[2], arr[3], arr[4], \
	arr[5], arr[6], arr[7], arr[8], arr[9], \
	arr[10], arr[11], arr[12], arr[13], arr[14], \
	arr[15], arr[16], arr[17], arr[18], arr[19], \
	arr[20], arr[21], arr[22], arr[23], arr[24] \
}

struct tetromino {
    block_type type;
    int local_grid[25];
    int orientations;
    int direction;
    int cen_x;
    int cen_y;
    int grid_x;
    int grid_y;
    bool bottom;
    void init(block_type);
    void copy_grid(const int*);
    void draw_on_grid();
    void rotate();
    void move_left();
    void move_right();
    void slam_down();
};

struct move_result {
    bool success;
    int new_x;
    int new_y;
};

const int I_0[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};
const int I_1[25] = {
    0, 0, 0, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0,
    0, 0, 1, 0, 0
};

const int O_0[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 2, 2, 0,
    0, 0, 2, 2, 0,
    0, 0, 0, 0, 0
};


const int T_0[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 3, 3, 3, 0,
    0, 0, 3, 0, 0,
    0, 0, 0, 0, 0
};
const int T_1[25] = {
    0, 0, 0, 0, 0,
    0, 0, 3, 0, 0,
    0, 3, 3, 0, 0,
    0, 0, 3, 0, 0,
    0, 0, 0, 0, 0
};
const int T_2[25] = {
    0, 0, 0, 0, 0,
    0, 0, 3, 0, 0,
    0, 3, 3, 3, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};
const int T_3[25] = {
    0, 0, 0, 0, 0,
    0, 0, 3, 0, 0,
    0, 0, 3, 3, 0,
    0, 0, 3, 0, 0,
    0, 0, 0, 0, 0
};


const int Z_0[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 4, 4, 0, 0,
    0, 0, 4, 4, 0,
    0, 0, 0, 0, 0
};
const int Z_1[25] = {
    0, 0, 0, 0, 0,
    0, 0, 4, 0, 0,
    0, 4, 4, 0, 0,
    0, 4, 0, 0, 0,
    0, 0, 0, 0, 0
};


const int S_0[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 5, 5, 0,
    0, 5, 5, 0, 0,
    0, 0, 0, 0, 0
};
const int S_1[25] = {
    0, 0, 0, 0, 0,
    0, 0, 5, 0, 0,
    0, 0, 5, 5, 0,
    0, 0, 0, 5, 0,
    0, 0, 0, 0, 0
};


const int J_0[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    6, 6, 6, 0, 0,
    0, 0, 6, 0, 0,
    0, 0, 0, 0, 0
};
const int J_1[25] = {
    0, 0, 6, 0, 0,
    0, 0, 6, 0, 0,
    0, 6, 6, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};
const int J_2[25] = {
    0, 0, 0, 0, 0,
    0, 0, 6, 0, 0,
    0, 0, 6, 6, 6,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};
const int J_3[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 6, 6, 0,
    0, 0, 6, 0, 0,
    0, 0, 6, 0, 0
};


const int L_0[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 7, 7, 7,
    0, 0, 7, 0, 0,
    0, 0, 0, 0, 0
};
const int L_1[25] = {
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    0, 7, 7, 0, 0,
    0, 0, 7, 0, 0,
    0, 0, 7, 0, 0
};
const int L_2[25] = {
    0, 0, 0, 0, 0,
    0, 0, 7, 0, 0,
    7, 7, 7, 0, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};
const int L_3[25] = {
    0, 0, 7, 0, 0,
    0, 0, 7, 0, 0,
    0, 0, 7, 7, 0,
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0
};

#endif
