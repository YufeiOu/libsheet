// libsheet.h
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <type_info>
#include <bitset>
using namespace std;

class Sheet {
  public:
    Sheet() {};
    Sheet(vector<string>& entry);
    Sheet(Sheet) {
    
    };
  
    template<typename Oper, size_t N, typename T>
    bitset<N> filter(T column_index, Oper p) {
      // Column index can be int or string(for column name)
      // give column number, filter lambda expression, return a bitmap
    }
    
    template<typename T, typename col_type>
    void set(int y, col_type x, T value);
    
    
    template<typename T>
    Sheet get(int row, vector<T> cols);
  
    template<typename T>
    Sheet get(int row, T col);
  
    template<typename T>
    Sheet get(vecotr<int> rows, T cols);
  
    template<typename T>
    Sheet get(vecotr<int> rows,  vector<T> cols);
    
    
    void row_erase(int row);
    void row_erase(vector<int> rows);
    template<typename T>
    void column_erase(T col);
    
    template<typename T>
    void column_erase(vector<T> cols);
  
    void row_append(Sheet& new_sheet)
    void column_append(Sheet& new_col);
    
    template<typename T>
    void column_append(vector<T> new_col, string col_name);
  
  private:
    map<string, unsigned_int>  column_map;  // column name to index
    
    class column_head {
      std::string column_name;
      int flag; // int:0 double:1 string:2
      
      vector<int> vint;
      vector<double> vdouble;
      vector<std::string> vstring;
      /* other possible field */
      template<typename T>
      void push_back(vector<T> input);
      
    };
    vector<column_head> columns;  
  
};

