#include <SavitzkyGolayFilter.h>
#include "Common.h"
#include <ctime>

#define MITDBH_FILE "../../mit-bih-txt/mitdb100short.txt"

void plotOutput(std::string filename)
{
    //gnuplot -e "filename='$FILENAME'" ../../implem/src/plot.data
    std::string command = "gnuplot -e \"filename='";
    command += filename + "'\" ../../implem/src/plot.data &";

    system(command.c_str());
}

void clearConsole()
{
    std::cout << "\x1B[2J\x1B[H";
}

void printHelloMessage()
{
    std::vector<std::string> filenames;
    filenames.push_back("plik100.txt");
    filenames.push_back("plik200.txt");
    auto filteredFiles = filenames.size();

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
    std::cout << "||  1 - Read .txt data and smooth it                       ||\n";
    std::cout << "||  2 - Plot .txt data                                     ||\n";
    std::cout << "||  3 - Clear the console                                  ||\n";
    std::cout << "||  4 - Save to file as .png                               ||\n";
    std::cout << "||                                                         ||\n";
    std::cout << "||  Inserted files:                                        ||\n";

    const auto LINE_WIDTH = 51;
    for(size_t i = 0; i < filteredFiles; i++)
    {
        std::cout << "||  [" << i << "] " << filenames[i] << std::string(LINE_WIDTH-filenames[i].length(), ' ') << "||\n";
    }

    std::cout << "||                                                         ||\n";
    std::cout << "||  0 - Exit                                               ||\n";
    std::cout << "||                                                         ||\n";
    std::cout << "||_________________________________________________________||\n";

}

//jakie funkcje w eigenie przydante:
// transpose()
// fill()
// linspaced()

EigenVector getImpulseResponse(const int range, const int polyfitOrder)
{
    EigenVector impulseY(2*range+1);
    EigenVector impulseX = EigenVector::LinSpaced(2*range+1, -range, range);
    EigenVector zeroVec = EigenVector::Zero(range);

    impulseY << zeroVec,1,zeroVec;

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

    EigenVector filterred(signal.rows());

    for(auto n = 0; n < signal.rows(); n++)
    {
        filterred[n] = 0;
        for(auto m = n-range; m <= n+range; m++)
        {
            filterred[n] += impulseResponse[n-m+range]*ecg[m+range];
        }
    }

    return filterred;
}

int is_numeric (const std::string& str)
{
    std::istringstream ss(str);
    int num;
    ss >> num;      // try to read the number
    ss >> std::ws;  // eat whitespace after number

    if (!ss.fail() && ss.eof())
    {
        return num;  // is-a-number
    }
    else
    {
        return -1; // not-a-number
    }
}

int processData(std::string inputFile, std::string outputFile, bool outputPlotEnabled)
{
    std::cout << "Following arguments provided:\n";
    std::cout << "Input file: " << inputFile << "\n";
    std::cout << "Output file: " << outputFile << "\n";
    std::cout << "Output plot: " << (outputPlotEnabled ? "enabled\n" : "disabled\n");

    SavitzkyGolayFilter dataHandler;

    clock_t begin = clock();

    if(!dataHandler.readMITBHDataFromTxt(inputFile))
    {
        return -1;
    }

    const int M = 3; // half of the range
    const int N = 2; // order of the polyfit

    EigenVector time = dataHandler.getTime();
    EigenVector mlii = dataHandler.getMlii();
    EigenVector v5 = dataHandler.getV5();

    EigenVector mliiFilterred = filterSignal(mlii, M, N);
    EigenVector v5Filterred = filterSignal(v5, M, N);

    dataHandler.saveMliiFilterred(mliiFilterred);
    dataHandler.saveV5Filterred(v5Filterred);

    dataHandler.saveSignalsToFile(outputFile);

    clock_t end = clock();

    std::cout << "Elapsed time: " << 1000 * ((double)(end - begin) / CLOCKS_PER_SEC) << " [ms]\n";

    if(outputPlotEnabled)
    {
        plotOutput(outputFile);
    }

    return 0;
}

int main(int argc, char** argv)
{
    if(1 == argc)
    {
        printHelloMessage();

        while(true)
        {
            std::string input;
            std::cout << "> ";
            std::cin >> input;

            int decision = is_numeric(input);

            if(decision == 0)
            {
                clearConsole();
                return 0;
            }
            else if(decision == 1)
            {
                SavitzkyGolayFilter dataHandler;

                //Read data from MIT-BIH (parsed to txt: ../../mit-bih-txt/mitdb100short.txt)
                std::string fileToRead;
                std::cout << "Insert filename: ";
                std::cin >> fileToRead;

                const std::string outputFile = "../../implem/output/pliczek.txt";
                processData(fileToRead, outputFile, true);
            }
            else if(decision == 2)
            {
                //plot
            }
            else if(decision == 3)
            {
                printHelloMessage();
            }
            else
            {
                std::cout << "Unsupported decision. Please insert correct number ;)" << std::endl;
            }
        }
    }
    else if (4 == argc)
    {
        //run from command line in format:
        //SavGol inputFile outputFile [plot]
        std::string inputFile = argv[1];
        std::string outputFile = argv[2];
        bool outputPlotEnabled = "plot" == std::string(argv[3]);

        processData(inputFile, outputFile, outputPlotEnabled);
    }
}
