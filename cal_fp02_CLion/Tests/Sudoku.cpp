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
    if (isComplete() && checkMistakes()) return true;

    if (!checkMistakes()) return false;

    int min = calcCands();

    cout<<"Min: "<<min<<endl;

    for (int i = 0; i < 9; ++i) {
        for (int i2 = 0; i2 < 9; ++i2) {
            int count = -1;
            if (candNum[i2][i] == min && numbers[i2][i] == 0)
            {
                do
                {
                    if (count == (candNum[i2][i] - 1)) break;
                    placeNum(i2,i,cands[i2][i].at(++count));
                    if (solve()) return true;
                    else removeNum(i2,i,cands[i2][i].at(count));
                }
                while (!checkMistakes());
            }
        }
    }


	return false;
}

void Sudoku::printCands()
{
    for (int i = 0; i < 9; i++)
    {
        for (int a = 0; a < 9; a++)
            cout << this->candNum[i][a] << " ";

        cout << endl;
    }
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
            if (find(nums.begin(),nums.end(),numbers[i2][i]) != nums.end()) nums.erase(find(nums.begin(),nums.end(),numbers[i2][i]));
        }
    }
    return nums;
}

int Sudoku::calcCands()
{
    int min=90;
    for (int i=0;i<9;i++)
    {
        for (int i2=0;i2<9;i2++)
        {
            cout<<"x: "<<i<<endl<<"y: "<<i2<<endl<<endl;
            if (numbers[i2][i] != 0)
            {
                candNum[i2][i] = 0;
                vector<int> vect;
                vect.clear();
                cands[i2][i] = vect;
            } else
            {vector<int> nums = {1,2,3,4,5,6,7,8,9};
            nums = checkCol(i,nums);
            cout<<nums.size()<<endl;
            nums = checkRow(i2,nums);
            cout<<nums.size()<<endl;
            nums = checkBox(i,i2,nums);
            cout<<nums.size()<<endl<<endl;
            if (nums.size() < min && nums.size() > 0) min = nums.size();
            candNum[i2][i] = nums.size();
            cands[i2][i] = nums;}
        }
    }
    return min;
}

void Sudoku::placeNum(int x,int y,int num)
{
    numbers[y][x] = num;
    countFilled++;
}

void Sudoku::removeNum(int x,int y,int num)
{
    numbers[y][x] = 0;
    countFilled--;
}

bool Sudoku::checkRowMistake(int y)
{
    vector<int> freqs = {0,0,0,0,0,0,0,0,0};

    for (int i=0;i<9;i++)
    {
        switch (numbers[y][i])
        {
            case 1: {freqs.at(0)++; break;}
            case 2: {freqs.at(1)++; break;}
            case 3: {freqs.at(2)++; break;}
            case 4: {freqs.at(3)++; break;}
            case 5: {freqs.at(4)++; break;}
            case 6: {freqs.at(5)++; break;}
            case 7: {freqs.at(6)++; break;}
            case 8: {freqs.at(7)++; break;}
            case 9: {freqs.at(8)++; break;}
            default: break;
        }
    }

    for (int i: freqs) if (i > 1) return false;

    return true;
}

bool Sudoku::checkColMistake(int x)
{
    vector<int> freqs = {0,0,0,0,0,0,0,0,0};

    for (int i=0;i<9;i++)
    {
        switch (numbers[i][x])
        {
            case 1: {freqs.at(0)++; break;}
            case 2: {freqs.at(1)++; break;}
            case 3: {freqs.at(2)++; break;}
            case 4: {freqs.at(3)++; break;}
            case 5: {freqs.at(4)++; break;}
            case 6: {freqs.at(5)++; break;}
            case 7: {freqs.at(6)++; break;}
            case 8: {freqs.at(7)++; break;}
            case 9: {freqs.at(8)++; break;}
            default: break;
        }
    }

    for (int i: freqs) if (i > 1) return false;

    return true;
}

bool Sudoku::checkBoxMistake(int x,int y)
{
    int boxX = (int) x/3;
    int boxY = (int) y/3;

    vector<int> freqs = {0,0,0,0,0,0,0,0,0};

    for (int i = (boxX*3); i < ((boxX+1)*3); ++i) {
        for (int i2 = (boxY*3); i2 < ((boxY+1)*3); ++i2) {
            switch (numbers[i2][i])
            {
                case 1: {freqs.at(0)++; break;}
                case 2: {freqs.at(1)++; break;}
                case 3: {freqs.at(2)++; break;}
                case 4: {freqs.at(3)++; break;}
                case 5: {freqs.at(4)++; break;}
                case 6: {freqs.at(5)++; break;}
                case 7: {freqs.at(6)++; break;}
                case 8: {freqs.at(7)++; break;}
                case 9: {freqs.at(8)++; break;}
                default: break;
            }
        }
    }

    for (int i: freqs) if (i > 1) return false;

    return true;
}

bool Sudoku::checkMistakes()
{
    for (int i = 0; i < 9; ++i) {
        for (int i2 = 0; i2 < 9; ++i2) {
            if (!(checkColMistake(i2) && checkRowMistake(i) && checkBoxMistake(i2,i))) return false;
        }
    }
    return true;
}