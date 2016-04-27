// libsheet.cpp
#include "libsheet.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctype.h>
#include <exception>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <typeinfo>
using namespace std;

/*** Utility function ***/
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
/*** End of utility function ***/

/*** Construction of Sheet ***/ 
void load_data(Sheet& sheet, const string& path, bool header, int NAN_int, double NAN_double, string NAN_string) {
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


		
		Sheet new_sheet(data, column_name);
        // set default values
      	new_sheet.set_NAN_int(NAN_int);
      	new_sheet.set_NAN_double(NAN_double);
      	new_sheet.set_NAN_string(NAN_string);
		
		while ( getline (in,line) ) {
			iss.clear();
			iss.str(line);
			data.clear();
			while (iss){
				string s;
				if ( !getline( iss, s, ',' )) break;
				data.push_back( trim(s) );
			}
			new_sheet.row_append(data);
		}
		in.close();
		sheet = move(new_sheet);
	}
	else cout << "no file open" << endl;
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

/* ---Append functions--- */
/* row append - 1/2 (sheet) */
// to-do

/* row append - 2/2 (new_rows) */
void Sheet::row_append(vector<string> &new_row) {
	assert(new_row.size() == columns.size());
	auto ch = columns.begin();
	for (auto &r : new_row) {
		switch (ch->flag) {
			case 0:
                if (r.size()) ch->vint.push_back(stoi(r));
          		else ch->vint.push_back(NAN_int);
				break;
			case 1:
          		if (r.size()) ch->vdouble.push_back(stod(r));
				else ch->vdouble.push_back(NAN_double);
				break;
			case 2:
				if (r.size()) ch->vstring.push_back(r);
          		else ch->vstring.push_back(NAN_string);
				break;
			default:
				throw "Unexpected input type";
		}
		++ch;
	}
}

/* column append - 1/3 (new int cols) */
void Sheet::col_append(vector<int> &new_col, const string & col_name) {
	if (columns.size()) {
		int row_len = max(max(columns[0].vint.size(), columns[0].vdouble.size()), columns[0].vstring.size());
		assert(row_len == new_col.size());
	}
	auto result = column_map.insert(pair<string, unsigned int>(col_name, columns.size()));
	if (!result.second) {
		throw "Duplicate column name at" + to_string(result.first->second);
	}
	ColumnHead ch(col_name, 0);
	ch.vint = new_col;
	columns.push_back(ch);
}

/* column append - 2/3 (new double cols) */
void Sheet::col_append(vector<double> &new_col, const string & col_name) {
	if (columns.size()) {
		int row_len = max(max(columns[0].vint.size(), columns[0].vdouble.size()), columns[0].vstring.size());
		assert(row_len == new_col.size());
	}
	auto result = column_map.insert(pair<string, unsigned int>(col_name, columns.size()));
	if (!result.second) {
		throw "Duplicate column name at" + to_string(result.first->second);
	}
	ColumnHead ch(col_name, 1);
	ch.vdouble = new_col;
	columns.push_back(ch);
}

/* column append - 3/3 (new string cols) */
void Sheet::col_append(vector<string> &new_col, const string & col_name) {
	if (columns.size()) {
		int row_len = max(max(columns[0].vint.size(), columns[0].vdouble.size()), columns[0].vstring.size());
		assert(row_len == new_col.size());
	}
	auto result = column_map.insert(pair<string, unsigned int>(col_name, columns.size()));
	if (!result.second) {
		throw "Duplicate column name at" + to_string(result.first->second);
	}
	ColumnHead ch(col_name, 2);
	ch.vstring = new_col;
	columns.push_back(ch);
}

/* column erase - 1/4 (1 col_id) */
void Sheet::col_erase(int col) {
	string col_name = columns.at(col).column_name;
	column_map.erase(col_name);
	columns.erase(columns.begin() + col);
	for (auto i = col; i < columns.size(); ++i) {
		col_name = columns.at(i).column_name;
		--column_map[col_name];
	}
}

/* column erase - 2/4 (1 col_name) */
void Sheet::col_erase(const string& col) {
	int col_id;
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	col_id = got->second;
	
	column_map.erase(col);
	columns.erase(columns.begin() + col_id);
	for (auto i = col_id; i < columns.size() ; ++i ) {
		string name = columns.at(i).column_name;
		--column_map[name];
	}
}

/* column erase - 3/4 (col names) */
void Sheet::col_erase(const vector<string>& col) {
	for (auto &s: col) {
		col_erase(s);
	}
}

/* column erase - 4/4 (col ids) */
void Sheet::col_erase(const vector<int>& col) {
	vector<string> cols;
	for (auto &i : col) {
		cols.push_back(columns.at(i).column_name);
	}
	for (auto &s : cols) {
		col_erase(s);
	}
}

/* row erase - 1/2 */
void Sheet::row_erase(int row) {
	for (auto &c : columns) {
		switch (c.flag) {
			case 0:
				c.vint.erase(c.vint.begin() + row);
				break;
			case 1:
				c.vdouble.erase(c.vdouble.begin() + row);
				break;
			case 2:
				c.vstring.erase(c.vstring.begin() + row);
				break;
			default:
				throw "Unexpected type";
		}
	}
}

/* row erase - 2/2 (rows) */
void Sheet::row_erase(vector<int>& rows) {
	sort(rows.begin(), rows.end());
	int j = 0;
	for (auto &r : rows) {
		row_erase(r - j++);
	}
}


/* ---Get functions--- */
Sheet Sheet::get(const int& row, const string& col) {
	int col_id;
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	col_id = got->second;
	
	string col_name{columns.at(col_id).column_name};
	int type_flag{columns.at(col_id).flag};
	
	ColumnHead ch = ColumnHead(col_name, type_flag);
	
	// Construct the content in ColumnHead based on the requested element type
	switch (type_flag) {
		case 0:
			ch.vint.push_back(columns.at(col_id).vint.at(row));
			break;
		case 1:
			ch.vdouble.push_back(columns.at(col_id).vdouble.at(row));
			break;
		case 2:
			ch.vstring.push_back(columns.at(col_id).vstring.at(row));
			break;
		default:
			throw "Unexpected type";
	}
	
	Sheet new_sheet;
	new_sheet.columns.push_back(ch);
	new_sheet.column_map.insert(pair<string, unsigned int>(col_name, 0));
	return new_sheet;
}

Sheet Sheet::get(const int& row, const int& col) {
	int col_id = col;
	
	string col_name{columns.at(col_id).column_name};
	int type_flag{columns.at(col_id).flag};
	
	ColumnHead ch = ColumnHead(col_name, type_flag);
	
	// Construct the content in ColumnHead based on the requested element type
	switch (type_flag) {
		case 0:
			ch.vint.push_back(columns.at(col_id).vint.at(row));
			break;
		case 1:
			ch.vdouble.push_back(columns.at(col_id).vdouble.at(row));
			break;
		case 2:
			ch.vstring.push_back(columns.at(col_id).vstring.at(row));
			break;
		default:
			throw "Unexpected type";
	}
	
	Sheet new_sheet;
	new_sheet.columns.push_back(ch);
	new_sheet.column_map.insert(pair<string, unsigned int>(col_name, 0));
	return new_sheet;
}

Sheet Sheet::get(const int& row, const vector<int>& cols) {
	
	Sheet new_sheet;
	for (auto i = cols.begin(); i != cols.end() ; ++i) {
		
		string col_name{columns.at(*i).column_name};
		int type_flag{columns.at(*i).flag};
		
		ColumnHead ch = ColumnHead(col_name, type_flag);
		
		// Construct the content in ColumnHead based on the requested element type
		switch (type_flag) {
			case 0:
				ch.vint.push_back(columns.at(*i).vint.at(row));
				break;
			case 1:
				ch.vdouble.push_back(columns.at(*i).vdouble.at(row));
				break;
			case 2:
				ch.vstring.push_back(columns.at(*i).vstring.at(row));
				break;
			default:
				throw "Unexpected type";
		}
		
		
		new_sheet.columns.push_back(ch);
		new_sheet.column_map.insert(pair<string, unsigned int>(col_name, i - cols.begin()));
	}
	return new_sheet;
}

Sheet Sheet::get(const int& row, const vector<string>& cols) {
	Sheet new_sheet;
	int col_id;
	for ( auto i = cols.begin(); i != cols.end(); ++i) {
		auto got = column_map.find(*i);
		if (got == column_map.end())
			throw "No such column";
		col_id = got->second;
		string col_name{columns.at(col_id).column_name};
		int type_flag{columns.at(col_id).flag};
		ColumnHead ch = ColumnHead(col_name, type_flag);
		// Construct the content in ColumnHead based on the requested element type
		switch (type_flag) {
			case 0:
				ch.vint.push_back(columns.at(col_id).vint.at(row));
				break;
			case 1:
				ch.vdouble.push_back(columns.at(col_id).vdouble.at(row));
				break;
			case 2:
				ch.vstring.push_back(columns.at(col_id).vstring.at(row));
				break;
			default:
				throw "Unexpected type";
		}
		new_sheet.columns.push_back(ch);
		new_sheet.column_map.insert(pair<string, unsigned int>(col_name, i - cols.begin()));
	}
	return new_sheet;
}

Sheet Sheet::get(const vector<int>& rows, const int& col) {
	
	string col_name{columns.at(col).column_name};
	int type_flag{columns.at(col).flag};
	
	ColumnHead ch = ColumnHead(col_name, type_flag);
	
	Sheet new_sheet;
	
	// Construct the content in ColumnHead based on the requested element type
	switch (type_flag) {
		case 0:
			for (auto &i : rows) ch.vint.push_back(columns.at(col).vint.at(i));
			break;
		case 1:
			for (auto &i : rows) ch.vdouble.push_back(columns.at(col).vdouble.at(i));
			break;
		case 2:
			for (auto &i : rows) ch.vstring.push_back(columns.at(col).vstring.at(i));
			break;
		default:
			throw "Unexpected type";
	}
	
	
	new_sheet.columns.push_back(ch);
	new_sheet.column_map.insert(pair<string, unsigned int>(col_name, 0));
	return new_sheet;
}

Sheet Sheet::get(const vector<int>& rows, const string& col) {
	
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	int col_id = got->second;
	
	string col_name{columns.at(col_id).column_name};
	int type_flag{columns.at(col_id).flag};
	
	ColumnHead ch = ColumnHead(col_name, type_flag);
	
	Sheet new_sheet;
	
	// Construct the content in ColumnHead based on the requested element type
	switch (type_flag) {
		case 0:
			for (auto i : rows) ch.vint.push_back(columns.at(col_id).vint.at(i));
			break;
		case 1:
			for (auto i : rows) ch.vdouble.push_back(columns.at(col_id).vdouble.at(i));
			break;
		case 2:
			for (auto i : rows) ch.vstring.push_back(columns.at(col_id).vstring.at(i));
			break;
		default:
			throw "Unexpected type";
	}
	new_sheet.columns.push_back(ch);
	new_sheet.column_map.insert(pair<string, unsigned int>(col_name, 0));
	return new_sheet;
}

Sheet Sheet::get(const vector<int>& rows, const vector<string>& cols) {
	Sheet new_sheet;
	int col_id;
	for ( auto i = cols.begin(); i != cols.end(); ++i) {
		auto got = column_map.find(*i);
		if (got == column_map.end())
			throw "No such column";
		col_id = got->second;
		
		string col_name{columns.at(col_id).column_name};
		int type_flag{columns.at(col_id).flag};
		ColumnHead ch = ColumnHead(col_name, type_flag);
		// Construct the content in ColumnHead based on the requested element type
		switch (type_flag) {
			case 0:
				for (auto& r : rows) ch.vint.push_back(columns.at(col_id).vint.at(r));
				break;
			case 1:
				for (auto& r : rows) ch.vdouble.push_back(columns.at(col_id).vdouble.at(r));
				break;
			case 2:
				for (auto &r: rows) ch.vstring.push_back(columns.at(col_id).vstring.at(r));
				break;
			default:
				throw "Unexpected type";
		}
		new_sheet.columns.push_back(ch);
		new_sheet.column_map.insert(pair<string, unsigned int>(col_name, i - cols.begin()));
	}
	return new_sheet;
}

Sheet Sheet::get(const vector<int>& rows, const vector<int>& cols) {
	Sheet new_sheet;
	for ( auto i = cols.begin(); i != cols.end(); ++i) {
		string col_name{columns.at(*i).column_name};
		int type_flag{columns.at(*i).flag};
		ColumnHead ch = ColumnHead(col_name, type_flag);
		// Construct the content in ColumnHead based on the requested element type
		switch (type_flag) {
			case 0:
				for (auto& r : rows) ch.vint.push_back(columns.at(*i).vint.at(r));
				break;
			case 1:
				for (auto& r : rows) ch.vdouble.push_back(columns.at(*i).vdouble.at(r));
				break;
			case 2:
				for (auto &r: rows) ch.vstring.push_back(columns.at(*i).vstring.at(r));
				break;
			default:
				throw "Unexpected type";
		}
		new_sheet.columns.push_back(ch);
		new_sheet.column_map.insert(pair<string, unsigned int>(col_name, i - cols.begin()));
	}
	return new_sheet;
}

/* get_row, get_col */
Sheet Sheet::get_row(const vector<int>& rows) {
	Sheet new_sheet;
	for (auto c = columns.begin(); c != columns.end(); ++c) {
		ColumnHead ch = ColumnHead(c->column_name, c->flag);
		switch (c->flag) {
			case 0:
				for (auto& r : rows) ch.vint.push_back(c->vint.at(r));
				break;
			case 1:
				for (auto& r : rows) ch.vdouble.push_back(c->vdouble.at(r));
				break;
			case 2:
				for (auto& r : rows) ch.vstring.push_back(c->vstring.at(r));
				break;
			default:
				throw "Unexpected type";
		}
		new_sheet.columns.push_back(ch);
		new_sheet.column_map.insert(pair<string, unsigned int>(c->column_name, c - columns.begin()));
	}
	return new_sheet;
}

Sheet Sheet::get_column(const vector<int>& cols){
	Sheet new_sheet;
	int row_len = max(max(columns[0].vint.size(), columns[0].vdouble.size()), columns[0].vstring.size());
	for (auto c = 0; c < cols.size(); ++c) {
		string col_name{columns.at(cols.at(c)).column_name};
		int type_flag{columns.at(cols.at(c)).flag};
		ColumnHead ch = ColumnHead(col_name, type_flag);
		// Construct the content in ColumnHead based on the requested element type
		switch (type_flag) {
			case 0:
				for (auto r = 0; r < row_len; ++r) ch.vint.push_back(columns.at(cols.at(c)).vint.at(r));
				break;
			case 1:
				for (auto r = 0; r < row_len; ++r) ch.vdouble.push_back(columns.at(cols.at(c)).vdouble.at(r));
				break;
			case 2:
				for (auto r = 0; r < row_len; ++r) ch.vstring.push_back(columns.at(cols.at(c)).vstring.at(r));
				break;
			default:
				throw "Unexpected type";
		}
		new_sheet.columns.push_back(ch);
		new_sheet.column_map.insert(pair<string, unsigned int>(col_name, c));
	}
	return new_sheet;
}

Sheet Sheet::get_column(const vector<string>& cols){
	Sheet new_sheet;
	int col_id;
	
	int row_len = max(max(columns[0].vint.size(), columns[0].vdouble.size()), columns[0].vstring.size());
	for (auto i = cols.begin(); i != cols.end(); ++i) {
		auto got = column_map.find(*i);
		if (got == column_map.end())
			throw "No such column";
		col_id = got->second;
		string col_name{columns.at(col_id).column_name};
		int type_flag{columns.at(col_id).flag};
		ColumnHead ch = ColumnHead(col_name, type_flag);
		// Construct the content in ColumnHead based on the requested element type
		switch (type_flag) {
			case 0:
				for (auto r = 0; r < row_len; ++r) ch.vint.push_back(columns.at(col_id).vint.at(r));
				break;
			case 1:
				for (auto r = 0; r < row_len; ++r) ch.vdouble.push_back(columns.at(col_id).vdouble.at(r));
				break;
			case 2:
				for (auto r = 0; r < row_len; ++r) ch.vstring.push_back(columns.at(col_id).vstring.at(r));
				break;
			default:
				throw "Unexpected type";
		}
		new_sheet.columns.push_back(ch);
		new_sheet.column_map.insert(pair<string, unsigned int>(col_name, i - cols.begin()));
	}
	return new_sheet;
}

/*** print sheet ****/
void Sheet::print(bool header, bool show_NAN) {
	if (!columns.size()) return;
	int row_len = max(max(columns[0].vint.size(), columns[0].vdouble.size()), columns[0].vstring.size());
	
	if (header) {
		for (auto c = 0; c < columns.size() - 1;  ++c) {
			cout << columns[c].column_name << ", ";
		}
		cout << columns[columns.size() - 1].column_name << endl;
	}
	
	for (auto r = 0; r < row_len ; ++r) {
		for (auto c = 0; c < columns.size();  ++c) {
			switch (columns[c].flag) {
				case 0:
              	{
              		int& i = columns[c].vint[r]; 
              		if (isNAN(i) && show_NAN) cout << "NAN";
					else cout << i;
					break;
                }
                case 1:
              	{
              		double& d = columns[c].vdouble[r];
              		if (isNAN(d)  && show_NAN ) cout << "NAN";
					else cout << d;
					break;
                }
                case 2:
              	{
              		string& s = columns[c].vstring[r];
              		if (isNAN(s)  && show_NAN ) cout << "NAN";
					else cout << s;
					break;
                }
                default:
					throw "Unexpected type";
			}
			if (c < columns.size() - 1) cout << ", ";
			else cout << endl;
		}
	}
}

/*** set functions ****/
void Sheet::set(const int& row, const int& col, const int& value) {
	int col_id = col;
	int type_flag{columns.at(col_id).flag};
	
	switch (type_flag) {
		case 0:
			columns.at(col_id).vint.at(row) = value;
			break;
		default:
			throw "Type does not match! Expect int";
	}
}

void Sheet::set(const int& row, const int& col, const double& value) {
	int col_id = col;
	int type_flag{columns.at(col_id).flag};
	
	switch (type_flag) {
		case 1:
			columns.at(col_id).vdouble.at(row) = value;
			break;
		default:
			throw "Type does not match! Expect double";
	}
}

void Sheet::set(const int& row, const int& col, const string& value) {
	int col_id = col;
	int type_flag{columns.at(col_id).flag};
	
	switch (type_flag) {
		case 2:
			columns.at(col_id).vstring.at(row) = value;
			break;
		default:
			throw "Type does not match! Expect string";
	}
}

void Sheet::set(const int& row, const string& col, const int& value) {
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	int col_id = got->second;
	int type_flag{columns[col_id].flag};
	
	switch (type_flag) {
		case 0:
			columns[col_id].vint.at(row) = value;
			break;
		default:
			throw "Type does not match! Expect int";
	}
}

void Sheet::set(const int& row, const string& col, const double& value) {
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	int col_id = got->second;
	int type_flag{columns[col_id].flag};
	
	switch (type_flag) {
		case 1:
			columns[col_id].vdouble.at(row) = value;
			break;
		default:
			throw "Type does not match! Expect double";
	}
}

void Sheet::set(const int& row, const string& col, const string& value) {
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	int col_id = got->second;
	int type_flag{columns[col_id].flag};
	
	switch (type_flag) {
		case 2:
			columns[col_id].vstring.at(row) = value;
			break;
		default:
			throw "Type does not match! Expect string";
	}
}

/* ---Algorithm--- */

/* sort 1/2 - by col_id */
void Sheet::sort_by_column(int col, bool descend) {
	vector<int> indices;
	switch (columns.at(col).flag) {
		case 0:
		{
			vector<int>& vi = columns.at(col).vint;
			vector<pair<int,int> > vp;
			for (size_t i = 0 ; i != vi.size() ; ++i) {
				vp.push_back(make_pair(vi[i], i));
			}
			if (descend) {
				sort(vp.begin(), vp.end(), greater<pair<int, int>>());
			} else {
				sort(vp.begin(), vp.end());
			}
			for (auto &p : vp) {
				indices.push_back(p.second);
			}
			break;
		}
		case 1:
		{
			vector<double>& vd = columns.at(col).vdouble;
			vector<pair<double,int> > vp;
			for (size_t i = 0 ; i != vd.size() ; ++i) {
				vp.push_back(make_pair(vd[i], i));
			}
			if (descend) {
				sort(vp.begin(), vp.end(), greater<pair<double, int>>());
			} else {
				sort(vp.begin(), vp.end());
			}
			for (auto &p : vp) {
				indices.push_back(p.second);
			}
			break;
		}
		case 2:
		{
			vector<string>& vs = columns.at(col).vstring;
			vector<pair<string,int> > vp;
			for (size_t i = 0 ; i != vs.size() ; ++i) {
				vp.push_back(make_pair(vs[i], i));
			}
			if (descend) {
				sort(vp.begin(), vp.end(), greater<pair<string, int>>());
			} else {
				sort(vp.begin(), vp.end());
			}
			for (auto &p : vp) {
				indices.push_back(p.second);
			}
			break;
		}
		default:
			throw "Unexpected type";
	}
	for (auto &c : columns) {
		switch (c.flag) {
			case 0:
				reorder(indices, c.vint);
				break;
			case 1:
				reorder(indices, c.vdouble);
				break;
			case 2:
				reorder(indices, c.vstring);
				break;
			default:
				throw "Unexpected type";
		}
	}
}

/* sort 2/2 - by col_name */
void Sheet::sort_by_column(const string& col, bool descend) {
	vector<int> indices;
	int col_id;
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	col_id = got->second;
	
	switch (columns.at(col_id).flag) {
		case 0:
		{
			vector<int>& vi = columns.at(col_id).vint;
			vector<pair<int,int> > vp;
			for (size_t i = 0 ; i != vi.size() ; ++i) {
				vp.push_back(make_pair(vi[i], i));
			}
			if (descend) {
				sort(vp.begin(), vp.end(), greater<pair<int, int>>());
			} else {
				sort(vp.begin(), vp.end());
			}
			for (auto &p : vp) {
				indices.push_back(p.second);
			}
			break;
		}
		case 1:
		{
			vector<double>& vd = columns.at(col_id).vdouble;
			vector<pair<double,int> > vp;
			for (size_t i = 0 ; i != vd.size() ; ++i) {
				vp.push_back(make_pair(vd[i], i));
			}
			if (descend) {
				sort(vp.begin(), vp.end(), greater<pair<double, int>>());
			} else {
				sort(vp.begin(), vp.end());
			}
			for (auto &p : vp) {
				indices.push_back(p.second);
			}
			break;
		}
		case 2:
		{
			vector<string>& vs = columns.at(col_id).vstring;
			vector<pair<string,int> > vp;
			for (size_t i = 0 ; i != vs.size() ; ++i) {
				vp.push_back(make_pair(vs[i], i));
			}
			if (descend) {
				sort(vp.begin(), vp.end(), greater<pair<string, int>>());
			} else {
				sort(vp.begin(), vp.end());
			}
			for (auto &p : vp) {
				indices.push_back(p.second);
			}
			break;
		}
		default:
			throw "Unexpected type";
	}
	for (auto &c : columns) {
		switch (c.flag) {
			case 0:
				reorder(indices, c.vint);
				break;
			case 1:
				reorder(indices, c.vdouble);
				break;
			case 2:
				reorder(indices, c.vstring);
				break;
			default:
				throw "Unexpected type";
		}
	}
}

/**** Filter by mask ****/
Sheet Sheet::filter(const vector<bool>& vb) {
	vector<int> indices;
	for (auto r = 0; r < vb.size(); ++r) {
		if (vb[r]) indices.push_back(r);
	}
	return get_row(indices);
}

/* Operator overloading for vector<bool> */
vector<bool> operator&&(const vector<bool>& mask1, const vector<bool>& mask2){
	assert(mask1.size() == mask2.size());
	vector<bool> result;
	for (int i = 0; i < mask1.size() ; ++i) {
		result.push_back(mask1[i] && mask2[i]);
	}
	return result;
}

vector<bool> operator||(const vector<bool>& mask1, const vector<bool>& mask2){
	assert(mask1.size() == mask2.size());
	vector<bool> result;
	for (int i = 0; i < mask1.size() ; ++i) {
		result.push_back(mask1[i] || mask2[i]);
	}
	return result;
}

vector<bool> operator!(const vector<bool>& mask1){
	
	vector<bool> result;
	for (int i = 0; i < mask1.size() ; ++i) {
		result.push_back(!mask1[i]);
	}
	return result;
}



