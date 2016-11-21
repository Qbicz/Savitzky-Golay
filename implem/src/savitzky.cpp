#include <iostream>
#include "MITDbHandler.h"

#define MITDBH_FILE "/home/piotr/Studia/ESDMiT_Savitzky-Golay/mit-bih-txt/mitdb100short.txt"

void printHelloMessage()
{
    std::cout << " __________________________________________________________\n";
    std::cout << "||                                                        ||\n";
    std::cout << "||                               /\\                       ||\n";
    std::cout << "||           /\\             /\\  /  \\                      ||\n";
    std::cout << "||    ______/  \\  /\\       /  \\/    \\    __________       ||\n";
    std::cout << "||              \\/  \\_____/          \\  /                 ||\n";
    std::cout << "||                                    \\/                  ||\n";
    std::cout << "||                                                        ||\n";
    std::cout << "||  Welcome to SavGol - tool for processing ECG signal.   ||\n";
    std::cout << "||                                                        ||\n";
    std::cout << "||  What do you want to do?                               ||\n";
    std::cout << "||  1 -                                                   ||\n";
    std::cout << "||  2 -                                                   ||\n";
    std::cout << "||  3 -                                                   ||\n";
    std::cout << "||                                                        ||\n";
    std::cout << "||  0 - Exit                                              ||\n";
    std::cout << "||                                                        ||\n";
    std::cout << "||________________________________________________________||\n";
}

int main()
{
    printHelloMessage();

    MITDbHandler dataHandler;

    //Read data from MIT-BIH (parsed to txt: ../../mit-bih-txt/mitdb100short.txt)
    std::vector<mitRecord> signals = dataHandler.readMITBHDataFromTxt(MITDBH_FILE);

    //Print example record
    mitRecord exampleRecord = signals[77];
    std::cout <<"Time, MLII, V5\n";
    std::cout << std::get<0>(exampleRecord) << "," << std::get<1>(exampleRecord) << "," << std::get<2>(exampleRecord) << std::endl;
}
