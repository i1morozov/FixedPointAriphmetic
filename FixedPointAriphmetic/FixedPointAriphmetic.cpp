#include <windows.h>
#include <conio.h>    // для нормальной отрисовки
#include <cmath>      // для синуса 
#include "easy_plot.hpp"

/*int main(int argc, char* argv[]) {
    easy_plot::EasyPlot::init(&argc, argv);

    //std::vector<double> x = { 0,1,0,1,2,0,1 };
    //easy_plot::EasyPlot::plot("X", x);

    // ставим красный цвет линии
    std::vector<double> y = { 0,1,4,9,16,32,64 };
    //easy_plot::EasyPlot::plot("Y", y, easy_plot::LineSpec(1, 0, 0));


    std::vector<double> x2 = { 0,1,2,3,4,5,6 };
    easy_plot::EasyPlot::plot("XY", x2, y, easy_plot::LineSpec(1, 1, 0));

    easy_plot::WindowSpec wstyle; // тут можно настроить стиль графика (цвет фона и пр.)
    // выводим три графика в одном
    //easy_plot::EasyPlot::plot<double>("Y1Y2Y3", wstyle, 3, x, easy_plot::LineSpec(1, 0, 0), x2, easy_plot::LineSpec(1, 0, 1), y, easy_plot::LineSpec(0, 1, 0));

    while (true) {
        std::this_thread::yield();
    }
    return 0;
}*/

#include <iostream>
#include "FixedPoint.h"
#include <vector>
#include "stdint.h";
#include <fstream>
#include <string>
#include "easy_plot.hpp"
//#include <gsl/gsl_fit.h>
//#include <gsl/gsl_statistics_double.h>

void filtration(std::vector<FixedPoint> &outputData, std::vector<FixedPoint> filterCoefficients, std::vector<FixedPoint> inputData)
{
    int count = inputData.size() - filterCoefficients.size();
    for (int i = 0; i < count; ++i) {
        filterCoefficients.push_back(FixedPoint((double)0));
    }
    for (int n = 0; n < inputData.size(); ++n) {
        int32_t sum = 0;
        for (int k = 0; k < inputData.size(); ++k) {
            
            FixedPoint tmp(filterCoefficients.at(k) * inputData.at(((n - k) > 0) ? (n - k) : 0));
            //tmp.printBinary();
            int32_t a = FixedPoint::mul(filterCoefficients.at(k), inputData.at(((n - k) > 0) ? (n - k) : 0));
            //std::cout << a << " "<< std::bitset<32>(a) <<std::endl;
            sum += a;
        }
       std::cout << std::bitset<32>(sum)<<std::endl;
        FixedPoint((int16_t)(sum / 65536)).printBinary();
        FixedPoint((int16_t)((sum<<1) >> 15)).printBinary();
        outputData.push_back(FixedPoint((int16_t)((sum<<1) >> 16)));
    }
}


void filtrationDouble(std::vector<double>& outputData, std::vector<double> filterCoefficients, std::vector<double> inputData)
{
    int count = inputData.size() - filterCoefficients.size();
    for (int i = 0; i < count; ++i) {
        filterCoefficients.push_back(0);
    }
    for (int n = 0; n < inputData.size(); ++n) {
        double sum = 0;
        for (int k = 0; k < inputData.size(); ++k) {

            double tmp(filterCoefficients.at(k) * inputData.at(((n - k) > 0) ? (n - k) : 0));
            //tmp.printBinary();
            //int32_t a = FixedPoint::mul(filterCoefficients.at(k), inputData.at(((n - k) > 0) ? (n - k) : 0));
            //std::cout << a << " "<< std::bitset<32>(a) <<std::endl;
            sum += tmp;
        }
        std::cout << std::bitset<32>(sum) << std::endl;
        //FixedPoint((int16_t)(sum / 65536)).printBinary();
        //FixedPoint((int16_t)((sum << 1) >> 15)).printBinary();
        outputData.push_back(sum);
    }
}



void readFromFile(std::vector<double>& tmpVector, std::string filename) {
    std::ifstream in(filename);
    std::string line;
    if (in.is_open())
    {
        while (std::getline(in, line))
        {
            tmpVector.push_back(stod(line));
        }
    }
    in.close();
}
void transformToFixedPoint(std::vector<FixedPoint>& fixedCoefficients, std::vector<double>& doubleCoefficients) {
    for (int i = 0; i < doubleCoefficients.size(); ++i) {
        fixedCoefficients.push_back(FixedPoint(doubleCoefficients.at(i)));
    }
}
void printVector(std::vector<FixedPoint> vec) {
    for (auto i : vec)
        std::cout << i << std::endl;
}
int main(int argc, char* argv[])
{
    std::vector<double> filterCoefficients;
    std::vector<double> inputCoefficients;
    std::vector<double> outputCoefficients;
    std::vector<FixedPoint> filterCoefficientsFixed;
    std::vector<FixedPoint> inputCoefficientsFixed;
    std::vector<FixedPoint> outputCoefficientsFixed;
    readFromFile(filterCoefficients, "filterCoefficients.txt");
    readFromFile(inputCoefficients, "inputCoefficients.txt");
    transformToFixedPoint(filterCoefficientsFixed, filterCoefficients);
    transformToFixedPoint(inputCoefficientsFixed, inputCoefficients);
    //filtration(outputCoefficientsFixed, filterCoefficientsFixed, inputCoefficientsFixed);
    //printVector(outputCoefficientsFixed);
    filtrationDouble(outputCoefficients, filterCoefficients, inputCoefficients);
    std::vector<int> coefficients;
    for (int i = 0; i < outputCoefficients.size(); ++i) {
        coefficients.push_back(i);
    }
    easy_plot::EasyPlot::init(&argc, argv);
    easy_plot::EasyPlot::plot("XY", coefficients, outputCoefficients, easy_plot::LineSpec(0, 0, 1));
    easy_plot::WindowSpec wstyle;
    while (true) {
        std::this_thread::yield();
    }
    return 0;

}

