#include "milkweed.hpp"
#include <cstdlib>

Milkweed::Milkweed(int x, int y, Map* map) : Plant(x, y, 0, 0, map) {};

void Milkweed::action(){
    for (int i = 0; i < 3; i++){
        if(rand() % 20 == 0){
            Plant::action();
        }
    }
}

Plant* Milkweed::reproduce(int newX, int newY) const {
    return new Milkweed(newX, newY, map);
}