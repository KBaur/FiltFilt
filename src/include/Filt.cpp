#ifndef __FILT_CPP__
#define __FILT_CPP__
#include "Filt.h"

namespace kb{
namespace math{

template<typename T>
Filt<T>& Filt<T>::operator=(const Filt& p_filt)
{
    m_FilterCoefficients = p_filt.GetCoefficients();
    m_Output = p_filt.GetOutput();
    return *this;
}

template<typename T>
Filt<T>::Filt(const Filt& p_filt)
{
    m_FilterCoefficients = p_filt.GetCoefficients();
    m_Output = p_filt.GetOutput();
}

template<typename T>
Filt<T>::Filt(const FilterCoefficients<T>& p_coefficients)
{
    SetCoefficients(p_coefficients);
}


template<typename T>
T Filt<T>::CalculateOneStep(T p_input)
{
    T output = m_FilterCoefficients.m_CoefficientsB.at(0) * p_input + m_Delay.at(0);
    for (size_t j = 1; j <  m_FilterCoefficients.m_CoefficientsA.size(); ++j) 
    {
      m_Delay.at(j - 1) = m_FilterCoefficients.m_CoefficientsB.at(j) * p_input + m_Delay.at(j) -  m_FilterCoefficients.m_CoefficientsA.at(j) * output;
    }
    return output;
}

template<typename T>
std::vector<T>&& Filt<T>::Filter(const std::vector<T>& p_inputVector)
{
    return Filter(p_inputVector,std::vector<T>(m_FilterCoefficients.m_CoefficientsA.size()-1,0.0));
}

template<typename T>
std::vector<T>&& Filt<T>::Filter(const std::vector<T>& p_inputVector, const std::vector<T>& p_initialState)
{
  if(p_inputVector.size()<1)
  {
      throw std::invalid_argument("Signal must be larger than 0");
  }
  if(p_initialState.size()<m_FilterCoefficients.m_CoefficientsA.size()-1)
  {
      throw std::invalid_argument("Initial states must have coefficient size-1");
  }
  const size_t order = m_FilterCoefficients.m_CoefficientsA.size();
  m_Output = std::vector<T>(p_inputVector.size(),0);
  m_Delay = std::move(p_initialState);
  m_Delay.push_back(0);

  for (size_t i = 0; i < p_inputVector.size(); ++i) 
  {
      m_Output.at(i) = CalculateOneStep(p_inputVector.at(i));
  }
  return std::move(m_Output);
}

template<typename T>
std::vector<T>&& Filt<T>::operator()(const std::vector<T>& p_inputVector)
{
    return Filter(p_inputVector);
}

template<typename T>
FilterCoefficients<T> Filt<T>::GetCoefficients() const
{
    return m_FilterCoefficients;
}

template<typename T>
template<typename U>
void Filt<T>::SetCoefficients(U&& p_coefficients)
{
    m_FilterCoefficients = std::forward<U>(p_coefficients);
    if(m_FilterCoefficients.m_CoefficientsA.size()!=m_FilterCoefficients.m_CoefficientsB.size())
    {
        throw std::invalid_argument("Coefficients of A and B must have the same size!");
    }
}

template<typename T>
std::vector<T> Filt<T>::GetOutput() const
{
    return m_Output;
}

}
}
#endif