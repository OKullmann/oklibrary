// Oliver Kullmann, 3.7.2002 (Swansea)

#include <string>
#include <algorithm>
#include <memory>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <fstream>
#include <sstream>
#include <ostream>
#include <limits>
#include <cmath>
#include <cassert>
#include <numeric>

#include <boost/test/floating_point_comparison.hpp>

#include <Transitional/General/DatabaseHandler01.hpp>
#include <Transitional/General/TimeHandling.hpp>
#include <Transitional/General/Kommandozeile.hpp>
#include <Transitional/General/Algorithms.hpp>
#include <Transitional/General/ErrorHandling.hpp>

using namespace std;

using namespace DatabaseHandler01;
using namespace StringHandling;
using namespace TimeHandling;

// Messages -----------------------------------

char *Selbst = "CreateStatistic";

const int number_languages = 2;
KMZ::Sprachen language = KMZ::Englisch;

const char * const Version = "0.75";
const char * const Datum = "30.7.2002";
const char * const Autor = "Oliver Kullmann (Swansea); O.Kullmann@Swansea.ac.uk";
const char * const Uebersetzungsdatum = __DATE__ " " __TIME__;

const char * const Meldungen[][number_languages] = {
  {"Einlesen der Tabelle bcls", // 0
   "Reading of table bcls"},
  {"Zeilen gelesen: ", // 1
   "Rows read: "},
  {"Geschaetzte Restzeit fuer diese Tabelle: ", // 2
   "Estimated remaining time for this table: "},
  {"Bestimmung des maximalen Indexes von cls_id in Tabelle bcls.", // 3
   "Computation of the maximal index of cls_id in table bcls."},
  {"Bestimmung des maximalen Indexes von b_info_id in Tabelle bcls_info.", // 4
   "Computation of the maximal index of b_info_id in table bcls_info."},
  {"Einlesen der Tabelle bcls_info", // 5
   "Reading of table bcls_info"},
  {"Einlesen der Tabelle b_unit_reductions", // 6
   "Reading of table b_unit_reductions"},
  {"Der maximale index ist ", // 7
   "The maximal index is "},
  {"Berechnung der statistischen Werte.", // 8
   "Computation of statistical values."},
  {"Die gesamte Berechnung dauerte", // 9
   "The whole computation needed"},
  {"FEHLER in", // 10
   "ERROR in"},
  {"Erzeugte Dateien:", // 11
   "Files created:"},
  {"Erzeugte Tabellen:", // 12
   "Tables created:"},
  {"Erfolgreiche Beendung um", // 13
   "Successful completion at"},
  {"Begin um", // 14
   "Start at"},
  {"Berechnung der Mittelwerte fuer die erfuellbaren bzw. unerfuellbaren Klauselmengen.", // 15
   "Computation of mean values for satisfiable resp. unsatisfiable formulas."},
  {"Berechnung der Wahrscheinlichkeit von Erfuellbarkeit.", // 16
   "Computation of probability of satisfiability."},
  {"Hinzuziehung der Tabelle SatUnsatAggregates fuer Wahrscheinlichkeit von Erfuellbarkeit.", // 17
    "Additional reading of table SatUnsatAggregates for probability of satisfiability."},
  {"Neusortierung der Matrizen.", // 18
   "New sorting of the matrices."},
  {"Dateien-Ausgabe", // 19
   "File output"},
  {"Tabellen in der Datenbank aktualisieren.", // 20
   "Updating of tables in database."},
  {"(Nochmaliges) Einlesen der Tabelle bcls_info zur Berechnung des Medians der Knotenanzahl.", // 21
   "(Second) reading of table bcls_info to compute the median of the node numbers."},
  {"Platz reservieren fuer die Berechnung des Medians der Knoten-Anzahlen.", // 22
   "Reserving space for the computation of the median values for the node counts."},
  {"Bereitstellung des Speicherplatzes fuer den Index der Tabelle bcls_info.", // 23
   "Preparation of space for the index of table bcsl_info."},
};


// global constants -------------------------------------

const string database = "OKRandGen";
const string SQL_true = "t";

const string dim = "1";
const unsigned int p = 3; // can be other values
const string length = "{" + toString(p) + "}";
// given dim and length, the frame_id is uniquely determined

const string table_densities = "bd_" + dim;

// output files

const string directory = "/h/21/GemeinsameBasis";

const string extension = ".csv";

const string output_sat = directory + "/" + "OKgp" + toString(p) + "alleS" + extension;
const string output_unsat = directory + "/" + "OKgp" + toString(p) + "alleU" + extension;
const string output_stat_sat = directory + "/" + "OKgp" + toString(p) + "prob" + extension;

// output tables

const string output_table_sat = "statsatp" + toString(p);
const string output_table_unsat = "statunsatp" + toString(p);
const string output_table_prob = "probsatp" + toString(p);

// formatting data

const string separator = ",";

const string null_sql = "NULL";
const string null_r = "NA";

// minimal resolution complexity for unsatisfiable clause-sets

inline unsigned int small_2_pow(unsigned int e) {
  return 1U << e;
}
const unsigned int min_complexity = small_2_pow(p-1) - 1;

// buffer sizes

const int cursor_step_bcls = 20000000;
const int cursor_step_bcls_info = 10000000;
const int cursor_step_b_unit_reductions = 20000000;
const int cursor_step_intervalls = 20000000;
const int cursor_step_bcls_info_median = 20000000;

// watching the progress

const int progress_step = 1000000;


// querying the database -----------

DatabaseHandler DH(database);
Select Sel(DH);
Command Comm(DH);


// computation of maximal indices
class Max_index {
public :
  Max_index(const string& field, const string& table) {
    Sel.issue("select max(" + field + ") from " + table + ";");
    ++Sel;
    max = fromString<int>(Sel.value("max"));
  }
  operator int() const { return max; }
private :
  int max;
};


// computation of the (unique) frame_id -----------------------

class Frame_id {
public :
  struct no_frame_id {};
  Frame_id() {
    TableHandler TFrames(DH, "frames");
    AllAttObj AFrames(TFrames);
    AFrames("l") << length;
    AFrames("dim") << dim; // for checking
    if (! TFrames.completion())
      throw no_frame_id();
    fi = Value(AFrames("frame_id"));
    nfi = fromString<int>(fi);
  }
  operator string() const { return fi; }
  operator int() const { return nfi; }
private :
  string fi;
  int nfi;
};

// density handling -------------------------------------------

struct density {
  bool fi; // whether equal to the unique frame identity or not
  string d1; // not as double to avoid rounding problems
  density(bool f, const string& d)
    : fi(f), d1(d) {}
  density() {}
};

class Densities {
  // the map from m_dens_id to densities
public :
  Densities(const string& fi) {
    v.resize(Max_index("m_dens_id", table_densities) + 1);
    Sel.issue("select m_dens_id, frame_id, d1 from " + table_densities + ";");
    while (++Sel) {
      v[fromString<int>(Sel.value("m_dens_id"))] = density((string(Sel.value("frame_id")) == fi), Sel.value("d1"));
    }
  }
  const density& operator [] (int index) const {
    return v[index];
  }
private :
  vector<density> v;
};

// handling of statistical values ----------------------------------------

const string header_common = "n,d1,nds,count,depth,pl,aut,sn,qsn,l2r,an,countwa,ndswa,countws,ndsws,countwq,ndswq,maxnds,minnds,stdnds,mednds";
const string header_unsat =  header_common + ",prm";
const string header_sat = header_common + ",trv,nb,nl2r,ndswb";

const string header_prob_sat = "n,d1,sat,c";

class activate_median;

class stat_values {
public :

  stat_values()
    : nds(0), count(1), depth(0), pl(0), aut(0), sn(0), qsn(0), l2r(0), an(0),  countwa(0), ndswa(0), countws(0), ndsws(0), countwq(0), ndswq(0), maxnds(0), minnds(numeric_limits<double>::max()), stdnds(0), sum_sq_nds(0), vecndsp(0) {}
  virtual ~stat_values() {}

  virtual void enter_bcls_data(const Select& S) { ++count; }

  virtual void enter_bcls_info_data(const Select& S) {
    current_nds = fromString<double>(S.value("nodes"));
    nds += current_nds;
    current_depth = fromString<double>(S.value("tree_depth"));
    depth += current_depth;
    pl += fromString<double>(S.value("pure_literals"));
    const double current_aut = fromString<double>(S.value("real_autarkies"));
    aut += current_aut;
    const double current_sn = fromString<double>(S.value("single_nodes"));
    sn += current_sn;
    const double current_qsn = fromString<double>(S.value("quasi_s_nodes"));
    qsn += current_qsn;
    an += fromString<double>(S.value("an"));
    if (current_aut != 0) {
      ++countwa;
      ndswa += current_nds;
    }
    if (current_sn != 0) {
      ++countws;
      ndsws += current_nds;
    }
    if (current_qsn != 0) {
      ++countwq;
      ndswq += current_nds;
    }
    maxnds = max(maxnds, current_nds);
    minnds = min(minnds, current_nds);
    sum_sq_nds += current_nds * current_nds;
  }

  virtual void enter_b_unit_reductions_data(const Select& S) {
    current_l2r = fromString<double>(S.value("u_count"));
    l2r += current_l2r;
  }

  virtual void enter_bcls_info_mean_data(const Select& S) {
    vecndsp -> push_back(fromString<double>(S.value("nodes")));
  }

  virtual void compute_mean() {
    // must be called only once
    if (count != 1)
      stdnds = sqrt((count * sum_sq_nds - nds * nds) / count / (count - 1));
    nds /= count; depth /= count; pl /= count; aut /= count; sn /= count; qsn /= count; l2r /= count; an /= count;
    if (countwa != 0)
      ndswa /= countwa;
    if (countwq != 0)
      ndswq /= countwq;
    const std::vector<double> medianassert(*vecndsp);
    std::vector<double> medianasserttemp(*vecndsp);
    mednds = Algorithms::median_with_sorting(vecndsp -> begin(), vecndsp -> end());
    assert(mednds == Algorithms::median_with_sorting(medianasserttemp.begin(), medianasserttemp.end()));
    std::vector<double> medianassert2(*vecndsp);
    {
      assert(count == vecndsp -> size());
      assert(boost::test_tools::close_at_tolerance<double>(boost::test_tools::fraction_tolerance(10 * std::numeric_limits<double>::epsilon()))(nds, std::accumulate(vecndsp -> begin(), vecndsp -> end(), double(0)) / count));
      std::sort(vecndsp -> begin(), vecndsp -> end());
      const vector<double>::size_type size = vecndsp -> size();
      const vector<double>::size_type middle = size / 2;
      const double median2 = (size % 2 == 0) ? ((*vecndsp)[middle-1] + (*vecndsp)[middle]) / 2.0 : (*vecndsp)[middle];
      if (not boost::test_tools::close_at_tolerance<double>(boost::test_tools::fraction_tolerance(5 * std::numeric_limits<double>::epsilon()))(mednds, median2)) {
	std::cerr << "FEHLER!\ncount = " << count << ", mednds = " << mednds << ", median2 = " << median2 << "\n sortierter Vektor =\n";
	std::copy(vecndsp -> begin(), vecndsp -> end(), std::ostream_iterator<double>(std::cerr, " "));
	std::cerr << "\noriginaler Vektor =\n";
	std::copy(medianassert.begin(), medianassert.end(), std::ostream_iterator<double>(std::cerr, " "));
	std::cerr << "\nund nach Median-Berechnung:\n";
	std::copy(medianassert2.begin(), medianassert2.end(), std::ostream_iterator<double>(std::cerr, " "));
	std::cerr << "\n";
	assert(boost::test_tools::close_at_tolerance<double>(boost::test_tools::fraction_tolerance(5 * std::numeric_limits<double>::epsilon()))(mednds, median2));
      }
    }
    delete vecndsp;
    vecndsp = 0;
  }

  virtual void output(ostream& f, const string& null, const string& sep) const {
    f << nds << sep << count << sep << depth << sep << pl << sep << aut << sep << sn << sep << qsn << sep << l2r << sep << an;
    f << sep << countwa << sep;
    if (countwa == 0)
      f << null;
    else
      f << ndswa;
    f << sep << countws << sep;
    if (countws == 0)
      f << null;
    else
      f << ndsws;
    f << sep << countwq << sep;
    if (countwq == 0)
      f << null;
    else
      f << ndswq;
    f << sep << maxnds << sep << minnds << sep;
    if (count != 1)
      f << stdnds;
    else
      f << null;
    f << sep << mednds;
  }

private :
  friend class SAT_values;
  friend class UNSAT_values;
  friend class update_sat;

  double nds, count, depth, pl, aut, sn, qsn, l2r, an;
  double countwa, ndswa, countws, ndsws, countwq, ndswq;
  double maxnds, minnds, stdnds, sum_sq_nds;

  vector<double>* vecndsp;
  double mednds;
  friend class activate_median;

  static double current_nds;
  static double current_depth;
  static double current_l2r;
};

double stat_values::current_nds;
double stat_values::current_depth;
double stat_values::current_l2r;

class SAT_values : public stat_values {
public :

  SAT_values()
  : stat_values(), trv(0), nb(0), nl2r(0), ndswb(0) {}

  virtual void enter_bcls_info_data(const Select& S) {
    stat_values::enter_bcls_info_data(S);
    trv += (current_nds == 1);
    if (current_nds == current_depth + 1)
      ++nb;
    else
      ndswb += current_nds;
    
  }

  virtual void enter_b_unit_reductions_data(const Select& S) {
    stat_values::enter_b_unit_reductions_data(S);
    nl2r += (current_l2r == 0);
  }

  virtual void compute_mean() {
    stat_values::compute_mean();
    if (double count_wb = (count - nb))
      ndswb /= count_wb;
    trv /= count; nb /= count; nl2r /= count;
    
  }

  virtual void output(ostream& f, const string& null = null_r, const string& sep = separator) const {
    stat_values::output(f, null, sep);
    f << sep << trv << sep << nb << sep << nl2r << sep << ndswb;
  }

private :
  double trv, nb, nl2r, ndswb;
};

class UNSAT_values : public stat_values {
public :

  UNSAT_values()
    : stat_values(), prm(0) {}

  virtual void enter_bcls_info_data(const Select& S) {
    stat_values::enter_bcls_info_data(S);
    prm += (current_nds == min_complexity);
  }
  
  virtual void compute_mean() {
    stat_values::compute_mean();
    prm /= count;
  }

  virtual void output(ostream& f, const string& null = null_r, const string& sep = separator) const {
    stat_values::output(f, null, sep);
    f << sep << prm;
  }

private :
  double prm;
};

class prob_sat {
public:

  prob_sat()
    : total(0), sat(0) {}

  void update_sat(const double m) {
    total += m;
    sat += m;
  }

  void update_unsat(const double m) {
    total += m;
  }

  void output(ostream& f, const string& null = null_r, const string& sep = separator) const {
    f << sat / total << sep << total;
  }

private :
  double total, sat;
};


// the "matrices" containing all statistics -------------------------

struct Matrix_key {
  int m_dens_id;
  int n;
};
inline bool operator < (const Matrix_key& k1, const Matrix_key& k2) {
  return (k1.m_dens_id < k2.m_dens_id) or (k1.m_dens_id == k2.m_dens_id and k1.n < k2.n);
}
inline bool operator == (const Matrix_key& k1, const Matrix_key& k2) {
  return (k1.m_dens_id == k2.m_dens_id) and (k1.n == k2.n);
}
inline bool operator != (const Matrix_key& k1, const Matrix_key& k2) {
  return !(k1 == k2);
}
class readable_sorting {
public :
  readable_sorting(const Densities& D) : Den(D) {}
  bool operator () (const Matrix_key& k1, const Matrix_key& k2) {
    return (k1.n < k2.n) or (k1.n == k2.n and fromString<double>(Den[k1.m_dens_id].d1) < fromString<double>(Den[k2.m_dens_id].d1));
  }
private :
  const Densities& Den;
};

typedef map<Matrix_key, UNSAT_values> UNSAT_matrices;
typedef map<Matrix_key, SAT_values> SAT_matrices;
typedef map<Matrix_key, prob_sat, readable_sorting> prob_matrices;

UNSAT_matrices UNSAT_matrix;
SAT_matrices SAT_matrix;

// preparing for the median computation

struct activate_median {
  template <class T> void operator() (pair<const Matrix_key, T>& x) const {
    x.second.vecndsp = new vector<double>;
    x.second.vecndsp -> reserve(int(x.second.count));
  }
};

// processing (after completed collection from the database)

struct mean {
  template <class T> void operator() (pair<const Matrix_key, T>& x) const {
    x.second.compute_mean();
  }
};

class update_sat {
public :
  update_sat(prob_matrices& pm) : prob_matrix(pm) {}
  template <class T> void operator() (const pair<const Matrix_key, T>& x) const;
private :
  prob_matrices& prob_matrix;
};
template <> inline void update_sat::operator() (const pair<const Matrix_key, UNSAT_values>& x) const {
    prob_matrix[x.first].update_unsat(x.second.count);
  }
template <> inline void update_sat::operator() (const pair<const Matrix_key, SAT_values>& x) const {
    prob_matrix[x.first].update_sat(x.second.count);
  }


// the output functions --------------------------------


template <class Transformer, class Matrix>
inline void file_output(const string& filename, const string& header, const Matrix& M, const Densities& D) {
  ofstream out(filename.c_str());
  out << header << "\n";
  for_each(M.begin(), M.end(), Transformer(out, D));
}

template <class Transformer, class Matrix>
inline void table_output(const string& tablename, const string& header, const Matrix& M, const Densities& D) {
  const string query_0 = "insert into " + tablename + "(" + header + ") values ";
  ostringstream s;
  const Transformer T(s, D, false);
  for (typename Matrix::const_iterator i = M.begin(); i != M.end(); ++i) {
#ifdef DEBUG
    cout << "(1) s = " << s.str() << ";";
#endif
    s.str("");
    T(*i);
#ifdef DEBUG
    cout << " (2) s = " << s.str() << ";";
#endif
    Comm.issue(query_0 + s.str() + ";");
#ifdef DEBUG
    cout << "Comm.issue(query_0 + s.str() + \";\"); erfolgreich" << endl;
#endif
  }
}


// internal output facilities (the "Transformers")

class output_line {
  // line of values

public :
  output_line(ostream& f, const Densities& D, const bool fR = true) : out(f), Den(D), for_R(fR) {}
  template <class T> void operator() (const pair<const Matrix_key, T>& x) const {
    if (for_R) {
      out << x.first.n << separator << Den[x.first.m_dens_id].d1 << separator;
      x.second.output(out);
      out << "\n";
    }
    else { // the value list for the insert command
      out << "(" << x.first.n << "," << Den[x.first.m_dens_id].d1 << ",";
      x.second.output(out, null_sql);
      out << ")";
    }
  }

#ifdef DEBUG
  ~output_line() {
    cout << "Destruktor output_line" << endl;
  }
#endif

private :
  ostream& out;
  const Densities& Den;
  const bool for_R;
};

class output_prob_sat_line {
  // line of values

public :
  output_prob_sat_line(ostream& f, const Densities& D, const bool fR = true) : out(f), Den(D), for_R(fR) {}
  void operator() (const pair<const Matrix_key, prob_sat>& x) const {
    if (for_R) {
      out << x.first.n << separator << Den[x.first.m_dens_id].d1 << separator;
      x.second.output(out);
      out << "\n";
    }
    else {
      out << "(" << x.first.n << "," << Den[x.first.m_dens_id].d1 << ",";
      x.second.output(out, null_sql);
      out << ")";
    }
  }

private :
  ostream& out;
  const Densities& Den;
  const bool for_R;
};



// Indices --------------

typedef vector<stat_values*> Matrix_indices;


// main program --------------------------------------------

int main() {

  try {

    cout << "[" << Selbst << "] " << Meldungen[14][language] << " " << currentDateTime() << endl;

    WallTime Total_time;

    const Frame_id frame_id;
    const Densities densities(frame_id);

    cout << Meldungen[3][language] << endl;
    const Max_index max_cls_id("cls_id", "bcls");
    cout << Meldungen[7][language] << max_cls_id << endl;
    Matrix_indices* cls_id_index_p = new Matrix_indices(max_cls_id+1);

    { // Reading from bcls
      cout << Meldungen[0][language] << endl;
      Progress P(cout, max_cls_id, progress_step, Meldungen[1][language], Meldungen[2][language]);
      Sel.set_cursor_step(cursor_step_bcls);
      Sel.issue("select cls_id, n, m_dens_id, dim, sat from bcls", true);
      while (++Sel) {
	P();
	if (string(Sel.value("dim")) != dim)
	  continue;
	const int m_dens_id = fromString<int>(Sel.value("m_dens_id"));
	if (! densities[m_dens_id].fi)
	  continue;
	const int n = fromString<int>(Sel.value("n"));
	const bool sat = string(Sel.value("sat")) == SQL_true;
	const int cls_id = fromString<int>(Sel.value("cls_id"));
	const Matrix_key key = {m_dens_id, n};
	if (sat) {
	  SAT_matrices::iterator i = SAT_matrix.lower_bound(key);
	  if (i == SAT_matrix.end() or i -> first != key)
	    i = SAT_matrix.insert(i, make_pair(key, SAT_values()));
	  else
	    i -> second.enter_bcls_data(Sel);
	  (*cls_id_index_p)[cls_id] = &(i -> second);
	}
	else {
	  UNSAT_matrices::iterator i = UNSAT_matrix.lower_bound(key);
	  if (i == UNSAT_matrix.end() or i -> first != key)
	    i = UNSAT_matrix.insert(i, make_pair(key, UNSAT_values()));
	  else
	    i -> second.enter_bcls_data(Sel);
	  (*cls_id_index_p)[cls_id] = &(i -> second);
	}
      }
      Sel.truncate_result();
      cout << endl;
    }
    
    cout << Meldungen[4][language] << endl;
    const Max_index max_b_info_id("b_info_id", "bcls_info");
    cout << Meldungen[7][language] << max_b_info_id << endl;
    cout << Meldungen[23][language] << endl;
    Matrix_indices* b_info_id_index_p = new Matrix_indices(max_b_info_id+1);
    
    { // Reading from bcls_info
      cout << Meldungen[5][language] << endl;
      Progress P(cout, max_b_info_id, progress_step, Meldungen[1][language], Meldungen[2][language]);
      Sel.set_cursor_step(cursor_step_bcls_info);
      Sel.issue("select b_info_id, cls_id, tree_depth, pure_literals, real_autarkies, nodes, single_nodes, quasi_s_nodes, an from bcls_info", true);
      while (++Sel) {
	P();
	const int cls_id = fromString<int>(Sel.value("cls_id"));
#ifdef DEBUG
	if (cls_id > max_cls_id)
	  cout << "Fehler: cls_id = " << cls_id << ", max_cls_id = " << max_cls_id << endl;
#endif
	stat_values* const p = (*cls_id_index_p)[cls_id];
	if (! p)
	  continue;
	const int b_info_id = fromString<int>(Sel.value("b_info_id"));
#ifdef DEBUG
	if (b_info_id > max_b_info_id)
	  cout << "Fehler: b_info_id = " << b_info_id << ", max_b_info_id = " << max_b_info_id << endl;
#endif
	(*b_info_id_index_p)[b_info_id] = p;
	p -> enter_bcls_info_data(Sel);
      }
      Sel.truncate_result();
      cout << endl;
    }

    delete cls_id_index_p;

    { // Reading from b_unit_reductions
      cout << Meldungen[6][language] << endl;
      cout << Meldungen[7][language] << max_b_info_id << endl;
      Progress P(cout, max_b_info_id, progress_step, Meldungen[1][language], Meldungen[2][language]);
      Sel.set_cursor_step(cursor_step_b_unit_reductions);
      Sel.issue("select * from b_unit_reductions", true);
      while (++Sel) {
	P();
	const int b_info_id = fromString<int>(Sel.value("b_info_id"));
	stat_values* const p = (*b_info_id_index_p)[b_info_id];
	if (! p or string(Sel.value("u_level")) != "2")
	  continue;
	p -> enter_b_unit_reductions_data(Sel);
      }
      Sel.truncate_result();
      cout << endl;
    }

    { // reading from bcls_info to compute median(nds)
      cout << Meldungen[22][language] << endl;
      for_each(UNSAT_matrix.begin(), UNSAT_matrix.end(), activate_median());
      for_each(SAT_matrix.begin(), SAT_matrix.end(), activate_median());
      cout << Meldungen[21][language] << endl;
      cout << Meldungen[7][language] << max_b_info_id << endl;
      Progress P(cout, max_b_info_id, progress_step, Meldungen[1][language], Meldungen[2][language]);
      Sel.set_cursor_step(cursor_step_bcls_info_median);
      Sel.issue("select b_info_id, nodes from bcls_info", true);
      while (++Sel) {
	P();
	const int b_info_id = fromString<int>(Sel.value("b_info_id"));
	stat_values* const p = (*b_info_id_index_p)[b_info_id];
	if (! p)
	  continue;
	p -> enter_bcls_info_mean_data(Sel);
      }
      Sel.truncate_result();
      cout << endl;
    }

    delete b_info_id_index_p;
    
    // computation of the mean values

    cout << Meldungen[8][language] << endl;
    cout << Meldungen[15][language] << endl;
    for_each(UNSAT_matrix.begin(), UNSAT_matrix.end(), mean());
    for_each(SAT_matrix.begin(), SAT_matrix.end(), mean());
    
    // computation of sat-statistics
    cout << Meldungen[16][language] << endl;

    prob_matrices prob_matrix((readable_sorting(densities)));

    for_each(UNSAT_matrix.begin(), UNSAT_matrix.end(), update_sat(prob_matrix));
    for_each(SAT_matrix.begin(), SAT_matrix.end(), update_sat(prob_matrix));

    {
      cout << Meldungen[17][language] << endl;
      Sel.set_cursor_step(cursor_step_intervalls);
      Sel.issue("select n, m_dens_id, dim, count_all, count_sat, stored_sat, stored_unsat from SatUnsatAggregates;");
      while (++Sel) {
	if (string(Sel.value("dim")) != dim)
	  continue;
	const int m_dens_id = fromString<int>(Sel.value("m_dens_id"));
	if (! densities[m_dens_id].fi)
	  continue;
	const int n = fromString<int>(Sel.value("n"));
	const Matrix_key key = {m_dens_id, n};
	const double count_all = fromString<double>(Sel.value("count_all"));
	const double count_sat = fromString<double>(Sel.value("count_sat"));
	const double count_unsat = count_all - count_sat;
	if (string(Sel.value("stored_sat")) != SQL_true)
	  prob_matrix[key].update_sat(count_sat);
	if (string(Sel.value("stored_unsat")) != SQL_true)
	  prob_matrix[key].update_unsat(count_unsat);
      }
    }

    // now the computations are finished, and we can write to the files

    // new sorting of the matrices for SAT and UNSAT
    cout << Meldungen[18][language] << endl;
    map<Matrix_key, UNSAT_values, readable_sorting> UNSAT_matrix_n(UNSAT_matrix.begin(), UNSAT_matrix.end(), readable_sorting(densities));
    UNSAT_matrix.clear();
    map<Matrix_key, SAT_values, readable_sorting> SAT_matrix_n(SAT_matrix.begin(), SAT_matrix.end(), readable_sorting(densities));
    SAT_matrix.clear();
      
    // file output
    cout << Meldungen[19][language] << endl;
    file_output<output_line>(output_unsat, header_unsat, UNSAT_matrix_n, densities);
    file_output<output_line>(output_sat, header_sat, SAT_matrix_n, densities);
    file_output<output_prob_sat_line>(output_stat_sat, header_prob_sat, prob_matrix, densities);      

    // table output
    cout << Meldungen[20][language] << endl;
    Comm.issue("truncate table " + output_table_unsat + ";");
    table_output<output_line>(output_table_unsat, header_unsat, UNSAT_matrix_n, densities);
#ifdef DEBUG
    cout << "UNSAT" << endl;
#endif
    Comm.issue("truncate table " + output_table_sat + ";");
    table_output<output_line>(output_table_sat, header_sat, SAT_matrix_n, densities);
#ifdef DEBUG
    cout << "SAT" << endl;
#endif
    Comm.issue("truncate table " + output_table_prob + ";");
    table_output<output_prob_sat_line>(output_table_prob, header_prob_sat, prob_matrix, densities);     

    // final messages

    cout << Meldungen[11][language] << " " << output_unsat << " " << output_sat << " " << output_stat_sat << endl;
    cout << Meldungen[12][language] << " " << output_table_unsat << " " << output_table_sat << " " << output_table_prob << endl;
    cout << Meldungen[9][language] << " " << Total_time / 60 << " m" << endl;
    cout << "[" << Selbst << "] " << Meldungen[13][language] << " " << currentDateTime() << endl;
  }

  catch (const ErrorHandling::Error& e) {
    cerr << ErrorHandling::Error2string(e) << endl;
    return 1;
  }
  catch (const std::exception& e) {
    cerr << ErrorHandling::Error2string(e) << endl;
    return 1;
  }
  catch (...) {
    cerr << "Unknown exception!\n";
    return 1;
  }
 
  return 0;
}


