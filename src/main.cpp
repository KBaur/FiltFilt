#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

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

int main(int argc, char ** argv)
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

    //you can also read the coefficients from a stream, with the pattern [A;B\n]
    kb::math::FilterCoefficients<double> fc2;
    std::stringstream ss;
    ss << "1.0000,-2.374094743709352,1.929355669091215,-0.532075368312092;2.898194633721429e-03,8.694583901164288e-03,8.694583901164288e-03,2.898194633721429e-03\n";
    ss >> fc2;

    std::vector<double> signal = splitCsv("../FiltFilt/samples/signal1.csv");

    kb::math::Filt<double> filt(fc);
    kb::math::FiltFilt<double> filtfilt(fc2);
    
    auto filtered = filt.Filter(signal);
    auto zeroPhaseFiltered = filtfilt.ZeroPhaseFiltering(signal);

    for(auto e:filtered)
    {
        std::cout << e << ",";
    }
    std::cout << std::endl;

    for(auto e:zeroPhaseFiltered)
    {
        std::cout << e << ",";
    }
    std::cout << std::endl;
    
    return EXIT_SUCCESS;
}