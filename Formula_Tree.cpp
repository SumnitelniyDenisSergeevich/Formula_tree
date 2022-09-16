#include <iostream>
#include <string>
#include <list>


#include "Tree.h"

using namespace std::literals;

int main()
{
    setlocale(LC_ALL, "Russian");
    std::cout << "Введите формулу:" << std::endl;
    std::string formula;// = "2*3+(x+1)*2+21"s;
    std::getline(std::cin, formula);
    Tree tree(formula);
    std::cout << "Введите начало и конец участка поиска экстремума:" << std::endl;
    double start, end;
    std::cin >> start >> end;
    if (start > end) {
        std::swap(start, end);
    }


    //найти все экстремумы
    std::list<std::pair<double,double>> extremums;
    double step = 1e-3;
    double old_val = tree.Calc(start);
    bool up;
    if (double oldest = tree.Calc(start - step); oldest > old_val) {
        up = false;
    }
    else {
        up = true;
    }
    while (start < end) {
        double new_val = tree.Calc(start + step);
        if (new_val > old_val && up == false) {
            extremums.push_back({ start,old_val });
            up = true;
        }
        else if (new_val < old_val && up == true) {
            extremums.push_back({ start,old_val });
            up = false;
        }
        old_val = new_val;
        start += step;
    }
    std::cout << "Экстремумы:" << std::endl;
    up = false;
    for (const auto [extr, val] : extremums) {
        std::cout << "x: "s << extr << "   y:"s << val << std::endl;
    }
}
