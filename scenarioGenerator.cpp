#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <string>

void caseGenerator(std::ostringstream &oss){
    size_t cases = 10;
    size_t letters = 15;
    int rng;

    for(size_t i = 0; i < cases; i++){
        for(size_t j = 0; j < letters; j++){
            rng = rand() % 101;

            if(rng <= 30)
                oss << "w";
            else
                oss << "r";
        }
        oss << "\n";
    }
    return;
}

int main(){
    std::ofstream p;
    std::ostringstream oss;

    p.open("scenarios.txt");

    caseGenerator(oss);
    p << oss.str();

    p.close();

    return 0;    
}