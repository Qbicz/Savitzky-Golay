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

class MITDbHandler {
public:
    MITDbHandler();
    virtual ~MITDbHandler();
    void readMITBHDataFromTxt(const std::string filename);
    void printMITBHDataFromTxt(const std::string filename);

    Eigen::VectorXf& getTime();
    Eigen::VectorXf& getMlii();
    Eigen::VectorXf& getV5();
    bool saveSignalsToFile(const std::string filename);
    void saveMliiFilterred(const Eigen::VectorXf signal);
    void saveV5Filterred(const Eigen::VectorXf signal);
private:
    Eigen::VectorXf time;
    Eigen::VectorXf mlii;
    Eigen::VectorXf mliiFiltered;
    Eigen::VectorXf v5;
    Eigen::VectorXf v5Filtered;

    int getNumberOfRecords(const std::string& filename);
    void resizeSignalBuffers(int numberOfRecords);
};

#endif /* INCLUDE_MITDBHANDLER_H_ */
