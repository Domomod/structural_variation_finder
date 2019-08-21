//
// Created by dominik on 20.08.19.
//


#include "VCF_Finder.h"
#include "Quast_Block.h"

long Quast_Block::calculate_local_distance(const Quast_Block &second) const
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

long Quast_Block::calculate_global_distance(const Quast_Block &second) const
{
    assert(this->reference  == second.reference);
    return std::max(this->global_end - second.global_start, second.global_end - this->global_start);
}