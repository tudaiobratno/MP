
#include <string>
/// @brief Линейный поиск
/// @param players Массив исходных данных
/// @param key  Ключ поиска
/// @return Массив элементов key
std::vector<Player> linear_search(const std::vector<Player>& players, const std::string& key) {
    std::vector<Player> result; //Итоговой вектор игроков

    for (const Player& player : players) {
        if (player.country == key) result.push_back(player);    
    }

    return result;
}

/// @brief Структура бинарного дерева поиска
struct BinaryTreeNode {
    /// @brief Данные вершины
    Player data;
    /// @brief Левый сын
    BinaryTreeNode* left;
    /// @brief Правый сын
    BinaryTreeNode* right;

    BinaryTreeNode(const Player& player) : data(player), left(nullptr), right(nullptr) {}
};

/// @brief Класс бинарного дерева поиска
class BinarySearchTree {
    public:
      /// @brief Корень дерева
        BinaryTreeNode* root; 
        
        /// @brief Рекурсивная функция вставки вершины в бинарное дерево
        /// @param node  Начальная вершина, от которой идет рекурсивная вставка
        /// @param player Элемент с данными
        /// @return Указатель на новую вершину
        BinaryTreeNode* insert(BinaryTreeNode* node, const Player& player) {
            if (!node) return new BinaryTreeNode(player); //Если вершина является листом
            if (player.country < node->data.country) node->left = insert(node->left, player); //Если ключ для вершины меньше, чем тот, что уже есть в вершине, отправляем налево
            else node->right = insert(node->right, player); //Иначе направляем направо
            return node;
        }
    
        /// @brief Поиск всех игроков из страны по ключю key
        void find_elemets_by_key(BinaryTreeNode* node, const std::string& key, std::vector<Player>& result) {
            if (!node) return; //Дошли до листа
    
            if (node->data.country == key) { 
                result.push_back(node->data);// Если значение равно, нужно добавить в список
                find_elemets_by_key(node->right, key, result); //Продолжаем поиск справа
            } 
            else if (key < node->data.country) find_elemets_by_key(node->left, key, result); //Если значениие ключа меньше, чем у текущего игрока, идем налево
            else find_elemets_by_key(node->right, key, result); // Иначе ищем справа
        }
    
        BinarySearchTree() : root(nullptr) {} // Создание пустого дерева
      
        void insert(const Player& player) { //Вставка без вершины
            root = insert(root, player);
        }
      
        std::vector<Player> find_elemets_by_key(const std::string& key) { //Поиск по ключу
            std::vector<Player> result;
            find_elemets_by_key(root, key, result);
            return result;
        }
};

/// @brief Структура вершины красно-черного дерева
struct RBTreeNode {
    /// @brief Данные
    Player data;
    /// @brief Левый сын
    RBTreeNode* left;
    /// @brief Правый сын
    RBTreeNode* right;
    /// @brief Отец
    RBTreeNode* parent;
    /// @brief Цвет. Значение True - красная вершина, иначе - черная
    bool color = 1; ; // True - красная вершина, False - черная

    RBTreeNode(const Player& player) : data(player), left(nullptr), right(nullptr), parent(nullptr), color(1) {}
};

/// @brief Класс красно-черного дерева
class RBTree {
    public:
        RBTreeNode* root = nullptr;
        /// @brief Вставка вершины
        /// @param player Данные
        void insert(const Player& player){
            RBTreeNode* new_node = new RBTreeNode(player);
            RBTreeNode* parent_of_new_node = nullptr;
            RBTreeNode* node_to_find_place = root;

            while (node_to_find_place) { //пока не дошли до листа
                parent_of_new_node = node_to_find_place;
                if (new_node->data.country < node_to_find_place->data.country) node_to_find_place = node_to_find_place->left; //ключ меньше, поэтому идем влево
                else node_to_find_place = node_to_find_place->right;
            }

            new_node->parent = parent_of_new_node;
            if (!parent_of_new_node) {
                root = new_node;
                root->color = 0;
            }
            else if (new_node->data.country < parent_of_new_node->data.country) parent_of_new_node->left = new_node; //добавляем сына родителю
            else parent_of_new_node->right = new_node;

            fix_insert(new_node);
        }
        
        /// @brief Поворот влево
        /// @param node Вершина, вокруг которой поворачивают
        void rotate_left(RBTreeNode* node) {
            RBTreeNode* node_right_son = node->right;
            
            node->right = node_right_son->left;
            if (node_right_son->left != nullptr) node_right_son->left->parent = node;
   
            node_right_son->parent = node->parent;
        
            if (node->parent == nullptr) root = node_right_son;  // node был корнем
            else if (node == node->parent->left)
                node->parent->left = node_right_son;
            else node->parent->right = node_right_son;
            
        
            node_right_son->left = node;
            node->parent = node_right_son;
        }
        /// @brief Поворот вправо
        /// @param node Вершина, вокруг которой поворачивают
        void rotate_right(RBTreeNode* node) {
    
            RBTreeNode* node_left_son = node->left;
            node->left = node_left_son->right;
            if (node_left_son->right != nullptr) node_left_son->right->parent = node;  // меняем отца
                
            node_left_son->parent = node->parent;
        
            if (node->parent == nullptr) root = node_left_son;  
            else if (node == node->parent->left) node->parent->left = node_left_son;
            else node->parent->right = node_left_son;
        
            node_left_son->right = node;
            node->parent = node_left_son;
        }
        
        /// @brief Вставка с проверкой корректности свойств КЧД
        /// @param node Вершина, на место которой вставляется текущая
        void fix_insert(RBTreeNode* node){
            RBTreeNode* uncle_node;
            if (node->parent == nullptr) {
                root->color = 0;
                return;
            }
            
            while (node != root && node->parent->color == 1){
                if (node->parent == node->parent->parent->left) { //Если отец - левый ребенок
                    uncle_node = node->parent->parent->right;
                    if (uncle_node && uncle_node->color == 1){
                        node->parent->color = 0;
                        uncle_node->color = 0;
                        node->parent->parent->color = 1;
                        node = node->parent->parent;
                    }
                    else {
                        if (node == node->parent->right) {
                            node = node->parent;
                            rotate_left(node);
                        }
                        node->parent->color = 0;
                        node->parent->parent->color = 1;
                        rotate_right(node->parent->parent);
                    }
                }
                else {
                    uncle_node = node->parent->parent->left;
                    if (uncle_node && uncle_node->color == 1){
                        node->parent->color = 0;
                        uncle_node->color = 0;
                        node->parent->parent->color = 1;
                        node = node->parent->parent;
                    }
                    else {
                        if (node == node->parent->left){
                            node = node->parent;
                            rotate_right(node);
                        }
                        node->parent->color = 0;
                        node->parent->parent->color = 1;
                        rotate_left(node->parent->parent);
                    }
                }
            }
            root->color = 0;
        }
        
        /// @brief Поиск по КЧД
        /// @param node Вершина, с которой идет поиск
        /// @param key Ключ поиска
        /// @param result Вектор, в который записываются данные
        void dfs_searh_country(RBTreeNode* node, const std::string& key, std::vector<RBTreeNode*>& result) const {
            if (node == nullptr) return;
            if (key == node->data.country) {
                result.push_back(node);
                dfs_searh_country(node->left, key, result);
                dfs_searh_country(node->right, key, result);
            }
            else if (key < node->data.country) dfs_searh_country(node->left, key, result);
            else dfs_searh_country(node->right, key, result);
    
        }
        
        /// @brief Поиск по КЧД
        /// @param key Ключ поиска
        /// @return Возвращаеи все вхождения элемента
        std::vector<Player> RB_search(const std::string& key) const {
            std::vector<Player> result;
            std::vector<RBTreeNode*> node_list;
            dfs_searh_country(root, key, node_list);
            for (RBTreeNode* node : node_list) result.push_back(node->data);
            return result;
        }

    };
    

/// @brief Структура хэш-таблицы
struct Entry {
    Player player;
    bool occupied = false;
};

/// @brief Класс, реализующий хэш таблицу
class HashTable {
    public:
        /// @brief Массив элементов (сам элемент и занята клетка или нет)
        std::vector<Entry> table;
        /// @brief Размер таблицы   
        int size;       
        /// @brief Число коллизий      
        int collision_number = 0;
        
        /// @brief  Хэш функция
        /// @param key Ключ поиска
        /// @return Значение хэш-функции
        int hash_function(const std::string& key) const {
            unsigned int hash = 0;
            for (int i = 0; i < key.length(); i++) {
                hash += (unsigned char)(key[i]);
                hash -= (hash << 13) | (hash >> 19);
            }
            return hash % size;
        }
    
        /// @brief Обработка коллизий (метод открыйто адресации)
        /// @param hash  Значение хэша
        /// @param attempt Номер попытки
        /// @return Индекс, куда будет вставлен элемент
        int prob_sequence(int hash, int attempt) const {
            return (hash + attempt * attempt) % size;
        }
    
    
        HashTable(int sz) : size(sz), table(sz) {}
        
        /// @brief Вставка элемента
        /// @param player 
        void insert(const Player& player) {
            std::string key = player.country;  // Ключом будет страна игрока
            int key_hash = hash_function(key);
            int attempt = 0;
            while (true) {
                int i = prob_sequence(key_hash, attempt++); //вычисляем позицию в хэш таблице
                if (!table[i].occupied) { // если позиция еще не занята
                    table[i].player = player;
                    table[i].occupied = true;
                    break;
                }
                else if (table[i].player.country != key) collision_number++;
            }
        }
       

        /// @brief Поиск по хэш таблице
        /// @param key 
        /// @return 
        std::vector<Player> search_hash(const std::string& key) {
            std::vector<Player> result;
            int hash = hash_function(key);
            int attempt = 0;
            while (attempt < size) {
                int i = prob_sequence(hash, attempt++);
                if (!table[i].occupied) break; //Если клетка пуста, то останавливаемся
                if (table[i].player.country == key) result.push_back(table[i].player); 
                 // в клетке не всегда находится элемент с нужным ключом 
            }
            return result;
        }
    
 };
    
    
    



