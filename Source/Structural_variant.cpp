//
// Created by dominik on 30.08.19.
//

#include <iostream>
#include <iomanip>
#include "Structural_variant.h"


std::ostream &operator<<(std::ostream &os, Structural_variant const &variant)
{
    if(variant.name == "header")
    {
        os << std::setw(20) << std::left << "Name" << std::setw(20) << std::left << "Start" << std::setw(20) << std::left << "End"
            << std::setw(20) << std::left << "Length" <<  std:: setw(10) << std::left << "Reference";
    }
    else
    {
        os << std::setw(20) << std::left << variant.name << std::setw(20) << std::left << variant.start << std::setw(20) << std::left << variant.end
            << std::setw(20) << std::left << variant.length <<  std:: setw(10) << std::left << variant.refrence;
    }
    return os;
}

Structural_variant::Structural_variant(const std::string &name, const std::string &refrence, long start, long end,
                                       long length) : name(name), refrence(refrence), start(start), end(end),
                                                      length(length)
{
}
