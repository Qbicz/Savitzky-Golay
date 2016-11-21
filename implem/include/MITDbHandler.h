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

typedef std::tuple<float, float, float> mitRecord;

class MITDbHandler {
public:
    MITDbHandler();
    virtual ~MITDbHandler();
    std::vector<mitRecord> readMITBHDataFromTxt(const std::string filename);
    void printMITBHDataFromTxt(const std::string filename);
};

#endif /* INCLUDE_MITDBHANDLER_H_ */
