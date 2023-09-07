#include <iostream>
float readNumber()
{
    std::cout << "Enter an integer:";
    float x;
    std::cin >> x;
    return x;
};
void writeAnswer(float ans)
{
    std::cout << ans;
}