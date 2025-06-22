#include <string>

/// @file Player.h
/// @brief Класс, реализующий объект "Игрок сборной"


class Player {
    
    public:
         /// Страна игрока
        std::string country; 
        /// Имя игрока
        std::string name;  
        /// Клуб игрока
        std::string club; 
        /// Позиция игрока
        std::string position;
        /// Количество игр 
        unsigned int games; 
        /// Количество голов
        int goals;  
        

        bool operator==(const Player& other) const {
            return other.country == country;
        }

        /// @brief Оператор сравнения <=
        bool operator<=(const Player& other) const {
            return other > *this;
        }
        
        /// @brief Оператор сравнения <
        bool operator<(const Player& other) const {
            return country < other.country;
        }
        
        /// @brief Оператор сравнения >
        bool operator>(const Player& other) const {
            return other < *this;
        }
        
        /// @brief Оператор сравнения >=
        bool operator>=(const Player& other) const {
            return !(*this < other);
        }
        
};
