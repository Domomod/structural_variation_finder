//
// Created by dominik on 20.08.19.
//

#ifndef GAMEENGINE_QUAST_BLOCK_H
#define GAMEENGINE_QUAST_BLOCK_H

#include <map>
#include <list>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>

struct Quast_Block{
    long global_start, global_end, local_start, local_end;
    std::__cxx11::string reference;
    std::__cxx11::string contig;
    bool global_orientation, local_orientation;

    Quast_Block(long global_start, long global_end, long local_start, long local_end, const std::__cxx11::string &reference,
                const std::__cxx11::string &contig) : global_start(global_start), global_end(global_end),
                                                      local_start(local_start), local_end(local_end), reference(reference),
                                                      contig(contig)
    {
        global_orientation = global_start < global_end;
        assert(global_orientation); // I assume globally quast always map alignments in the same direction
        local_orientation  = local_start  < local_end ;
    }

    long calculate_local_distance(const Quast_Block & second) const;

    long calculate_global_distance(const Quast_Block & second) const;
};

#endif //GAMEENGINE_QUAST_BLOCK_H
