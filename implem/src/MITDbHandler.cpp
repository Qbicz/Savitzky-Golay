/*
 * MITDbHandler.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: piotr
 */

#include "MITDbHandler.h"
#include <sstream>
#include <fstream>
#include <iostream>

MITDbHandler::MITDbHandler() {
}

MITDbHandler::~MITDbHandler() {
}

void MITDbHandler::printMITBHDataFromTxt(const std::string filename)
{
    std::ifstream inputFile(filename);
    if(!inputFile.is_open())
    {
        std::cout << "Unable to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while(std::getline(inputFile, line))
    {
        std::cout << line << '\n';
    }

    inputFile.close();
}

std::vector<mitRecord> MITDbHandler::readMITBHDataFromTxt(const std::string filename)
{

    std::ifstream inputFile(filename);
    if(!inputFile.is_open())
    {
        std::cout << "Unable to open file: " << filename << std::endl;
        return std::vector<mitRecord>();
    }

    //get the first line that contains headers, not the data
    std::string header;
    std::getline(inputFile, header);

    //read the data
    std::istringstream ss;
    std::string line;

    std::vector<mitRecord> signals;

    while(std::getline(inputFile, line))
    {
        ss = std::istringstream(line);
        char comma; // buffer for comma
        float time, mlii, v5;
        ss >> time >> comma >> mlii >> comma >> v5;

        signals.push_back(std::make_tuple(time, mlii, v5));
    }

    inputFile.close();
    return signals;
}
