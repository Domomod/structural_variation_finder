//
// Created by dominik on 30.08.19.
//

#ifndef GAMEENGINE_SV_RESTRICTIONS_H
#define GAMEENGINE_SV_RESTRICTIONS_H


class SV_restrictions
{
public:
    int insertion_max_reference_gap = 5;
    int deltion_max_alignment_gap = 5;
    int deltion_max_size = 500;
    int invertion_max_reference_gap = 5;
};


#endif //GAMEENGINE_SV_RESTRICTIONS_H
