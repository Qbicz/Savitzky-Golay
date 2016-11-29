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

bool MITDbHandler::saveSignalsToFile(const std::string filename)
{
    std::ofstream outputFile;
    outputFile.open(filename);
    if(outputFile.is_open())
    {
        Eigen::MatrixXf data(time.rows(), 5);
        data << time, mlii, mliiFiltered, v5, v5Filtered ;

        outputFile << "Time[s] MLII[mV] MLII_Filtered[mV] V5[mV] V5_Filterred[mV]\n";
        outputFile << data << "\n";
        outputFile.close();

        return true;
    }

    return false;
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

int MITDbHandler::getNumberOfRecords(const std::string& filename)
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

void MITDbHandler::resizeSignalBuffers(int numberOfRecords)
{
    time.resize(numberOfRecords);
    mlii.resize(numberOfRecords);
    mliiFiltered.resize(numberOfRecords);
    v5.resize(numberOfRecords);
    v5Filtered.resize(numberOfRecords);
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

    resizeSignalBuffers(getNumberOfRecords(filename));

    for(int i = 0; std::getline(inputFile, line); i++)
    {
        ss = std::istringstream(line);
        float aTime, aMlii, aV5;
        ss >> aTime >>   aMlii  >> aV5;

        time(i) = aTime;
        mlii(i) = aMlii;
        v5(i) = aV5;
    }

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

void MITDbHandler::saveMliiFilterred(const Eigen::VectorXf signal)
{
    mliiFiltered = signal;
}

void MITDbHandler::saveV5Filterred(const Eigen::VectorXf signal)
{
    v5Filtered = signal;
}
