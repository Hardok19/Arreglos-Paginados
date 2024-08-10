//
// Created by harold-madriz on 5/08/24.
//
#include <iostream>
#include <fstream>
#include <vector>
#include "generator.h"
#include <random>
#include <algorithm>
#include <ctime>



long long getSizeInBytes(const std::string& size) { //Retorna el tamaño de 512MB, 1GB o 2GB en bytes
    if (size == "SMALL"){
        return 512LL * 1024 * 1024; //512MB
    }
    if (size == "MEDIUM") {
        return 1LL * 1024 * 1024 * 1024; //1GB
    }
    if (size == "LARGE") {
        return 2LL * 1024 * 1024 * 1024; //2GB
    }
    return 0;
}





int main(int argc, char* argv[]) { //argc numero de argumetos del string. argv argumentos en sí
    if (argc != 5) {
        std::cerr << "Usage: generator -size <SIZE> -output <OUTPUT FILE PATH>\n";
        return 1;
    }

    std::string sizeArg = argv[2];
    std::string outputFilePath = argv[4];


    std::random_device rd;  // Generador de números aleatorios
    std::mt19937 gen(rd()); // Motor Mersenne Twister
    // Definir el rango del número aleatorio
    std::uniform_int_distribution<> dis(1000, 9999);

    // Generar un número aleatorio
    int random_number = dis(gen);


    // Convertir el número aleatorio a string
    std::string randomstr = std::to_string(random_number);

    long long fileSize = getSizeInBytes(sizeArg);//verifica si el tamaño es válido
    if (fileSize == 0) {
        std::cerr << "Invalid. Use SMALL, MEDIUM, or LARGE.\n";
        return 1;
    }


    const char barra = *"/";

    if(outputFilePath[outputFilePath.size()-1] != barra) {
        outputFilePath = outputFilePath + "/";
    }


    std::ofstream outputFile(outputFilePath + randomstr, std::ios::binary);

    if (!outputFile) {
        std::cerr << "Failed to open output file: " << outputFilePath << "\n";
        return 1;
    }

    std::srand(std::time(0)); // Inicia el generador de números aleatorios con la hora actual

    long long numIntegers = fileSize / 4;
    for (long long i = 0; i < numIntegers; ++i) {
        int randomInt = std::rand();
        outputFile.write(reinterpret_cast<char*>(&randomInt), 4);

    }

    outputFile.close();
    return 0;
}
