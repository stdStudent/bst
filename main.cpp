#include <iostream>
#include <string>

#include "ddt.h"

int main() {
    ddt<std::string> d("abcd", "Maks");

    d.insert("aa", "Car");
    d.insert("aa", "Car1"); // Yay, no duplicates!
    d.insert("bb", "Car");
    d.insert("badc", "Train");
    d.insert("bcdc", "Heaven");
    d.insert("a1a", ".");
    d.insert("b2b", "loop");

    d.print();
    std::cout << d.find("bcdc") << std::endl;

    d.remove("a1a");
    d.remove("abcd");
    d.print();

    d.reinit("a4r22", "try");
    d.insert("abbbd", "cool");
    d.insert("accbdd", "not cool");
    d.print();

    return 0;
}
