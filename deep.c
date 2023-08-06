#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <fenv.h>
#include "util.c"

// 重みをランダムに決定
void generateBiasWaitRand(double ***wait)
{
    //waitに乱数をセット
    for (int a = 0; a < NUMBER_OF_LAYERS + 1; a++)
    {
        for (int b = 0; b < NUMBER_OF_MIDLAYER_NODES + 1; b++)
        {
            for (int c = 0; c < NUMBER_OF_MIDLAYER_NODES; c++)
            {
                wait[a][b][c] = randDouble();
            }
        }
    }
}

//ノードの出力値を次のノードに加算
void sendNextLayer(double *next, double *before, double **wait)
{
    for (int i = 0; i < NUMBER_OF_MIDLAYER_NODES + 1; i++)
    {
        for (int j = 0; j < NUMBER_OF_MIDLAYER_NODES; j++)
        {
            next[j] += before[i] * wait[i][j];
        }
    }
}

//inputLayerからoutputLayerまで計算
void calculateOne(double *input, double *output, double **mid, double ***wait)
{
    //midLayerの最終層まで計算
    for (int a = 0; a < NUMBER_OF_LAYERS; a++)
    {
        if (a == 0)
        {
            sendNextLayer(mid[a], input, wait[a]);
        }
        else
        {
            sendNextLayer(mid[a], mid[a - 1], wait[a]);
        }

        for (int b = 0; b < NUMBER_OF_MIDLAYER_NODES; b++)
        {
            mid[a][b] = tanh(mid[a][b]);
        }

        //バイアス部分は、-1に設定する
        mid[a][NUMBER_OF_MIDLAYER_NODES] = -1;
    }

    //outputLayerの計算
    sendNextLayer(output, mid[NUMBER_OF_LAYERS - 1], wait[NUMBER_OF_LAYERS]);
    for (int a = 0; a < NUMBER_OF_OUTPUTLAYER_NODES; a++)
    {
        output[a] = tanh(output[a]);
    }

}

void fitting(double ***wait, double *input, double *output, double **mid, double *teach)
{
    // 誤差
    double **loss = (double **)calloc(NUMBER_OF_LAYERS + 1, sizeof(double *));
    for (int o = 0; o < NUMBER_OF_LAYERS + 1; o++)
    {
        loss[o] = (double *)calloc(NUMBER_OF_MIDLAYER_NODES, sizeof(double));
    }

    // 出力層から入力層へ逆伝播
    for (int i = NUMBER_OF_LAYERS; i > -1; i--)
    {
        // 最終層の場合
        if (i == NUMBER_OF_LAYERS)
        {
            for (int j = 0; j < NUMBER_OF_OUTPUTLAYER_NODES; j++)
            {
                loss[i][j] = (output[j] - teach[j]) * derivedTanh(output[j]);
            }
        }
        // それ以外の層の場合
        else
        {
            for (int j = 0; j < NUMBER_OF_MIDLAYER_NODES; j++)
            {
                double sum = 0.0;
                if (i == NUMBER_OF_LAYERS - 1)
                {
                    for (int k = 0; k < NUMBER_OF_OUTPUTLAYER_NODES; k++)
                    {
                        sum += loss[i + 1][k] * wait[i + 1][j][k];
                    }
                }
                else
                {
                    for (int k = 0; k < NUMBER_OF_MIDLAYER_NODES; k++)
                    {
                        sum += loss[i + 1][k] * wait[i + 1][j][k];
                    }
                }
                loss[i][j] = sum * derivedTanh(mid[i][j]);
            }
        }

        //重みの更新
        for (int j = 0; j < NUMBER_OF_MIDLAYER_NODES + 1; j++)
        {
            for (int k = 0; k < NUMBER_OF_MIDLAYER_NODES; k++)
            {
                if (i == 0)
                {
                    wait[i][j][k] -= LEARNING_LATE * input[j] * loss[i][k];
                }
                else
                {
                    wait[i][j][k] -= LEARNING_LATE * mid[i - 1][j] * loss[i][k];
                }

                // パラメータが過剰に大きい数字にならないよう調整
                if (wait[i][j][k] > NUMBER_OF_ABS_OF_WAIT)
                {
                    wait[i][j][k] = NUMBER_OF_ABS_OF_WAIT;
                }
                else if (wait[i][j][k] < -NUMBER_OF_ABS_OF_WAIT)
                {
                    wait[i][j][k] = -NUMBER_OF_ABS_OF_WAIT;
                }
            }
        }
    }

    for (int o = 0; o < NUMBER_OF_LAYERS + 1; o++)
    {
        free(loss[o]);
    }
    free(loss);
}