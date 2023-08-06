#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <fenv.h>

//学習回数
#define NUMBER_OF_LEARNING_TURN 1000

//学習率
#define LEARNING_LATE 0.01

//隠れ層の層数(層を増やすと発散してしまうので、これは変更しない)
#define NUMBER_OF_LAYERS 1

//学習データの数(単一ファイル内)
#define NUMBER_OF_MAKING_LEARNING_DATA 1000000

//学習データファイルの数
#define NUMBER_OF_MAKING_LEARNING_FILES 1

//入力層のノード数
#define NUMBER_OF_INPUTLAYER_NODES 128

//出力層のノード数
#define NUMBER_OF_OUTPUTLAYER_NODES 128

//隠れ層のノード数
#define NUMBER_OF_MIDLAYER_NODES 128

// 指数関数がオーバーフローするのを防ぐための線引き用の数
#define NUMBER_OF_BORDER_OF_OVERFLOW 2.0

// waitの最大/最小値（abs)
#define NUMBER_OF_ABS_OF_WAIT 10.0

// シグモイド関数
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// シグモイド関数の導関数
double derivedSigmoid(double x)
{
    return sigmoid(x) * (1.0 - sigmoid(x));
}

// tanh関数
double tanh(double x)
{
    if (x > NUMBER_OF_BORDER_OF_OVERFLOW)
    {
        return (double)1;
    }
    else if (x < -NUMBER_OF_BORDER_OF_OVERFLOW)
    {
        return (double)-1;
    }
    else
    {
        return (double)(exp(x) - exp(-x)) / (exp(x) + exp(-x));
    }
}

// tanh関数の導関数
double derivedTanh(double x)
{
    return 1.0 - pow(tanh(x), 2.0);
}

// ReLu関数
double relu(double x)
{
    if (x > 0)
    {
        return (double)x;
    }
    else
    {
        return (double)x;
    }
}

// ReLu関数の導関数
double derivedRelu(double x)
{
    if (x > 0)
    {
        return (double)1;
    }
    else
    {
        return (double)0;
    }
}

// leaky-ReLu関数
double lrelu(double x)
{
    if (x <= 0)
    {
        return (double)(0.01 * x);
    }
    else
    {
        return (double)x;
    }
}

// ReLU6&leaky-ReLu関数の導関数
double derivedLRelu(double x)
{
    if (x <= 0)
    {
        return (double)0.01;
    }
    else
    {
        return (double)1;
    }
}

//乱数を生成
double randDouble()
{
    return (double) rand() / RAND_MAX * 10;
}

int randBetween(int max, int min)
{
    srand((unsigned int)time(NULL)); // 現在時刻の情報で初期化
    return (rand()%(max - min + 1)) + min;
}

unsigned long long powll(int base, int power)
{
    unsigned long long result = 1;
    for (int i = 0; i < power; i++)
    {
        result *= base;
    }
    return result;
}