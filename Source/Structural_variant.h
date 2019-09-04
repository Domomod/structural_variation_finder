//
// Created by dominik on 30.08.19.
//

#ifndef GAMEENGINE_STRUCTURAL_VARIANT_H
#define GAMEENGINE_STRUCTURAL_VARIANT_H


#include <string>

class Structural_variant
{
public:
    Structural_variant(const std::string &name, const std::string &refrence, long start, long end, long length);

    static Structural_variant create_header()
    {
        return Structural_variant("header", "header", 0, 0 ,0);
    }

    friend std::ostream &operator<<(std::ostream &os, Structural_variant const &m);

private:
    std::string name, refrence;
    long start, end, length;
};


#endif //GAMEENGINE_STRUCTURAL_VARIANT_H
