#include "cpp-challenge.h"
using namespace std;

//gets the instant accelerations and timestamps from a file
void read_instant_accelerations(ifstream& file, tuple<size_t, vector3>& output)
{
    string current = "";
    getline(file, current, ','); //get timestamp
    if (file.eof())
        return;
    get<0>(output) = stoul(current.c_str());

    for (int i = 0; i < 3; i++)
    {
        getline(file, current, ','); //get timestamp
        get<1>(output)[i] = stof(current.c_str());
    }

    getline(file, current, '\n'); //advance to the next line
}

//get the mean acceleration vector for a specified time window from a csv file
// file: the open file
// start: the timestamp of the window start
// end: the timestamp of the window end
// instant_data_current: tuple from previous time frame. On initial use (-1, [0 0 0]) to prevent including in average
vector3 mean_accelerations(ifstream& file, const size_t& start, const size_t& end, tuple<size_t, vector3>& instant_data_last)
{
    //the average vector
    vector3 mean_acc = {0.0f, 0.0f, 0.0f};
    size_t n = 0;

    //if last line was in this time frame, include it in the sum
    if (get<0>(instant_data_last) >= start && get<0>(instant_data_last) < end)
    {
        for (int i = 0; i < 3; i++)
            mean_acc[i] += get<1>(instant_data_last)[i];
        n++;
    }

    //the file data
    tuple<size_t, vector3> instant_data_current = make_tuple<size_t, vector3>(0, vector3{0});

    while (!file.eof())
    {
        //read data from file
        read_instant_accelerations(file, instant_data_current);

        //include in average
        if (get<0>(instant_data_current) < end)
        {
            for (int i = 0; i < 3; i++)
                mean_acc[i] += get<1>(instant_data_current)[i];
            n++;
        }

        //now out of this time window
        else
        {
            instant_data_last = instant_data_current;
            break;
        }
    }

    for (int i = 0; i < 3 && n > 0; i++)
        mean_acc[i] /= n;
    return mean_acc;
}

//get the mean acceleration vectors for each time window of size stride  from a csv file
//csv_path: path to the data. Return empty table if bad
//window_size: the total size (ms) of the data that needs to be reduced to averages (left inclusive). Value of n yields window [0, n)
//stride: the size of each disjoint subwindow (ms) again left inclusive. Value of q will split [0, n) into [0, q), [q, 2q), ...
vector_table get_mean_accelerations(const string& csv_path, const size_t& window_size, const size_t& stride)
{
    auto output = vector_table(window_size / stride);

    ifstream file(csv_path.c_str());

    if (!file.good()) return vector_table(0);
    while (file.get() != '\n');

    size_t start = 0;
    size_t end = stride;
    tuple<size_t, vector3> instant_data_last = make_tuple<size_t, vector3>(-1, vector3{0}); //-1 for timestamp to prevent averaging this in initial run
    for (int i = 0; i < output.n; ++i)
    {
        //read each mean vector
        output[i] = mean_accelerations(file, start, end, instant_data_last);
        //increment time window
        start += stride;
        end += stride;
    }

    return output;
}
