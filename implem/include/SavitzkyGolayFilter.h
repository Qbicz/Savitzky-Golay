#ifndef INCLUDE_SAVITZKYGOLAYFILTER_H_
#define INCLUDE_SAVITZKYGOLAYFILTER_H_

#include "Common.h"

class SavitzkyGolayFilter {
public:
    SavitzkyGolayFilter();
    virtual ~SavitzkyGolayFilter();
    bool readMITBHDataFromTxt(const std::string filename);
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

#endif /* INCLUDE_SAVITZKYGOLAYFILTER_H_ */
