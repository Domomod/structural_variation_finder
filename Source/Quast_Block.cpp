//
// Created by dominik on 20.08.19.
//


#include "VCF_Finder.h"
#include "Quast_Block.h"
#include <climits>

long Quast_Block::calculate_alignment_gap(const Quast_Block &second) const
{
    /* When calculating local distance between blocks one must take into acccount:
     *  -> which way they are oriented
     *  -> are they both oriented the same way
     *  -> which one is first in contig
     * This equation allows to avoid overly complicated statements.
     * */
    if(std::min(this->local_start, this->local_end) >= std::max(second.local_start, second.local_end)  )
    {
        return std::min(this->local_start, this->local_end) - std::max(second.local_start, second.local_end);
    }
    else
    {
        return std::min(second.local_start, second.local_end) - std::max(this->local_start, this->local_end);
    }

}

/*  @Description:
 *      A function to calculate the distance between two mapped blocks. Global means it's the number of base pairs between
 *      the two allignments in the reference genome.
 *      IF ONE BLOCK IS FULLY CONTAINED IN THE OTHER, THE FUNCTION WILL BREAK THE PROGRAM (I assume such input is broken)
 *  @Input:
 *      second - the other block, there is no restriction, on which chromosome or in what order is it alligned compared
 *      to Quast_Block calling this method.
 *  @Output:
 *      LONG_MIN if the blocks are on mapped onto diffrent chromosomes
 *      otherwise
 *      Positive value, if the blocks do not overlap
 *      Negative value if the blocks overlap.
 **/
long Quast_Block::calculate_reference_gap(const Quast_Block &second) const
{
    if(this->reference != second.reference){
        return LONG_MIN;
    }
    //Check which block maps earlier onto the genome
    if(this->global_start < second.global_start)
    {
        assert(this->global_end <= second.global_end); //Assert that one block is not fully contained in the other block
        return second.global_start - this->global_end;
    }
    else{
        assert(this->global_end >= second.global_end); //Assert that one block is not fully contained in the other block
        return this->global_start - second.global_end;
    }
}

std::pair<long,long> Quast_Block::calculate_global_gap_coords(const Quast_Block &second) const
{
    if(this->reference != second.reference){
        return std::pair<long,long>(LONG_MIN, LONG_MIN);
    }
    //Check which block maps earlier onto the genome
    if(this->global_start < second.global_start)
    {
        assert(this->global_end <= second.global_end); //Assert that one block is not fully contained in the other block
        //Assure correct order of those two coordinates (blocks might intersect)
        if(this->global_end < second.global_start)
        {
            return std::pair<long, long>(this->global_end, second.global_start);
        }
        else
        {
            return std::pair<long, long>(second.global_start, this->global_end);
        }
    }
    else{
        assert(this->global_end >= second.global_end); //Assert that one block is not fully contained in the other block
        if(this->global_start < second.global_end)
        {
            return std::pair<long, long>(this->global_start, second.global_end);
        }
        else
        {
            return std::pair<long, long>(second.global_end, this->global_start);
        }
    }
}

bool Quast_Block::mapped_onto_same_chromosome_as(const Quast_Block &second) const
{
    return reference == second.reference;
}

std::pair<long, long> Quast_Block::calculate_outer_coords(const Quast_Block &second) const
{
    if(this->reference != second.reference){
        return std::pair<long,long>(LONG_MIN, LONG_MIN);
    }
    //Check which block maps earlier onto the genome
    if(this->global_start < second.global_start)
    {
        assert(this->global_end <= second.global_end); //Assert that one block is not fully contained in the other block
        return std::pair<long, long>(this->global_start, second.global_end);
    }
    else{
        assert(this->global_end >= second.global_end); //Assert that one block is not fully contained in the other block
        return std::pair<long, long>(second.global_start, this->global_end);
    }
}
