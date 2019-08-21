//
// Created by dominik on 20.08.19.
//

#ifndef GAMEENGINE_VCF_FINDER_H
#define GAMEENGINE_VCF_FINDER_H

#include "Quast_Block.h"
#include <map>
#include <list>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>

class Vcf_finder{
public:
    std::__cxx11::string local_orientation_names[2] = {"right-increasing", "left-increasing"};
    std::__cxx11::string local_orientation_symb[2] = {"->", "<-"};

    void pretty_write_list(const std::__cxx11::list<Quast_Block> & list);

    void compare_reads(std::vector<std::list<Quast_Block>>)

    void find_vcf(std::map<int, std::list<Quast_Block>> reads);
private:
};

#endif //GAMEENGINE_VCF_FINDER_H
