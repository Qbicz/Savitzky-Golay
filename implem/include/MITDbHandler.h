/*
 * MITDbHandler.h
 *
 *  Created on: Nov 21, 2016
 *      Author: piotr
 */

#ifndef INCLUDE_MITDBHANDLER_H_
#define INCLUDE_MITDBHANDLER_H_

#include <tuple>
#include <vector>
#include <Eigen/Dense>

typedef std::tuple<float, float, float> mitRecord;

class MITDbHandler {
public:
    MITDbHandler();
    virtual ~MITDbHandler();
    void readMITBHDataFromTxt(const std::string filename);
    void printMITBHDataFromTxt(const std::string filename);
    int getNumberOfRecords(const std::string filename);
    Eigen::VectorXf& getTime();
    Eigen::VectorXf& getMlii();
    Eigen::VectorXf& getV5();
    bool saveSignalToFile(const std::string filename, Eigen::VectorXf& signal);
private:
    std::vector<mitRecord> signals;
    Eigen::VectorXf time;
    Eigen::VectorXf mlii;
    Eigen::VectorXf v5;
};

#endif /* INCLUDE_MITDBHANDLER_H_ */
