//
// Created by dominik on 20.08.19.
//

#ifndef GAMEENGINE_VCF_FINDER_H
#define GAMEENGINE_VCF_FINDER_H

#include "Quast_Block.h"
#include "SV_restrictions.h"
#include "Structural_variant.h"
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
    std::__cxx11::string local_orientation_symb[2] = {"<-", "->"};

    void set_sv_restrictions(const SV_restrictions &sv_restrictions);

    void pretty_write_list(const std::__cxx11::list<Quast_Block> & list);

    void find_vcf(std::map<int, std::list<Quast_Block>> reads);

    void write_result_to_files(std::string folder_name);
private:

    SV_restrictions sv_restrictions;

    void same_chromosome_analysis(const Quast_Block &current_block, int i,
                                  const Quast_Block &next_block);

    void diffrent_chromosome_analysis(const Quast_Block &current_block, int i,
                                      const Quast_Block &next_block);

    bool inversion_check(const Quast_Block& first, const Quast_Block& second);

    std::stringstream number_printer;

    void write_list_to_file(std::string folder_name, std::string file_name, const std::list<Structural_variant>& list);

    int insertion_num;
    int deletion_num;
    int inversion_num;
    int translocation_num;
    int relocation_num;

    std::list<Structural_variant> insertions;
    std::list<Structural_variant> deletions;
    std::list<Structural_variant> inversions;
    std::list<Structural_variant> translocations;
    std::list<Structural_variant> relocations;
};

#endif //GAMEENGINE_VCF_FINDER_H
