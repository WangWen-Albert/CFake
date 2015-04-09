/*
 * Copyright 2015 Nokia
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/******************************************************************************
 * @File    GameOfLife.c
 * @Brief   Please refer the following wiki for the background of the program.
 *          http://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define SLIFE_SIZE(rowSize, colSize) \
    (sizeof(SLife) + (rowSize) * (colSize) * sizeof(char))

typedef int (* TCellNextForThis) (int neighborNum);

typedef struct SLife {
    int  rowSize;
    int  colSize;
    char cells[0];
} SLife;

int     gRowSize = 25;
int     gColSize = 70;
SLife * gLife;
int     gSeconds = 0;

static void AbortLife(char * format, ...)
{
    va_list args;

    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    exit(1);
}

int CellNextForLive(int neighborNum)
{
    return (neighborNum >= 2) && (neighborNum <= 3);
}

int CellNextForDead(int neighborNum)
{
    return (neighborNum == 3);
}

void CellSetLive(SLife * life, int row, int col, char isLive)
{
    life->cells[row * life->colSize + col] = isLive;
}

char CellIsLive(SLife * life, int row, int col)
{
    int  validRow; 
    int  validCol;
    char isLive;

    validRow = row >= 0 && row < life->rowSize;
    validCol = col >= 0 && col < life->colSize;
    isLive   = validRow && validCol && life->cells[row * life->colSize + col];

    return isLive;
}

char CellToChar(SLife * life, int row, int col)
{
    if (row < 0 || row >= life->rowSize ||
        col < 0 || col >= life->colSize)
    {
        return '#';
    }
    else
    {
        return CellIsLive(life, row, col) ? '*' : '.';
    }
}

int CellGetNeighborNum(SLife * life, int row, int col)
{
    const int rowVector[] = { 0, 0, -1, 1, -1,  1, -1, 1};
    const int colVector[] = {-1, 1,  0, 0, -1, -1,  1, 1};

    int num;
    int i;

    num = 0;
    for (i = 0; i < 8; i++)
    {
        num += CellIsLive(life, row + rowVector[i], col + colVector[i]);
    }
    return num;
}

int CellNext(SLife * life, int row, int col)
{
    const TCellNextForThis isLiveCbPtr[] = {CellNextForDead, CellNextForLive};

    int neighborNum;
    int isLiveNow;
    int isLiveNext;

    neighborNum = CellGetNeighborNum(life, row, col);
    isLiveNow   = CellIsLive(life, row, col);
    isLiveNext  = isLiveCbPtr[isLiveNow](neighborNum);

    return isLiveNext;
}

void LifeSetSize(SLife * life, int rowSize, int colSize)
{
    memset(life, 0, SLIFE_SIZE(rowSize, colSize));
    life->rowSize = rowSize;
    life->colSize = colSize;
}

SLife * LifeNew(int rowSize, int colSize)
{
    SLife * newLife;

    newLife = (SLife *)malloc(SLIFE_SIZE(rowSize, colSize));
    if (newLife != NULL)
    {
        LifeSetSize(newLife, rowSize, colSize);
    }

    return newLife;
}

static void inline LifeFree(SLife * life)
{
    if (life != NULL);
    {
        free(life);
    }
}

SLife * LifeCopy(SLife * life)
{
    SLife * newLife;

    newLife = LifeNew(life->rowSize, life->colSize);
    if (newLife != NULL)
    {
        memcpy(newLife, life, SLIFE_SIZE(life->rowSize, life->colSize));
    }

    return newLife;
}

void LifeUpdate(SLife * newLife, SLife * oldLife)
{
    int  row;
    int  col;
    char isLive;

    for (row = 0; row < newLife->rowSize; row++)
    {
        for (col = 0; col < newLife->colSize; col++)
        {
            isLive = CellNext(oldLife, row, col);
            CellSetLive(newLife, row, col, isLive);
        }
    }
}

void LifeNext(SLife * life)
{
    SLife * tempLife;

    tempLife = LifeCopy(life);
    if (tempLife == NULL)
    {
        AbortLife("Not enough memory to copy old life!");
    }

    LifeUpdate(life, tempLife);
    LifeFree(tempLife);
}

void LifeShow(SLife * life)
{
    int  row;
    int  col;

    /* print boundary by -1 and size */
    for(row = -1; row < life->rowSize + 1; row++)
    {
        for(col = -1; col < life->colSize + 1; col++)
        {
            printf("%c", CellToChar(life, row, col));
        }
        printf("\n");
    }
}

SLife * InitLife(int rowSize, int colSize)
{
    SLife * newLife;
    int     row;
    int     col;

    newLife = LifeNew(rowSize, colSize);
    if (newLife == NULL)
    {
        AbortLife("Not enough memory to create a new life!");
    }

    srand(time(NULL));
    for (row = 0; row < newLife->rowSize; row++)
    {
        for (col = 0; col < newLife->colSize; col++)
        {
            CellSetLive(newLife, row, col, rand() % 2);
        }
    }

    return newLife;
}

void StartGame(void)
{
    gLife = InitLife(gRowSize, gColSize);
    while (1)
    {
        system("clear");
        LifeShow(gLife);
        LifeNext(gLife);
        sleep(1);
        gSeconds++;
    }
}

int main(int argc, char * argv[])
{
    StartGame();
    return 0;
}

