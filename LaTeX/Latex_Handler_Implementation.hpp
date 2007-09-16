// Tony H. Bao, 6.10.2004 (Swansea)

//Program: Functions to handle LaTeX Commands
//Last Modified Date: 2004.10.14
//Version: 0.2.013

#ifndef Latex_Handler_jhocr4879kncaewe76
#define Latex_Handler_jhocr4879kncaewe76

#include <iostream>
#include <boost/lexical_cast.hpp>

namespace Latex_Handler {
  template <typename InputType>
  InputType no_option_cmd(const InputType& tex_cmd, const InputType& user_cmd) {
    InputType base_cmd="\\"+tex_cmd;
    InputType options="";
    InputType spec_cmd='{'+user_cmd+'}';
    return base_cmd+options+spec_cmd;
  }
  template <typename InputType>
  InputType star_option_cmd(const InputType& tex_cmd, const InputType& user_cmd) {
    InputType base_cmd="\\"+tex_cmd;
    InputType options="*";
    InputType spec_cmd='{'+user_cmd+'}';
    return base_cmd+options+spec_cmd;
  }
  
  template <typename InputType>
  InputType with_option_cmd(const InputType& tex_cmd, const InputType& user_options, const InputType& user_cmd) {
    InputType base_cmd="\\"+tex_cmd;
    InputType options='['+user_options+']';
    InputType spec_cmd='{'+user_cmd+'}';
    return base_cmd+options+spec_cmd;
  }

  template <typename InputType>
  InputType doc_class(const InputType& tex_class, const int& font_size, const InputType& paper_style="a4paper"){
    InputType option_spec=paper_style + ',' + boost::lexical_cast<InputType>(font_size)+"pt";
    InputType final_cmd=with_option_cmd(InputType("documentclass"),option_spec,tex_class);
    return final_cmd;
  }

  template <typename InputType>
  InputType usepackage(const InputType& package) {
    InputType final_cmd=no_option_cmd(InputType("usepackage"),package);
    return final_cmd;
  }

  template <typename InputType>
  InputType begin(const InputType& cmd) {
    InputType final_cmd=no_option_cmd(InputType("begin"),cmd);
    return final_cmd;
  }

  template <typename InputType>
  InputType end(const InputType& cmd) {
    InputType final_cmd=no_option_cmd(InputType("end"),cmd);
    return final_cmd;
  }

  template <typename InputType>
  InputType hspace_n(const InputType& cmd) {
    InputType final_cmd=no_option_cmd(InputType("hspace"),cmd);
    return final_cmd;
  }

  template <typename InputType>
  InputType s_cmd(const InputType& cmd) {
    InputType final_cmd=InputType("\\")+cmd;
    return final_cmd;
  }

  template <typename InputType>
  InputType hspace_s(const InputType& cmd) {
    InputType final_cmd=star_option_cmd(InputType("hspace"),cmd);
    return final_cmd;
  }

  template <typename InputType>
  InputType vspace_n(const InputType& cmd) {
    InputType final_cmd=no_option_cmd(InputType("vspace"),cmd);
    return final_cmd;
  }
  
  template <typename InputType>
  InputType cmd_repeat(const InputType& cmd, const int n) {
    InputType final_cmd;
    for (int i = 0; i < n; i++){
      final_cmd = final_cmd + cmd;
    }
    return final_cmd;
  }

  template <typename InputType>
  InputType env_center(const InputType& content) {	
    InputType final_cmd=begin<InputType>("center")+'\n'+content+'\n'+end<InputType>("center");
    return final_cmd;
  }

  template <typename InputType>
  InputType env_tabular_begin(const InputType& options="l", const InputType& format="lll") {	
    InputType final_cmd=with_option_cmd(InputType("begin{tabular}"), options, format);
    return final_cmd;
  }
  
  template <typename InputType>
  InputType env_tabular_row(const InputType& cmd) {
    InputType final_cmd = cmd + InputType("\\\\");
    return final_cmd;
  }
  
  template <typename InputType>
  InputType env_tabular_end(const InputType& cmd="tabular") {	
    InputType final_cmd=end(cmd);
    return final_cmd;
  }
  
  template <typename InputType>
  InputType user_cmd(const InputType& cmd){
    return cmd;
  }
  
  template <typename InputType>
  InputType with_color(const InputType& cmd) {
    InputType final_cmd=no_option_cmd(InputType("color"),cmd);
    return final_cmd;
  }

  template <typename InputType>
  InputType macro_now() {
    InputType final_cmd="\\newcount\\minute\n\\newcount\\hour\n\\newcount\\hourMins\n\\def\\zeroPadTwo#1{\\ifnum #1<10 0\\fi#1}\n\\def\\now\n{\\minute=\\time\n\\hour=\\time \\divide \\hour by 60\n\\hourMins=\\hour \\multiply\\hourMins by 60\n\\advance\\minute by -\\hourMins\n\\zeroPadTwo{\\the\\hour}:\\zeroPadTwo{\\the\\minute}}";
    return final_cmd;
  }

  
} 

#endif
