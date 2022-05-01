#ifndef __FILTFILT_CPP__
#define __FILTFILT_CPP__
#include "FiltFilt.h"

namespace kb{
namespace math{

template<typename T>
std::vector<T> FiltFilt<T>::ResizeSignal(std::vector<T>& p_output, unsigned int p_shrink) {
  return std::vector<T>(p_output.begin() + p_shrink, p_output.end() - p_shrink);
}

template<typename T>
std::vector<T> FiltFilt<T>::CumulativeSum(std::vector<T>& p_signal) {
  std::vector<T> sum(p_signal.size());
  std::partial_sum(p_signal.cbegin(), p_signal.cend(), sum.begin(), std::plus<double>());
  return sum;
}

template<typename T>
void FiltFilt<T>::FlipLR(std::vector<T>& p_signal) {
  std::reverse(p_signal.begin(), p_signal.end());
}

template<typename T>
std::vector<T> FiltFilt<T>::InitializeSi()
{
    const std::vector<T>& a = this->m_FilterCoefficients.m_CoefficientsA;
    const std::vector<T>& b = this->m_FilterCoefficients.m_CoefficientsB;
    const size_t order = this->m_FilterCoefficients.m_CoefficientsA.size();

    size_t lrefl = static_cast<size_t>(3 * (order - 1));

    T kdc = std::accumulate(b.begin(), b.end(), 0.0) /
                std::accumulate(a.begin(), a.end(), 0.0);

    std::vector<T> si;
    if (std::isfinite(std::abs(kdc))) 
    {
        std::vector<T> lv(order);
        for (int i = 0; i < order; ++i) {
            lv.at(i) = b.at(i) - kdc * a.at(i);
        }
        FlipLR(lv);
        si = CumulativeSum(lv);
        FlipLR(si);
    } else {
        //Initialize with empty vector if an initialization is not possible
        si = std::vector<T>(order);
    }
    si.erase(si.begin());
    return si;
}

template<typename T>
void FiltFilt<T>::PadSignal(std::vector<T> &p_signal,unsigned int p_size, T p_value) {
  std::vector<T> leftpad = std::vector<T>(p_size,p_value);
  std::vector<T> rightpad = std::vector<T>(p_size,p_value);
  p_signal.insert(p_signal.begin(), leftpad.cbegin(), leftpad.cend());
  p_signal.insert(p_signal.end(), rightpad.cbegin(), rightpad.cend());
}

template<typename T>
std::vector<T>&& FiltFilt<T>::FilterDirectFormII(const std::vector<T> &p_inputSignal) {
  std::vector<T> a = this->m_FilterCoefficients.m_CoefficientsA;
  std::vector<T> b = this->m_FilterCoefficients.m_CoefficientsB;
  const size_t order = a.size();
  std::vector<T> signal = p_inputSignal;
  PadSignal(signal,order);
  this->m_Output = std::vector<double>(signal.size());

  std::reverse(a.begin(), a.end());
  std::reverse(b.begin(), b.end());

  T cacheA, cacheB = 0;
  for (size_t i = 1; i < p_inputSignal.size(); ++i) {
    cacheA = cacheB = 0;
    for (int j = 0; j < order; ++j) {
      cacheB += b.at(j) * signal.at(i + j);
      cacheA += (-a.at(j)) * this->m_Output.at(i + j);
    }
    this->m_Output.at(i + (order - 1)) = (cacheA + cacheB);
  }

  this->m_Output = ResizeSignal(this->m_Output, order);
  return this->m_Output;
}

template<typename T>
std::vector<T>&& FiltFilt<T>::ZeroPhaseFiltering(const std::vector<T>& p_inputVector)
{
    const size_t order = this->m_FilterCoefficients.m_CoefficientsA.size();
    size_t lrefl = static_cast<size_t>(3 * (order - 1));
    std::vector<T> si = InitializeSi();
    std::vector<T> v;
    v.reserve(lrefl+p_inputVector.size());
    for (int i = lrefl; i >= 1; --i) {
        v.push_back(2 * p_inputVector.at(0) - p_inputVector.at(i));
    }
    for (size_t i = 0; i < p_inputVector.size(); ++i) {
        v.push_back(p_inputVector.at(i));
    }
    std::vector<T> lv;
    lv.reserve(lrefl-1);
    T end = p_inputVector.at(p_inputVector.size() - 1);
    for (size_t i = p_inputVector.size() - 2; i >= (p_inputVector.size() - lrefl - 1); --i) {
        v.push_back(2 * end - p_inputVector.at(i));
        lv.push_back(2 * end - p_inputVector.at(i));
    }
    std::vector<T> zzi;
    for (size_t i = 0; i < si.size(); ++i) {
        zzi.push_back(si.at(i) * v.at(0));
    }

    std::vector<T> forwardFiltered = this->Filter(v,zzi);

    std::reverse(forwardFiltered.begin(), forwardFiltered.end());
    std::vector<T> zi;
    for (size_t i = 0; i < si.size(); ++i) {
        zi.push_back(si.at(i) * forwardFiltered.at(0));
    }
    std::vector<T> reverseFiltered = this->Filter(forwardFiltered,zi);

    std::reverse(reverseFiltered.begin(), reverseFiltered.end());
    this->m_Output = ResizeSignal(reverseFiltered, lrefl);
    return std::move(this->m_Output);
}
}
}
#endif