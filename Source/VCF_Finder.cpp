//
// Created by dominik on 20.08.19.
//


#include "VCF_Finder.h"
#include <iomanip>

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
                if(current_block->reference != next_block->reference)
                {
                    std::cout << "Two blocks: " << i << " and " << i + 1 << " were mapped onto diffrent chromosomes.\n";
                    if(current_block->local_orientation == next_block->local_orientation)
                    {
                        std::cout << "Blocks are both oriented: " << local_orientation_names[current_block->local_orientation];
                    }
                    else
                    {
                        std::cout << "Blocks are oriented: " << local_orientation_names[current_block->local_orientation] << "\t" << local_orientation_names[next_block->local_orientation] << "\n -> This might be inversion + translocation.";
                    }
                    std::cout << "\n";
                    long local_distance = current_block->calculate_local_distance(*next_block);
                    std::cout << "The local distance between mapped blocks is " << local_distance << "\n";
                }
                else
                {
                    std::cout << "\nTwo blocks: " << i << " and " << i + 1 << " were mapped onto the same chromosome.\n";
                    long local_distance = current_block->calculate_local_distance(*next_block);
                    std::cout << "The local distance between mapped blocks is " << local_distance << "\n";
                    long global_distance = current_block->calculate_global_distance(*next_block);
                    std::cout << "The global distance between mapped blocks is " << global_distance;
                    if(local_distance > global_distance)
                    {
                        std::cout << "\n -> This might be an insertion\n\n";
                    }
                    else
                    {
                        std::cout << "\n -> This might be a deletion or relocation \n\n";
                    }
                }
                current_block++;
            }
            std::cout << "================================================\n";
        }
    }
}

