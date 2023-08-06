
//機械学習用データ読み取り
void inputLearningData(char filename[256], double **input, double **teach)
{
    FILE *file;

    file = fopen(filename, "r");
    for (int a = 0; a < NUMBER_OF_MAKING_LEARNING_DATA; a++)
    {
        int b = 0;
        int r1, r2, r3, r4;
        for (int c = 0; c < NUMBER_OF_INPUTLAYER_NODES; c++)
        {
            fscanf(file, "[%d][%d][%d]:%d,", &r1, &r2, &r3, &r4);
            input[a][r3] = (double)r4;
        }
        input[a][NUMBER_OF_INPUTLAYER_NODES] = -1;
        b = 1;
        int count = 0;
        for (int c = 0; c < NUMBER_OF_INPUTLAYER_NODES; c++)
        {
            fscanf(file, "[%d][%d][%d]:%d,", &r1, &r2, &r3, &r4);
            teach[a][r3] = (double)r4;
        }
    }
    fclose(file);
}

void makeCsvFile(char filename[256], double *data)
{
    /*
    ファイル形式
    a,b\n
    a: index,
    b: data
    */
    FILE *fp;

    fp = fopen(filename, "w");
    for (int i = 0; i < NUMBER_OF_LEARNING_TURN; i++)
    {
        fprintf(fp, "%d,%lf\n", i, data[i]);
    }
    fclose(fp);
}

//パラメータファイル作成
void makeParamFile(char filename[256], double ***wait)
{
    /*
    パラメータファイル形式
    [b][c][d]:e,
    b: Layer Index
    c: Node Index
    d: Node Index (Optional)
    e: Value
    */
    FILE *fp;

    fp = fopen(filename, "w");

    for (int b = 0; b < NUMBER_OF_LAYERS + 1; b++)
    {
        for (int c = 0; c < NUMBER_OF_MIDLAYER_NODES + 1; c++)
        {
            for (int d = 0; d < NUMBER_OF_MIDLAYER_NODES; d++)
            {
                fprintf(fp, "[%d][%d][%d]:%lf,\n", b, c, d, wait[b][c][d]);
            }
        }
    }
    fclose(fp);
}

//パラメータファイル読み取り
void inputParamFile(char filename[256], double ***wait)
{
    FILE *fp;

    fp = fopen(filename, "r");

    int b, c, d;
    double e;
    for (int i = 0; i < (NUMBER_OF_LAYERS + 1) * (NUMBER_OF_MIDLAYER_NODES + 1) * NUMBER_OF_MIDLAYER_NODES; i++)
    {
        fscanf(fp, "[%d][%d][%d]:%lf,\n", &b, &c, &d, &e);
        wait[b][c][d] = e;
    }
    fclose(fp);
}