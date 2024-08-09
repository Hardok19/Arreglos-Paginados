//
// Created by harold-madriz on 9/08/24.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <algorithm>

class PagedArray {
private:
    static const int PAGE_SIZE = 1024; // Tamaño de cada página en enteros
    static const int MAX_PAGES = 4;    // Número máximo de páginas en memoria
    std::string filePath;
    int numPagesLoaded;
    std::unordered_map<int, std::vector<int>> pages; // Mapa de índice de página a contenido de la página
    std::unordered_map<int, int> pageTable;          // Mapa de índice a página cargada en memoria

    int pageFaults = 0;
    int pageHits = 0;

    void loadPage(int pageIndex) {
        if (pages.find(pageIndex) != pages.end()) {
            pageHits++;
            return; // La página ya está cargada
        }

        if (pages.size() >= MAX_PAGES) {
            unloadPage(); // Descarga una página si ya alcanzamos el límite
        }

        std::ifstream file(filePath, std::ios::binary);
        file.seekg(pageIndex * PAGE_SIZE * sizeof(int));

        std::vector<int> page(PAGE_SIZE);
        file.read(reinterpret_cast<char*>(page.data()), PAGE_SIZE * sizeof(int));
        pages[pageIndex] = page;
        pageTable[pageIndex] = pageIndex;
        pageFaults++;

        file.close();
    }

    void unloadPage() {
        int pageIndexToUnload = pageTable.begin()->first; // Escogemos la primera página cargada (puedes implementar una política de reemplazo más sofisticada si lo deseas)
        pages.erase(pageIndexToUnload);
        pageTable.erase(pageIndexToUnload);
    }

public:
    PagedArray(const std::string& filePath) : filePath(filePath), numPagesLoaded(0) {}

    int& operator[](int index) {
        int pageIndex = index / PAGE_SIZE;
        loadPage(pageIndex);
        return pages[pageIndex][index % PAGE_SIZE];
    }

    int getPageFaults() const { return pageFaults; }
    int getPageHits() const { return pageHits; }
};

class Sorter {
private:
    PagedArray* pagedArray;

    void quickSort(int left, int right) {
        if (left < right) {
            int pivot = partition(left, right);
            quickSort(left, pivot - 1);
            quickSort(pivot + 1, right);
        }
    }

    int partition(int left, int right) {
        int pivot = (*pagedArray)[right];
        int i = left - 1;

        for (int j = left; j < right; ++j) {
            std::cout << "Algoritmo especificado: " << j << std::endl;
            if ((*pagedArray)[j] <= pivot) {
                i++;
                std::swap((*pagedArray)[i], (*pagedArray)[j]);
            }
        }
        std::swap((*pagedArray)[i + 1], (*pagedArray)[right]);
        return i + 1;
    }

    void insertionSort(int size) {
        for (int i = 1; i < size; ++i) {
            int key = (*pagedArray)[i];
            int j = i - 1;

            while (j >= 0 && (*pagedArray)[j] > key) {
                (*pagedArray)[j + 1] = (*pagedArray)[j];
                j = j - 1;
            }
            (*pagedArray)[j + 1] = key;
        }
    }

    void bubbleSort(int size) {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if ((*pagedArray)[j] > (*pagedArray)[j + 1]) {
                    std::swap((*pagedArray)[j], (*pagedArray)[j + 1]);
                }
            }
        }
    }

public:
    Sorter(PagedArray* arr) : pagedArray(arr) {}

    void sort(const std::string& algorithm, int size) {
        auto start = std::chrono::high_resolution_clock::now();

        if (algorithm == "QS") {
            quickSort(0, size - 1);
        } else if (algorithm == "IS") {
            insertionSort(size);
        } else if (algorithm == "BS") {
            bubbleSort(size);
        } else {
            std::cerr << "Algoritmo no reconocido: " << algorithm << std::endl;
            return;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "Tiempo durado: " << duration.count() << " segundos" << std::endl;
        std::cout << "Algoritmo utilizado: " << algorithm << std::endl;
        std::cout << "Page faults: " << pagedArray->getPageFaults() << std::endl;
        std::cout << "Page hits: " << pagedArray->getPageHits() << std::endl;
    }
};









