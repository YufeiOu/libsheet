// libsheet.cpp
#include "libsheet.h"
#include <iostream>
#include <string>
#include <ctype.h>
#include <exception>
#include <cassert>
using namespace std;

int get_type(const string& input) {
  try{
    // First check if it's int
    int a = stoi(input);
    if (to_string(a).size() != input.size()) {
      throw invalid_argument("Not valid int");
    } else {
      return 0;
    }
  }
  catch(invalid_argument& e) {
    bool found_dot = false;
    for (string::const_iterator i = input.begin(); i != input.end(); ++i) {
      if (!isdigit(*i)) {
        if (*i == '.' && !found_dot) {
          found_dot = true;
        } else return 2; // it's a string
      }
    }
    return found_dot ? 1 : 2;
  }
}

Sheet::Sheet(vector<string>& entry, vector<string>& col_names) {
  assert(entry.size() == col_names.size()); // assume every entry is provided a column name
  auto j = col_names.begin();
  for (auto i = entry.begin(); i != entry.end(); ++i, ++j) {
    int type = get_type(*i);
        ColumnHead ch(*j, type);
        auto result = column_map.insert(pair<string, unsigned int>(*j, i - entry.begin()));
        if (!result.second) {
          throw "Duplicate column name at" + to_string(i - entry.begin());
        }
        switch (type) {
          case 0:
              ch.vint.push_back(stoi(*i));
              break;
            case 1:
              ch.vdouble.push_back(stod(*i));
              break;
            case 2:
              ch.vstring.push_back(*i);
              break;
            default:
              throw "Unexpected input type";
        }
    
    columns.push_back(ch);
  }
}

/* Don't know how to deal runtime type decision issue
template<typename T>
void Sheet::ColumnHead::push_back(T input) {
  switch (flag) {
    case 0:
      vint.push_back(input);
      break;
    case 1:
      vdouble.push_back(input);
      break;
    case 2:
      vstring.push_back(input);
      break;
    default:
      throw "Unexpected ColumnHead push_back flag";
  }
}
*/


