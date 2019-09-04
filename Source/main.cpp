#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cstring>
#include <list>
#include <map>

#include "Quast_Block.h"
#include "Quast_TSV_Reader.h"
#include "VCF_Finder.h"
#include "TinyXML2/tinyxml2.h"

const std::string ma_T1_path  = "../quast_wyniki/quast_results/results_2019_03_07_11_37_32/contigs_reports/all_alignments_ma_T1.tsv";
const std::string ma_T0_path  = "../quast_wyniki/quast_results/results_2019_03_07_11_37_32/contigs_reports/all_alignments_ma_T0.tsv";
const std::string ma_ref_path = "../quast_wyniki/quast_results/results_2019_03_07_11_37_32/contigs_reports/all_alignments_ref_ma.tsv";

std::pair<std::string, SV_restrictions> read_init();

int main ()
{
    std::pair<std::string, SV_restrictions> init_params = read_init();
    Quast_TSV_Reader quast_tsv_reader;
    Vcf_finder vcf_finder;
    std::map<int, std::list<Quast_Block>> reads1 = quast_tsv_reader.read_file(init_params.first);
    vcf_finder.set_sv_restrictions(init_params.second);
    vcf_finder.find_vcf(reads1);
    vcf_finder.write_result_to_files("Results");
}



std::pair<std::string, SV_restrictions> read_init()
{
    tinyxml2::XMLDocument program_init;
    program_init.LoadFile("../init.xml");
    const char* data_path = program_init.FirstChildElement("program_init")
            ->FirstChildElement("alignment_data")
            ->GetText();

    SV_restrictions sv_restrictions;

    sv_restrictions.invertion_max_reference_gap = static_cast<int>(std::strtol(program_init.FirstChildElement("program_init")
                                                                        ->FirstChildElement("insertion_max_reference_gap")->GetText(), nullptr, 10));

    sv_restrictions.deltion_max_alignment_gap = static_cast<int>(std::strtol(program_init.FirstChildElement("program_init")
                                                                        ->FirstChildElement("deltion_max_alignment_gap")->GetText(), nullptr, 10));

    sv_restrictions.deltion_max_size = static_cast<int>(std::strtol(program_init.FirstChildElement("program_init")
                                                                        ->FirstChildElement("deltion_max_size")->GetText(), nullptr, 10));

    sv_restrictions.invertion_max_reference_gap = static_cast<int>(std::strtol(program_init.FirstChildElement("program_init")
                                                                        ->FirstChildElement("invertion_max_reference_gap")->GetText(), nullptr, 10));

    return std::pair<std::string, SV_restrictions>(std::string(data_path), sv_restrictions);
}
#pragma clang diagnostic pop