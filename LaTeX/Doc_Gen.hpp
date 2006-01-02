/*!
  \file Doc_Gen.hpp
  \brief Created by TB for the purpose of writing the latex-files for the departmental timetable.
*/

//Program: Class for Generating documents using Doc_Objects and different handlers;
//Author: Tony H. Bao
//Location: Swansea
//Date: 2004.10.7
//Last Modified Date: 2004.10.14
//Version: 0.1.040

#ifndef Doc_Gen_nbhguay7nq2
#define Doc_Gen_nbhguay7nq2

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <limits>

#include <boost/lexical_cast.hpp>

#include "Latex_Handler_Implementation.hpp"
#include "Doc_Object_Implementation.hpp"
#include "Table_Object_Implementation.hpp"

namespace Doc_Gen {
  
  template <typename T>
  class Sort_Time {
  public:
    bool operator() (const T& x, const T& y) const {
      return boost::lexical_cast<int>(x) < boost::lexical_cast<int>(y);	 
    }
  };
  
  template <typename T>
  class Sort_Module {
  public:
    bool operator() (const T& x, const T& y) const {
      const std::string t_module = "module_abbr";
      return (*(x.find(t_module))).second < (*(y.find(t_module))).second;
    }
  };
  
  template <typename T>
  class Sort_Lecturer {
  public:
    bool operator() (const T& x, const T& y) const {
      const std::string t_last_name = "last_name";
      return (*(x.find(t_last_name))).second < (*(y.find(t_last_name))).second;	   
    }
  };
  
  template <typename T>
  class Sort_Building_Room {
  public:
    bool operator() (const T& x, const T& y) const {
      const std::string t_building = "build_abbr";
      const std::string t_room = "room_abbr";
      if ((*(x.find(t_building))).second == (*(y.find(t_building))).second)
	return (*(x.find(t_room))).second < (*(y.find(t_room))).second;
      else
	return (*(x.find(t_building))).second < (*(y.find(t_building))).second;
    }
  };
  
  template <typename T>
  class Unique_Module_Name {
  public:
    bool operator() (const T& x, const T& y) const {
      const std::string t_module = "module_abbr";
      return (*(x.find(t_module))).second == (*(y.find(t_module))).second;	 
    }
  };
  
  template <typename T>
  class Unique_Lecturer {
  public:
    bool operator() (const T& x, const T& y) const {
      const std::string t_last_name = "last_name";
      return (*(x.find(t_last_name))).second == (*(y.find(t_last_name))).second;	   	 
    }
  };
  
  template <typename T>
  class Unique_Building_Room{
  public:
    bool operator() (const T& x, const T& y) const {
      const std::string t_building = "build_abbr";
      const std::string t_room = "room_abbr";
      return (*(x.find(t_building))).second == (*(y.find(t_building))).second and 
	(*(x.find(t_room))).second == (*(y.find(t_room))).second;	 
    }
  };
  
  template <typename T>
  class Unique_Module {
  public:
    bool operator() (const T& x, const T& y) const {
      const std::string t_module = "module_abbr";
      const std::string t_level = "level";
      return (*(x.find(t_module))).second == (*(y.find(t_module))).second and (*(x.find(t_level))).second != (*(y.find(t_level))).second;
    }
  };

  class CSTimeTable {
    std::ofstream Output_file;
    typedef Doc_Object::Latex_Object<std::string> DocType;
    typedef Table_Object::Table<std::string> TableType;
    typedef Table_Object::Table<std::string>::Key_Type Key_Type;
    typedef Table_Object::Table<std::string>::Value_Type Value_Type;

  public:
    CSTimeTable(){};

    CSTimeTable(const char* infilename){
      TableType in_table(infilename);
      DocType out_doc;
      
      std::cout<<(generate(in_table, out_doc));
    }
    
    CSTimeTable(const char* infilename, const char* outfilename){
      TableType in_table(infilename);
      DocType out_doc;
      
      if (not Output_file.is_open()) 
	f_open(outfilename);
      
      Output_file<<(generate(in_table, out_doc)); 
    }
    
    CSTimeTable(const char* infilename, const char* outfilename, const Value_Type& val){
      TableType in_table(infilename);
      Key_Type t_tblock = in_table.keys.at(15), t_level = in_table.keys.at(2);
      if (val[0] == 'T')
	in_table.t_filter_with(t_tblock, val);
      else
        in_table.t_filter_with(t_level, val);
      
      DocType out_doc;
      
      if (not Output_file.is_open()) 
	f_open(outfilename);
      
      Output_file<<(generate(in_table, out_doc)); 
    }
    
    CSTimeTable(const char* infilename, const char* outfilename, const Value_Type& teaching_block, const Value_Type& level){
      TableType in_table(infilename);
      Key_Type t_tblock = in_table.keys.at(15), t_level = in_table.keys.at(2);
      in_table.t_filter_with(t_tblock, t_level, teaching_block, level);
      DocType out_doc;
      
      if (not Output_file.is_open()) 
	f_open(outfilename);
      
      Output_file<<(generate(in_table, out_doc)); 
    }
    
    ~CSTimeTable(){
      if (Output_file.is_open()) 
	f_close();
    }

  protected: 
    void f_open(const char* outfilename){
      if (not Output_file.is_open())
        Output_file.open(outfilename,std::ios_base::out);
      assert(Output_file.is_open() == true);
    }
    
    void f_close(){
      if (Output_file.is_open())
	Output_file.close();
      assert(Output_file.is_open() == false);
    }
    
    bool is_even(const int& n) {
      return (n == (n / 2 ) * 2);
    }
    
    bool is_odd(const int& n) {
      return not is_even(n);
    }
    
    const DocType& generate(TableType& in, DocType& out){
      
      const Table_Object::Table<std::string>::Key_Type t_time = in.keys.at(14), t_date = in.keys.at(13), 
	t_module = in.keys.at(0), t_module_name = in.keys.at(1), t_building = in.keys.at(9), t_building_name = in.keys.at(10),
	t_room = in.keys.at(11), t_room_name = in.keys.at(12), t_lecturer = in.keys.at(5), t_tblock = in.keys.at(15), 
	t_year = in.keys.at(16), t_level = in.keys.at(2), t_first_name = in.keys.at(7), t_last_name = in.keys.at(6), 
	t_module_url = in.keys.at(4), t_lecturer_url = in.keys.at(8), t_credit = in.keys.at(3);
      
      const std::string tex_class="article", paper_style="a4paper,landscape",
	package1="colortbl",package2="a4",package3="fullpage",
	doc = "document",
	spacing1 = "4ex", spacing2="-3em", spacing3="1ex",
	format1= "hline", hdbox = "\\newcommand{\\hdbox}{\\makebox[4.6cm][c]}",
	heading="\\bf Time & \\bf \\hdbox{Monday} & \\bf \\hdbox{Tuesday} & \\bf \\hdbox{Wednesday} & \\bf \\hdbox{Thursday} & \\bf \\hdbox{Friday}"; 
     
      const std::string colors[5]={"\\newcommand{\\Cheading}{\\rowcolor[rgb]{.9,.6,.6}}","\\newcommand{\\Crows}{\\rowcolor{yellow}}", 
				   "\\newcommand{\\CLone}{\\color{red}}", "\\newcommand{\\CLtwo}{\\color{blue}}",
				   "\\newcommand{\\CLthree}{\\color{green}}"};

      const std::string heading_row_color = "\\Cheading", content_row_color = "\\Crows";
      using namespace Latex_Handler;
      
      //Document Setup
      
      doc_class(tex_class,11,paper_style) >> out;
      usepackage(package1) >> out;
      usepackage(package2) >> out;
      usepackage(package3) >> out;
      macro_now<std::string>() >> out;
      user_cmd(hdbox) >> out;
      for (int i = 0; i <= 4; i++) user_cmd(colors[i]) >> out;
      begin(doc) >> out;
      no_option_cmd<std::string>("enlargethispage","4em") >> out;
      no_option_cmd<std::string>("pagestyle","empty") >> out;
      
      //Setting title
      const std::string title_prefix="\\Large \\bf Computer Science Time Table ";
      const std::string title_line_break= " \\\\~\\\\ ";
      
      //Getting academic year
      std::vector<Table_Object::Table<std::string>::Value_Type> v_year = in.gather_distinct(t_year);
      assert(v_year.size()==1);
      int y = boost::lexical_cast<int>((*v_year.begin())[3]);
      y++; 
      const std::string title_academic_year = (*v_year.begin())+'/'+(*v_year.begin())[2]+boost::lexical_cast<std::string>(y); 
      
      //Getting level
      std::vector<Table_Object::Table<std::string>::Value_Type> v_level = in.gather_distinct(t_level);
      std::string title_level;
      const std::string print_nums[] = {"One \\& Two", "One", "Two", "Three", "Four"};
      if (v_level.size()==1) 
	if (*v_level.begin() == "MSc" or *v_level.begin() == "MEng") 
	  title_level = *v_level.begin() + " Level, "; 
	else 
	  title_level = "Level " + print_nums[boost::lexical_cast<int>(*v_level.begin())] + ", ";
      else 
	title_level = "All Levels, ";
      
      //Getting Teaching Block
      std::vector<Table_Object::Table<std::string>::Value_Type> v_tblock = in.gather_distinct(t_tblock);
      std::string title_tblock;
      if (v_tblock.size()==1) 
	title_tblock = "Teaching Block " + print_nums[boost::lexical_cast<int>((*v_tblock.begin())[2])]; 
      else 
	title_tblock = "Teaching Block " + print_nums[0];
      
      const std::string title = title_prefix + title_academic_year + title_line_break + title_level + title_tblock;
      env_center(title) >> out;
      
      vspace_n(spacing1) >> out;
      hspace_s(spacing2) >> out;
      env_tabular_begin<std::string>("c","|c|*{5}{|p{4.6cm}}|") >> out;
      s_cmd(format1) >> out;
      user_cmd(heading_row_color) + env_tabular_row(heading) >> out;
      s_cmd(format1) >> out;
      s_cmd(format1) >> out;
      
      //Getting Table contents by time and date
      
      std::string c;
      const std::string deli=" & ", termi="\\\\";
      
      
      //Gathering and sorting the times entries from the table
      
      std::vector<Table_Object::Table<std::string>::Value_Type> v_times = in.gather_distinct(t_time);
      std::sort(v_times.begin(),v_times.end(), Sort_Time<Table_Object::Table<std::string>::Value_Type>()); 
      
      std::string v_dates[] = {"Mon", "Tue", "Wed", "Thu", "Fri"};      
      Table_Object::Table<std::string>::Table_Vector tr;
      for (unsigned int x = 0; x < v_times.size(); ++x){
        c = v_times[x];      
        if (is_odd(x)) user_cmd(content_row_color) >> out;
        user_cmd(c) >> out;
        user_cmd(deli) >> out;
        for (int y = 0; y <= 4; ++y){
          tr = in.search(t_time, t_date, v_times[x], v_dates[y]);
	  
          //Sorting and filter out repeated modules
          std::sort(tr.begin(),tr.end(),Sort_Module<Table_Object::Table<std::string>::Entry_Type>());
          tr.erase(std::unique(tr.begin(),tr.end(),Unique_Module<Table_Object::Table<std::string>::Entry_Type>()),tr.end());
	  
          //Generating cells for the output
          std::string ent;
          const std::string c_level[] = {"","\\CLone ", "\\CLtwo ", "\\CLthree "};
          for (Table_Object::Table<std::string>::Entry_Iterator i = tr.begin(); i != tr.end();i++){
            Table_Object::Table<std::string>::Entry_Type e=*i;
            int cl = 0;
            if (v_level.size() != 1) {
              if ( (*(e.find(t_level))).second != "MSc" and (*(e.find(t_level))).second != "MEng") 
                cl = boost::lexical_cast<int>((*(e.find(t_level))).second);	      
              ent = ent + c_level[cl] + (*(e.find(t_module))).second + deli + 
                c_level[cl] + (*(e.find(t_building))).second + "-" + (*(e.find(t_room))).second + deli + 
                c_level[cl] + (*(e.find(t_lecturer))).second + termi;
            }
            else 
              ent = ent + (*(e.find(t_module))).second + deli + 
                (*(e.find(t_building))).second + "-" +
                (*(e.find(t_room))).second + deli +
                (*(e.find(t_lecturer))).second + termi; 
          }
          c = env_tabular_begin<std::string>("l","*{3}{l@{\\hspace{2mm}}}") + '\n' 
            + ent + '\n' + env_tabular_end<std::string>() ; 
          user_cmd(c) >> out;
          if (y != 4 ) user_cmd(deli) >> out;
        }
        user_cmd(termi) >> out;
        s_cmd(format1) >> out;  
      }
      
      env_tabular_end<std::string>() >> out;
      vspace_n(spacing3) >> out;user_cmd(termi) >> out;      
      no_option_cmd<std::string>("footnotesize","O Kullmann, O.Kullmann@Swansea.ac.uk; \\today; \\now") >> out;
      
      //List of modules with long names
      Table_Object::Table<std::string>::Table_Vector m_tr;
      m_tr = in.gather_e(t_module);
      std::sort(m_tr.begin(),m_tr.end(),Sort_Module<Table_Object::Table<std::string>::Entry_Type>());
      m_tr.erase(std::unique(m_tr.begin(),m_tr.end(),Unique_Module_Name<Table_Object::Table<std::string>::Entry_Type>()),m_tr.end());
      s_cmd<std::string>("newpage") >> out;
      s_cmd<std::string>("twocolumn") >> out;
      std::string spacing4 = "-2em";
      hspace_s(spacing4) >> out;
      env_tabular_begin<std::string>("c","|r|l|r|") >> out;
      s_cmd(format1) >> out;
      const std::string heading2 = "Code & Module Name & Lecturer";
      env_tabular_row(heading2) >> out;
      s_cmd(format1) >> out;
      s_cmd(format1) >> out;
      for (Table_Object::Table<std::string>::Entry_Iterator i = m_tr.begin(); i != m_tr.end();i++){
	std::string ent; Table_Object::Table<std::string>::Entry_Type e=*i;
	ent = (*(e.find(t_module))).second + deli + (*(e.find(t_module_name))).second + deli + 
	  (*(e.find(t_lecturer))).second + termi;
	user_cmd(ent) >> out;
	s_cmd(format1) >> out;	
      }
      env_tabular_end<std::string>() >> out;
      
      //List of Buildings and Rooms
      Table_Object::Table<std::string>::Table_Vector b_tr;
      b_tr = in.gather_e(t_building);
      std::sort(b_tr.begin(),b_tr.end(),Sort_Building_Room<Table_Object::Table<std::string>::Entry_Type>());
      b_tr.erase(std::unique(b_tr.begin(),b_tr.end(),Unique_Building_Room<Table_Object::Table<std::string>::Entry_Type>()),b_tr.end());
      s_cmd<std::string>("newpage") >> out;
      hspace_s(spacing4) >> out;
      env_tabular_begin<std::string>("c","|r|l|r|l|") >> out;
      s_cmd(format1) >> out;
      const std::string heading3 = "Building & Building Name & Room & Room Name";
      env_tabular_row(heading3) >> out;
      s_cmd(format1) >> out;
      s_cmd(format1) >> out;
      for (Table_Object::Table<std::string>::Entry_Iterator i = b_tr.begin(); i != b_tr.end();i++){
	std::string ent; Table_Object::Table<std::string>::Entry_Type e=*i;
	ent = (*(e.find(t_building))).second + deli + (*(e.find(t_building_name))).second + deli + 
	  (*(e.find(t_room))).second + deli + (*(e.find(t_room_name))).second +  termi;
	user_cmd(ent) >> out;
	s_cmd(format1) >> out;	
      }
      env_tabular_end<std::string>() >> out;
      
      /*
      //List of Lecturers and URLs
      Table_Object::Table<std::string>::Table_Vector l_tr;
      l_tr = in.gather_e(t_lecturer);
      std::sort(l_tr.begin(),l_tr.end(),Sort_Lecturer<Table_Object::Table<std::string>::Entry_Type>());
      l_tr.erase(std::unique(l_tr.begin(),l_tr.end(),Unique_Lecturer<Table_Object::Table<std::string>::Entry_Type>()),l_tr.end());
      //s_cmd<std::string>("newpage") >> out;
      hspace_s(spacing4) >> out;
      env_tabular_begin<std::string>("c","|r|l|") >> out;
      s_cmd(format1) >> out;
      const std::string heading4 = "Lecturer & URL";
      env_tabular_row(heading4) >> out;
      s_cmd(format1) >> out;
      s_cmd(format1) >> out;
      for (Table_Object::Table<std::string>::Entry_Iterator i = l_tr.begin(); i != l_tr.end();i++){
      std::string ent; Table_Object::Table<std::string>::Entry_Type e=*i;
      ent = (*(e.find(t_first_name))).second + " " + (*(e.find(t_last_name))).second + deli + 
	  (*(e.find(t_lecturer_url))).second + termi;
	  user_cmd(ent) >> out;
	  s_cmd(format1) >> out;	
	  }
	  env_tabular_end<std::string>() >> out;
      */
      end(doc) >> out;
      return out;
    }
  };
  
}
#endif

