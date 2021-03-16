#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "neillsimplescreen.h"

#define NOINDEX -1
#define EXIT_FAILURE 1
#define MAX_H 9
#define MAX_W 9
#define MAX_LEN 100000
#define MAX_LEV 10

typedef enum bool{false, true} bool;
typedef struct bookcase{
    char book[MAX_H][MAX_W];
    int h;
    int w;
    int prt_index;
    int level;
} bookcase;

int read_file(FILE *fp, bookcase list[]);
int create_level(bookcase *b);
int create_child(char b[MAX_H][MAX_W], bookcase l[], int par_index);
int rightmost_index(char b[], int w);
bool is_duplicate(bookcase list[], char b[MAX_H][MAX_W], int len);
bool has_no_child(bookcase list[], int l);
bool is_happy(char book[MAX_H][MAX_W], int wide, int tall);
void initial(bookcase list[], int hei, int wid);
void print_process(bookcase l[], int i);
void swap(char *x, char *y);
void error(char p[]);
void test();

int main(int argc, char *argv[]){
    int happy_index = NOINDEX, hint;
    FILE *fp;
    bookcase *list = calloc(MAX_LEN, sizeof(bookcase));

    if(list == NULL){
        error("CALLOC ERROR");
    }
    if((argc != 2)&&(argc != 3)){
        error("Wrong command.");
    }
    if((fp=fopen(argv[1],"r")) == NULL) {
        error("File cannot be opened");
    }
    hint = read_file(fp, list);
    happy_index = create_level(list);
    /* Print out the answer and check the optional hint. */
    if(happy_index == NOINDEX){
        printf("No Solution?\n");
    }
    else{
        printf("%d\n", list[happy_index].level + 1);
        if(argc == 3){
            if(!strcmp(argv[2], "verbose")){
                print_process(list, happy_index);
            }
            else{
                error("Wrong command flag.");
            }
        }
    }
    if((hint != NOINDEX)&&(happy_index != NOINDEX)
        &&(hint != (list[happy_index].level + 1))){
        error("The optional hint is not equal to my answer.");
    }

    if(fclose(fp)!=0) {
	       error("File cannot be closed.");
    }
    free(list);
    list = NULL;
    /*test();*/
}
int read_file(FILE *fp, bookcase list[]){
    int i = 0, col, row, hint = NOINDEX, counter;
    char temp[MAX_W];
    /* Get and check the header of the bookcase.*/
    if((fscanf(fp, "%d", &list[0].h) == 1) && (fscanf(fp, "%d", &list[0].w) == 1)){}
    else{
        error("Invalid file : Header is wrong.");
    }
    fscanf(fp, "%d",&hint);
    /* Initialize the start bookcase and check if it is valid.*/
    initial(list, list[0].h, list[0].w);
    list[0].level = 0;
    for(i = 0; i < list[0].h; i++){
        if(fscanf(fp, "%s", temp) != 1){
            error("Invalid file : Size of the bookcase is wrong.");
        }
        if((int)strlen(temp) != list[0].w){
            error("Invalid file : Size of the bookcase is wrong.");
        }
        strcpy(list[0].book[i], temp);
    }
    if(fscanf(fp, "%s", temp) != EOF){
        error("Invalid file : Size of the bookcase is wrong.");
    }
    for (row = 0; row < list[0].h; row++){
        for(col = 0; col < list[0].w; col++){
            if((list[0].book[row][col] >= 'a')&&(list[0].book[row][col] <= 'z')){
                list[0].book[row][col] = toupper(list[0].book[row][col]);
            }
            else if ((list[0].book[row][col] >= 'A')&&(list[0].book[row][col] <= 'Z'));
            else if (list[0].book[row][col] == '.');
            else{
                error("Invalid file.");
            }
        }
        counter = 0;
        for(col = 0; col < list[0].w; col++){
            if(list[0].book[row][col] == '.'){
                counter++;
            }
        }
        if((rightmost_index(list[0].book[row], list[0].w) + counter) != (list[0].w - 1)){
            error("Invalid file : There is space between books.");
        }
    }
    return hint;
}
/* Create each level and tell whether there is any bookcase that is happy, or,
all the possible bookcases have been created and stored in the list. */
int create_level(bookcase l[]){
    int i = 0, pre_index = 0, cur_index = 0, cur_level = 0;

    if(is_happy(l[i].book, l[0].w, l[0].h)){
        return i;
    }
    do{
        for(i = 0; i <= pre_index; i++){
            if(l[i].level == cur_level){
                cur_index = create_child(l[i].book, l, i);
            }
        }
        cur_level++;
        for(i = (pre_index + 1); i <= cur_index;i++){
            l[i].level = cur_level;
            if(is_happy(l[i].book, l[0].w, l[0].h)){
                return i;
            }
        }
        pre_index = cur_index;
    }while(!has_no_child(l, cur_level));
    return NOINDEX;
}
/* Create all child bookcase of the pass-in bookcase b. Check if the new-borned
bookcase is duplicate as anyone existing in the list. If no, add the new one.
Finally return the latest index of array list.*/
int create_child(char b[MAX_H][MAX_W], bookcase l[], int prt){
    char temp[MAX_H][MAX_W];
    static int index = 0;
    int col, row, i, j, dest, start, x, y;

    for(row = 0; row < MAX_H; row++){
        for(col = 0; col < MAX_W; col++){
            temp[row][col] = b[row][col];
        }
    }
    for(i = 0; i < l[0].h; i++){
        for(j = 0; j < l[0].h; j++){
            dest = rightmost_index(b[j], l[0].w);
            start = rightmost_index(b[i], l[0].w);
            if((dest < (l[0].w - 1))&&(j != i)&&(start > -1)){
                swap(&temp[j][dest + 1], &temp[i][start]);
                if(!is_duplicate(l, temp, index)){
                    index++;
                    for(y = 0; y < MAX_H; y++){
                        for(x = 0; x < MAX_W; x++){
                            l[index].book[y][x] = temp[y][x];
                        }
                    }
                    l[index].prt_index = prt;
                }
                swap(&temp[i][start], &temp[j][dest + 1]);
            }
        }
    }
    return index;
}
/* Check if the new borned bookcase is as the same as anyone that has been
put into the list. If it is, abandon it. Or, put it into the list. */
bool is_duplicate(bookcase list[], char b[MAX_H][MAX_W], int index){
    int i, col, row, counter;

    for(i = 0; i < index + 1; i++){
        counter = 0;
        for(row = 0; row < list[0].h; row++){
            for(col = 0; col <list[0].w; col++){
                if(list[i].book[row][col] == b[row][col]){
                    counter++;
                }
            }
        }
        if (counter == (list[0].h * list[0].w)){
            return true;
        }
    }
    return false;
}
/*Tell the rightmost place(the array's index).
if there is no space for more book, return -1*/
int rightmost_index(char b[], int width){
    int col = 0, counter = 0;

    while((col < width)&&(b[col] != '.')){
        if((b[col] <= 'Z')&&(b[col] >= 'A')){
            counter++;
        }
        col++;
    }
    return (counter - 1);
}
/* Tell whether this level has a child level */
bool has_no_child(bookcase list[], int l){
    int i;

    for(i = 0; i < MAX_LEN; i++){
        if (list[i].level == l){
            return false;
        }
    }
    return true;
}
/* Initialize all the bookcase struct and set the bookcase to NONE inside */
void initial(bookcase list[], int hei, int wid){
    int index, row, col;

    for(index = 0; index < MAX_LEN; index++){
        for(row = 0; row < MAX_H; row++){
            for(col = 0; col < MAX_W; col++){
                list[index].book[row][col] = '\0';
            }
        }
        list[index].h = hei;
        list[index].w = wid;
        list[index].prt_index = NOINDEX;
        list[index].level = NOINDEX;
    }
}
/* Swap the order of two character */
void swap(char *x, char *y){
    char z;
    z = *x;
    *x = *y;
    *y = z;
}
/* Check if the bookcase is happy */
bool is_happy(char book[9][9], int wide, int tall){
    int i, j;

    for(i = 0; i < tall; i++){
        for(j = 1; j <= rightmost_index(book[i], wide); j++){
            if(book[i][j] != book[i][j-1]){
                return false;
            }
        }
    }
    for(i = 1; i < tall; i++){
        for(j = 0; j < i; j++){
            if(book[j][0] == book[i][0]){
                return false;
            }
        }
    }
    return true;
}
/* Print out the error and exit */
void error(char p[]){
    printf("%s\n",p);
    exit(EXIT_FAILURE);
}
/* Printout the solution in order */
void print_process(bookcase l[], int happyindex){
    int i, col, row, index = 0;
    int print[MAX_LEV];

    print[0] = happyindex;
    while(print[index] != 0){
        index++;
        print[index] = l[print[index - 1]].prt_index;
    }

    for(i = index; i >= 0; i--){
        printf("\n");
        for(row = 0; row < l[0].h; row++){
            for(col = 0; col < l[0].w; col++){
                switch (l[print[i]].book[row][col]){
                    case 'K': neillfgcol(black); break;
                    case 'R': neillfgcol(red); break;
                    case 'G': neillfgcol(green); break;
                    case 'Y': neillfgcol(yellow); break;
                    case 'B': neillfgcol(blue); break;
                    case 'M': neillfgcol(magenta); break;
                    case 'C': neillfgcol(cyan); break;
                    case 'W': neillfgcol(white); break;
                    default:neillfgcol(black);
                }
                printf("%c", l[print[i]].book[row][col]);
            }
            printf("\n");
        }
    }
    neillfgcol(black);
}
void test(){
    int i, j, k;
    bookcase *list = calloc(MAX_LEN, sizeof(bookcase));
    char temp1[9][9] = {"KKK","MMM","M.."};
    char temp2[9][9] = {"KK.","MLM","M.."};
    char temp3[9][9] = {"RG.","R..","G.."};
    char temp4[9][9] = {"R..","RG.","G.."};
    char temp5[9][9] = {"RGG","R..","..."};
    char temp6[9][9] = {"R..","RGG","..."};

/* Test the function:initial*/
    initial(list, 3, 3);
    for(i = 0; i < MAX_LEN; i++){
        assert(list[i].h == 3);
        assert(list[i].w == 3);
        assert(list[i].prt_index == NOINDEX);
        assert(list[i].level == NOINDEX);
        for(j = 0; j < MAX_H; j++)
            for(k = 0; k < MAX_W; k++)
                assert(list[i].book[j][k] == '\0');
    }
    strcpy(list[0].book[0], "KK.");
    strcpy(list[0].book[1], "M..");
    strcpy(list[0].book[2], "...");
    strcpy(list[1].book[0], "KKK");
    strcpy(list[1].book[1], "MMM");
    strcpy(list[1].book[2], "M..");
    strcpy(list[2].book[0], "KLL");
    strcpy(list[2].book[1], "M..");
    strcpy(list[2].book[2], "BC.");
/* Test the function:is_happy */
    assert(is_happy(list[0].book,3,3));
    assert(!is_happy(list[1].book,3,3));
    assert(!is_happy(list[2].book,3,3));
/* Test the function:has_no_child */
    list[0].level = 0;
    list[1].level = 1;
    list[2].level = 1;
    list[3].level = 2;
    list[4].level = 2;
    list[5].level = 2;
    assert(!has_no_child(list, 2));
    assert(has_no_child(list, 3));
/* Test the function:rightmost_index */
    assert(rightmost_index(list[1].book[0], 3) == 2);
    assert(rightmost_index(list[0].book[0], 3) == 1);
    assert(rightmost_index(list[0].book[1], 3) == 0);
    assert(rightmost_index(list[0].book[2], 3) == -1);
/* Test the function:is_duplicate */
    assert(is_duplicate(list, temp1, 3));
    assert(!is_duplicate(list, temp2, 3));
/* Test the function:swap */
    assert(temp1[0][0] == 'K');
    assert(temp1[1][0] == 'M');
    swap(&temp1[0][0],&temp1[1][0]);
    assert(temp1[0][0] == 'M');
    assert(temp1[1][0] == 'K');
/* Test the function:create_child */
    initial(list, 3, 3);
    strcpy(list[0].book[0], "RG.");
    strcpy(list[0].book[1], "RG.");
    strcpy(list[0].book[2], "...");
    list[0].h = 3;
    list[0].w = 3;
    list[0].level = 0;
    assert((create_child(list[0].book, list, 3)) == 4);
    assert(is_duplicate(list, temp3, 4));
    assert(is_duplicate(list, temp4, 4));
    assert(is_duplicate(list, temp5, 4));
    assert(is_duplicate(list, temp6, 4));

    free(list);
}
