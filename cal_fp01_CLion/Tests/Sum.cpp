/*
 * Sum.cpp
 */

#include "Sum.h"
#include <chrono>
#include <vector>
#include <iostream>

using namespace std;

string calcSum(int* sequence, int size)
{
    auto start=std::chrono::high_resolution_clock::now();

    vector<int> minSum, index;
    string result = "";
    int sum = 0, count = 0;
    for (int i = 0; i < size ; i++) {
        for (int j = i; j < size ; j++) {
            if(i == 0){
                sum += sequence[j];
                minSum.push_back(sum);
                index.push_back(i);
            }
            else{
                sum += sequence[j];
                if(minSum.at(count) > sum){
                    minSum.at(count) = sum;
                    index.at(count) = i;
                }
                count++;
            }
        }
        count = 0;
        sum = 0;
    }
    for (int k = 0; k < minSum.size() ; k++) {
        result += to_string(minSum.at(k)) + "," + to_string(index.at(k)) + ";";
    }

    auto finish=std::chrono::high_resolution_clock::now();

    auto mili=chrono::duration_cast<chrono::milliseconds>(finish-start).count();

    cout<<"Time in milliseconds: "<<mili<<endl;

    return result;

}
