/*
 * labirinth.cpp
 */

#include "Labirinth.h"

#include <iostream>
using namespace std;



Labirinth::Labirinth(int values[10][10])
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			labirinth[i][j] = values[i][j];
    initializeVisited();
}


void Labirinth::initializeVisited()
{
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 10; j++)
			visited[i][j] = false;
}




void  Labirinth::printLabirinth()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			cout << labirinth[i][j] << " ";

		cout << endl;
	}
}


bool Labirinth::findGoal(int x, int y)
{

    if (visited[y][x]) return false;
    else visited[y][x] = true;

    if (labirinth[y][x] == 0) return false;

	if (labirinth[y][x] == 2)
	{
	    cout<<"Reached the end!\n";
	    return true;
	}

	//UP
	if (findGoal(x,y-1)) return true;
	else
    {
	    //RIGHT
	    if (findGoal(x+1,y)) return true;
        else
	    {
	        //DOWN
	        if (findGoal(x,y+1)) return true;
            else
	        {
	            //LEFT
	            return findGoal(x-1,y);
            }
        }
    }

}


