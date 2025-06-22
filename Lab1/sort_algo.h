#include <vector>
/// @file sort_algo.h
/// @brief Файл с реализацией сортировок

/// @brief Функция слияния двух списков
template <class T> 
void merge(std::vector<T>& a, long low, long mid, long high) {
    std::vector<T> b;
    long left_index = low;
    long right_index = mid + 1;

    while (left_index <= mid && right_index <= high) {
        if (a[left_index] <= a[right_index]) {
            b.push_back(a[left_index]);
            left_index++;
        } else {
            b.push_back(a[right_index]);
            right_index++;
        }
    }
 
    while (left_index <= mid) b.push_back(a[left_index++]);
    while (right_index <= high) b.push_back(a[right_index++]);

    for (long k = 0; k < b.size(); k++) {
        a[k + low] = b[k];
    }
}

/// @brief Сортировка слиянием
template <class T> 
void merge_sort(std::vector<T>& a, long low, long high) {
    if (low < high) {
        long mid = low + (high - low) / 2; // избегаем переполнения
        merge_sort(a, low, mid);
        merge_sort(a, mid+1, high);
        merge(a, low, mid, high);
    }
}


/// @brief Быстрая сортировка
template <class T> 
void quick_sort(std::vector<T>& a, long low, long high) {
    if (low >= high) return;  
    long i = low;
    long j = high;
    T p = a[low + (high - low) / 2]; 
    
    while (i <= j) {
        while (a[i] < p) i++;
        while (a[j] > p) j--;
        if (i <= j) std::swap(a[i++], a[j--]);
    }

    if (low < j) quick_sort(a, low, j);
    if (i < high) quick_sort(a, i, high);
}


/// @brief Просеивание элемента кучи
template <class T>
void down_heap(std::vector<T>& a, long k, long n) {
    while (2 * k + 1 < n) { //пока есть потомки
        long child = 2*k + 1;

        if (child + 1 < n && a[child] < a[child + 1]) child++;
        if (a[k] >= a[child]) break;

        std::swap(a[k], a[child]);
        k = child; // далее будут сравнения с потомками на один уровень ниже
    }
}

/// @brief Построение кучи из произвольного массива
template <class T> // строим пирамиду, проверяем все вершины, у которых есть потомок
void build_heap(std::vector<T>& a, long n) {
    for (long i = n / 2 - 1; i >= 0; i--) down_heap(a, i, n);
}


/// @brief Пирамидальная сортировка
template<class T> 
void heap_sort(std::vector<T>& a) {
    long size = a.size();
    build_heap(a, size); // начальная пирамида

    for(long i = size - 1; i > 0; --i) {
        std::swap(a[0], a[i]);  // перекидываем корень на последнее место
        down_heap(a, 0, i); // перестраиваем пирамиду без учета последних элементов
    }
}