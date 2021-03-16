#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define G 250
#define L (10 * G)
#define WIDTH 80
#define HEIGHT 30
#define NUM_NEIGHBORS 8
#define TOTAL_GENERATION 1000

#define EMPTY (' ')
#define TREE ('@')
#define FIRE ('*')


typedef enum bool{true = 1, false = 0} bool;

bool neighbor_on_fire(char cells[HEIGHT][WIDTH], int row, int col);
void next_generation(char new_forest[HEIGHT][WIDTH], char old_forest[HEIGHT][WIDTH]);
void initiate_forest(char cells[HEIGHT][WIDTH]);
void print_forest(char cells[HEIGHT][WIDTH]);
void test();

int main(){
    int cur_idx, g;
    char cell_0[HEIGHT][WIDTH];
    char cell_1[HEIGHT][WIDTH];
    cur_idx = 0;
    srand(time(NULL));
    test();

    initiate_forest(cell_0); /* Initialize each cell to empty. */
    initiate_forest(cell_1);

    for (g = 0; g < TOTAL_GENERATION; ++g){
        printf("This is generation %d:\n", (g + 1));
        if (cur_idx == 0){
            next_generation(cell_1, cell_0);
            print_forest(cell_1);
        }
        else{
            next_generation(cell_0, cell_1);
            print_forest(cell_0);
        }
        printf("\n");
        cur_idx = 1 - cur_idx;
    }
    return 0;
}

/* Returns true if the cell at (row, col) has a neighbor which is 'fire'. */
bool neighbor_on_fire(char cells[HEIGHT][WIDTH], int row, int col){
    int drow[] = {0, 1, 1, 1, 0, -1, -1, -1};
    int dcol[] = {1, 1, 0, -1, -1, -1, 0, 1};
    int k;
    for (k = 0; k < NUM_NEIGHBORS; ++k){
        int r = row + drow[k];
        int c = col + dcol[k];
        if ((r >= 0) && (r < HEIGHT) && (c >= 0)
            && (c < WIDTH) && (cells[r][c] == FIRE)){
                return true;
            }
    }
    return false;
}

/* Update the status of each cell. */
void next_generation(char new_forest[HEIGHT][WIDTH], char old_forest[HEIGHT][WIDTH]){
    int row, col;

    for (row = 0; row < HEIGHT; ++row){
        for (col = 0; col < WIDTH; ++col){
            if (old_forest[row][col] == FIRE){
                new_forest[row][col] = EMPTY;
            }
            else if (old_forest[row][col] == TREE){
                if (neighbor_on_fire(old_forest, row, col)){
                    new_forest[row][col] = FIRE;
                }
                else{
                    int r = rand() % L;
                    if (r == 0){
                        new_forest[row][col] = FIRE;
                    }
                    else{
                        new_forest[row][col] = TREE;
                    }
                }
            }
            else{
                int r = rand() % G;
                if (r == 0){
                    new_forest[row][col] = TREE;
                }
                else{
                    new_forest[row][col] = EMPTY;
                }
            }
        }
    }
}

void initiate_forest(char cells[HEIGHT][WIDTH]){ /* Set forest to empty. */
    int i, j;
    for (i = 0; i < HEIGHT; ++i){
        for (j = 0; j < WIDTH; ++j)
            cells[i][j] = EMPTY;
    }
}

void print_forest(char cells[HEIGHT][WIDTH]){
    int i, j;
    for (i = 0; i < HEIGHT; ++i){
        for (j = 0; j < WIDTH; ++j){
            printf("%c", cells[i][j]);
        }
        printf("\n");
    }
}

/* The test. */
void test(){
    char old_cell[HEIGHT][WIDTH];
    char new_cell[HEIGHT][WIDTH];
    int i, j;

    initiate_forest(old_cell);
    initiate_forest(new_cell);
    for (i = 0; i < HEIGHT; ++i){
        for (j = 0; j < WIDTH; ++j){
            assert(old_cell[i][j] == EMPTY);
            assert(new_cell[i][j] == EMPTY);
        }
    }

    old_cell[0][0] = FIRE;
    next_generation(new_cell, old_cell);
    assert(new_cell[0][0] == EMPTY);
    for (i = 0; i < HEIGHT; ++i){
        for (j = 0; j < WIDTH; ++j){
            assert(new_cell[i][j] != FIRE);
        }
    }

    old_cell[1][1] = TREE;
    next_generation(new_cell, old_cell);
    for (i = 0; i < HEIGHT; ++i){
        for (j = 0; j < WIDTH; ++j){
            if (i != 1 || j != 1){
                assert(new_cell[i][j] != FIRE);
            }
        }
    }
    assert(new_cell[1][1] == FIRE);

    old_cell[28][78] = TREE;
    next_generation(new_cell, old_cell);
    assert(new_cell[28][78] != EMPTY);

    old_cell[27][77] = FIRE;
    next_generation(new_cell, old_cell);
    assert(new_cell[28][78] == FIRE);
}
