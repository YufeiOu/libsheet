// libsheet.cpp
#include "libsheet.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <exception>
#include <cassert>
#include <sstream>
using namespace std;

string trim(string& str)
{
  if (!str.size()) return str;
  size_t first = str.find_first_not_of(' ');
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last-first+1));
}

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

void Sheet::row_append(vector<string> &new_row) {
  assert(new_row.size() == columns.size());
  auto ch = columns.begin();
  for (auto &r : new_row) {
    switch (ch->flag) {
      case 0:
        ch->vint.push_back(stoi(r));
        break;
      case 1:
        ch->vdouble.push_back(stod(r));
        break;
      case 2:
        ch->vstring.push_back(r);
        break;
      default:
        throw "Unexpected input type";
    }
    ++ch;
  }
}

void load_data(Sheet& sheet, const string& path, bool header) {
  ifstream in(path);
  
  if (in.is_open())
  {
    string line;
    istringstream iss;
    vector<string> column_name;
    if (header) { // first line is column name
      if (!getline(in, line)) throw "No first line";
      iss.str(line);
      while (iss){
        string s;
        if (!getline( iss, s, ',' )) break;
        column_name.push_back( trim(s) );
      }
    }
    
    if ( !getline(in, line) ) throw "No data";
    iss.clear();
    iss.str(line);
    int t = 0;
    vector<string> data;
    while (iss){
      string s;
      if ( !getline( iss, s, ',' )) break;
      if(!header) column_name.push_back("c" + to_string(t++));
      data.push_back( trim(s) );
    }
    
    Sheet sheet(data, column_name);
    
    while ( getline (in,line) ) {
      iss.clear();
      iss.str(line);
      data.clear();
      while (iss){
        string s;
        if ( !getline( iss, s, ',' )) break;
        data.push_back( trim(s) );
      }
      sheet.row_append(data);
    }
    in.close();
  }
  else cout << "no file open" << endl;
}

int main (){
  Sheet sh;
  string s = "/Users/mcchu/Documents/16S-C++/sheet/sheet/test.txt";
  load_data(sh, s, true);
}