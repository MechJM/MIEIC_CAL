#include "gtest/gtest.h"
#include "gmock/gmock.h"


#include "Tests/Sum.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <chrono>



int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    std::cout << "CAL Lab Class 01" << std::endl;


    /*
    //Statistics for calcSum
    srand(time(NULL));

    ofstream csv;
    csv.open("../calcSum_stat.csv",ios_base::trunc);


    for (int i=10;i<=500;i+=10)
    {
        long int totalTime=0;
        for (int i2=0;i2<1000;i2++)
        {
            int sequence[i];
            for (int i3=0;i3<i;i3++)
            {
                int randNum = rand() % (10*i) + 1;
                sequence[i3] = randNum;
            }
            auto start=std::chrono::high_resolution_clock::now();
            calcSum(sequence,i);
            auto finish=std::chrono::high_resolution_clock::now();
            auto mili=chrono::duration_cast<chrono::milliseconds>(finish-start).count();
            totalTime+=mili;
        }
        csv<<i<<','<<totalTime<<"\n";
    }

    csv.close();

    //End of statistics
    */

    return RUN_ALL_TESTS();;
}