#ifndef __FILT_H__
#define __FILT_H__

#include <vector>
#include <list>
#include <array>
#include <iostream>
#include <fstream>
#include <initializer_list>
#include <type_traits>

namespace kb{
namespace math{

template<typename T>
/**
 * @struct FilterCoefficients 
 * @author kbaur
 * @date 2022
 */
struct FilterCoefficients{
static_assert(std::is_floating_point<T>::value,"Template must be a floating point type!");
    std::vector<T> m_CoefficientsA;
    std::vector<T> m_CoefficientsB;
    /**
     * @brief 
     * 
     * @param p_ostream 
     * @param p_filterCoefficients 
     * @return std::ostream& 
     */
    friend std::ostream& operator<<(std::ostream& p_ostream, FilterCoefficients& p_filterCoefficients)
    {
      std::vector<std::vector<T>> vec{p_filterCoefficients.m_CoefficientsA,p_filterCoefficients.m_CoefficientsB};
      for(std::vector<T> & a: vec)
      {
        for(int i = 0; i<a.size()-1;++i)
        {
          p_ostream << a.at(i) << ",";
        }
        p_ostream << *(a.end()-1);
        p_ostream << std::endl;
      }
      return p_ostream;
    }
    /**
     * @brief 
     * 
     * @param p_istream 
     * @param p_filterCoefficients 
     * @return std::istream& 
     */
    friend std::istream& operator>>(std::istream& p_istream, FilterCoefficients& p_filterCoefficients)
    {
      char input;
      bool end {false};
      std::string buff;
      std::vector<double> currentVector;
      double val;
      int counter = 0;
      while(p_istream.get(input) && counter <248)
      {
        if(input=='\n'||input==','||input==';')
        {
          val = std::stod(buff);
          buff.clear();
          currentVector.push_back(val);
          if(input=='\n')
          {
            p_filterCoefficients.m_CoefficientsB = currentVector;
            end=true;
          }else if(input==';')
          {
            p_filterCoefficients.m_CoefficientsA = currentVector;
            currentVector.clear();
          }
        }else{
          buff.push_back(input);
        }
        ++counter;
      }
      return p_istream;
    }
};

template<typename T>
/**
 * @class Filt 
 * @details Digital filter class
 * @author kbaur
 * @date 2022
 */
class Filt{
public:
static_assert(std::is_floating_point<T>::value,"Template parameter must be a floating point type!");
    /**
     * @brief Construct a new Filt object
     * 
     * @param p_coefficients 
     */
    Filt(const FilterCoefficients<T>& p_coefficients);
    /**
     * @brief Construct a new Filt object
     * 
     * @param p_coefficients 
     */
    Filt(const Filt& p_coefficients);
    /**
     * @brief copy
     * 
     * @param p_filt 
     * @return Filt<T>& 
     */
    Filt<T>& operator=(const Filt& p_filt);
    /**
     * @brief Filter
     * 
     * @param p_inputVector 
     * @return std::vector<T>&& 
     */
    std::vector<T>&& Filter(const std::vector<T>& p_inputVector);
    /**
     * @brief Filter
     * 
     * @param p_inputVector 
     * @param p_initialState 
     * @return std::vector<T>&& 
     */
    std::vector<T>&& Filter(const std::vector<T>& p_inputVector, const std::vector<T>& p_initialState);
    /**
     * @brief operator()
     * 
     * @param p_inputVector 
     * @return std::vector<T>&& 
     */
    std::vector<T>&& operator()(const std::vector<T>& p_inputVector);
    /**
     * @brief Get the Coefficients object
     * 
     * @return FilterCoefficients<T> 
     */
    FilterCoefficients<T> GetCoefficients() const;
    /**
     * @brief Get the Output object
     * 
     * @return std::vector<T> 
     */
    std::vector<T> GetOutput() const;
    template<typename U>
    /**
     * @brief Set the Coefficients object
     * 
     * @param p_coefficients 
     */
    void SetCoefficients(U&& p_coefficients);
protected:
    T CalculateOneStep(T input);
    FilterCoefficients<T> m_FilterCoefficients;
    std::vector<T> m_Output;
    std::vector<T> m_Delay;
};
}
}

#include "Filt.cpp"

#endif