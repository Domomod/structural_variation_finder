//
// Created by dominik on 20.08.19.
//


#include "VCF_Finder.h"
#include <iomanip>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>

void Vcf_finder::pretty_write_list(const std::__cxx11::list<Quast_Block> &list)
{
    const int column_size = 15;
    std::cout << std::setw(column_size) << "Chromosomes:";
    for(auto & block : list)
    {
        std::cout << std::setw(column_size) << std::right << block.reference;
    }
    std::cout << "\n";

    std::cout << std::setw(column_size) << "Orientation:";
    for(auto & block : list)
    {
        std::cout << std::setw(column_size) << std::right << local_orientation_symb[block.local_orientation];
    }
    std::cout << "\n";

    std::cout << std::setw(column_size) << "Length:";
    for(auto & block : list)
    {
        std::cout << std::setw(column_size) << std::right << std::abs(block.local_end - block.local_start);
    }
    std::cout << "\n";

    std::cout << std::setw(column_size) << "Local cords:";
    for(auto & block : list)
    {
        std::stringstream s;
        s << block.local_start << " - "  << block.local_end;
        std::cout << std::setw(column_size) << std::right << s.str();
    }
    std::cout << "\n";

    std::cout << std::setw(column_size) << "Global start:";
    for(auto & block : list)
    {
        std::cout << std::setw(column_size) << std::right << block.global_start;
    }
    std::cout << "\n";

    std::cout << std::setw(column_size) << "Global end:";
    for(auto & block : list)
    {
        std::cout << std::setw(column_size) << std::right << block.global_end;
    }
    std::cout << "\n";
}



void Vcf_finder::find_vcf(std::map<int, std::list<Quast_Block>> reads)
{
    insertion_num = 0;
    deletion_num = 0;
    inversion_num = 0;
    translocation_num = 0;
    relocation_num = 0;

    for(const auto &  [block_name, blocks_list] : reads)
    {
        if(blocks_list.size() > 1)
        {
            std::cout << "\n" << block_name << " is a potential VCF (block divided into " << blocks_list.size() << " pieces)\n";

            pretty_write_list(blocks_list);

            auto current_block = blocks_list.begin();

            current_block = blocks_list.begin();
            for(int i = 0; i < blocks_list.size() - 1; i++)
            {
                auto next_block = current_block;
                next_block++;
                if(current_block->mapped_onto_same_chromosome_as(*next_block))
                {
                    same_chromosome_analysis(*current_block, i, *next_block);
                }
                else
                {
                    diffrent_chromosome_analysis(*current_block, i, *next_block);

                }
                current_block++;
            }
            std::cout << "================================================\n";
        }
    }

    std::cout << "\n\n\tEnd.\n";
}



void
Vcf_finder::same_chromosome_analysis(const Quast_Block &current_block,
                                     int i,
                                     const Quast_Block &next_block)
{
    std::cout << "\nTwo blocks: " << i << " and " << i + 1 << " were mapped onto the same chromosome.\n";
    long alignment_gap = current_block.calculate_alignment_gap(next_block);
    std::cout << "The alignment distance between mapped blocks is " << alignment_gap << "\n";
    long reference_gap = current_block.calculate_reference_gap(next_block);
    std::cout << "The refrence distance between mapped blocks is " << reference_gap << "\n";
    auto anomaly_coordinates = current_block.calculate_global_gap_coords(next_block);

    if(current_block.local_orientation != next_block.local_orientation && reference_gap < sv_restrictions.invertion_max_reference_gap)
    {
        number_printer << inversion_num;
        inversion_num++;
        auto outer_coordinates = current_block.calculate_outer_coords(next_block);
        inversions.emplace_back("Inversion" + number_printer.str(), current_block.reference, outer_coordinates.first, outer_coordinates.second, outer_coordinates.second - outer_coordinates.first);
        number_printer.str("");
        std::cout << "THIS IS AN INVERSION\n";
        return; //EXIT IF STRUCTURAL VARIANT FOUND
    }

    if(alignment_gap > reference_gap && reference_gap < sv_restrictions.insertion_max_reference_gap)
    {
        number_printer << insertion_num;
        insertion_num++;
        insertions.emplace_back("Insertion" + number_printer.str(), current_block.reference, anomaly_coordinates.first, anomaly_coordinates.second, alignment_gap);
        number_printer.str("");
        std::cout << "THIS IS AN INSERTION\n";
        return; //EXIT IF STRUCTURAL VARIANT FOUND
    }

    if(reference_gap > alignment_gap && reference_gap << sv_restrictions.deltion_max_size && alignment_gap < sv_restrictions.deltion_max_alignment_gap)
    {
        number_printer << deletion_num;
        deletion_num++;
        deletions.emplace_back("Deletion" + number_printer.str(), current_block.reference, anomaly_coordinates.first, anomaly_coordinates.second, reference_gap);
        number_printer.str("");
        std::cout << "THIS IS A DELETION\n";
        return; //EXIT IF STRUCTURAL VARIANT FOUND
    }

    if(reference_gap == alignment_gap)
    {
        std::cout << "THIS I DO NOT KNOW WHAT IS. BOTH GAPS ARE OF THE SAME SIZE.\n";
        return; //EXIT IF STRUCTURAL VARIANT FOUND
    }
}



void Vcf_finder::diffrent_chromosome_analysis(const Quast_Block &current_block, int i, const Quast_Block &next_block){
    std::cout << "Two blocks: " << i << " and " << i + 1 << " were mapped onto diffrent chromosomes.\n";
    long alignment_gap = current_block.calculate_alignment_gap(next_block);
    std::cout << "The local distance between mapped blocks is " << alignment_gap << "\n";

    number_printer << translocation_num;
    translocation_num++;
    translocations.emplace_back("Translocation" + number_printer.str(), current_block.reference + " & " + next_block.reference, current_block.global_start, next_block.global_start, 0);
    number_printer.str("");
    std::cout << "THIS IS A TRANSLOCATION\n";

    return; //EXIT IF STRUCTURAL VARIANT FOUND
}



void Vcf_finder::write_result_to_files(std::string folder_name)
{
    write_list_to_file(folder_name, "Insertions", insertions);
    write_list_to_file(folder_name, "Deletions", deletions);
    write_list_to_file(folder_name, "Inversions", inversions);
    write_list_to_file(folder_name, "Translocations", translocations);
    write_list_to_file(folder_name, "Relocations", relocations);
}



void Vcf_finder::write_list_to_file(std::string folder_name, std::string file_name,
                                    const std::list<Structural_variant> &list)
{
    if(!list.empty())
    {
        std::fstream file;
        file.open(folder_name + "/" + file_name, std::ofstream::out);
        if(!file.is_open())
        {
            std::cerr << "Could not create file " << folder_name << "/" << file_name
                      << "\nWill try to create the folder\n";
            if (mkdir(folder_name.c_str(), 0700) == -1)
            {
                std::cerr << "Error :  " << strerror(errno) << "\n";
                std::cerr << "Could not create folder, aborting program.\n";
                abort();
            }
            else
            {
                 std::cout << "Directory created, trying to open file again\n";
                file.open(folder_name + "/" + file_name, std::ofstream::out);
                if(file.is_open())
                {
                    std::cerr << "File created succesfully\n";
                }
                else
                {
                    std::cerr << "Task failed succesfully ;), aborting program.\n The file that could not be created was " << folder_name <<"/" <<file_name <<"\n";
                }
            }
        }

        file << Structural_variant::create_header() << "\n";
        for(auto & structural_variant : list)
        {
            file << structural_variant << "\n";
        }
    }
}

bool Vcf_finder::inversion_check(const Quast_Block &first, const Quast_Block &second)
{
    return first.local_orientation != second.global_orientation;
}

void Vcf_finder::set_sv_restrictions(const SV_restrictions &sv_restrictions)
{
    Vcf_finder::sv_restrictions = sv_restrictions;
}
