#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Tests/Sum.h"


int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    std::cout << "CAL Lab Class 01" << std::endl;

    //Statistics for calcSum
    int sequence2[9] = {6,1,10,3,2,6,7,2,4};
    calcSum(sequence2,500);

    return RUN_ALL_TESTS();;
}