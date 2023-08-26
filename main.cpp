#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <sstream>

const int PAGE_SIZE = 256;

// Definición de la clase PagedArray
class PagedArray {
private:
    std::ifstream input;
    std::vector<int> currentPage;
    int currentPageIndex;
    int pageSize;

public:
    PagedArray(const std::string& filename, int page_size) : currentPageIndex(0), pageSize(page_size) {
        input.open(filename);
        loadPage();
    }

    ~PagedArray() {
        input.close();
    }

    void loadPage() {
        currentPage.clear();
        currentPage.reserve(pageSize);

        std::string line;
        if (std::getline(input, line)) {
            std::stringstream ss(line);
            int num;
            while (ss >> num) {
                currentPage.push_back(num);
                if (currentPage.size() >= pageSize) {
                    break;
                }
                if (ss.peek() == ',') {
                    ss.ignore();
                }
            }
        }
    }

    std::vector<int>& getPage() {
        return currentPage;
    }

    bool hasMoreData() const {
        return !currentPage.empty();
    }
};

// Algoritmo de QuickSort
void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = arr[low];
        int left = low + 1;
        int right = high;
        while (true) {
            while (left <= right && arr[left] <= pivot) {
                ++left;
            }
            while (arr[right] >= pivot && right >= left) {
                --right;
            }
            if (right < left) {
                break;
            }
            std::swap(arr[left], arr[right]);
        }
        std::swap(arr[low], arr[right]);

        quickSort(arr, low, right - 1);
        quickSort(arr, right + 1, high);
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Tamaños de archivos de prueba
    const int fileSizes[] = {1024, 4096, 8192, 12288, 24576, 36864};
    const std::string sizeNames[] = {"1KB", "4KB", "8KB", "12KB", "24KB", "36KB"};

    for (int i = 0; i < sizeof(fileSizes) / sizeof(fileSizes[0]); ++i) {
        int fileSize = fileSizes[i];
        std::string sizeName = sizeNames[i];

        // Generar archivo de prueba
        std::ofstream inputFile(sizeName + ".txt");
        for (int i = 0; i < fileSize; ++i) {
            inputFile << rand() % 10000; // Números entre 0 y 9999
            if (i < fileSize - 1) {
                inputFile << ",";
            }
        }
        inputFile.close();

        // Cargar archivo en PagedArray y ordenar
        PagedArray pagedArray(sizeName + ".txt", PAGE_SIZE);

        std::vector<int> completeArray;
        while (pagedArray.hasMoreData()) {
            std::vector<int>& currentPage = pagedArray.getPage();
            completeArray.insert(completeArray.end(), currentPage.begin(), currentPage.end());
            quickSort(completeArray, 0, completeArray.size() - 1);
            pagedArray.loadPage();
        }

        // Escribir resultado ordenado en el archivo de salida
        std::ofstream sortedFile(sizeName + "_resultado.txt");
        for (int i = 0; i < completeArray.size(); ++i) {
            sortedFile << completeArray[i];
            if (i < completeArray.size() - 1) {
                sortedFile << ",";
            }
        }
        sortedFile.close();
    }

    return 0;
}