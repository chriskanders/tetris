/*process header only once */
#ifndef TETROMINO_H 
#define TETROMINO_H

struct tetromino {
    block_type type;
    const int *local_grid; /* storage of tetromino blocks */
    int grid_size; /* will be 2 (O), 3(T,Z,S,J,L), or 4(I) */
    int orientations; /* number of diff directions of tetromino */
    int direction; /* current direction */
	int center; /* center of local grid */
    //int cen_x; 
    //int cen_y; 
    int grid_x; /* x coord of tetro on global grid */
    int grid_y; /* y coord of tetro on global grid */
    bool drawn; /* true if drawn currently on global_grid */
    bool bottom; /* touching blocks below */
    bool locked; /* locked in place, no further movement */
	// methods
	void clear();
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

/* function declarations */
internal const int *get_new_local_grid(block_type, int);

/* I(stick)tetromino orientations */
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


/* O(square)tetromino orientations */
const int O_0[4] = {
    2, 2,
    2, 2
};


/* T(T turn) tetromino orientations */
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


/* Z(left snake) tetromino orientations */
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


/* S(right snake) tetromino orientations */
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


/* J(left gun) tetromino orientations */
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


/* L(right gun) tetromino orientations */
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
