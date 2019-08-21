//
// Created by dominik on 20.08.19.
//

#ifndef GAMEENGINE_QUAST_TSV_READER_H
#define GAMEENGINE_QUAST_TSV_READER_H


#include "Quast_Block.h"
#include <map>
#include <list>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <regex>

class Quast_TSV_Reader{
public:
    Quast_TSV_Reader()
    {

    }


    const std::regex number_regex {"^[^0-9]*([0-9]+)$"};
    int name_to_number(const std::string & name)
    {
        std::smatch matchResult;
        if (!std::regex_match(name, matchResult, number_regex)) {
            // Handle failure to match
            abort();
        }
        return std::stoi(matchResult.str(1));
    }


    bool is_first_word_number(const std::string &s)
    {
        size_t first_space_position = s.find_first_of(" \t\n");
        return first_space_position != std::string::npos && !s.empty() && std::all_of(s.begin(), s.begin() + first_space_position, ::isdigit);
    }


    std::map<int, std::__cxx11::list<Quast_Block>> read_file(const std::__cxx11::string & path)
    {
        file.open(path);
        if( file.fail() ){
            std::cerr << "File " << path << " - Error: " << strerror(errno);
        }

        std::map<int, std::__cxx11::list<Quast_Block>> reads;
        int previous_block_number = -1;
        if (file.is_open())
        {
            std::__cxx11::string line;
            while ( getline(file, line) )
            {
                if(is_first_word_number(line))
                {
                    std::stringstream sstream(line);
                    long global_start, global_end, local_start, local_end;
                    std::__cxx11::string reference, block_name;
                    sstream >> global_start >> global_end >> local_start >> local_end >> reference >> block_name;
                    int block_number = name_to_number(block_name);
                    if(previous_block_number != block_number)
                    {
                        reads.emplace(block_number, std::__cxx11::list<Quast_Block>());
                    }
                    reads.at(block_number).emplace_back(global_start, global_end, local_start, local_end, reference, block_name);
                    previous_block_number = block_number;
                }
            }
        }
        file.close();

        return reads;
    }


    ~Quast_TSV_Reader(){
    }

private:
    std::fstream file;
};

#endif //GAMEENGINE_QUAST_TSV_READER_H
