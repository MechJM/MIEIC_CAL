/*
 * Sudoku.cpp
 *
 */

#include "Sudoku.h"
#include <algorithm>

using namespace std;

/** Inicia um Sudoku vazio.
 */
Sudoku::Sudoku()
{
	this->initialize();
}

/**
 * Inicia um Sudoku com um conte�do inicial.
 * Lan�a excep��o IllegalArgumentException se os valores
 * estiverem fora da gama de 1 a 9 ou se existirem n�meros repetidos
 * por linha, coluna ou bloc 3x3.
 *
 * @param nums matriz com os valores iniciais (0 significa por preencher)
 */
Sudoku::Sudoku(int nums[9][9])
{
	this->initialize();

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (nums[i][j] != 0)
			{
				int n = nums[i][j];
				numbers[i][j] = n;
				lineHasNumber[i][n] = true;
				columnHasNumber[j][n] = true;
				block3x3HasNumber[i / 3][j / 3][n] = true;
				countFilled++;
			}
		}
	}
}

void Sudoku::initialize()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			for (int n = 0; n < 10; n++)
			{
				numbers[i][j] = 0;
				lineHasNumber[i][n] = false;
				columnHasNumber[j][n] = false;
				block3x3HasNumber[i / 3][j / 3][n] = false;
			}
		}
	}

	this->countFilled = 0;
}

/**
 * Obtem o conteudo actual (so para leitura!).
 */
int** Sudoku::getNumbers()
{
	int** ret = new int*[9];

	for (int i = 0; i < 9; i++)
	{
		ret[i] = new int[9];

		for (int a = 0; a < 9; a++)
			ret[i][a] = numbers[i][a];
	}

	return ret;
}

/**
 * Verifica se o Sudoku j� est� completamente resolvido
 */
bool Sudoku::isComplete()
{
	return countFilled == 9 * 9;
}



/**
 * Resolve o Sudoku.
 * Retorna indica��o de sucesso ou insucesso (sudoku imposs�vel).
 */
bool Sudoku::solve()
{
    if (isComplete()) return true;

    vector<int> nums = {1,2,3,4,5,6,7,8,9};


	return false;
}



/**
 * Imprime o Sudoku.
 */
void Sudoku::print()
{
	for (int i = 0; i < 9; i++)
	{
		for (int a = 0; a < 9; a++)
			cout << this->numbers[i][a] << " ";

		cout << endl;
	}
}

//My functions

vector<int> Sudoku::checkRow(int y,vector<int> nums)
{
    for (int i=0;i<9;i++)
    {
        if (find(nums.begin(),nums.end(),numbers[y][i]) != nums.end()) nums.erase(find(nums.begin(),nums.end(),numbers[y][i]));
    }
    return nums;
}

vector<int> Sudoku::checkCol(int x,vector<int> nums)
{
    for (int i=0;i<9;i++)
    {
        if (find(nums.begin(),nums.end(),numbers[i][x]) != nums.end()) nums.erase(find(nums.begin(),nums.end(),numbers[i][x]));
    }
    return nums;
}

vector<int> Sudoku::checkBox(int x,int y,vector<int> nums)
{
    int boxX = (int) x/3;
    int boxY = (int) y/3;

    for (int i = (boxY*3);i<((boxY+1)*3);i++)
    {
        for (int i2 = (boxX*3);i2<((boxX+1)*3);i2++)
        {
            if (find(nums.begin(),nums.end(),numbers[i][i2]) != nums.end()) nums.erase(find(nums.begin(),nums.end(),numbers[i][i2]));
        }
    }
}

void Sudoku::calcCands()
{
    for (int i=0;i<9;i++)
    {
        for (int i2=0;i2<9;i2++)
        {
            vector<int> nums = {1,2,3,4,5,6,7,8,9};
            nums = checkCol(i,nums);
            nums = checkRow(i2,nums);
            nums = checkBox(i,i2,nums);
            candNum[i2][i] = nums.size();
            cands[i2][i] = nums;
        }
    }
}