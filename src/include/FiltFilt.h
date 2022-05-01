#ifndef __FILTFILT_H__
#define __FILTFILT_H__

#include <numeric>
#include <algorithm>
#include <cmath>

#include "Filt.h"

namespace kb{
namespace math{
template<typename T>
/**
 * @clas FiltFilt
 * @details FiltFilt class for zero phase filtering
 * @author kbaur
 * @date 2022
 */
class FiltFilt: public Filt<T>{
public:
using Filt<T>::Filt;
    /**
     * @brief Constructor
     * @details Filtfilt function from 'Likhterov & Kopeika, 2003. "Hardware-efficient technique for minimizing startup transients in Direct Form II digital filters"'
     * @param p_inputVector 
     * @return std::vector<T>&& 
     */
    std::vector<T>&& ZeroPhaseFiltering(const std::vector<T>& p_inputSignal);
    /**
     * @brief FilterDirectFormII
     * 
     * @param signal1 
     * @return std::vector<double> 
     */
    std::vector<T>&& FilterDirectFormII(const std::vector<T> &p_inputSignal);
protected:
    /**
     * @brief PadSignal
     * @details Pad a signal with a window size p_size and a value p_value
     * @param p_signal 
     * @param p_size 
     * @param p_value 
     */
    void PadSignal(std::vector<T> &p_signal,unsigned int p_size, T p_value = 0.0);
    /**
     * @brief ResizeSignal
     * @details Shrink a signal by a window size p_shrink
     * @param p_output 
     * @param p_shrink 
     * @return std::vector<T> 
     */
    std::vector<T> ResizeSignal(std::vector<T>& p_output,unsigned int p_shrink);
    /**
     * @brief CumulativeSum
     * 
     * @param p_signal 
     * @return std::vector<T> 
     */
    std::vector<T> CumulativeSum(std::vector<T>& p_signal);
    /**
     * @brief InitializeSi
     * @details Initialize Si a factor needed by the filtfilt function
     * @return std::vector<T> 
     */
    std::vector<T> InitializeSi();
    /**
     * @brief FlipLR
     * 
     * @param p_signal 
     */
    void FlipLR(std::vector<T>& p_signal);
};
}
}

#include "FiltFilt.cpp"

#endif