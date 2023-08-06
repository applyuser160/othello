#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <fenv.h>
#include "deep.c"
#include "file.c"
#include "board.c"

// 一度学習を行い、損失関数を返す
double learnone(double ***wait, double *input, double *teach)
{
    // 出力データ
    double *output = (double *)calloc(NUMBER_OF_OUTPUTLAYER_NODES, sizeof(double));

    // 隠れ層データ
    double **mid = (double **)calloc(NUMBER_OF_LAYERS, sizeof(double *));
    for (int o = 0; o < NUMBER_OF_LAYERS; o++)
    {
        mid[o] = (double *)calloc(NUMBER_OF_OUTPUTLAYER_NODES + 1, sizeof(double));
    }

    calculateOne(input, output, mid, wait);
    fitting(wait, input, output, mid, teach);

    double errorFunc = 0.0;
    for (int a = 0; a < NUMBER_OF_MIDLAYER_NODES; a++)
    {
        errorFunc += pow((output[a] - teach[a]), 2) / 2;
    }
    free(output);
    for (int o = 0; o < NUMBER_OF_LAYERS; o++)
    {
        free(mid[o]);
    }
    free(mid);
    return errorFunc;
}

// 学習用の盤面を１手進め、学習データを作成する
void genelateLearningData(int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE], int *turnColor, int *isEnd, double *input, double *teach)
{
    int startBoard[NUMBER_OF_SIDE][NUMBER_OF_SIDE] = {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 2, 0, 0, 0},
        {0, 0, 0, 2, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
    };
    int listAblePlace[NUMBER_OF_SQUARES];
    int listRockCount[NUMBER_OF_SQUARES][NUMBER_OF_DIRECTIONS];

    if (*isEnd == 1)
    {
        *isEnd = 0;
        memcpy(board, startBoard, NUMBER_OF_SQUARES * sizeof(int));
        *turnColor = BLACK;
    }
    convertToVector(board, input);
    setToAblePlace(teach, BLACK, board);
    setToAblePlace(teach, WHITE, board);
    int fc = getListAblePlace(board, listAblePlace, listRockCount, *turnColor);

    if (fc == 0)
    {
        if (getListAblePlace(board, listAblePlace, listRockCount, turnChange(*turnColor)) == 0)
        {
            *isEnd = 1;
            return;
        }
        else
        {
            *turnColor = turnChange(*turnColor);
            return;
        }
    }

    //ランダムに手を指す
    int ind = choice2(fc);
    flipRock(board, listRockCount[ind], listAblePlace[ind] / NUMBER_OF_SIDE, listAblePlace[ind] % NUMBER_OF_SIDE, *turnColor);

    // ターンチェンジ
    *turnColor = turnChange(*turnColor);
}

void sub(char paramFilename[256], char dataFilename[256])
{
    // 重み[層][from][to]
    double ***wait = (double ***)calloc(NUMBER_OF_LAYERS + 1, sizeof(double**));
    for (int o = 0; o < NUMBER_OF_LAYERS + 1; o++)
    {
        wait[o] = (double **)calloc(NUMBER_OF_MIDLAYER_NODES + 1, sizeof(double*));
        for (int n = 0; n < NUMBER_OF_MIDLAYER_NODES + 1; n++)
        {
            wait[o][n] = (double *)calloc(NUMBER_OF_MIDLAYER_NODES, sizeof(double));
        }
    }

    // ターンごとの誤差関数
    double *errorFunc = (double *)calloc(NUMBER_OF_LEARNING_TURN, sizeof(double));

    // 盤面
    int board[NUMBER_OF_SIDE][NUMBER_OF_SIDE];

    // ターン
    int turnColor = BLACK;

    // 終了
    int isEnd = 0;


    // 重みを設定
    // generateBiasWaitRand(wait);
    // makeParamFile(paramFilename, wait);
    // printf("param.dat create\n");
    inputParamFile(paramFilename, wait);
    printf("param data import\n");

    // 時間
    time_t start = time(NULL);
    printf("learing start\n");

    for (int i = 0; i < NUMBER_OF_LEARNING_TURN; i++) {

        errorFunc[i] = 0;

        for (int p = 0; p < NUMBER_OF_MAKING_LEARNING_DATA; p++)
        {
            // 入力データ
            double *input = (double *)calloc(NUMBER_OF_MAKING_LEARNING_DATA, sizeof(double));

            // 教師データ
            double *teach = (double *)calloc(NUMBER_OF_MAKING_LEARNING_DATA, sizeof(double));

            // 盤面の生成
            genelateLearningData(board, &turnColor, &isEnd, input, teach);

            // 学習
            errorFunc[i] += learnone(wait, input, teach);

            free(input);
            free(teach);
        }
        printf("errorFunc: %lf\n", errorFunc[i]);
        printf("turn%d end\n", i);

        time_t now = time(NULL);
        time_t estimatedEndTime = now + (now - start) / (i + 1) * (NUMBER_OF_LEARNING_TURN - i);
        printf("Estimated end time is %s\n", ctime(&estimatedEndTime));

        if (i % (NUMBER_OF_LEARNING_TURN / 100) == 0)
        {
            makeCsvFile(dataFilename, errorFunc);
            printf("data.csv create\n");
            makeParamFile(paramFilename, wait);
            printf("%s create\n", paramFilename);
        }
    }

    makeParamFile(paramFilename, wait);
    printf("param.dat create\n");

    makeCsvFile(dataFilename, errorFunc);
    printf("data.csv create\n");
}