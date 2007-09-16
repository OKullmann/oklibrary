// Tony H. Bao, 7.10.2004 (Swansea)

//Program: Class for different documnet objects; LaTeX, XML, etc...
//Last Modified Date: 2004.10.8
//Version: 0.2.007

#ifndef Doc_Object_huaytblaisd863
#define Doc_Object_huaytblaisd863

#include <vector>
#include <iostream>

namespace Doc_Object {
  
  template <typename InputType>
  class Latex_Object {
 
    typedef InputType In;
    typedef std::vector<InputType> Internal;
    Internal s;

   public:
    Latex_Object() {}
  
    template <typename T, typename charT, typename traits> friend 
    Latex_Object<T>& operator>> (std::basic_istream<charT, traits>& is,  Latex_Object<T>& obj); 
    
    template <typename T, typename charT, typename traits> friend 
    std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const Latex_Object<T>& obj);

    template <typename T> friend      
      Latex_Object<T>& operator>> (const T& str,  Latex_Object<T>& obj);
  
    ~Latex_Object() {}
  };

  template <typename T, typename charT, typename traits>
  Latex_Object<T>& operator>> (std::basic_istream<charT, traits>& is,  Latex_Object<T>& obj){
    T tmp;
    is >> tmp;
    obj.s.push_back(tmp);
    return obj;
  }
  
  template <typename T>
  Latex_Object<T>& operator>> (const T& str,  Latex_Object<T>& obj){
    obj.s.push_back(str);
    return obj;
  }

  template <typename T, typename charT, typename traits>
  std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& os, const Latex_Object<T>& obj) {
    for(typename Latex_Object<T>::Internal::const_iterator i=obj.s.begin(); i!=obj.s.end(); i++)
      os<<*i<<'\n';
    return os;
  }
}

#endif
