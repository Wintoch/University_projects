// L = 2k+1
//P = 2k+2
#include <iostream>
#include <vector>
#include <sstream>

void heapify(std::vector<int> &arrayToSort, int i, int heapsize){
    int length = heapsize;

    while((2*i + 1) < length){
        int biggestIndex = i;
        int left = 2*i + 1;
        int right = 2*i + 2;

        if((left < length) && (arrayToSort[left] > arrayToSort[biggestIndex])){
            biggestIndex = left;
        } 
        
        if((right < length) && (arrayToSort[right] > arrayToSort[biggestIndex])){
            biggestIndex = right;
        }

        if(biggestIndex != i){
            std::swap(arrayToSort[i], arrayToSort[biggestIndex]);
            i = biggestIndex;
        }else{
            break;
        }
    }
}

void HeapSort(std::vector<int> &arrayToSort){
    for(int i = (arrayToSort.size()/2) - 1; i >= 0; i--){
        heapify(arrayToSort, i, arrayToSort.size());
    }

    for(int i = (arrayToSort.size()-1); i > 0; i--){
        std::swap(arrayToSort[0], arrayToSort[i]);
        heapify(arrayToSort, 0, i);
    }
}

int main(){
    std::vector<int> arrayToSort;
    std::string line;

    while (std::getline(std::cin, line)) {
        std::stringstream ss(line);
        int n;

        
        while (ss >> n) {
            arrayToSort.push_back(n);
        }

        HeapSort(arrayToSort);
        for (size_t i = 0; i < arrayToSort.size(); i++){
            std::cout << arrayToSort[i] << " ";
        }

        std::cout << "\n";
        arrayToSort.clear();
    }

    return 0;
}