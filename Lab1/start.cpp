#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <chrono> 
#include "Player.h"
#include "sort_algo.h"
#include <algorithm>

/// @file start.cpp
/// @brief Основной файл программы для тестирования сортировки игроков

/// @brief Считывает данные игроков из CSV-файла
/// @return Вектор объектов Player
std::vector<Player> getPlayers(const std::string& filename) {

    std::vector<Player> players;
    std::fstream file;
    file.open(filename, std::ios::in);

    std::string line;
    getline(file, line);

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        Player p;

        getline(ss, p.country, ',');
        getline(ss, p.name, ',');
        getline(ss, p.club, ',');
        getline(ss, p.position, ','); 
        getline(ss, field, ','); 
        p.games = stoi(field);

        getline(ss, field);
        p.goals = stoi(field);

        players.push_back(p);
    }

    file.close();
    return players;
}






/// @brief Тестирование файлов
int main() {
    std::vector<std::string> filenames = {
        "data_algo/output_players100.csv",
        "data_algo/output_players174.csv",
        "data_algo/output_players305.csv",
        "data_algo/output_players534.csv",
        "data_algo/output_players935.csv",
        "data_algo/output_players1635.csv",
        "data_algo/output_players2859.csv",
        "data_algo/output_players5000.csv",
        "data_algo/output_players8743.csv",
        "data_algo/output_players15289.csv",
        "data_algo/output_players26736.csv",
        "data_algo/output_players46753.csv",
        "data_algo/output_players81756.csv",
        "data_algo/output_players142965.csv",
        "data_algo/output_players250000.csv",
    };
    
    for (std::string current_file_name : filenames) {
    //std::string current_file_name = "data_algo/output_players100.csv";
        std::string key_country = "Russia";
        std::vector<Player> st = getPlayers(current_file_name);
        
        int N = st.size();

        auto start_time = std::chrono::high_resolution_clock::now();
        // Сортировка слиянием
        merge_sort(st, 0, N);
        
        //Быстрая ортировка
        //quick_sort(st, 0, N);
        
        //Пирамидальная сортировка
        //heap_sort(st);
        
        //sort
        //std::sort(st.begin(), st.end());
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end_time - start_time;
        std::cout << duration.count() << " ms\n";
}

    return 0;
};


