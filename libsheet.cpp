// libsheet.cpp
#include "libsheet.h"
#include <iostream>
#include <string>
#include <ctype.h>
#include <exception>
using namespace std;

int get_type(const string& input) {

  try{
    // First check if it's int
    int a = stoi(input);
    if (to_string(a).size() != input.size()) {
    	throw invalid_argument();
    } else {
    	return 0;
    }
  } 
  catch(invalid_argument& e) {
    bool found_dot = false;
    for (auto &i = input.begin(); i != input.end(); ++i) {
      if (!isdigit(*i)) {
        if (*i == '.' && !found_dot) {
        	found_dot = true;
        } else return 2; // it's a string
      } 
      return 1;
    }
  }
}

Sheet::Sheet(vector<string>& entry) {
	 
}