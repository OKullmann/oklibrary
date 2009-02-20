// Oliver Kullmann, 29.5.2002 (Swansea)
/* Copyright 2002 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef STREAMHANDLINGWAECHTER

#define STREAMHANDLINGWAECHTER

#include <istream>
#include <limits>
#include <string>
#include <iterator>
#include <fstream>
#include <memory> // for auto_ptr
#include <new> // for bad_alloc
#include <ios> // for ios_base::failure
#include <cstdio> // for tmpnam, remove
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstddef>
#include <streambuf>
#include <iomanip>

#include <unistd.h> // for close, write
#include <stdio.h> // for mktemp
#include <fcntl.h> // for open, fcntl
#include <sys/stat.h> // for mode macros
#include <sys/types.h>

#include <OKlib/General/ErrorHandling.hpp>

namespace StreamHandling {

  struct Error : ErrorHandling::Error {
    Error(const std::string& what) : ErrorHandling::Error(what) {}
  };

}

// ---------------------------------------------------------------------------
// Redirection
// ---------------------------------------------------------------------------

namespace StreamHandling {

  // noch nicht fertig:

  class RedirectingStd {
  public :

    RedirectingStd() :
      in_buf(0), out_buf(0), err_buf(0), activated(false) {}

    void set(std::istream& in, std::ostream& out, std::ostream& err) {
      if (not activated) {
	using namespace std;
	in_buf = cin.rdbuf(); out_buf = cout.rdbuf(); err_buf = cerr.rdbuf();
	in_state = cin.rdstate(); out_state = cin.rdstate(); err_state = cerr.rdstate();
	cin.rdbuf(in.rdbuf()); cout.rdbuf(out.rdbuf()); cerr.rdbuf(err.rdbuf());
	cin.exceptions(ios_base::goodbit); cout.exceptions(ios_base::goodbit); cerr.exceptions(ios_base::goodbit);
	cin.clear(in.rdstate()); cout.clear(out.rdstate()); cerr.clear(err.rdstate());
      }
    }
    void unset() {
      if (activated) {
	using namespace std;
	cin.rdbuf(in_buf); cout.rdbuf(out_buf); cerr.rdbuf(err_buf);
      }
    }

  private :

    std::streambuf* in_buf; std::streambuf* out_buf; std::streambuf* err_buf;
    std::ios_base::iostate in_state; std::ios_base::iostate out_state; std::ios_base::iostate err_state;
    bool activated;

  };

}

// ---------------------------------------------------------------------------
// Advancing lines in streams
// ---------------------------------------------------------------------------

namespace StreamHandling {

  inline std::istream& line_advance(std::istream& s, unsigned long int a) throw(std::ios_base::failure) {
    // If on entry s is false, then nothing happens. Else the following happens:
    // After completion, s.eof() is true (and thus bool(s) is false) iff 
    // not a-many complete lines (that is, including "\n") could be read;
    // otherwise, the stream pointer points to the first character after the
    // last eof-character (this first character may in fact be end-of-file).

    for (unsigned long int i = 0; i < a and s; i++)
      s.ignore(std::numeric_limits<int>::max(), '\n');
    return s;
  }

}

// ---------------------------------------------------------------------------
// A very simple container for files, supporting an input line iterator
// ---------------------------------------------------------------------------

namespace StreamHandling {

  namespace Error_FileLines {

    struct open_file : Error {
      open_file(const std::string& what) : Error(what) {}
    };
    struct read_file : Error {
      read_file(const std::string& what) : Error(what) {}
    };
  }

  // std::ios_base::failure is only thrown if activated by the user

  class FileLinesInputIterator;

  class FileLines {

  public :

    typedef FileLinesInputIterator iterator;

    explicit FileLines(const std::string&) throw();
    FileLines(const FileLines&) throw();
    FileLines& operator = (const FileLines&) throw();
    bool operator == (const FileLines&) const throw();
    bool operator != (const FileLines&) const throw();

    std::streamsize size() const throw(Error_FileLines::open_file, Error_FileLines::read_file, std::ios_base::failure);
    // requires linear complexity
    
    iterator begin() const throw(Error_FileLines::open_file, std::ios_base::failure, std::bad_alloc);
    iterator end() const throw();

    // the end-iterator is a generic one, and doesn't look at the file

  private :

    std::string filename;
  };

  // Additionally to the usual operations for input iterators we have the
  // operation +, which is "lazily" implemented, so that running through
  // the file only occurs when we have to:
  // 1. if we want to read a line;
  // 2. we compare it with an end-iterator.
  // Additionally to the usually guarantees for input iterators (which assume,
  // that dereferenciation happens only in the valid range, and that
  // pointer arithmetic never goes beyond the end), we have that
  // "iterator + n" is compared equal to an end-iterator if from the current
  // position of "iterator" there are less than n lines to the end
  // (when using "iterator + n" it is assumed, that iterator has not
  // become an end-iterator, which in case of using "+" becomes only
  // apparent when using "*", "==", "!=" or "++" thereafter).
  // Each assignment or copying for these iterators transfers ownership.

  // Examples:

  /*
    FileLines f(name);
    FileLines::iterator it = f.begin();
    it = it + 7;
    cout << (it == f.end()) << endl;
    cout << *it << endl;
    cout << (it == f.end()) << endl; // didn't change (of course)
    cout << *++it << endl;
    cout << *it << endl;
    copy(f.begin() + 2, f.begin() + 4, ostream_iterator<string>(cout));
    copy(f.begin() + 6, f.begin() + std::numeric_limits<unsigned long int>::max(), ostream_iterator<string>(cout)); // if the file is not too big, this will have the same effect as
    copy(f.begin() + 6, f.end(), ostream_iterator<string>(cout));
  */

  class FileLinesInputIterator : public std::iterator<std::input_iterator_tag, std::string, void, void, void> {

  public :

    FileLinesInputIterator(const FileLinesInputIterator&) throw();
    // attention: fake "const" (otherwise return in the above begin and end
    // does not compile)
    FileLinesInputIterator& operator = (FileLinesInputIterator&) throw();

    bool operator == (const FileLinesInputIterator&) const throw();
    bool operator != (const FileLinesInputIterator&) const throw();

    value_type operator *() const throw(Error_FileLines::read_file, std::ios_base::failure);
    // the line-end-symbol is not included in the string returned by *;
    // evaluation needs to go to the file (always in forward direction) from
    // the position of the last read to the current position --- this is not
    // done at the time when applying operator +, and not for the last line
    // when applying operator ++;
    FileLinesInputIterator& operator++() throw(Error_FileLines::read_file, std::ios_base::failure);

    FileLinesInputIterator& operator+(unsigned long int) throw();
    // does not read the file;

  private :

    friend class FileLines;

    FileLinesInputIterator() throw();
    FileLinesInputIterator(const std::string&) throw(Error_FileLines::open_file, std::ios_base::failure, std::bad_alloc);

    struct _impl {
      std::ifstream s;
      value_type line;
      std::streamsize current_line;
      // implicitely points to the beginning of the current line, while
      // last_line_read implicitely points to the end-of-line of that
      // last line;
      std::streamsize last_line_read;
      // invariant: current_line >= last_line_read

      _impl (const std::string&) throw(Error_FileLines::open_file, std::ios_base::failure);
    };

    mutable std::auto_ptr<_impl> p;

    void update() const throw(Error_FileLines::read_file, std::ios_base::failure);

  };

}

// -----------------------------
// Temporary files
// -----------------------------

namespace StreamHandling {

  namespace Error_TempFile {

    struct open_file : Error {
      open_file(const std::string& what) : Error(what) {}
    };
    struct remove_file : Error {
      remove_file(const std::string& what) : Error(what) {}
    };
    struct call_mktemp : Error {
      call_mktemp(const std::string& what) : Error(what) {}
    };
    struct call_open : Error {
      call_open(const std::string& what) : Error(what) {}
    };
    struct call_close : Error {
      call_close(const std::string& what) : Error(what) {}
    };
  }


  // Policy classes for creating a temporary file and its name
  // ---------------------------------------------------------

  const char * const template_part = "XXXXXX";
  const int length_template_part = 6;
  
  class path_template {
  public :
    struct Parameter {
      Parameter(const std::string& directory, const std::string& basename) :
	dir(directory), base(basename) {}
      const std::string& dir, base;
    };
  protected :
    ~path_template() { delete[] buffer; }
    // throw()

    path_template(const Parameter& P) :
      length(P.dir.size() + 1 + P.base.size() + length_template_part + 1), buffer(new char[length])
    {
      char * pointer = std::copy(P.dir.begin(), P.dir.end(), buffer);
      *(pointer++) = '/';
      pointer = std::copy(P.base.begin(), P.base.end(), pointer);
      std::copy(template_part, template_part + length_template_part + 1, pointer);
      if (! mktemp(buffer))
	throw Error_TempFile::call_mktemp("StreamHandling::path_template::path_template(const Parameter& P) : Call to mktemp failed");
      const int file_descriptor = open(buffer, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
      if (file_descriptor == -1)
	throw Error_TempFile::call_open("StreamHandling::path_template::path_template(const Parameter& P) : Call to open failed");
      if (close(file_descriptor))
	throw Error_TempFile::call_close("StreamHandling::path_template::path_template(const Parameter& P) : Call to close failed");
     }
    // throw(Error_TempFile::call_mkstemp, Error_TempFile::call_open, Error_TempFile::call_close)

    std::string create_name() const {
      return std::string(buffer, buffer + (length - 1));
    }
    // throw(std::length_error )

  private :
    const std::size_t length;
    char * buffer;

    path_template(const path_template&);
    path_template& operator = (const path_template&);
  };

  class basename_template : protected path_template {
  public :
    struct Parameter {
      Parameter(const std::string& basename) :
	base(basename) {}
      const std::string& base;
    };
  protected :
    basename_template(const Parameter& P) :
      path_template(path_template::Parameter("/tmp", P.base)) {}
  };


  class self_named {
  public :

    struct Parameter {
      Parameter(const std::string& full_name) :
	name(full_name) {}
      const std::string& name;
    };

  protected :

    ~self_named() {}
    // throw()

    self_named(const Parameter& P) :
      name_(P.name) {
      const int file_descriptor = open(name_.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
      if (file_descriptor == -1)
	throw Error_TempFile::call_open("StreamHandling::self_named::self_named(const Parameter& P) : Call to open failed for " + name_);
      if (close(file_descriptor))
	throw Error_TempFile::call_close("StreamHandling::self_named::self_named(const Parameter& P) : Call to close failed for " + name_);
    }
    // throw(Error_TempFile::call_open, Error_TempFile::call_close)

    std::string create_name() const { return name_; }

  private :
    const std::string name_;
  };


  // Main class for creating temporary files
  // ----------------------------------------------------------

  template < class Creator >
  class TemporaryFile : public Creator {
  public :

    TemporaryFile(const typename Creator::Parameter& P) :
      Creator(P), name_(Creator::create_name()) {}
    // throw(Error_TempFile::open_file)

    void open() {
      file.open(name_.c_str(), std::ios::in | std::ios::out | std::ios::trunc);
      if (! file)
	throw Error_TempFile::open_file("StreamHandling::TemporaryFile::open() : Can not open file " + name_);
    }
    // throw(Error_TempFile::open_file, std::ios_base::failure)
    // opens for reading and writing with truncation

    std::fstream& f() { return file; }
    // throw()

    std::string name() const { return name_; };
    // throw()

    ~TemporaryFile() {
      if (file.is_open())
	file.close();
      if (std::remove(name_.c_str()))
	throw Error_TempFile::remove_file("StreamHandling::TemporaryFile::~TemporaryFile() : Can not remove file " + name_);
    }
    // throw(Error_TempFile::remove_file)

  private :

    const std::string name_; // full name of the temporary file
    std::fstream file;

    TemporaryFile(const TemporaryFile&);
    TemporaryFile& operator = (const TemporaryFile&);
  };

  template <class C, typename T>
  TemporaryFile<C>& operator << (TemporaryFile<C>& tf, const T& x) {
    (std::fstream&)(tf) << x;
    return tf;
  }

  class TemporaryFile_path_template : public TemporaryFile<path_template> {
  public :
    TemporaryFile_path_template(const std::string& directory, const std::string& basename) :
      TemporaryFile<path_template>(path_template::Parameter(directory, basename)) {}
  };

  class TemporaryFile_basename_template : public TemporaryFile<basename_template> {
  public :
    TemporaryFile_basename_template(const std::string& basename) :
      TemporaryFile<basename_template>(basename_template::Parameter(basename)) {}
  };

  class TemporaryFile_self_named : public TemporaryFile<self_named> {
  public :
    TemporaryFile_self_named(const std::string& name) :
      TemporaryFile<self_named>(self_named::Parameter(name)) {}
  };
 


  // DEPRICATED -------------------------------------------------
  // Warning: If used by multiple processes, TempFile is not safe!
  // ------------------------------------------------------------

  class TempFile {
    // can not be copied or assigned

  public :

    TempFile() throw(Error_TempFile::open_file, std::ios_base::failure);
    TempFile(const std::string& name) throw(Error_TempFile::open_file, std::ios_base::failure);

    operator std::fstream&() throw();
    std::fstream& f() throw();

    std::string name() const { return temp_name; }

    ~TempFile() throw(Error_TempFile::remove_file);

  private :

    const std::string temp_name;
    std::fstream file;
  };

}


// -----------------------------
// Removing lines from files
// -----------------------------

namespace StreamHandling {

  namespace Error_remove_lines {

    struct temporary_file : Error {
      temporary_file(const std::string& what) : Error(what) {}
    };
  }

  void remove_lines(std::string name, long int n) throw(Error_TempFile::open_file, Error_FileLines::open_file, Error_FileLines::read_file, Error_remove_lines::temporary_file, Error_TempFile::remove_file, std::ios_base::failure, std::bad_alloc) {

    // If n < 0 then the trailing n lines are removed, and
    // if n > 0 then the leading n lines are removed;
    // for n <> 0 the final line is always concluded with "\n";
    // if abs(n) is greater or equal than the number of lines in the file,
    // then the file becomes empty.

    if (n == 0)
      return;
    TempFile t;
    {
      FileLines f(name);
      if (n > 0)
	std::copy(f.begin() + n, f.end(), std::ostream_iterator<std::string>(t, "\n"));
      else {
	std::copy(f.begin(), f.begin() + std::max(f.size() + n, 0L), std::ostream_iterator<std::string>(t, "\n"));
      }
    }
    t.f().seekg(0);
    std::ofstream f(name.c_str());
    f << t.f().rdbuf();
    if (! t.f())
      throw Error_remove_lines::temporary_file("StreamHandling::remove_lines : Error when writing to temporary file " + t.name());
  }

}

// ------------------------------------------
// Pouring the content of a file into a string
// ------------------------------------------

namespace StreamHandling {

  namespace Error_file2string {

    struct open_file : Error {
      open_file(const std::string& what) : Error(what) {}
    };
  }

  inline std::string file2string(const char* const name) throw(Error_file2string::open_file, std::ios_base::failure) {
    std::ifstream f(name);
    if (! f)
      throw Error_file2string::open_file("StreamHandling::file2string : Can not open file " + std::string(name));
    std::ostringstream s;
    s << f.rdbuf();
    return s.str();
  }
  inline std::string file2string(const std::string& name) throw(Error_file2string::open_file, std::ios_base::failure) {
    return file2string(name.c_str());
  }

}

// -----------------------------
// Opening an std::ostream object which writes its output
// into a stream given by a Unix file descriptor
// -----------------------------

// From \cite{Jo1999}, page 672

namespace StreamHandling {

  class fdoutbuf : public std::streambuf {
  protected :
    int fd; // file descriptor
  public :
    fdoutbuf(int _fd) : fd(_fd) {}
  protected :
    virtual int_type overflow(int_type c) {
      if (c != EOF) {
	char z = c;
	if (write(fd, &z, 1) != 1)
	  return EOF;
      }
      return c;
    }

    virtual std::streamsize xsputn(const char* s, std::streamsize num) {
      return write(fd, s, num);
    }
  };

  struct fdostream : protected fdoutbuf, public std::ostream {
    fdostream(int fd) : fdoutbuf(fd), std::ostream(static_cast<fdoutbuf*>(this))  {}
  };

}

// --------------------------------------------------------------------------
// Opening a file for writing (appending at the end) so that
// the file is locked for other processes (using fcntl)
// --------------------------------------------------------------------------

namespace StreamHandling {

  namespace Error_LockFile {

    struct lock_file : Error {
      lock_file(const std::string& what) : Error(what) {}
    };
    struct unlock_file : Error {
      unlock_file(const std::string& what) : Error(what) {}
    };
     struct call_open : Error {
      call_open(const std::string& what) : Error(what) {}
    };
    struct call_close : Error {
      call_close(const std::string& what) : Error(what) {}
    };
  }

  struct LockedFileOstream : fdostream {
    LockedFileOstream(const std::string& _name) :
      fdostream(open(_name.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR)), name(_name)  {
      if (fd == -1)
	throw Error_LockFile::call_open("StreamHandling::LockedFileOstream::LockedFileOstream(const std::string& _name) : Can not open file " + name);
      flock set_lock = { F_WRLCK, 0, SEEK_SET, 0};
      if (fcntl(fd, F_SETLKW, &set_lock) == -1)
	throw Error_LockFile::lock_file("StreamHandling::LockedFileOstream::LockedFileOstream(const std::string& _name) : Can not lock file " + name);
	}
    ~LockedFileOstream() {
      flock release_lock = { F_UNLCK, 0, SEEK_SET, 0};
      if (fcntl(fd, F_SETLK, &release_lock) == -1)
	throw Error_LockFile::unlock_file("StreamHandling::LockedFileOstream::~LockedFileOstream(const std::string& _name) : Can not unlock file " + name);
      if (close(fd))
	throw Error_LockFile::call_open("StreamHandling::LockedFileOstream::~LockedFileOstream(const std::string& _name) : Can not close file " + name);
    }
    const std::string name;
  };

}

// ----------------------------------------------------------------------
// Copying files
// ----------------------------------------------------------------------

namespace StreamHandling {

  bool copy(const std::string& from, const std::string& to) {
    std::ifstream in(from.c_str());
    std::ofstream out(to.c_str());
    out << in.rdbuf();
    return in and out;
  }

}

// ----------------------------------------------------------------------
// Output iterator adapter with field width
// ----------------------------------------------------------------------

namespace StreamHandling {

  template <typename T, int width>
  class Wrapper {
    const T& t;
  public :
    Wrapper(const T& t) : t(t) {}
    friend inline std::ostream& operator <<(std::ostream& o, const Wrapper& w) {
      return o << std::setw(width) << w.t;
    }
  };

  template <typename T, int width>
  std::ostream_iterator<Wrapper<T, width> > output_iterator(std::ostream& o, const char* sep) {
    return std::ostream_iterator<Wrapper<T, width> > (o, sep);
  }

}

// ----------------------------------------------------------------------

// -----------------------------
// Implementation of class FileLines
// -----------------------------

namespace StreamHandling {

  inline FileLines::FileLines(const std::string& f) throw() : filename(f) {}

  inline FileLines::FileLines(const FileLines& f) throw() : filename(f.filename) {}

  inline FileLines& FileLines::operator = (const FileLines& f) throw() {
    filename = f.filename;
    return *this;
  }

  inline bool FileLines::operator == (const FileLines& f) const throw() {
    return filename == f.filename;
  }

  inline bool FileLines::operator != (const FileLines& f) const throw() {
    return filename != f.filename;
  }

  inline std::streamsize FileLines::size() const throw(Error_FileLines::open_file, Error_FileLines::read_file, std::ios_base::failure) {
    std::ifstream s(filename.c_str());
    if (! s)
      throw Error_FileLines::open_file("StreamHandling::FileLines::size : Can not open file " + filename);
    std::streamsize n = 0;
    bool last_character_is_eol = false;
    do {
      if (s.peek() == std::char_traits<char>::eof()) {
	last_character_is_eol = true;
	break;
      }
      s.ignore(std::numeric_limits<int>::max(), '\n');
      ++ n;
    } while (s);
    if (s.bad() or (not last_character_is_eol and not s.eof()))
      throw Error_FileLines::read_file("StreamHandling::FileLines::size : Error when reading from file " + filename);
    return n;
  }

  inline FileLinesInputIterator FileLines::begin() const throw(Error_FileLines::open_file, std::ios_base::failure, std::bad_alloc) {
    return FileLinesInputIterator(filename);
  }

  inline FileLinesInputIterator FileLines::end() const throw() {
    return FileLinesInputIterator();
  }
}


// -----------------------------
// Implementation of class FileLinesInputIterator
// -----------------------------

namespace StreamHandling {
  
  // _impl

  inline FileLinesInputIterator::_impl::_impl (const std::string& filename) throw(Error_FileLines::open_file, std::ios_base::failure) : s(filename.c_str()), current_line(0), last_line_read(-1) {
    if (! s)
      throw Error_FileLines::open_file("StreamHandling::FileLinesInputIterator::_impl::_impl : Can not open file " + filename);
  }

  // private constructors

  inline FileLinesInputIterator::FileLinesInputIterator() throw() : p(0) {}

  inline FileLinesInputIterator::FileLinesInputIterator(const std::string& filename) throw(Error_FileLines::open_file, std::ios_base::failure, std::bad_alloc) : p(new _impl(filename)) {
    if (p -> s.peek() == std::char_traits<char>::eof())
      p.release();
  }

  // copy constructor and assignments

  inline FileLinesInputIterator::FileLinesInputIterator(const FileLinesInputIterator& it) throw() : p(it.p) {}
  // fake "const" (but it seems necessary (??))

  inline FileLinesInputIterator& FileLinesInputIterator::operator = (FileLinesInputIterator& it) throw() {
      p = it.p;
      return *this;
  }

  // comparisons

  inline bool FileLinesInputIterator::operator == (const FileLinesInputIterator& it) const throw() {
    if (p.get() == 0 and it.p.get() == 0)
      return true;
    if (p.get() != 0 and it.p.get() != 0)
      return p -> current_line == it.p -> current_line;
    if (p.get() == 0) {
      it.update();
      return it.p.get() == 0;
    }
    else {
      update();
      return p.get() == 0;
    }
  }

  inline bool FileLinesInputIterator::operator != (const FileLinesInputIterator& it) const throw() {
    return not (*this == it);
  }

  // update

  inline void FileLinesInputIterator::update() const throw(Error_FileLines::read_file, std::ios_base::failure) {

    std::ifstream& s(p -> s);
    const std::streamsize& current(p -> current_line);
    const std::streamsize& currentm1(current - 1);
    std::streamsize& last(p -> last_line_read);
    const std::streamsize diff = currentm1 - last;

    if (diff == -1)
      return;
    line_advance(s, diff);
    if (s.bad() or (s.fail() and not s.eof()))
      throw Error_FileLines::read_file("StreamHandling::FileLinesInputIterator::update : Error when reading the stream");
    if (s.eof() or s.peek() == std::char_traits<char>::eof())
      p.release();
    else
      last = currentm1;
  }

  // dereference

  inline FileLinesInputIterator::value_type FileLinesInputIterator:: operator *() const throw(Error_FileLines::read_file, std::ios_base::failure) {

    const std::streamsize& current(p -> current_line);
    std::streamsize& last(p -> last_line_read);
    std::ifstream& s(p -> s);
    std::string& l(p -> line);

    if (current == last)
      return l;
    update();
    getline(s, l);
    if (s.bad() or (s.fail() and not (s.eof())))
      throw Error_FileLines::read_file("StreamHandling::FileLinesInputIterator::operator * : Error when reading the stream");
    last = current;
    return l;
  }

  // pointer arithmetic

  inline FileLinesInputIterator& FileLinesInputIterator::operator++() throw(Error_FileLines::read_file, std::ios_base::failure) {

    ++(p -> current_line);
    update();
    return *this;
  }

  inline FileLinesInputIterator&  FileLinesInputIterator::operator+(unsigned long int n) throw() {
    p -> current_line += n;
    return *this;
  }
}


// -----------------------------
// Implementation of class TempFile
// -----------------------------

namespace StreamHandling {

  inline TempFile::TempFile() throw(Error_TempFile::open_file, std::ios_base::failure) : temp_name(const_cast<const char*>(std::tmpnam(0))), file(temp_name.c_str(), std::ios::in | std::ios::out | std::ios::trunc) {
    if (! file)
      throw Error_TempFile::open_file("StreamHandling::TempFile::TempFile : Can not open file " + temp_name);
  }

  inline TempFile::TempFile(const std::string& name) throw(Error_TempFile::open_file, std::ios_base::failure) : temp_name(name.c_str()), file(temp_name.c_str(), std::ios::in | std::ios::out | std::ios::trunc) {
    if (! file)
      throw Error_TempFile::open_file("StreamHandling::TempFile::TempFile : Can not open file " + temp_name);
  }

  inline TempFile::operator std::fstream&() throw() {
    return file;
  }
  inline std::fstream& TempFile::f() throw() {
    return file;
  }

  inline TempFile::~TempFile() throw(Error_TempFile::remove_file) {
    if (std::remove(temp_name.c_str()))
      throw Error_TempFile::remove_file("StreamHandling::TempFile::TempFile : Can not remove file " + temp_name);
  }

}



#endif
