#include "pch_server.h"
#include "bol_database.h"


using namespace  sqlite;
using namespace std;

int main()
{
	try {
		// creates a database file 'dbfile.db' if it does not exists.
		database db("dbfile.db");

		// executes the query and creates a 'user' table
		db <<
			"create table if not exists user ("
			"   _id integer primary key autoincrement not null,"
			"   age int,"
			"   name text,"
			"   weight real"
			");";

		// inserts a new user record.
		// binds the fields to '?' .
		// note that only types allowed for bindings are :
		//      int ,long, long long, float, double
		//      string, u16string
		// sqlite3 only supports utf8 and utf16 strings, you should use std::string for utf8 and std::u16string for utf16.
		// If you're using C++17, `std::string_view` and `std::u16string_view` can be used as string types.
		// note that u"my text" is a utf16 string literal of type char16_t * .
		db << "insert into user (age,name,weight) values (?,?,?);"
			<< 20
			<< u"bob"
			<< 83.25;

		int age = 21;
		float weight = 68.5;
		string name = "jack";
		db << u"insert into user (age,name,weight) values (?,?,?);" // utf16 query string
			<< age
			<< name
			<< weight;

		cout << "The new record got assigned id " << db.last_insert_rowid() << endl;

		// selects from user table on a condition ( age > 18 ) and executes
		// the lambda for each row returned .
		db << "select age,name,weight from user where age > ? ;"
			<< 18
			>> [&](int age, string name, double weight) {
			cout << age << ' ' << name << ' ' << weight << endl;
		};

		// a for loop can be used too:
		// with named variables
		for (auto &&row : db << "select age,name,weight from user where age > ? ;" << 18) {
			int age; string name; double weight;
			row >> age >> name >> weight;
			cout << age << ' ' << name << ' ' << weight << endl;
		}
		// or with a tuple
		for (tuple<int, string, double> row : db << "select age,name,weight from user where age > ? ;" << 18) {
			cout << get<0>(row) << ' ' << get<1>(row) << ' ' << get<2>(row) << endl;
		}

		// selects the count(*) from user table
		// note that you can extract a single column single row result only to : int,long,long long,float,double,string,u16string
		int count = 0;
		db << "select count(*) from user" >> count;
		cout << "cout : " << count << endl;

		// you can also extract multiple column rows
		db << "select age, name from user where _id=1;" >> tie(age, name);
		cout << "Age = " << age << ", name = " << name << endl;

		// this also works and the returned value will be automatically converted to string
		string str_count;
		db << "select count(*) from user" >> str_count;
		cout << "scount : " << str_count << endl;
	}
	catch (const exception& e) {
		cout << e.what() << endl;
	}
}
