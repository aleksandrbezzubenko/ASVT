#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <sstream>
#include <map>
#include <iomanip>
#include <fstream>
#include <set>
#include <algorithm>

    std::string to_binary_string(unsigned int a, int count_variables)
    {
        long long n = 0;
        int k, m = 0;
        while (a) {
            k = a % 2;
            a = a / 2;
            n += k * pow(10, m);
            m++;
        }
        std::string resault = std::to_string(n);
        while (resault.size() < count_variables) {
            resault.insert(resault.begin(), '0');
        }
        return resault;
    }
    void SDNF(std::string function, int count_variables)
    {
        bool flag_plus = false;
        std::cout << "SDNF = ";
        for (int i = 0; i < function.size(); ++i) {
            if (function[i] == '1') {
                if (flag_plus) {
                    std::cout << " + ";
                }
                flag_plus = true;
                std::string set_of_variables = to_binary_string(i, count_variables);
                for (int j = 0; j < count_variables; ++j) {
                    if (set_of_variables[j] == '0') {
                        std::cout << "!X" << j;
                    }
                    else {
                        std::cout << "X" << j;
                    }
                }
                
            }
        }
    }

class Quine_McCluskey_method
{
private:
    int count_variables;
    std::string output_file_first = "first_implicants.txt";
    std::string output_file_segnificant = "significant_implicants.txt";
    std::vector<std::string> implicants;
    std::vector<std::string> first_implicants;
    std::vector<std::string> significant_implicants;
    std::vector<std::vector<bool>> table;
    std::vector<int> Haming_distance(std::string str1, std::string str2)
    {
        std::vector<int> result = {};
        for (int i = 0; i < str1.size(); ++i) {
            if (str1[i] != str2[i]) {
                result.push_back(i);
            }
        }
        return result;
    }
    bool Comparison_implicants(std::string first_implicant, std::string implicant)
    {
        for (int i = 0; i < implicant.size(); ++i) {
            if (first_implicant[i] == '~') {continue;}
            if (first_implicant[i] != implicant[i]) {
                return false;
            }
        }
        return true;
    }
    void Print_table(std::vector<std::string> str, std::string output_file)
    {
        std::ofstream fout;
        fout.open(output_file);
        fout << std::endl;
        for (int i = 0; i < implicants.size() + 1; ++i) {
            for (int j = 0; j < count_variables + 3; ++j) {
                fout << '-';
            }
        }
        fout << std::endl;
        fout << '|' << std::setfill(' ') << std::setw(count_variables + 3) << '|';
        for (auto imp : implicants) {
            fout << ' ' << imp << " |";
        }
        fout << std::endl;
        for (int i = 0; i < implicants.size() + 1; ++i) {
            for (int j = 0; j < count_variables + 3; ++j) {
                fout << '-';
            }
        }
        fout << std::endl;
        for (int i = 0; i < str.size(); ++i) {
            fout << "| " << str[i] << " |";
            for (int j = 0; j < table.size(); ++j) {
                fout << std::setfill(' ') << std::setw(count_variables + 2) << table[j][i] << '|';
            }
            fout << std::endl;
        }
        fout.close();
    }
    bool Is_original_set(std::vector<std::string> arr, std::string str)
    {
        for (auto tmp : arr) {
            if (tmp == str) { return false; }
        }
        return true;
    }

    void Find_first_implicants(std::vector<std::string> sets)
    {
        bool flag = false;
        bool last_el = false;
        std::vector<int> tmp = {};
        std::vector<std::string> results = {};
        for (int i = 0; i < sets.size() - 1; ++i) {
            bool changed = false;
            for (int j = i + 1; j < sets.size(); ++j) {
                if (j == sets.size()) {
                    continue;
                }
                std::vector<int> haming_distance = Haming_distance(sets[i], sets[j]);
                if (haming_distance.size() == 1) {

                    std::string result = "";
                    if (j + 1 == sets.size()) {
                        last_el = true;
                    }
                    for (int k = 0; k < sets[i].size(); ++k) {
                        if (k == haming_distance[0]) {
                            result.push_back('~');
                        }
                        else {
                            result.push_back(sets[i][k]);
                        }
                    }
                    changed = true;
                    tmp.push_back(i);
                    tmp.push_back(j);
                    if (Is_original_set(results, result)) {
                        
                        flag = true;
                        results.push_back(result);
                    }
                }
            }
            if (std::find(tmp.begin(), tmp.end(), i) == tmp.end()) {
                results.push_back(sets[i]);
            }
        }
        if (!last_el) {
            results.push_back(sets[sets.size() - 1]);
        }
        if (flag) {
            Find_first_implicants(results);
        }
        else {
            first_implicants = results;
        }
    }
    void Find_significant_implicants()
    {
        table.resize(implicants.size());
        std::vector<int> indexes_significant_implicants;
        for (int i = 0; i < implicants.size(); ++i) {
            table[i].resize(first_implicants.size());
            std::vector<int> indexes_first_implicants = {};
            for (int j = 0; j < first_implicants.size(); ++j) {
                if (Comparison_implicants(first_implicants[j], implicants[i])) {
                    indexes_first_implicants.push_back(j);
                    table[i][j] = true;
                }
                else {
                    table[i][j] = false;
                }
            }
            if (indexes_first_implicants.size() == 1) {
                indexes_significant_implicants.push_back(indexes_first_implicants[0]);
            }
        }
        Print_table(first_implicants, output_file_first);
        int deleted_implicants = 0;
        for (int i = 0; i < first_implicants.size(); ++i) {
            int count_1 = 0;
            for (int j = 0; j < table.size(); ++j) {
                if (table[j][i] == 1) {
                    count_1 = 0;
                    for (int k = 0; k < table[j].size(); ++k) {
                        if (table[j][k] == 1) {
                            ++count_1;
                        }
                    }
                    if (count_1 == 1) {
                        break;
                    }
                }
            }
            if (count_1 == 1) {
                significant_implicants.push_back(first_implicants[i]);
            }
            else {
                for (int j = 0; j < table.size(); ++j) {
                    table[j].erase(table[j].begin() + i);
                }
                first_implicants.erase(first_implicants.begin() + i);
                --i;
                ++deleted_implicants;
            }
        }
        Print_table(significant_implicants, output_file_segnificant);
    }
    void Print_result()
    {
        std::cout << "MDNF = ";
        bool flag_plus = false;
        for (auto imp : significant_implicants) {
            if (flag_plus) {
                std::cout << " + ";
            }
            flag_plus = true;
            int index = imp.size() - 1;
            for (int i = 0; i < imp.size(); ++i) {
                if (imp[i] == '~') { 
                    --index;
                    continue;
                }
                if (imp[i] == '0') {
                    std::cout << "!X" << index;
                }
                else {
                    std::cout << "X" << index;
                }
                --index;
            }
        }
    }
public:
    Quine_McCluskey_method(std::vector<std::string> _implicants, int _count_variables)
        : implicants(_implicants), count_variables(_count_variables) 
    {
        Find_first_implicants(implicants);
        Find_significant_implicants();
        Print_result();
    }
};


class Undefined_coefficient_method
{
private:
    int count_variables;
    std::string function;
    std::ofstream fout;
    std::vector<std::vector<std::pair<std::string, std::string>>> system;
    std::set<std::pair<std::string, std::string>> zero_coefficients;
    std::set<std::pair<std::string, std::string>> no_zero_coefficients;
    std::vector<std::pair<std::string, std::string>> significant_coefficients;
    std::string to_binary_string(unsigned int a)
    {
        long long n = 0;
        int k, m = 0;
        while (a) {
            k = a % 2;
            a = a / 2;
            n += k * pow(10, m);
            m++;
        }
        std::string resault = std::to_string(n);
        while (resault.size() < count_variables) {
            resault.insert(resault.begin(), '0');
        }
        return resault;
    }
    std::set<int> Find_indexes()
    {
        std::set<int> indexes;
        for (int i = 1; i < pow(2, count_variables); ++i) {
            std::string set_var = to_binary_string(i);
            std::string index = "";
            for (int j = 0; j < set_var.size(); ++j) {
                if (set_var[j] == '1') {
                    index.append(std::to_string(j + 1));
                }
            }
            indexes.insert(std::stoi(index));
        }
        return indexes;
    }
    void Print_equation(std::vector<std::pair<std::string, std::string>> coefficients, int j)
    {
        fout << j << ":   ";
        bool flag_plus = false;
        for (int i = 0; i < coefficients.size(); ++i) {
            if (flag_plus) {
                fout << " + ";
            }
            flag_plus = true;
            fout << coefficients[i].first << 'K' << coefficients[i].second;
        }
        fout << " = " << function[j] << std::endl;
    }
    void Print_set(std::vector<std::pair<std::string, std::string>> coefficients)
    {
        std::cout << "Significant coefficients:  ";
        bool flag_plus = false;
        for (auto coef : coefficients) {
            if (flag_plus) {
                std::cout << " + ";
            }
            flag_plus = true;
            std::cout << coef.first << 'K' << coef.second;
        }
        std::cout << std::endl;
    }
    void System_initialization(std::set<int> indexes)
    {
        for (int i = 0; i < pow(2, count_variables); ++i) {
            std::vector<std::pair<std::string, std::string>> coefficients = {};
            std::string set_var = to_binary_string(i);
            for (auto index : indexes) {
                std::string index_str = std::to_string(index);
                std::string value = "";
                for (int j = 0; j < index_str.size(); ++j) {
                    int a = static_cast<int>(index_str[j]) - 49;
                    value.push_back(set_var[a]);
                }
                coefficients.push_back(std::make_pair(index_str, value));
            }
            if (function[i] == '0') {
                for (auto coef : coefficients) {
                    zero_coefficients.insert(coef);
                }
            }
            else {
                system.push_back(coefficients);
            }
            Print_equation(coefficients, i);
        }
    }
    void Removing_zero_coefficients()
    {
        for (int i = 0; i < system.size(); ++i) {
            for (int j = 0; j < system[i].size(); ++j) {
                if (zero_coefficients.find(system[i][j]) != zero_coefficients.end()) {
                    system[i].erase(system[i].begin() + j);
                    --j;
                }
            }
            Print_equation(system[i], i);
        }
        for (int i = 0; i < system.size(); ++i) {
            for (int j = 0; j < system[i].size(); ++j) {
                no_zero_coefficients.insert(system[i][j]);
            }
        }
        std::sort(system.begin(), system.end(), [](const std::vector<std::pair<std::string, std::string>>& a, 
            const std::vector<std::pair<std::string, std::string>>& b) { return a.size() < b.size(); });
        for (int i = 0; i < system.size(); ++i) {
            Print_equation(system[i], i);
        }
    }
    void Find_significant_coefficients()
    {
        std::vector<int> all_indexes;
        for (int i = 0; i < system.size(); ++i) {
            all_indexes.push_back(i);
        }
        while (all_indexes.size() > 0) {
            std::pair<std::string, std::string> max_count_element;
            std::vector<int> max_indexes;
            for (auto coef : no_zero_coefficients) {
                std::vector<int> buf_indexes = {};
                for (int i = 0; i < all_indexes.size(); ++i) {
                    if (std::find(system[all_indexes[i]].begin(), system[all_indexes[i]].end(), coef) != system[all_indexes[i]].end()) {
                        buf_indexes.push_back(all_indexes[i]);
                    }
                }
                if (max_indexes.size() < buf_indexes.size()) {
                    max_count_element = coef;
                    max_indexes = buf_indexes;
                }
                else if (max_indexes.size() == buf_indexes.size()) {
                    if (max_count_element.first.size() > coef.first.size()) {
                        max_count_element = coef;
                        max_indexes = buf_indexes;
                    }
                }
            }
            significant_coefficients.push_back(max_count_element);
            for (auto index : max_indexes) {
                std::vector<int>::iterator to_erase = std::find(all_indexes.begin(), all_indexes.end(), index);
                all_indexes.erase(to_erase);
            }
        }
        Print_set(significant_coefficients);
    }
    void Print_result()
    {
        std::cout << "MDNF = ";
        bool flag_plus = false;
        for (auto coef : significant_coefficients) {
            if (flag_plus) {
                std::cout << " + ";
            }
            flag_plus = true;
            for (int i = 0; i < coef.first.size(); ++i) {
                if (coef.second[i] == '0') {
                    std::cout << '!';
                }
                std::cout << 'X' << coef.first[i];
            }
        }
    }
public:
    Undefined_coefficient_method(std::string _function, int _count_variables) :
        function(_function), count_variables(_count_variables)
    {
        fout.open("NK.txt");
        std::set<int> indexes = Find_indexes();
        System_initialization(indexes);
        Removing_zero_coefficients();
        Find_significant_coefficients();
        Print_result();
        fout.close();
    }

};


int main()
{
    std::string function = "0000010110000000011111011100110110101111110111110011000010001010";
    int count_variables = log2(function.size());
    std::vector<std::string> miniterms = { "000101", "000111", "001000", "010001", "010010", "010011", "010100", "010101", "010111", "011000", "011001", "011100", "011101", "011111", "100000", "100010", "100100", "100101", "100110", "100111", "101000", "101001", "101011", "101100", "101101", "101110", "101111", "110010", "110011", "111000", "111100", "111110" };
    SDNF(function, count_variables);
    Quine_McCluskey_method qm(miniterms, count_variables);
    Undefined_coefficient_method mt(function, count_variables);
}
