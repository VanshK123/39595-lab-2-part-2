#include <iostream>

#include "hash_map.h"

int main(int argc, char *argv[])
{
    hash_map<int,float> map(11, 0.25, 0.75);

    if (map.get_size() != 0)
    {
        std::cout << "Invalid size" << std::endl;
        exit(1);
    }

    map.insert(3, 4);
    map.insert(4, 5);

    if (map.get_size() != 2)
    {
        std::cout << "Invalid size" << std::endl;
        exit(1);
    }

    if (!map.get_value(3).has_value())
    {
        std::cout << "expected 3 to be in map but it wasn't" << std::endl;
        exit(1);
    }

    if (!map.remove(3))
    {
        std::cout << "Failed to remove 3 from map" << std::endl;
        exit(1);
    }

    if (map.get_value(3).has_value())
    {
        std::cout << "Unexpected 3 in map" << std::endl;
        exit(1);
    }
}
