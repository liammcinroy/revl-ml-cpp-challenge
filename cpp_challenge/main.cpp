#include <iostream>
#include <iomanip>

#include "cpp-challenge.h"

using namespace std;

//output the values to compare to true values
bool run_test(const string& path, const size_t& window_size, const size_t& stride)
{
    auto table = get_mean_accelerations(path, window_size, stride);
    cout << "RUNNING TEST (given path:" << path
    << ", window: [0, " << window_size << "], stride: " << stride << ")" << endl;

    int start = 0, end = stride;
    for (int i = 0; i < table.n; ++i)
    {
        cout <<"\t [" << start << ":" << end << ")" << "  [";
        for (int j = 0; j < 3; ++j)
            cout << table[i][j] << (j < 2 ? ", " : "]");
        cout << endl;
        start += stride;
        end += stride;
    }
}

int main(int argc, char const *argv[])
{

    run_test("cpp-challenge-data.csv", 500, 100); //simple test
    run_test("cpp-challenge-data.csv", 100, 100); //check works the same for different size windows
    run_test("cpp-challenge-data.csv", 140200, 140200); //check average over everything, going over filesize
    run_test("cpp-challenge-data.csv", 1000, 33); //check small stride for disjoint

    run_test("badpath", 0, 1); //check bad path handling

    return 0;
}
