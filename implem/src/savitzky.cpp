#include <iostream>
#include "MITDbHandler.h"

#define MITDBH_FILE "/home/piotr/Studia/ESDMiT_Savitzky-Golay/mit-bih-txt/mitdb100short.txt"

int main()
{
    MITDbHandler dataHandler;

    //Read data from MIT-BIH (parsed to txt: ../../mit-bih-txt/mitdb100short.txt)
    std::vector<mitRecord> signals = dataHandler.readMITBHDataFromTxt(MITDBH_FILE);

    //Print example record
    mitRecord exampleRecord = signals[77];
    std::cout <<"Time, MLII, V5\n";
    std::cout << std::get<0>(exampleRecord) << "," << std::get<1>(exampleRecord) << "," << std::get<2>(exampleRecord) << std::endl;
}
