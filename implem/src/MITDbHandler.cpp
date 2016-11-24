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

    std::cout << "File has: " << getNumberOfRecords(filename) << " records\n";

    inputFile.close();
}

int MITDbHandler::getNumberOfRecords(const std::string filename)
{
    std::ifstream f(filename);
    if(!f.is_open())
    {
        std::cout << "Unable to open file: " << filename << std::endl;
        return -1;
    }

    std::string line;
    auto records = 0;

    for (int i = 0; std::getline(f, line); ++i, records++)
        ;

    return records-1;
}

void MITDbHandler::readMITBHDataFromTxt(const std::string filename)
{
    std::ifstream inputFile(filename);
    if(!inputFile.is_open())
    {
        std::cout << "Unable to open file: " << filename << std::endl;
        return;
    }

    //get the first line that contains headers, not the data
    std::string header;
    std::getline(inputFile, header);

    //read the data
    std::istringstream ss;
    std::string line;

    auto numberOfRecords = getNumberOfRecords(filename);
    std::vector<mitRecord> signals;
    time.resize(numberOfRecords);
    mlii.resize(numberOfRecords);
    v5.resize(numberOfRecords);

    for(int i = 0; std::getline(inputFile, line); i++)
    {
        ss = std::istringstream(line);
        char comma; // buffer for comma
        float aTime, aMlii, aV5;
        ss >> aTime >> comma >> aMlii >> comma >> aV5;

        signals.push_back(std::make_tuple(aTime, aMlii, aV5));
        time(i) = aTime;
        mlii(i) = aMlii;
        v5(i) = aV5;
    }

    //Print example record
//    mitRecord exampleRecord = signals[77];
//    std::cout <<"Time, MLII, V5\n";
//    std::cout << std::get<0>(exampleRecord) << "," << std::get<1>(exampleRecord) << "," << std::get<2>(exampleRecord) << std::endl;

    inputFile.close();
    return;
}

Eigen::VectorXf& MITDbHandler::getTime()
{
    return time;
}

Eigen::VectorXf& MITDbHandler::getMlii()
{
    return mlii;
}

Eigen::VectorXf& MITDbHandler::getV5()
{
    return v5;
}
