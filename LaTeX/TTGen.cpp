// Tony H. Bao, 6.10.2004 (Swansea)

//Program: Automatic Time Table Generation (From clear text table outputed by SQL to LaTeX Document)
//Last Modified Date: 2004.10.14
//Version: 0.2.006


#include <Transitional/LaTeX/Doc_Gen.hpp>


int main() {
  
  const std::string fin_name = "Data/Timetable_SQL";
  
  typedef std::vector<Table_Object::Table<std::string>::Value_Type> Value_Vector;
  typedef std::vector<Table_Object::Table<std::string>::Value_Type>::iterator Value_Vector_Iterator;
 
  Table_Object::Table<std::string> in(fin_name.c_str());
  const Table_Object::Table<std::string>::Key_Type t_tblock = in.keys.at(15), t_level = in.keys.at(2);

  Value_Vector v_tblock = in.gather_distinct(t_tblock); std::sort(v_tblock.begin(),v_tblock.end());
  Value_Vector v_level = in.gather_distinct(t_level); std::sort(v_level.begin(),v_level.end());
  
  for (Value_Vector_Iterator i = v_tblock.begin(); i != v_tblock.end(); i++){
    for (Value_Vector_Iterator j = v_level.begin(); j != v_level.end(); j++){
      const std::string fout_name = "CSTimetable_" + *i + "_" + "Level" + *j + ".tex";  
      Doc_Gen::CSTimeTable T(fin_name.c_str(),fout_name.c_str(), *i, *j);
      std::cout<<"Generating "<<fout_name<<'\n';
    }
    const std::string fout_name = "CSTimetable_" + *i + "_" + "AllLevels" + ".tex";
    Doc_Gen::CSTimeTable T(fin_name.c_str(),fout_name.c_str(), *i);
    std::cout<<"Generating "<<fout_name<<'\n';
    }
  std::cout<<"Done!\n";
  return 0;
}
