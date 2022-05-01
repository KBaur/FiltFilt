#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <assert.h>

#include "FiltFilt.h"


std::vector<double> splitCsv(const std::string& p_path,char delim =',')
{
    std::ifstream fs;
    std::vector<double> vec;
    vec.reserve(128);
    fs.open(p_path,std::ios_base::in);
    if(fs.is_open())
    {
        char c;
        std::string cache;
        cache.reserve(20);
        bool exit{false};
        while(fs.get(c)&&!exit){
            if(c=='\n')
            {
                vec.push_back(std::stod(cache));
                exit=true;
            }
            if(c!=delim)
            {
                cache.push_back(c);
            }else{
                vec.push_back(std::stod(cache));
                cache.clear();
                cache.reserve(20);
            }
        }
        fs.close();
    }
    return vec;
}

bool near(double a, double b, double eps)
{
    return std::abs(a-b)<eps;
}

void test1()
{
    /*
    Copy paste from the octave filtfilt example:
    [b, a]=butter(3, 0.1);                  # 5 Hz low-pass filter

    # b: 2.8982e-03   8.6946e-03   8.6946e-03   2.8982e-03
    # a: 1.0000  -2.3741   1.9294  -0.5321
    */
    kb::math::FilterCoefficients<double> fc{ 
        m_CoefficientsA:{1.0000,-2.374094743709352,1.929355669091215,-0.532075368312092}, 
        m_CoefficientsB:{2.898194633721429e-03,8.694583901164288e-03,8.694583901164288e-03,2.898194633721429e-03}
    };
    std::vector<double> signal = splitCsv("../FiltFilt/samples/signal1.csv");
    std::vector<double> filtRes = splitCsv("../FiltFilt/samples/filt_output1.csv");
    std::vector<double> filtfiltRes = splitCsv("../FiltFilt/samples/filtfilt_output1.csv");

    kb::math::Filt<double> filt(fc);
    kb::math::FiltFilt<double> filtfilt(fc);

    auto filtered = filt.Filter(signal);
    auto zeroPhaseFiltered = filtfilt.ZeroPhaseFiltering(signal);

    for(int i = 0; i<filtered.size();++i)
    {
        assert(near(filtered.at(i),filtRes.at(i),1e-6));
        assert(near(zeroPhaseFiltered.at(i),filtfiltRes.at(i),1e-6));
    }    
}

int main(int argc, char ** argv)
{
    std::cout << "Starting Test" << std::endl;
    test1();
    std::cout << "Success!" << std::endl;
    return EXIT_SUCCESS;
}