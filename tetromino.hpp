#ifndef TETROMINO_HPP
#define TETROMINO_HPP

struct tetromino {
    block_type type;
    const int *local_grid;
    int grid_size;
    int orientations;
    int direction;
    int cen_x;
    int cen_y;
    int grid_x;
    int grid_y;
    bool drawn;
    bool bottom;
    bool locked;
    void init(block_type);
    void draw();
    void erase();
    bool valid();
    void rotate();
    void move_left();
    void move_right();
    void move_down();
    void slam_down();
};

const int I_0[16] = {
    0, 0, 0, 0,
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0
};
const int I_1[16] = {
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 0, 0,
    0, 1, 0, 0
};

const int O_0[4] = {
    2, 2,
    2, 2
};


const int T_0[9] = {
    0, 0, 0,
    3, 3, 3,
    0, 3, 0
};
const int T_1[9] = {
    0, 3, 0,
    3, 3, 0,
    0, 3, 0
};
const int T_2[9] = {
    0, 3, 0,
    3, 3, 3,
    0, 0, 0
};
const int T_3[9] = {
    0, 3, 0,
    0, 3, 3,
    0, 3, 0
};


const int Z_0[9] = {
    0, 0, 0,
    4, 4, 0,
    0, 4, 4,
};
const int Z_1[9] = {
    0, 0, 4,
    0, 4, 4,
    0, 4, 0,
};


const int S_0[9] = {
    0, 0, 0,
    0, 5, 5,
    5, 5, 0
};
const int S_1[9] = {
    5, 0, 0,
    5, 5, 0,
    0, 5, 0
};


const int J_0[9] = {
    0, 0, 0,
    6, 6, 6,
    0, 0, 6
};
const int J_1[9] = {
    0, 6, 0,
    0, 6, 0,
    6, 6, 0
};
const int J_2[9] = {
    6, 0, 0,
    6, 6, 6,
    0, 0, 0,
};
const int J_3[9] = {
    0, 6, 6,
    0, 6, 0,
    0, 6, 0
};


const int L_0[9] = {
    0, 0, 0,
    7, 7, 7,
    7, 0, 0
};
const int L_1[9] = {
    7, 7, 0,
    0, 7, 0,
    0, 7, 0
};
const int L_2[9] = {
    0, 0, 7,
    7, 7, 7,
    0, 0, 0
};
const int L_3[9] = {
    0, 7, 0,
    0, 7, 0,
    0, 7, 7
};

#endif
