#include <iostream>
#include "MITDbHandler.h"
#include <Eigen/Dense>


#include <cstdlib>
#include <sstream>

#define MITDBH_FILE "/home/piotr/Studia/ESDMiT_Savitzky-Golay/mit-bih-txt/mitdb100short.txt"
#define GNUPLOT_MITDB "gnuplot -p -c /home/piotr/Studia/ESDMiT_Savitzky-Golay/implem/src/plot.data"
#define GNUPLOT_COMMAND "gnuplot -p -c /home/piotr/Studia/ESDMiT_Savitzky-Golay/implem/src/plot.gn"

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
    std::cout << "||                                                         ||\n";
    std::cout << "||  0 - Exit                                               ||\n";
    std::cout << "||                                                         ||\n";
    std::cout << "||_________________________________________________________||\n";
}

//jakie funkcje w eigenie przydante:
// transpose()
// fill()
// linspaced()

int main()
{
    printHelloMessage();

    int decision;
    while(true)
    {
        std::cout << "> ";
        std::cin >> decision;

        if(decision == 0)
        {
            clearConsole();
            return 0;
        }
        else if(decision == 1)
        {
            MITDbHandler dataHandler;

            //Read data from MIT-BIH (parsed to txt: ../../mit-bih-txt/mitdb100short.txt)
            dataHandler.readMITBHDataFromTxt(MITDBH_FILE);

            Eigen::VectorXf time;
            Eigen::VectorXf mlii;
            Eigen::VectorXf v5;


            const int M = 3;
            const int N = 2; // for 2nd degree

            Eigen::VectorXf impulseY(2*M+1);
            Eigen::VectorXf impulseX = Eigen::VectorXf::LinSpaced(2*M+1, -M, M);

            //initialize with comma initializer
            impulseY << 0,0,0,1,0,0,0;

            Eigen::MatrixXf MatrixX(2*M+1, N + 1);
            Eigen::VectorXf MatrixY = impulseY;

            //polyfit
            for ( size_t nRow = 0; nRow < 2*M+1; nRow++ )
            {
                float nVal = 1.0f;
                for ( int nCol = 0; nCol < N + 1; nCol++ )
                {
                    MatrixX(nRow, nCol) = nVal;
                    nVal *= impulseX[nRow];
                }
            }

            Eigen::MatrixXf MatrixXt = MatrixX.transpose();
            Eigen::MatrixXf MatrixXtX = MatrixXt * MatrixX;
            Eigen::MatrixXf MatrixXtY = MatrixXt * MatrixY;
            Eigen::MatrixXf MatrixXtXInv = MatrixXtX.inverse();
            Eigen::VectorXf coefficents = MatrixXtXInv * MatrixXtY;

            std::cout << "coefficents::\n" << coefficents.transpose() << std::endl;
            std::cout << "MatrixX::\n" << MatrixX.transpose() << std::endl;
            //do we need to flip it?
            Eigen::VectorXf valuesAtPoints = coefficents.transpose()*MatrixX.transpose();

            std::cout << "velues at points:\n" <<valuesAtPoints.transpose() << std::endl;;

            Eigen::MatrixXf signal = Eigen::VectorXf::Random(100);
//            std::cout << "Signal:" << std::endl << signal << "\n";

            //generate pre and post signal data
            Eigen::VectorXf preX = signal.block(0,0,M,1);
            Eigen::VectorXf postX = signal.block(signal.rows()-M,0,M,1);
            Eigen::VectorXf mirror(preX.rows() + postX.rows());
            mirror << preX, postX;

//            std::cout << "preX:\n" << preX << "\npostX\n" << postX <<std::endl;
//            std::cout << "mirror:\n" << mirror <<std::endl;

            std::cout << "Time:\n" << dataHandler.getTime() << std::endl;
            std::cout << "MLII:\n" << dataHandler.getMlii() << std::endl;
            std::cout << "V5:\n" << dataHandler.getV5() << std::endl;

        }
        else if(decision == 2)
        {
            system(GNUPLOT_MITDB);
        }
        else if(decision == 3)
        {
            system(GNUPLOT_COMMAND);
        }
        else if(decision == 4)
        {
            printHelloMessage();
        }
    }
}
