#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <random>
#include <cstdint>

/// @brief  Объединенный LCG
/// @param seed1 Начальное значение для 1 LCG
/// @param k1 Коэффициент умножения для 1 LCG
/// @param b1 Коэффициент смещения для 1 LCG
/// @param m1 Модуль сравнения для 1 LCG
/// @param seed2 Начальное значение для 2 LCG
/// @param k2 Коэффициент умножения для 2 LCG
/// @param b2 Коэффициент смещения для 2 LCG
/// @param m2 Модуль сравнения для 2 LCG
void LCG2(unsigned int& seed1, unsigned int k1, unsigned int b1, unsigned int m1, unsigned int& seed2, unsigned int k2, unsigned int b2, unsigned int m2) {
    seed1 = (k1 * seed1 + b1) % m1;
    seed2 = (k2 * seed2 + b2) % m2;
}

/// @brief Получение данных для объединенного LCG
/// @param N Необходимое количество генерируемых значений
/// @param result  Вектор с результатом
/// @param params Вектор параметров, необходимых для использования объединенного LCG
void get_data(unsigned int N, std::vector<unsigned int>& result, std::vector<unsigned int>& params) {

    // Инициализация генераторов
    unsigned int s1 = 1;
    unsigned int s2 = 1;
    
    unsigned int k1 = params[0];
    unsigned int b1 = params[1];
    unsigned int m1 = params[2];

    unsigned int k2 = params[3];
    unsigned int b2 = params[4];
    unsigned int m2 = params[5];

    for (int i = 1; i <= N; i++){
        LCG2(s1, k1, b1, m1, s2, k2, b2, m2);
        result.push_back(s1 ^ s2); //Возвращаю xor двух результатов
    }

}

/// @brief Функция определения статистических параметров (среднее, отклонение, коэффициент вариации)
/// @param data 
/// @return Возвращает значение параметров в виде вектора
std::vector<double> mean_st_cv(std::vector<unsigned int>& data){
    
    //Среднее
    double sum = 0;
    for (double value: data) sum += value;
    double mean = sum / data.size();

    //Отклонение
    sum = 0;
    for (double value: data) sum += (value - mean) * (value - mean);
    double st = std::sqrt(sum / data.size());
    
    //Коэффициент вариации
    double cv = st / mean;

    std::vector<double> result = {mean, st, cv};
    return result;
   
}

/// @brief Вычисление chi-статистики
/// @param data Вектор данных
/// @param N Размер данных
/// @return 
double chi(std::vector<unsigned int>& data, unsigned int N){
    double k = 1.0 + std::log2(N); // применяем формулу
    unsigned int bins = static_cast<unsigned int>(ceil(k));
    double minVal = *std::min_element(data.begin(), data.end());
    double maxVal = *std::max_element(data.begin(), data.end());
    double range = maxVal - minVal;

    std::vector<unsigned int> observed(bins, 0);
    double binWidth = range / bins;

    for (double value: data) {
        int binIndex = static_cast<unsigned int>((value - minVal) / binWidth);
        if (binIndex == bins) binIndex--;
        observed[binIndex]++;
    }
    
    double expectedFrequency = static_cast<double>(data.size()) / bins;
    double chi = 0.0;
    for (int count: observed) {
        double diff = count - expectedFrequency;
        chi += (diff * diff) / expectedFrequency;
    }

    return chi;
    
}


/// @brief Класс модифицированного генератора Блюм-Блюм-Шуба
class BlumBlumShub {
    public:
        /// @brief Простое p
        unsigned long long p; 
        /// @brief Простое q
        unsigned long long q;
        /// @brief pq
        unsigned long long n;
        /// @brief Начальное значение
        unsigned long long seed;
        /// @brief Текущие состояние генератора
        unsigned long long current;
    
        /// @brief Проверка, что число простое
        bool is_prime(unsigned long long num) {
            if (num <= 1) return false;
            if (num == 2) return true;
            if (num % 2 == 0) return false;
            
            unsigned long long sqrtNum = static_cast<unsigned long long>(std::sqrt(num)) + 1;
            for (unsigned long long i=3; i < sqrtNum; i += 2) if (num % i == 0) return false;
            return true;

        }
    
        /// @brief Проверка на значение по модулю 3
        bool is_3_mod_4(unsigned long long num) {
            return (num % 4) == 3;
        }
    

        BlumBlumShub(unsigned long long p, unsigned long long q, unsigned long long seed) {
            
            if (!is_prime(p) || !is_prime(q)) throw std::invalid_argument("p and q must be prime numbers");
            if (!is_3_mod_4(p) || !is_3_mod_4(q)) throw std::invalid_argument("p and q must be congruent to 3 mod 4");
            if (std::gcd(p, q) != 1) throw std::invalid_argument("p and q must be co-prime");
            if (seed <= 1 || seed >= (p * q)) throw std::invalid_argument("seed must be in range (1, p*q)");
    
            this->p = p;
            this->q = q;
            this->n = p * q;
            this->seed = seed;
            this->current = (seed * seed) % n;
        }
        
        /// @brief Генерация одного бита
        bool next_bit() {
            unsigned long long t = current % n;
            current = t * t;
            return current & 1; //Бит
        }
    
        /// @brief Генерация следующего числа (bits = 13) 
        unsigned long long next_number(int bits = 13) {
            unsigned long long result = 0;
            for (int i=0; i < bits; i++) result = (result << 1) | next_bit();
            return result;
        }
};
    

/// @brief Функция, преобразующая 24 битное число (меняет половинки числа в битовом представлении местами)
/// @param input 
/// @return 
unsigned int transform_24bit(unsigned int input) {
    
        const unsigned int abcdef_mask = 0b111111111111000000000000;
        // const unsigned int g_mask = 0b0000001000000;     
        const unsigned int hijklm_mask = 0b000000000000111111111111;
        unsigned int abcdef = (input & abcdef_mask) >> 12;
        //unsigned int g = (input & g_mask) >> 6;
        unsigned int hijklm = input & hijklm_mask;
        unsigned int result = (hijklm << 12) | abcdef;
    
        return result;
    
}

/// @brief Модифицированный алгоритм Блюма-Микали
class BlumMicaliGenerator {
    public:
        /// @brief Простое число p
        unsigned long long p; 
        /// @brief Первообразный корень по модулю p 
        unsigned long long g; 
        /// @brief Начальное значение
        unsigned long long x0;
        /// @brief Текущее состояние генератора
        unsigned long long current;
        
        /// @brief Алгоритм проверки числа на простоту
        /// @param num 
        /// @return 
        bool is_prime(unsigned long long num) {
            if (num <= 1) return false;
            if (num == 2) return true;
            if (num % 2 == 0) return false;
            
            unsigned long long sqrtNum = static_cast<unsigned long long>(std::sqrt(num)) + 1;
            for (unsigned long long i=3; i < sqrtNum; i += 2) if (num % i == 0) return false;
            return true;
        }
        
        /// @brief Поиск первообразных корней по простому модулю p
        /// @param p Простое число - модуль
        /// @return  Первый первообразный корень
        unsigned long long find_primitive_root(unsigned long long p) {
            if (p == 2) return 1;
                
            //Поиск множителей
            unsigned long long phi = p - 1;
            std::vector<unsigned long long> factors;
            unsigned long long n = phi;
            
            for (unsigned long long i=2; i * i <= n; i++) {
                if (n % i == 0) {
                    factors.push_back(i);
                    while (n % i == 0) n /= i;
                }
            }

            if (n > 1) factors.push_back(n);

            for (unsigned long long g = 2; g <= p; g++) {
                bool flag = true;
                for (unsigned long long f: factors) {
                    if (modular_pow(g, phi / f, p) == 1) {
                        flag = false;
                        break;
                    }
                }

                if (flag) return g;
            }

            return 0;
        }
        
        /// @brief Алгоритм быстрого возведения в степень по модулю
        /// @param base основание степени
        /// @param exp  значение степени
        /// @param mod  модуль
        /// @return 
        unsigned long long modular_pow(unsigned long long base, unsigned long long exp, unsigned long long mod) {
            unsigned long long result = 1;
            base %= mod;
            while (exp > 0) {
                if (exp % 2 == 1) result = (result * base) % mod;
                exp >>= 1;
                base = (base * base) % mod;
            }
            return result;
        }

        BlumMicaliGenerator(unsigned long long prime) : p(prime) {
            if (!is_prime(p)) throw std::invalid_argument("Provided number must be a prime");

            g = find_primitive_root(p);
            x0 = 19270 % p; //Задаю seed одинаковый здесь
            current = x0;
        }

        /// @brief Генерация следующего бита
        /// @return 
        bool next_bit() {
            current = modular_pow(g, current, p);
            return current > (p - 1) / 2;
        }
        /// @brief Генерация следующего 24 битного числа
        /// @return 
        unsigned int next_number() {
            unsigned int result = 0;
            for (int i=0; i < 24; i++) result = (result << 1) | next_bit();
            return result;
        }
        
        void generate(std::vector<unsigned int>& output, size_t count) {
            
            for (size_t i=0; i < count; i++) output.push_back(transform_24bit(next_number()));
            
        }
};


/// @brief Запись в файл сгенерированных чисел
/// @param filename Имя файла записи
/// @param numbers Массив данных
void write_numbers_file(const std::string& filename, const std::vector<unsigned int>& numbers) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл " << filename << " для записи." << std::endl;
        return;
    }
    
    for (int num: numbers) file << num << "\n";
    
    file.close();
}

/// @brief Запись в файл чисел в бинарном представлении
/// @param filename Имя файла записи
/// @param result Массив данных
void write_bin_file(const std::string& filename, const std::vector<unsigned int>& result) {
    
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл " << filename << " для записи." << std::endl;
        return;
    }
    
    for (unsigned int num: result) {
       
        uint8_t byte1 = (num >> 16) & 0b11111111;
        uint8_t byte2 = (num >> 8)  & 0b11111111;
        uint8_t byte3 = num         & 0b11111111; 

        file.write(reinterpret_cast<const char*>(&byte1), 1);
        file.write(reinterpret_cast<const char*>(&byte2), 1);
        file.write(reinterpret_cast<const char*>(&byte3), 1);
    }
    
    file.close();
}

int main() {
    
    std::cout << "Тестирование генератора №1" << "\n\n";
    //Данные первого генератора
    std::vector<std::vector<unsigned int>> res_vec(20);
    std::vector<std::vector<unsigned int>> params = {

    {1417, 5, 6912, 2701, 7, 5760}, //OK
    {1417, 51, 6912, 2701, 73, 5760}, //OK
    {3205, 105, 6912, 2701, 191, 5760}, // OK
    {673, 57, 6912, 2701, 239, 5760}, // OK 

    {2017, 59, 5292, 3204, 31, 6912},
    {1401, 15, 6912, 3291, 34, 5292},
    {3205, 83, 6912, 1093, 1, 5292},
    {673, 419, 6912, 2701, 91, 5292},
    
    
    {1716, 1292, 8575, 1541, 1, 8470},
    {1716, 59, 8575, 2311, 79, 8470},
    {3291, 83, 8575, 2311, 157, 8470},
    {3291, 1063, 8575, 1541, 71, 8470},

    
    {1191, 256, 6125, 1191, 78, 4375},
    {736, 32, 6125, 2241, 932, 4375},
    {736, 107, 6125, 1191, 191, 4375}, 
    {1191, 13, 6125, 2241, 901, 4375},

    {905, 582, 8191, 2701, 27, 8191}, //OK
    {1417, 51, 8191, 2701, 73, 8191}, //OK
    {9, 5, 8191, 13, 7, 8191},// OK
    {725, 90, 8191, 6, 1, 8191}
    
    };
    
    std::vector<double> d;
    for (int i=0; i < 20; i++){
        std::cout << "Parametres: ";
        for (int k=0; k < 6; k++) std::cout << params[i][k] << ' ';
        std::cout << "\nStatistical data (mean, std, CV): ";

        auto start_time = std::chrono::high_resolution_clock::now();
        get_data(100000, res_vec[i], params[i]);
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end_time - start_time;
        
        //Запись в бинарный файл и числа
        std::string filename1 = "LCG_" + std::to_string(i) + ".bin";
        std::string filename2 = "LCG_num_" + std::to_string(i) + ".txt";

        write_bin_file(filename1, res_vec[i]);
        write_numbers_file(filename2, res_vec[i]);
        
        d = mean_st_cv(res_vec[i]);
        for (int j=0; j < 3; j++) std::cout << d[j] << ' ';
        std::cout << "\nChi-square statistic: " << chi(res_vec[i], 100000) << "\n\n";

    }
    

//Генератор №2
    std::cout << "Тестирование генератора №2" << "\n\n";

    std::vector<unsigned long long> primes1 = {275084291, 4406159, 2051719, 48751, 386887};
    std::vector<unsigned long long> primes2 = {24206023, 74219, 3590971, 87767};
    unsigned long long seed = 13921;
    unsigned int c1, c2;

    for (int ind1=0; ind1<5; ind1++){
        for (int ind2=0; ind2<4; ind2++){

            BlumBlumShub bbs(primes1[ind1], primes2[ind2], seed);
            std::vector<unsigned int> numbers;
            
            auto start_time = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < 100000; i++){
                c1 = bbs.next_number(24);
                c2 =  bbs.next_number(24);
                numbers.push_back(c1 ^ c2); //xor
            } 
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end_time - start_time;
        
            //Запись в бинарный файл и числа
            std::string filename1 = "BBS_" + std::to_string(ind1 * 4 + ind2) + ".bin";
            std::string filename2 = "BBS_num_" + std::to_string(ind1 * 4 + ind2) + ".txt";

            write_bin_file(filename1, numbers);
            write_numbers_file(filename2, numbers);

            std::cout << "Parametres: p = " << primes1[ind1] << ", q = " << primes2[ind2];
            std::cout << "\nStatistical data (mean, std, CV): ";
            d = mean_st_cv(numbers);
            for (int j=0; j < 3; j++) std::cout << d[j] << ' ';
            std::cout << "\nChi-square statistic: " << chi(numbers, 100000) << "\n";
            std::cout << "Время: " << duration.count() << " ms\n\n";
        }
    }
    
        

//Генератор №3

    std::cout << "Тестирование генератора №3" << "\n\n";

    std::vector<unsigned long long> primes3 = {24206023, 39141139, 3590971, 51449843, 815714047,
        275084291, 4406159, 2051719, 55275911, 386887,
        9704731, 21403579, 78645779, 59269787, 902626523,
        277905127, 65775247, 16805119, 4710187, 1743823};
    
    for (int i=0; i < 20; i++){
        BlumMicaliGenerator bmg(primes3[i]);
        std::vector<unsigned int> numbers;
        
        //Замер времени
        auto start_time = std::chrono::high_resolution_clock::now();
        bmg.generate(numbers, 100000);
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end_time - start_time;
        
        //Запись в бинарный файл и числа
        std::string filename1 = "BM_" + std::to_string(i) + ".bin";
        std::string filename2 = "BM_num_" + std::to_string(i) + ".txt";

        write_bin_file(filename1, numbers);
        write_numbers_file(filename2, numbers);

        std::cout << "\nParam: p = " << primes3[i] << '\n';
        std::cout << "Statistical data (mean, std, CV): ";
        d = mean_st_cv(numbers);
        for (int j=0; j < 3; j++) std::cout << d[j] << ' ';
        std::cout << "\nChi-square statistic: " << chi(numbers, 100000) << "\n";
        std::cout << "Время: " << duration.count() << " ms\n\n";
    }
        
    //Замер времени

    std::vector<unsigned int> times = {1000, 1635, 2859, 5000, 8743, 15289, 26736, 46753, 81756, 142965, 250000, 384000, 500000, 650000, 830000, 910000, 1000000};
    for (int i=0; i < 17; i++){
        //BlumMicaliGenerator bmg(902626523);  
       // BlumBlumShub bbs(2051719, 4406159, 182946);
        
        
        std::random_device rd;
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> distrib(0, 16777216);

        std::vector<unsigned int> numbers;

        auto start_time = std::chrono::high_resolution_clock::now();
        /*for (int j = 0; j < times[i]; j++){
            c1 = bbs.next_number(24);
            c2 =  bbs.next_number(24);
            numbers.push_back(c1 ^ c2); //xor
        } */
        
        //bmg.generate(numbers, times[i]);

        //get_data(times[i], numbers, params[10]);
        
        for (int j=0; j < times[i]; j++) numbers.push_back(distrib(gen));
        
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end_time - start_time;
        std::cout << "Количество элементов:" << times[i] << " Время: " << duration.count() << " ms\n\n";
    }
    
    return 0;
}
