#include <iostream>
#include "MITDbHandler.h"
#include <Eigen/Dense>

#include <iomanip>
#include <cstdlib>
#include <sstream>

#define MITDBH_FILE "/home/piotr/Studia/ESDMiT_Savitzky-Golay/mit-bih-txt/mitdb100short.txt"
#define GNUPLOT_MITDB "gnuplot -p -c /home/piotr/Studia/ESDMiT_Savitzky-Golay/implem/src/plot.data"
#define GNUPLOT_COMMAND "gnuplot -p -c /home/piotr/Studia/ESDMiT_Savitzky-Golay/implem/src/plot.gn"

void plotOutput(std::string filename)
{
    //gnuplot -e "filename='$FILENAME'" /home/piotr/Studia/ESDMiT_Savitzky-Golay/implem/src/plot.data
    std::string command = "gnuplot -e \"filename='";
    command += filename + "'\" /home/piotr/Studia/ESDMiT_Savitzky-Golay/implem/src/plot.data &";

    system(command.c_str());
}

void clearConsole()
{
    std::cout << "\x1B[2J\x1B[H";
}

void printHelloMessage()
{
    clearConsole();
    std::cout << " ___________________________________________________________\n";
    std::cout << "||                                                         ||\n";
    std::cout << "||                               /\\                        ||\n";
    std::cout << "||           /\\             /\\  /  \\                       ||\n";
    std::cout << "||    ______/  \\  /\\       /  \\/    \\    __________        ||\n";
    std::cout << "||              \\/  \\_____/          \\  /                  ||\n";
    std::cout << "||                                    \\/                   ||\n";
    std::cout << "||                                                         ||\n";
    std::cout << "||  Welcome to SavGol - tool for processing ECG signal.    ||\n";
    std::cout << "||                                                         ||\n";
    std::cout << "||  What do you want to do?                                ||\n";
    std::cout << "||  1 - Read mitdb100short.txt and print using Eigen types ||\n";
    std::cout << "||  2 - Plot mitdb100short.txt                             ||\n";
    std::cout << "||  3 - Plot simple function using gnuplot                 ||\n";
    std::cout << "||  4 - Clear the console                                  ||\n";
    std::cout << "||  5 - Save to file as png                                ||\n";
    std::cout << "||                                                         ||\n";
    std::cout << "||                                                         ||\n";
    std::cout << "||                                                         ||\n";
    std::cout << "||                                                         ||\n";
    std::cout << "||  0 - Exit                                               ||\n";
    std::cout << "||                                                         ||\n";
    std::cout << "||_________________________________________________________||\n";
}

//jakie funkcje w eigenie przydante:
// transpose()
// fill()
// linspaced()

typedef Eigen::VectorXf EigenVector;

EigenVector getImpulseResponse(const int range, const int polyfitOrder)
{
    EigenVector impulseY(2*range+1);
    EigenVector impulseX = EigenVector::LinSpaced(2*range+1, -range, range);

    //initialize with comma initializer
    impulseY << 0,0,0,1,0,0,0;

    Eigen::MatrixXf MatrixX(2*range+1, polyfitOrder + 1);
    EigenVector MatrixY = impulseY;

    //polyfit
    for (int nRow = 0; nRow < 2*range+1; nRow++ )
    {
        float nVal = 1.0f;
        for ( int nCol = 0; nCol < polyfitOrder + 1; nCol++ )
        {
            MatrixX(nRow, nCol) = nVal;
            nVal *= impulseX[nRow];
        }
    }

    Eigen::MatrixXf MatrixXt = MatrixX.transpose();
    Eigen::MatrixXf MatrixXtX = MatrixXt * MatrixX;
    Eigen::MatrixXf MatrixXtY = MatrixXt * MatrixY;
    Eigen::MatrixXf MatrixXtXInv = MatrixXtX.inverse();
    EigenVector coefficents = MatrixXtXInv * MatrixXtY;
    EigenVector valuesAtPoints = coefficents.transpose()*MatrixX.transpose();

    return valuesAtPoints;
}

EigenVector filterSignal(const EigenVector& signal, const int range, const int polyfitOrder)
{
    auto impulseResponse = getImpulseResponse(range, polyfitOrder);

    //generate pre and post signal data
    EigenVector preX = signal.block(0,0,range,1);
    EigenVector postX = signal.block(signal.rows()-range,0,range,1);

    EigenVector ecg(preX.rows() + signal.rows() + postX.rows());

    ecg << preX,signal,postX;

    EigenVector mliiFilterred(signal.rows());

    for(auto n = 0; n < signal.rows(); n++)
    {
        mliiFilterred[n] = 0;
        for(auto m = n-range; m <= n+range; m++)
        {
            mliiFilterred[n] += impulseResponse[n-m+range]*ecg[m+range];
        }
    }

    return mliiFilterred;
}

int main()
{
//    printHelloMessage();
//
//    int decision;
//    while(true)
//    {
//        std::cout << "> ";
//        std::cin >> decision;
//
//        if(decision == 0)
//        {
//            clearConsole();
//            return 0;
//        }
//        else if(decision == 1)
//        {
            MITDbHandler dataHandler;

            //Read data from MIT-BIH (parsed to txt: ../../mit-bih-txt/mitdb100short.txt)
            dataHandler.readMITBHDataFromTxt(MITDBH_FILE);

            const int M = 3; // half of the range
            const int N = 2; // order of the polyfit

            EigenVector time = dataHandler.getTime();
            EigenVector mlii = dataHandler.getMlii();
            EigenVector v5 = dataHandler.getV5();

            EigenVector mliiFilterred = filterSignal(mlii, M, N);
            EigenVector v5Filterred = filterSignal(v5, M, N);

            dataHandler.saveMliiFilterred(mliiFilterred);
            dataHandler.saveV5Filterred(v5Filterred);

            dataHandler.saveSignalsToFile("/home/piotr/Studia/ESDMiT_Savitzky-Golay/implem/pliczek.txt");
            plotOutput("/home/piotr/Studia/ESDMiT_Savitzky-Golay/implem/pliczek.txt");

//        }
//        else if(decision == 2)
//        {
//            system(GNUPLOT_MITDB);
//        }
//        else if(decision == 3)
//        {
//            system(GNUPLOT_COMMAND);
//        }
//        else if(decision == 4)
//        {
//            printHelloMessage();
//        }
//    }
}
