#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <sstream>
#include <chrono> 
#include "Player.h"
#include "search.h"
#include <map>

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
    
        std::string key_country = "Russia";
        std::vector<Player> st = getPlayers(current_file_name);
        
        BinarySearchTree bst;
        for (const auto& player : st)  bst.insert(player);
        
        RBTree rbt;
        for (const auto& player : st) rbt.insert(player);
        
        HashTable ht(st.size()*2);
        for (const auto& player : st) ht.insert(player);
        
        std::multimap<std::string, Player> datamap;
        for (const auto& item : st) {
            datamap.insert({item.country, item});  // Вставка пары (ключ, значение)
        }

        auto start_time = std::chrono::high_resolution_clock::now();
        // Линейный поиск
        std::vector<Player> res_lin = linear_search(st, key_country);
        
        //Бинарное дерево поиска
        //std::vector<Player> res_bst = bst.find_elemets_by_key(key_country);
        
        //Красно-черное дерево
        //std::vector<Player> res_rbt = rbt.RB_search(key_country);
        
        //Хэш таблицы
        //std::vector<Player> res_ht = ht.search_hash(key_country);
        
        //map
        //auto range = datamap.equal_range(key_country); 

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end_time - start_time;
        std::cout << duration.count() << " ms\n";
       // std::cout << ht.collision_number << "\n";

}


 //Проверка корректности поиска данных
/* std::cout << "Линейный поиск по стране: "<< key_country << "\n";
    for (int i=0; i<res_lin.size() ;i++){
        std::cout << res_lin[i].name << "\n";
    }*/
   /* //Поиск по бинарному дереву
    BinarySearchTree bst;
    for (const auto& player : st) {
        bst.insert(player);
    }
     
    std::vector<Player> res_bst = bst.find_elemets_by_key(key_country);

    std::cout << "Поиск с помощью БДП по стране: "<< key_country << "\n";
    for (int i=0; i<res_bst.size() ;i++){
        std::cout << res_bst[i].name << "\n";
    }

    std::cout << '\n';

    RBTree rbt;
    for (const auto& player : st) {
        rbt.insert(player);
    }
    

    std::vector<Player> res_rbt = rbt.RB_search(key_country);

    std::cout << "Поиск с помощью КЧД по стране: "<< key_country << "\n";
    for (int i=0; i<res_rbt.size(); i++){
        std::cout << res_rbt[i].name << "\n";
    }

    std::cout << '\n';

    HashTable ht(st.size() + st.size() / 100);
    
    for (const auto& player : st) {
        ht.insert(player);
    }

    auto res_ht = ht.search_hash(key_country);
    std::cout << "Поиск с помощью хэш таблицы по стране: "<< key_country << "\n";
    for (const auto& player : res_ht) {
        std::cout << player.name << ", " << player.club << "\n";
    }
        */
    return 0;
};


