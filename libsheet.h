// libsheet.h
#include <iostream>
#include <map>
#incldue <string>
#include <vector>
#include <type_info>
#include <bitset>
using namespace std;

class Sheet {
  public:
  	Sheet(){};
  	Sheet(Sheet) {
  	
    };
  
  	template<typename Oper, size_t N>
    bitset<N> filter(int column_num, Oper p) {
      // give column number, filter lambda expression, return a bitmap
    }
  
  private:
  	map<string, unsigned_int>  column_map;  // column name to index
		
  	struct column_head {
    	void *data;
      type_info type;
      /* other possible field */
      string name;
		};
  	vector<column_head> columns;
  
};