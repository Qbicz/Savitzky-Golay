#include <SavitzkyGolayFilter.h>
#include "Common.h"
#include <ctime>
#include <cstdlib>
#include <chrono>

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

void printUsage()
{
    std::cout << "Usage:\n";
    std::cout << "./SavGol.o -i file -o file [-n N] [-m M] [plot]\n";
    std::cout << "Options:\n";
    std::cout << " --input, -i file  : name of the input file\n";
    std::cout << " --output, -o file : name of the output file\n";
    std::cout << " --plot, -p        : plot filtered signal at the end; default: disabled\n";
    std::cout << " -n N              : half of the filtering range; default: 2\n";
    std::cout << " -m M              : half of the filtering range; default: 5\n";
    std::cout << "Example:\n";
    std::cout << "./SavGol.o -i ../../mit-bih-txt/mitdb100.txt -o ../output/out_mitdb100.txt -n 2 -m 100\n";
}

int processData(const std::string inputFile, const std::string outputFile, const int N, const int M, const bool outputPlotEnabled)
{
    std::cout << "Following arguments provided:\n";
    std::cout << "Input file: " << inputFile << "\n";
    std::cout << "Output file: " << outputFile << "\n";
    std::cout << "N - polynomial order: " << N << "\n";
    std::cout << "M - half of the range: " << M << "\n";
    std::cout << "Output plot: " << (outputPlotEnabled ? "enabled\n" : "disabled\n");

    SavitzkyGolayFilter dataHandler;

    if(!dataHandler.readMITBHDataFromTxt(inputFile))
    {
        return -1;
    }

    EigenVector time = dataHandler.getTime();
    EigenVector mlii = dataHandler.getMlii();
    EigenVector v5 = dataHandler.getV5();

    //clock_t begin = clock();
    // Microseconds count, since C++11
    auto start = std::chrono::high_resolution_clock::now();
    
    EigenVector mliiFilterred = filterSignal(mlii, M, N);
    
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    //clock_t end = clock();
    
    EigenVector v5Filterred = filterSignal(v5, M, N);

    dataHandler.saveMliiFilterred(mliiFilterred);
    dataHandler.saveV5Filterred(v5Filterred);

    dataHandler.saveSignalsToFile(outputFile);

    std::cout << "Elapsed " << microseconds << " [us]\n";
    //std::cout << "Elapsed time: " << 1000 * ((double)(end - begin) / CLOCKS_PER_SEC) << " [ms]\n";

    if(outputPlotEnabled)
    {
        plotOutput(outputFile);
    }

    return 0;
}

int main(int argc, char** argv)
{
    bool plotEnabled = false;
    std::string inputFile = "";
    std::string outputFile = "";
    int N = 2;
    int M = 5;

    for(auto i = 0; i < argc; i++)
    {
        std::string argument = argv[i];
        if ((argument == "-i" || argument == "--input") && (i < argc))
        {
            inputFile = argv[++i];
        }
        else if ((argument == "-o" || argument == "--output") && (i < argc))
        {
            outputFile = argv[++i];
        }
        else if ((argument == "-m") && (i < argc))
        {
            M = atoi(argv[++i]);
        }
        else if ((argument == "-n") && (i < argc))
        {
            N = atoi(argv[++i]);
        }
        else if (argument == "--plot" || argument == "-p")
        {
            plotEnabled = true;
        }
        else if (argc < 5)
        {
            printUsage();
            exit(1);
        }
    }

    processData(inputFile, outputFile, N, M, plotEnabled);
}
