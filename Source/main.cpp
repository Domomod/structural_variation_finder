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

const std::string ma_T1_path  = "../quast_wyniki/quast_results/results_2019_03_07_11_37_32/contigs_reports/all_alignments_ma_T1.tsv";
const std::string ma_T0_path  = "../quast_wyniki/quast_results/results_2019_03_07_11_37_32/contigs_reports/all_alignments_ma_T0.tsv";
const std::string ma_ref_path = "../quast_wyniki/quast_results/results_2019_03_07_11_37_32/contigs_reports/all_alignments_ref_ma.tsv";

int main ()
{
	Quast_TSV_Reader quast_tsv_reader;
	Vcf_finder vcf_finder;
    std::map<int, std::list<Quast_Block>> reads1 = quast_tsv_reader.read_file(ma_T1_path);
    std::map<int, std::list<Quast_Block>> reads2 = quast_tsv_reader.read_file(ma_T0_path);
    std::map<int, std::list<Quast_Block>> reads3 = quast_tsv_reader.read_file(ma_ref_path);
    vcf_finder.find_vcf(reads1);
    vcf_finder.find_vcf(reads2);
    vcf_finder.find_vcf(reads3);
}
#pragma clang diagnostic pop