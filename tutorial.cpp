#include <algorithm> // std::shuffle
#include <array>
#include <cassert>
#include <ctime> // std::time
#include <iostream>
#include <random> // std::mt19937
#include <assert.h>
#include <string>
#include "Random.h"
#include <vector>

int main()
{
    double numerator, denominator;
    std::cout << " Enter numerator: ";
    std::cin >> numerator;
    std::cout << " Enter denominator: ";
    std::cin >> denominator;
    double div = 0;

    try
    {
        if (denominator == 0)
        {
            throw std::runtime_error("Bad denominator");
        }
        div = numerator / denominator;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    std::cout << div;
    return 0;
}