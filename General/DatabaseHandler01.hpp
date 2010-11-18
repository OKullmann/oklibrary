// Oliver Kullmann, 12.4.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

// use with (g++)
// -I/usr/include/pgsql
// -lpq

#include <vector>
#include <set>
#include <map>
#include <stack>
#include <string>
#include <sstream>
#include <iterator>
#include <utility>
#include <ostream>

#include <pgsql/libpq-fe.h>
/*
// including the following causes throw-errors with gcc version 3.0.4
#include <postgres.h> // necessary for <catalog/pg_type.h>
#include <catalog/pg_type.h>
// in this order!
*/

#include <OKlib/General/StringHandling.hpp>
#include <OKlib/General/FunctionHandling.hpp>
#include <OKlib/General/ErrorHandling.hpp>

#ifndef DATABASEHANDLER01WAECHTER

namespace DatabaseHandler01 {

  using std::vector;
  using std::map;
  using std::set;
  using std::stack;
  using std::ostringstream;
  using std::stringstream;
  using std::ostream;
  using std::pair;

  // -------------------------------------------------------------------------
  // Error handling
  // -------------------------------------------------------------------------


  namespace Error {

    class General : public ErrorHandling::Error {
    public :
      General(const std::string& s) : ErrorHandling::Error(s) {}
      virtual std::string what() const {
#ifdef DEBUG
	std::cout << "General.what: w = " << what_ << std::endl;
#endif
	return what_;
      }
   };

    class Postgres : public General {
    public :
      Postgres(const std::string& message, const std::string& dbname) : General(message), name(dbname) {}
      virtual std::string getname() const { return name; }
      virtual std::string what() const {
#ifdef DEBUG
	std::cout << "Postgres.what: name = " << name << std::endl;
#endif
	return "Postgres[" + name + "] " + General::what();
      }
    private :
      std::string name;
    };
    struct bad_connection : Postgres {
      bad_connection(const std::string& message, const std::string& dbname) : Postgres(message, dbname) {}
    };
    class command_failed : public Postgres {
    public :
      command_failed(const std::string& message, const std::string& dbname, const std::string& result_status) : Postgres(message, dbname), status(result_status) {
#ifdef DEBUG
	std::cout << "constructor command_failed: message = " << message << ", dbname = " << dbname << ", result_status = " << result_status << std::endl;
#endif
      }
      virtual std::string getstatus() const { return status; }
      virtual std::string what() const {
#ifdef DEBUG
	std::cout << "command_failed.what: status = " << status << std::endl;
#endif
	return Postgres::what() + " (status: " + status + ")";
      }
    private :
      std::string status;
    };
    struct no_insert : Postgres {
      no_insert(const std::string& message, const std::string& dbname) : Postgres(message, dbname) {}
    };
    struct no_tuples : Postgres {
      no_tuples(const std::string& message, const std::string& dbname) : Postgres(message, dbname) {}
    };
    struct invalid_column_index : Postgres {
      invalid_column_index(const std::string& message, const std::string& dbname) : Postgres(message, dbname) {}
    };
    struct invalid_index : Postgres {
      invalid_index(const std::string& message, const std::string& dbname) : Postgres(message, dbname) {}
    };
    struct attribute_not_found : Postgres {
      attribute_not_found(const std::string& message, const std::string& dbname) : Postgres(message, dbname) {}
    };

    struct ObjectInterface : General {
      ObjectInterface(const std::string& message, const std::string& dbname) : General(message), name(dbname) {}
      virtual std::string getname() const { return name; }
      virtual std::string what() const { return "ObjectInterface[" + name + "] " + General::what(); }
    private :
      std::string name;
    };
    struct filling : ObjectInterface {
      filling(const std::string& s, const std::string& dbname) : ObjectInterface(s, dbname) {}
    };
    struct no_table : ObjectInterface {
      no_table(const std::string& s, const std::string& dbname) : ObjectInterface(s, dbname) {}
    };
    struct no_attribute : ObjectInterface {
      no_attribute(const std::string& s, const std::string& dbname) : ObjectInterface(s, dbname) {}
    };
    struct multiple_attributes : ObjectInterface {
      multiple_attributes(const std::string& s, const std::string& dbname) : ObjectInterface(s, dbname) {}
    };
    struct inconsistency : ObjectInterface {
      inconsistency(const std::string& s, const std::string& dbname) : ObjectInterface(s, dbname) {}
    };
  }

  // --------------------------------------------------------------------------
  // Auxiliary functions
  // --------------------------------------------------------------------------

  void introduce_escape_seq(std::string& s) {
    StringHandling::replace_all_substrings(s, "\\", "\\\\");
    StringHandling::replace_all_substrings(s, "'", "''");
  }
  const std::string introduce_escape_seq(std::string s) {
    StringHandling::replace_all_substrings(s, "\\", "\\\\");
    StringHandling::replace_all_substrings(s, "'", "''");
    return s;
  }
  

  // --------------------------------------------------------------------------
  // The PostgreSQL-Connection
  // --------------------------------------------------------------------------


  namespace AuxiliaryFunctions {
    inline const char* converttocstr(const std::string& s) {
      if (s.empty()) return 0;
      else return s.c_str();
    }
  }

  class Connection {

  public :

    explicit Connection(const std::string& database, const std::string& passwd = "", const std::string& user = "") 
      : conn(PQsetdbLogin(0, 0, 0, 0, database.c_str(), AuxiliaryFunctions::converttocstr(user), AuxiliaryFunctions::converttocstr(passwd))), name(database) {
      if (PQstatus(conn) != CONNECTION_OK)
	throw Error::bad_connection(PQerrorMessage(conn), name);
    }

    operator PGconn *() const { return conn; }

    std::string getName() const { return name; }

    set<std::string> getTableNames() const;

    ~Connection() {
#ifdef DEBUG
      std::cout << "Destruktor Connection Eingang: conn = " << conn << std::endl;
#endif
      PQfinish(conn);
#ifdef DEBUG
      std::cout << "Destruktor Connection Ausgang." << std::endl;
#endif
    }

  private :

    Connection(const Connection&); // not allowed
    Connection& operator = (const Connection&); // not allowed

    PGconn *conn;

    std::string name;

  };


  // -------------------------------------------------------------------------
  // Command handler
  // -------------------------------------------------------------------------


  class Command {

  public :

    explicit Command(const Connection& connection)
      : conn(connection), res(0) {}

    void issue(const std::string& command) {
      PQclear(res);
#ifdef DEBUG
      std::cout << "Command.issue mit " << command << std::endl;
#endif
      res = PQexec(conn, command.c_str());
#ifdef DEBUG
      std::cout << "PQexec erfolgreich, *res = " << res << std::endl;
#endif
      ExecStatusType status(PQresultStatus(res));
#ifdef DEBUG
      std::cout << "PQresultStatus erfolgreich, status = " << status << std::endl;
#endif
      if (status != PGRES_COMMAND_OK) {
#ifdef DEBUG
	std::cout << "status != PGRES_COMMAND_OK" << std::endl;
	std::cout << "PQresultErrorMessage(res): " << PQresultErrorMessage(res) << std::endl;
	std::cout << "conn.getName(): " << conn.getName() << std::endl;
	std::cout << "PQresStatus(status): " << PQresStatus(status) << std::endl;
#endif
	throw Error::command_failed(PQresultErrorMessage(res), conn.getName(), PQresStatus(status));
      }
#ifdef DEBUG
      std::cout << "PGRES_COMMAND_OK" << std::endl;
#endif
    }

    std::string answer() const { return PQcmdStatus(res); }

    int changed_tuples() const {
      std::string t = PQcmdTuples(res);
      if (t.empty())
	throw Error::no_tuples("", conn.getName());
      std::stringstream s;
      s << t;
      int r;
      s >> r;
      return r;
    }

    Oid oid_value() const {
      Oid o = PQoidValue(res);
      if (o == InvalidOid)
	throw Error::no_insert("", conn.getName());
      return o;
    }

    ~Command() {
#ifdef DEBUG
      std::cout << "Destruktor Command Eingang: conn = " << conn << std::endl;
#endif
      PQclear(res);
#ifdef DEBUG
      std::cout << "Destruktor Command Ausgang." << std::endl;
#endif
    }

  private :

    Command (const Command&); // not allowed (don't implement)
    Command& operator = (const Command&); // not allowed

    const Connection& conn;
    PGresult* res;

  };


  // Iterators over Tables ---------------------------------------------


  struct Attribute {

    Oid type;
    int column;
    std::string name;

    Attribute(Oid t, int c, const std::string& n)
      : type(t), column(c), name(n) {}
  };

  struct Value {

    std::string v;
    bool null;

    Value()
      : null(true) {}
    Value(std::string s)
      : v(s), null(false) {}

    operator std::string() const {
      if (null)
	return "NULL";
      else
	return v;
    }
  };

  inline ostream& operator << (ostream& o, Value v) {
    return (o << std::string(v));
  }
  // warum noetig ??

  /*

  bool is_integer_type(Oid t) {
    return t == INT8OID || t == INT2OID || t == INT4OID;
  }
  bool is_float_type(Oid t) {
    return t == FLOAT4OID || t == FLOAT8OID;
  }
  bool is_numeric_type(Oid t) {
    return t == NUMERICOID;
  }

  */
  // not really necessary at this time (14.4.2002), and causes trouble due
  // to possibly wrong header files


  class AttributeIterator : std::iterator<std::input_iterator_tag, Attribute> {
    // iterates over the attributes in the table under consideration

  public :

    AttributeIterator(const PGresult* result)
      : res(result), index(0) {}

    Attribute operator * () const {
      char * const p = PQfname(res, index);
      if (p == 0) {
	ostringstream o; o << index;
	throw Error::invalid_column_index(o.str(), "");
      }
      return Attribute(PQftype(res, index), index, p);
    }

    AttributeIterator operator + (int i) const {
      AttributeIterator it(*this);
      it.index += i;
      return it;
    }

    AttributeIterator& operator ++() {
      ++index;
      return *this;
    }

    AttributeIterator operator ++(int) {
      AttributeIterator it(*this);
      ++index;
      return it;
    }

    bool operator == (const AttributeIterator& it) const {
      return it.res == res && it.index == index;
    }

    bool operator != (const AttributeIterator& it) const {
      return it.res != res || it.index != index;
    }

  protected :

    const PGresult* const res;
    int index;
  };


  class FieldIterator : std::iterator<std::input_iterator_tag, Value> {
    // iterates over the fields in a given row

  public :

    FieldIterator(const PGresult* result, int row_index) : res(result), row(row_index), column(0) {}

    Value operator * () const {
      char * p = PQgetvalue(res, row, column);
      if (p == 0) {
	ostringstream o; o << "row: " << row << ", column: " << column;
	throw Error::invalid_index(o.str(), "");
      }
      if (PQgetisnull(res, row, column))
	return Value();
      else
	return Value(p);
    }

    FieldIterator operator + (int i) const {
      FieldIterator it(*this);
      it.column += i;
      return it;
    }

    FieldIterator& operator ++() {
      ++column;
      return *this;
    }

    FieldIterator operator ++(int) {
      FieldIterator it(*this);
      ++column;
      return it;
    }

    bool operator == (const FieldIterator& it) const {
      return it.res == res && it.row == row && it.column == column ;
    }

    bool operator != (const FieldIterator& it) const {
      return it.res != res || it.row != row || it.column != column;
    }

  private :

    const PGresult* const res;
    const int row;
    int column;
  };


 class RowIterator : std::iterator<std::input_iterator_tag, Value> {
    // iterates over the rows in a given column

  public :

    RowIterator(const PGresult* result, int column_index)
      : res(result), row(0), column(column_index) {}

    Value operator * () const {
      char * p = PQgetvalue(res, row, column);
      if (p == 0) {
	ostringstream o; o << "row: " << row << ", column: " << column;
	throw Error::invalid_index(o.str(), "");
      }
      if (PQgetisnull(res, row, column))
	return Value();
      else
	return Value(p);
    }

    RowIterator operator + (int i) const {
      RowIterator it(*this);
      it.row += i;
      return it;
    }

    RowIterator& operator ++() {
      ++row;
      return *this;
    }

    RowIterator operator ++(int) {
      RowIterator it(*this);
      ++row;
      return it;
    }

    bool operator == (const RowIterator& it) const {
      return it.res == res && it.row == row && it.column == column ;
    }

    bool operator != (const RowIterator& it) const {
      return it.res != res || it.row != row || it.column != column;
    }

  private :

    const PGresult* const res;
    int row;
    const int column;
  };



  // --------------------------------------------------------------------------
  // SELECT handler
  // --------------------------------------------------------------------------
  

  typedef map<std::string, Value> row_map;

  class Select {

  public :

    Select(const Connection& connection, const int df = 1000)
      : conn(connection), res(0), n_columns(0), row(0), n_rows(0), comm(conn), scan(false), default_step(df), step(default_step), cursor_name("c_select" + StringHandling::toString(&connection)), end_cursor_command("close " + cursor_name + "; commit;") {}

    void issue(const std::string& command, const bool use_cursor = false) {
      PQclear(res);
      res = 0;
      if (scan) {
	end_cursor();
	scan = false;
      }
      row = -1;
      if (use_cursor) {
	scan = true;
	comm.issue("begin; declare " + cursor_name + " cursor for " + command);
	fetch(); // sets res
      }
      else {
	res = PQexec(conn, command.c_str());
	ExecStatusType status(PQresultStatus(res));
	if (status != PGRES_TUPLES_OK)
	  throw Error::command_failed(PQresultErrorMessage(res), conn.getName(), PQresStatus(status));
      }
      n_rows = PQntuples(res);
      n_columns = PQnfields(res);
      attribute_value = map<std::string, Value>(AttributeIteratorInit1(res), AttributeIteratorInit1(res) + n_columns);
    }

    int current_row() const { return row; }
    int number_rows() const { return n_rows; }
    int number_columns() const { return attribute_value.size(); }
    // when scan is true, then these attributes are concerned only with
    // the currently read block (switching to the next block only via ++)

    const Attribute get_Attribute(const int index) const {
      return Attribute(PQftype(res, index), index, PQfname(res, index));
    }
    const Attribute get_Attribute(const std::string& name) const {
      int i = PQfnumber(res, name.c_str());
      if (i == -1) {
	ostringstream o; o << i;
	throw Error::invalid_column_index(o.str(), conn.getName());
      }
      return Attribute(PQftype(res, i), i, name);
    }

    operator PGresult*() const { return res; }

    operator bool() const { return row < n_rows; }
    // true : until now, reading of rows (including the header row) was succesful (in other words, yet we didn't hit the "end-of-rows")
    // reminder: the header has row number -1

    Select& operator ++() {
      if (scan) {
	if (n_rows == ++row) {
	  if (n_rows == step) {
	    PQclear(res);
	    fetch();
	    row = 0;
	    n_rows = PQntuples(res);
	    n_columns = PQnfields(res);
	    if (n_rows == 0) {
	      scan = false;
	      end_cursor();
	      return *this;
	    }
	  }
	  else {
	    row = n_rows;
	    scan = false;
	    end_cursor();
	    return *this;
	  }
	}
      }
      else if (n_rows == row || n_rows == ++row) // first disjunct: end was already reached
	return *this;
      map<std::string, Value>::iterator i;
      for (i = attribute_value.begin(); i != attribute_value.end(); i++) {
	int fi = PQfnumber(res, i -> first.c_str());
	if (PQgetisnull(res, row, fi))
	  i -> second = Value();
	else
	  i -> second = Value(PQgetvalue(res, row, fi));
      }
      return *this;
    }

    Select& set_row(int n) {
      if (n <= -1) {
	row = -1;
	attribute_value = map<std::string, Value>(AttributeIteratorInit1(res), AttributeIteratorInit1(res) + n_columns);
      }
      else if (n >= n_rows)
	row = n_rows;
      else {
	row = n;
	map<std::string, Value>::iterator i;
	for (i = attribute_value.begin(); i != attribute_value.end(); i++) {
	  int fi = PQfnumber(res, i -> first.c_str());
	  if (PQgetisnull(res, row, fi))
	    i -> second = Value();
	  else
	    i -> second = Value(PQgetvalue(res, row, fi));
	}
      }
      return *this;
    }

    const row_map& values() const { return attribute_value; }
    const Value value(const std::string& attribute) const {
      const DatabaseHandler01::row_map::const_iterator p(attribute_value.find(attribute));
      if (p == attribute_value.end())
	throw Error::attribute_not_found(attribute, conn.getName());
      return p -> second;
    }
    
    void set_cursor_step(const int s) { step = s; }

    void truncate_result() {
      PQclear(res);
      res = 0;
    }

    ~Select() {
#ifdef DEBUG
      std::cout << "Destruktor Select Eingang: conn = " << conn << std::endl;
#endif
      PQclear(res);
      if (scan)
	end_cursor();
#ifdef DEBUG
      std::cout << "Destruktor Select Ausgang." << std::endl;
#endif
    }

  private :

    Select (const Select&); // not allowed (don't implement)
    Select& operator = (const Select&); // not allowed

    const Connection& conn;
    PGresult* res;

    int n_columns;
    int row;
    int n_rows;

    row_map attribute_value;

    Command comm;
    bool scan;
    const int default_step;
    int step;
    const std::string cursor_name;
    const std::string end_cursor_command;

    void end_cursor() {
      comm.issue(end_cursor_command);
    }
    void fetch() {
      const std::string fetch_command = "fetch " + StringHandling::toString(step) + " from " + cursor_name;
      res = PQexec(conn, fetch_command.c_str());
      ExecStatusType status(PQresultStatus(res));
      if (status != PGRES_TUPLES_OK)
	throw Error::command_failed(PQresultErrorMessage(res), conn.getName(), PQresStatus(status));
    }

    class AttributeIteratorInit1 : public AttributeIterator {

    public :

      AttributeIteratorInit1(const PGresult* result) : AttributeIterator(result) {}

      std::pair<std::string, Value> operator *() { return std::make_pair(PQfname(res, index), Value()); }

      AttributeIteratorInit1 operator + (int i) const {
	return (*this).AttributeIterator::operator +(i);
      }
      
      AttributeIteratorInit1& operator ++() {
	(*this).AttributeIterator::operator ++();
	return (*this);
      }
      
      AttributeIteratorInit1 operator ++(int) {
	return (*this).AttributeIterator::operator ++(0);
      }

    private :

      AttributeIteratorInit1(const AttributeIterator& fi) : AttributeIterator(fi) {}

    };


  };


  // remaining implementations ------------------------
  
  inline set<std::string> Connection::getTableNames() const {
    Select query(*this);
    query.issue("select relname from pg_class where relowner = (select datdba from pg_database where datname = '" + name + "') and (relkind = 'r' or relkind = 'v');");
    return set<std::string>(RowIterator(query,0), RowIterator(query,0) + query.number_rows());
  }
  // returns all tables and views owned by the owner of the database



  // --------------------------------------------------------------------------
  // The special "object-interface"
  // --------------------------------------------------------------------------


  class DatabaseHandler : public Connection {

  public :

    DatabaseHandler (const std::string& database, const std::string& passwd = "", const std::string& user = "")
      : Connection(database, passwd, user), TableNames(static_cast<Connection*>(this) -> getTableNames()) { }

    const set<std::string>& getTableNames() const { return TableNames; }
    void refreshTableNames() { TableNames = static_cast<Connection*>(this) -> getTableNames(); }

  private :

    DatabaseHandler(const DatabaseHandler&); // not allowed
    DatabaseHandler& operator = (const DatabaseHandler&); // not allowed

    set<std::string> TableNames;

  };


  class AttributeObject;

  class TableHandler {

  public :

    TableHandler(DatabaseHandler& dh, const std::string& table)
      : name(table), source(dh), query(source), command(source), change(false), nresults(0) {
      if (dh.getTableNames().count(table) == 0)
	throw Error::no_table(table, source.getName());
      query.issue(std::string("select * from ") + name + std::string(" where false;"));
      attributes = map<std::string, Attribute>(AttributeIteratorInit2(query), AttributeIteratorInit2(query) + query.number_columns());
    }

    void submit();

    int completion(bool restricted = true);
    int get_last_nresults() const { return nresults; }
    void clear();

    const map<std::string, Attribute>& get_attributes() const { return attributes; }

    row_map get_values() const;
 
    const Select& get_query() const { return query; }
    const Command& get_command() const { return command; }
    const DatabaseHandler& get_source() const { return source; }

    const std::string name;

    bool changed() const { return change; }
    // always true after submit;
    // after completion changed is true if it was before, or if some value
    // changed from Null;
    // also set by AttributeObjects in case of a write operation;
    void reset_changed() { change = false; }

  private :

    DatabaseHandler& source;

    friend class AttributeObject;

    TableHandler(const TableHandler& th); // not allowed
    TableHandler& operator = (const TableHandler&); // not allowed

    map<std::string, Attribute> attributes;

    map<std::string, AttributeObject*> attribute_objects;
    
    Select query;

    Command command;

    bool change; // changes to the values (must be reset explicitly)

    int nresults; // number of results of last completion call;
    // value is 0 at the beginning and after clear (either used globally
    // for TableHandler or when used for an AttributeObject whose value
    // was not NULL;

    void enrol(const std::string& attribute, AttributeObject* p) {
      if (attributes.count(attribute) == 0)
	throw Error::no_attribute(attribute, source.getName());
      if (! attribute_objects.insert(std::make_pair(attribute, p)).second)
	throw Error::multiple_attributes(attribute, source.getName());
    }
    void leave(const std::string& attribute) {
      attributes.erase(attribute);
    }


    class AttributeIteratorInit2 : public AttributeIterator {

    public :

      AttributeIteratorInit2(const PGresult* result) : AttributeIterator(result) {}

      std::pair<std::string, Attribute> operator *() {
	Attribute a = (*this).AttributeIterator::operator *();
	return std::make_pair(a.name, a); }

      AttributeIteratorInit2 operator + (int i) const {
	return (*this).AttributeIterator::operator +(i);
      }
      
      AttributeIteratorInit2& operator ++() {
	(*this).AttributeIterator::operator ++();
	return (*this);
      }
      
      AttributeIteratorInit2 operator ++(int) {
	return (*this).AttributeIterator::operator ++(0);
      }

    private :

      AttributeIteratorInit2(const AttributeIterator& fi) : AttributeIterator(fi) {}

    };

  };


  class AttributeObject {

  public :

    AttributeObject(TableHandler& th, const std::string& attribute)
      : name(attribute), source(th), isNull(true) {
      // can be created for each attribute at most once
      source.enrol(attribute, this);
    }

    operator Value() const {
      if (isNull)
	return Value();
      else
	return Value(s.str());
    }

    Value getValue() const {
      if (isNull)
	return Value();
      else
	return Value(s.str());
    }
    // es scheint haeufiger (?!) die Umwandlung Value(AO) als Konstruktor der
    // Klasse Value missinterpretiert zu werden ?!

    std::string str() const {
      return std::string(getValue());
    }

    template<class T> AttributeObject& operator << (const T& x) {
      if (isNull) {
	s << x;
	isNull = false;
	source.change = true;
      }
      else {
	ostringstream s2;
	s2 << x;
	if (s2.str() != s.str())
	  throw Error::filling(name + ": " + s.str() + " <> " + s2.str(), source.get_source().getName());
      }
      return *this;
    }

    template<class T> AttributeObject& operator >> (T& x) {
      std::stringstream s(str()); s >> x;
      return *this;
    }


    const TableHandler& get_source() const {return source; }

    const std::string name;

    void clear() {
      if (! isNull) {
	s.str("");
	isNull = true;
	source.change = true;
	source.nresults = 0;
      }
    }

    ~AttributeObject() {
      source.leave(name);
    }

  private :

    friend class TableHandler;

    AttributeObject (const AttributeObject&); // not allowed
    AttributeObject& operator = (const AttributeObject&); // not allowed

    TableHandler& source;

    ostringstream s;
    bool isNull;

  };


// Auxiliary class to create all attribute objects for a table

  class AllAttObj {

  public :
    
    typedef map<std::string, AttributeObject*> St2AOP;
    
    AllAttObj(TableHandler& th)
      : table(th.name) {

      typedef map<std::string, Attribute> St2At;
      const St2At& attlist(th.get_attributes());
      
      St2AOP::iterator pos(aos.begin());
      for (St2At::const_iterator i = attlist.begin(); i != attlist.end(); i++)
	pos = aos.insert(pos, make_pair(i -> first, new AttributeObject(th, i -> first)));
    }

    AttributeObject& operator() (const std::string& name) const {
      const St2AOP::const_iterator i = aos.find(name);
      if (i == aos.end())
	throw Error::no_attribute(name, table);
      // it makes more sense to give the table than the database
      return *(i -> second);
    }
    
    ~AllAttObj() {
      for_each(aos.begin(), aos.end(), FunctionHandling::DeleteObjectSecond());
    }

  private:

    const std::string& table;
    St2AOP aos;
  };



  // remaining definitions ---------------------


  namespace local_get_values {
    typedef map<std::string, AttributeObject*>::const_iterator it;
    class val_it : public it {
    public :
      val_it(it i) : it(i) {}
      std::pair<std::string, Value> operator *() const {
	const it pao = static_cast<it> (*this);
	return std::make_pair(pao -> first, pao -> second -> getValue());
      }
    };
  }
  inline row_map TableHandler::get_values() const {
      return map<std::string, Value>(local_get_values::val_it(attribute_objects.begin()), local_get_values::val_it(attribute_objects.end()));
    }


  inline void TableHandler::submit() {
    // creates a new row in the table according to the current setting
    std::string insert_0 = "insert into " + name;
    std::string insert_a = " (";
    std::string insert_b = " values(";
    for (map<std::string, AttributeObject*>::const_iterator i = attribute_objects.begin(); i != attribute_objects.end(); i++)
      if (! Value(*(i -> second)).null) {
	insert_a += i -> first + ",";
	insert_b += std::string("'") + std::string(Value(*(i -> second))) + "',";
      }
    if (insert_a[insert_a.size() - 1] == ',') {
      insert_a.erase(insert_a.size() - 1); insert_a += ")";
      insert_b.erase(insert_b.size() - 1); insert_b += ");";
      command.issue(insert_0 + insert_a + insert_b);
    }
    else
      command.issue(insert_0 + insert_b + "NULL);");
    change = true;
  }

  namespace local_TableHandler_clear {
    struct clear {
      void operator () (const pair<std::string, AttributeObject*>& p) const {
	p.second -> clear();
      }
    };
  }
  inline void TableHandler::clear() {
    for_each(attribute_objects.begin(), attribute_objects.end(), local_TableHandler_clear::clear());
    nresults = 0;
  }

  inline int TableHandler::completion(bool restricted) {

    // returns the number of rows in the table consistent with the current setting;
    // if restricted is true, then only up to 2 rows are selected;
    // if return value = 1, then attribute values currently being NULL are filled and in case at least one such filling took place, change is set to true;
    // if the last completion had an unique result, but this not, then Error::inconsistency is thrown;

    stack<AttributeObject*> remaining;
    {
      const std::string select_a = "select * from " + name;
      std::string select_b = " where";
      for (map<std::string, AttributeObject*>::const_iterator i = attribute_objects.begin(); i != attribute_objects.end(); i++)
	if (Value(*(i -> second)).null)
	  remaining.push(i -> second);
	else
	  select_b += " " + i -> first + "='" + std::string(Value(*(i -> second))) + "' and";
      std::string select_c;
      if (select_b.size() == 6) // no constraints
	select_c = select_a;
      else {
	select_b.erase(select_b.size() - 4);
	select_c = select_a + select_b;
      }
      if (restricted)
	select_c += " LIMIT 2";
      select_c += ";";
      query.issue(select_c);
    }
    if (query.number_rows() != 1)
      if ((nresults == 1) and query.number_rows() == 0)
	throw Error::inconsistency(name, source.getName());
    else
      return (nresults = query.number_rows());
    nresults = 1;
    ++query;
    while (! remaining.empty()) {
      AttributeObject* const o = remaining.top();
      const row_map::const_iterator i = query.values().find(o -> name);
      if ( ! i -> second.null) {
	(*o) << std::string(i -> second);
	change = true;
      }
      remaining.pop();
    }
    return 1;
  }

}

#endif

#define DATABASEHANDLER01WAECHTER

