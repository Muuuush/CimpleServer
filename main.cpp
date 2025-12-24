#include <iostream>
#include <boost/lexical_cast.hpp>

int main() {
    std::cout << "Hello world!" << std::endl;

    std::cout << "Input your height (meter): ";
    float height;
    std::cin >> height;
    height *= 100;
    std::string heightString = boost::lexical_cast<std::string>(height);
    std::cout << "So your are " << heightString << " cm height." << std::endl;
}
