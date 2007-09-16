// Tony H. Bao, 7.10.2004 (Swansea)
/* Copyright 2004 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

//Program: Classes to Handle different kinds of tables (or pre-formated data streams)
//Last Modified Date: 2004.10.13
//Version: 0.2.018

#ifndef Table_Object_huuasd787y314j
#define Table_Object_huuasd787y314j

#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <map>
#include <utility>

namespace Table_Object {

  template <typename SourceType>
  class Table {
    
    std::ifstream input_file;

  public:
    
    typedef std::vector<SourceType> Source_Vector;
    typedef std::map<SourceType, SourceType> Source_Pair_Map;
    typedef std::vector<Source_Pair_Map> Table_Vector;
    typedef typename Source_Pair_Map::key_type Key_Type;
    typedef SourceType Value_Type;
    typedef typename Source_Pair_Map::iterator Iterator_Type;
   
    typedef typename Table_Vector::value_type Entry_Type;
    typedef typename Table_Vector::iterator Entry_Iterator;
    
  private:
    Table_Vector Final_Table;
    Table_Vector Search_Table;

  public:
    
    Source_Vector keys;
    
    Table(){};
    
    Table(const char* filename){
      if (not input_file.is_open()) 
	f_open(filename);
      f_load();
      Search_Table = Final_Table;
    }

    Table(const char* filename, const Key_Type& key, const Value_Type& val){
      if (not input_file.is_open()) 
	f_open(filename);
      f_load();
      t_filter_with(key, val);
    }

    Table(const char* filename, const Key_Type& key1, const Key_Type& key2, const Value_Type& val1, const Value_Type& val2){
      if (not input_file.is_open()) 
	f_open(filename);
      f_load();
      t_filter_with(key1, key2, val1, val2);
    }

    ~Table(){
     if (input_file.is_open()) 
	f_close();
    }
    
    Table_Vector search(const Key_Type& key1,const Key_Type& key2, const Value_Type& val1, const Value_Type& val2){
      Entry_Type r; Iterator_Type p,q; Table_Vector v; 
      for (Entry_Iterator i = Search_Table.begin();i!=Search_Table.end();i++) {
	r=*i; p = r.find(key1); q=r.find(key2);
	if (p != r.end() and (*p).second == val1) 
	  if (q != r.end() and (*q).second == val2)  
	    v.push_back(r);
      }
      return v;
    }
    
    Table_Vector search(const Key_Type& key, const Value_Type& val){
      Entry_Type r; Iterator_Type p; Table_Vector v; 
      for (Entry_Iterator i = Search_Table.begin();i!=Search_Table.end();i++) {
	r=*i; p = r.find(key);
	if (p != r.end() and (*p).second == val)   
	    v.push_back(r);
      }
      return v;
    }

      Table_Vector gather_e(const Key_Type& key){
      Entry_Type r; Iterator_Type p; Table_Vector v; 
      for (Entry_Iterator i = Search_Table.begin();i!=Search_Table.end();i++) {
	r=*i; p = r.find(key);
	if (p != r.end())   
	  v.push_back(r);
      }
      return v;
    }

    bool distinct(const typename std::vector<Value_Type>::iterator& begin, const typename std::vector<Value_Type>::iterator& end, const Value_Type& val) {
      for (typename std::vector<Value_Type>::iterator i = begin; i != end; i++)
	if (val == *i) return false;
      return true;
    }
    
    std::vector<Value_Type> gather_distinct(const Key_Type& key) {
      Entry_Type r; Iterator_Type p; std::vector<Value_Type> v;
      for (Entry_Iterator i = Search_Table.begin();i!=Search_Table.end();i++) {
	r=*i; p = r.find(key);
	if (p != r.end() and distinct(v.begin(),v.end(),(*p).second)) v.push_back((*p).second); 
      }
      return v;
    }
    
    std::vector<Value_Type> gather(const Key_Type& key) {
      Entry_Type r; Iterator_Type p; std::vector<Value_Type> v;
      for (Entry_Iterator i = Search_Table.begin();i!=Search_Table.end();i++) {
	r=*i; p = r.find(key);
	if (p != r.end()) v.push_back((*p).second); 
      }
      return v;
    }
    
    void t_filter_with(const Key_Type& key, const Value_Type& val) {
      Entry_Type r; Iterator_Type p; Table_Vector v;
      for (Entry_Iterator i = Final_Table.begin();i!=Final_Table.end();i++) {
	r=*i; p = r.find(key);
	if (p != r.end() and (*p).second == val) v.push_back(r); 
      }
      Search_Table = v;
    }
    
    void t_filter_with(const Key_Type& key1, const Key_Type& key2, const Value_Type& val1, const Value_Type& val2){
      Entry_Type r; Iterator_Type p,q; Table_Vector v; 
      for (Entry_Iterator i = Final_Table.begin();i!=Final_Table.end();i++) {
	r=*i; p = r.find(key1); q=r.find(key2);
	if (p != r.end() and (*p).second == val1) 
	  if (q != r.end() and (*q).second == val2)  
	    v.push_back(r);
      }
      Search_Table = v;
    }
    
    void t_revert_to_original(){
      Search_Table = Final_Table;
    }
    
  protected: 
    void f_open(const char* filename){
      if (not input_file.is_open())
        input_file.open(filename,std::ios_base::in);
      assert(input_file.is_open() == true);
    }
    
    void f_close(){
      if (input_file.is_open())
         input_file.close();
      assert(input_file.is_open() == false);
    }
    
    void f_load(){
      assert(input_file.is_open()==true);
      char c;
      Source_Vector vec_lines;
      std::string l="";
      
      while (input_file.get(c)) {
	l=l+c;
	if (c=='\n') {
	  vec_lines.push_back(l);l="";
	}	
      }
      
      //Getting key felids
      std::string k="";l=*vec_lines.begin();
      for (std::string::size_type j = 0;j<l.length();j++){  
	c=l[j];k=k+c;
	if (l[j+1]=='|' or l[j+1]=='\n'){keys.push_back(k);k="";j++;}
      }  

      //Organize and store value fields
      std::string v="";
      for (typename Source_Vector::iterator i = vec_lines.begin()+1;i!=vec_lines.end()-1;i++) {
	l=*i;
	typename Source_Vector::size_type id = 0;
	Source_Pair_Map x;
	for (std::string::size_type j = 0;j<l.length();j++){
	  c=l[j];v=v+c;
	  if (c == '|'){
	    x.insert(std::make_pair(keys.at(id),""));v="";id++;
	  } 
	  if ((l[j+1]=='|' or l[j+1]=='\n') and l[j]!='|'){
	    x.insert(std::make_pair(keys.at(id),v));v="";j++;id++;
	  }  
	}
	Final_Table.push_back(x);
      }
    }
  };

}
#endif
