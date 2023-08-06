#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// 盤面のマス目の数
#define NUMBER_OF_SQUARES 64

// 盤面の縦の長さ
#define NUMBER_OF_SIDE 8

// 盤面の横の長さ
#define NUMBER_OF_DIRECTIONS 8

enum DIRECTION {
    NORTH = 0,
    NORTH_EAST = 1,
    EAST = 2,
    SOUTH_EAST = 3,
    SOUTH = 4,
    SOUTH_WEST = 5,
    WEST = 6,
    NORTH_WEST = 7,
};

enum COLOR {
    NON = 0,
    WHITE = 1,
    BLACK = 2
};

void displayBoard(int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE])
{
    printf(
        "| |0|1|2|3|4|5|6|7|\n"\
        "|0|%d|%d|%d|%d|%d|%d|%d|%d|\n"\
        "|1|%d|%d|%d|%d|%d|%d|%d|%d|\n"\
        "|2|%d|%d|%d|%d|%d|%d|%d|%d|\n"\
        "|3|%d|%d|%d|%d|%d|%d|%d|%d|\n"\
        "|4|%d|%d|%d|%d|%d|%d|%d|%d|\n"\
        "|5|%d|%d|%d|%d|%d|%d|%d|%d|\n"\
        "|6|%d|%d|%d|%d|%d|%d|%d|%d|\n"\
        "|7|%d|%d|%d|%d|%d|%d|%d|%d|\n",
        board[0][0], board[0][1], board[0][2], board[0][3], board[0][4], board[0][5], board[0][6], board[0][7], 
        board[1][0], board[1][1], board[1][2], board[1][3], board[1][4], board[1][5], board[1][6], board[1][7], 
        board[2][0], board[2][1], board[2][2], board[2][3], board[2][4], board[2][5], board[2][6], board[2][7], 
        board[3][0], board[3][1], board[3][2], board[3][3], board[3][4], board[3][5], board[3][6], board[3][7], 
        board[4][0], board[4][1], board[4][2], board[4][3], board[4][4], board[4][5], board[4][6], board[4][7], 
        board[5][0], board[5][1], board[5][2], board[5][3], board[5][4], board[5][5], board[5][6], board[5][7], 
        board[6][0], board[6][1], board[6][2], board[6][3], board[6][4], board[6][5], board[6][6], board[6][7], 
        board[7][0], board[7][1], board[7][2], board[7][3], board[7][4], board[7][5], board[7][6], board[7][7]
    );
}

void generateRandamBoard(int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE])
{
    for (int a = 0; a < NUMBER_OF_SIDE; a++)
    {
        for (int b = 0; b < NUMBER_OF_SIDE; b++)
        {
            board[a][b] = randBetween(2, 0);
        }
    }
}

int whichTurn(int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE])
{
    int rockCount = 0;
    for (int a = 0; a < NUMBER_OF_SIDE; a++)
    {
        for (int b = 0; b < NUMBER_OF_SIDE; b++)
        {
            if (board[a][b] != NON)
            {
                rockCount++;
            }
        }
    }
    if (rockCount % 2 == 0)
    {
        return BLACK;
    }
    else
    {
        return WHITE;
    }
}

int turnChange(int color)
{
    if (color == BLACK)
    {
        return WHITE;
    }
    else
    {
        return BLACK;
    }
}

int isInRange(int row, int column)
{
    if (row > -1 && row < NUMBER_OF_SIDE && column > -1 && column < NUMBER_OF_SIDE)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

void moveToInfrontByDirection(int *row, int *column, int direction)
{
    switch (direction)
    {
    case NORTH:
        (*row)--;
        break;
    case NORTH_EAST:
        (*row)--;
        (*column)++;
        break;
    case EAST:
        (*column)++;
        break;
    case SOUTH_EAST:
        (*column)++;
        (*row)++;
        break;
    case SOUTH:
        (*row)++;
        break;
    case SOUTH_WEST:
        (*column)--;
        (*row)++;
        break;
    case WEST:
        (*column)--;
        break;
    case NORTH_WEST:
        (*column)--;
        (*row)--;
        break;
    }
}

void getListOfOneDirection(int result[], int board[8][8], int row, int column, int direction)
{
    memset(result, 0, sizeof(int) * NUMBER_OF_SIDE);
    int index = 0;
    while(isInRange(row, column) == 1)
    {
        result[index] = board[row][column];
        index++;
        moveToInfrontByDirection(&row, &column, direction);
    }
}

int getOppositionColor(int color)
{
    if (color == WHITE)
    {
        return BLACK;
    }
    else if (color == BLACK)
    {
        return WHITE;
    }
    else
    {
        return NON;
    }
}

int getOneDirectionToFlipCount(int vector[NUMBER_OF_SIDE], int color)
{
    if (getOppositionColor(vector[1]) == color)
    {
        for (int i = 2; i < NUMBER_OF_SIDE; i++)
        {
            if (getOppositionColor(vector[i]) == color)
            {
                continue;
            }
            else if (vector[i] == color)
            {
                return i - 1;
            }
            else
            {
                return 0;
            }
        }
        return 0;
    }
    else
    {
        return 0;
    }
}

int getNumberOfWayToPlace(int result[NUMBER_OF_DIRECTIONS], int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE], int row, int column, int color)
{
    if (board[row][column] != NON)
    {
        return 0;
    }
    int flipCount = 0;
    for (int i = 0; i < NUMBER_OF_DIRECTIONS; i++)
    {
        int vector[NUMBER_OF_DIRECTIONS];
        getListOfOneDirection(vector, board, row, column, i);
        result[i] = getOneDirectionToFlipCount(vector, color);
        flipCount += result[i];
    }
    return flipCount;
}

void flipRock(int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE], int allDirectionCount[NUMBER_OF_DIRECTIONS], int row, int column, int color)
{
    int defaultRow = row;
    int defaultColumn = column;
    for (int i = 0; i < NUMBER_OF_DIRECTIONS; i++)
    {
        row = defaultRow;
        column = defaultColumn;
        int count = 0;
        while(count <= allDirectionCount[i])
        {
            board[row][column] = color;
            moveToInfrontByDirection(&row, &column, i);
            count++;
        }
    }
}

int getListAblePlace(int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE], int listAblePlace[NUMBER_OF_SQUARES], int listRockCount[NUMBER_OF_SQUARES][NUMBER_OF_DIRECTIONS], int color)
{
    int index = 0;
    int fc = 0;
    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        if (getNumberOfWayToPlace(listRockCount[index], board, i / NUMBER_OF_SIDE, i % NUMBER_OF_SIDE, color) > 0)
        {
            fc++;
            listAblePlace[index] = i;
            index++;
        }
    }
    return fc;
}

int nextTurn(int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE], int listAblePlace[NUMBER_OF_SQUARES], int listRockCount[NUMBER_OF_SQUARES][NUMBER_OF_DIRECTIONS], int color, int turnNo)
{
    int fc = getListAblePlace(board, listAblePlace, listRockCount, color);
    printf("choices: %d\n", fc);
    if (fc > 0)
    {
        int idx = choice2(fc);
        printf("choice: %d\n", idx);
        flipRock(board, listRockCount[idx], listAblePlace[idx] / NUMBER_OF_SIDE, listAblePlace[idx] % NUMBER_OF_SIDE, color);
    }
    return fc;
}

int choice2(int choices)
{
    return randBetween(choices - 1, 0);
}

int countRock(int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE], int color)
{
    int sum = 0;
    for (int i = 0; i < NUMBER_OF_SIDE; i++)
    {
        for ( int j = 0; j < NUMBER_OF_SIDE; j++)
        {
            if (board[i][j] == color)
            {
                sum++;
            }
        }
    }
    return sum;
}

void convertToVector(int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE], double *input)
{
    for (int i = 0; i < NUMBER_OF_SQUARES; i++)
    {
        switch (board[i / 8][i % 8])
        {
        case BLACK:
            input[2 * i] = (double)1;
            input[2 * i + 1] = (double)0;
            break;
        case WHITE:
            input[2 * i] = (double)0;
            input[2 * i + 1] = (double)1;
            break;
        case NON:
            input[2 * i] = (double)0;
            input[2 * i + 1] = (double)0;
            break;
        }
    }
}

void setToAblePlace(double *teach, int color, int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE])
{
    int listAblePlace[NUMBER_OF_SQUARES];
    int listRockCount[NUMBER_OF_SQUARES][NUMBER_OF_DIRECTIONS];

    int fc = getListAblePlace(board, listAblePlace, listRockCount, color);

    for (int c = 0; c < fc; c++)
    {
        teach[listAblePlace[c] + (color % 2)] = (double)1.0;
    }
}