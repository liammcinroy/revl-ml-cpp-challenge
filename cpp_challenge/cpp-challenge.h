#include <array>
#include <fstream>
#include <memory>
#include <string>
#include <tuple>

//3D float vector
typedef std::array<float, 3> vector3;

//basic container for multiple vectors
struct vector_table
{
    //the number of vectors contained
    size_t n;
    //the data
    std::unique_ptr<vector3[]> data;

    //create an empty table
    vector_table(size_t size)
    {
        n = size;
        data = std::unique_ptr<vector3[]>(new vector3[n]);
    }

    //basic access
    inline vector3& operator[](int x)
    {
        return data[x];
    }
};

//get the mean acceleration vectors for each time window of size stride  from a csv file
vector_table get_mean_accelerations(const std::string&, const size_t&, const size_t&);
