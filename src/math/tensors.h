#ifndef TENSORS_H
#define TENSORS_H

#include <vector>
#include <iostream>
#include <map>
#include <cmath>

namespace neml {

// Forward declarations
class Vector;
class RankTwo;
class Symmetric;
class Skew;
class RankFour;
class SymSym;
class SymSkew;
class SkewSym;

class Tensor {
 public:
  Tensor(std::size_t n);
  Tensor(const Tensor & other);
  Tensor(Tensor && other);
  Tensor(const std::vector<double> flat);
  Tensor(double * flat, size_t n);
  Tensor(const double * flat, size_t n);
  virtual ~Tensor();
  
  /// Do I own my own data?
  bool istore() const {return istore_;};

  Tensor & operator=(const Tensor & rhs);
  Tensor & operator=(Tensor && rhs);

  const double * data() const {return s_;};
  double * s() {return s_;};
  std::size_t n() const {return n_;};

  void copy_data(const double * const indata);

  Tensor & operator*=(double s);
  Tensor & operator/=(double s);

 protected:
  /// Helper to add data
  void add_(const Tensor & other);
  
  /// Helper to negate
  void negate_();

 protected:
  double * s_;
  const std::size_t n_;
  bool istore_;
};

/// Dangerous but useful
bool operator==(const Tensor & a, const Tensor & b);
bool operator!=(const Tensor & a, const Tensor & b);

class Vector: public Tensor {
 public:
  Vector();
  Vector(const std::vector<double> v);
  Vector(double * v);
  Vector(const double * v);

  Vector opposite() const;
  Vector operator-() const;

  Vector & operator+=(const Vector & other);
  Vector & operator-=(const Vector & other);

  double & operator()(size_t i);
  const double & operator()(size_t i) const;

  double dot(const Vector & rhs) const;
  RankTwo outer(const Vector & o) const;

  double norm() const;
  Vector cross(const Vector & other) const;
  Vector & normalize();
};

// Binary operators with scalars
Vector operator*(double s, const Vector & v);
Vector operator*(const Vector & v, double s);
Vector operator/(const Vector & v, double s);

// Binary operators with vectors
Vector operator+(const Vector & a, const Vector & b);
Vector operator-(const Vector & a, const Vector & b);

/// io for vectors
std::ostream & operator<<(std::ostream & os, const Vector & v);

// Produces tensors
RankTwo outer(const Vector & a, const Vector & b);

/// Full Rank 2 tensor
class RankTwo: public Tensor {
 public:
  RankTwo();
  RankTwo(const std::vector<double> v);
  RankTwo(double * v);
  RankTwo(const double * v);
  RankTwo(const std::vector<std::vector<double>> A);
  /// Helper
  RankTwo(const Symmetric & other);
  RankTwo(const Skew & other);

  RankTwo opposite() const;
  RankTwo operator-() const;

  RankTwo & operator+=(const RankTwo & other);
  RankTwo & operator-=(const RankTwo & other);

  RankTwo & operator+=(const Symmetric & other);
  RankTwo & operator-=(const Symmetric & other);

  RankTwo & operator+=(const Skew & other);
  RankTwo & operator-=(const Skew & other);

  double & operator()(size_t i, size_t j);
  const double & operator()(size_t i, size_t j) const;

  RankTwo dot(const RankTwo & other) const;
  RankTwo dot(const Symmetric & other) const;
  RankTwo dot(const Skew & other) const;
  Vector dot(const Vector & other) const;

  RankTwo inverse() const;
  RankTwo transpose() const;

  double contract(const RankTwo & other) const;
  double contract(const Symmetric & other) const;
  double contract(const Skew & other) const;
};

// Binary operators with scalars
RankTwo operator*(double s, const RankTwo & v);
RankTwo operator*(const RankTwo & v, double s);
RankTwo operator/(const RankTwo & v, double s);

// Various forms of addition
RankTwo operator+(const RankTwo & a, const RankTwo & b);
RankTwo operator-(const RankTwo & a, const RankTwo & b);
RankTwo operator+(const RankTwo & a, const Symmetric & b);
RankTwo operator-(const RankTwo & a, const Symmetric & b);
RankTwo operator+(const Symmetric & a, const RankTwo & b);
RankTwo operator-(const Symmetric & a, const RankTwo & b);
RankTwo operator+(const RankTwo & a, const Skew & b);
RankTwo operator-(const RankTwo & a, const Skew & b);
RankTwo operator+(const Skew & a, const RankTwo & b);
RankTwo operator-(const Skew & a, const RankTwo & b);

// Various forms of multiplication
Vector operator*(const RankTwo & a, const Vector & b);
Vector operator*(const Vector & a, const RankTwo & b);
RankTwo operator*(const RankTwo & a, const RankTwo & b);
RankTwo operator*(const RankTwo & a, const Symmetric & b);
RankTwo operator*(const Symmetric & a, const RankTwo & b);
RankTwo operator*(const RankTwo & a, const Skew & b);
RankTwo operator*(const Skew & a, const RankTwo & b);
RankTwo operator*(const Skew & a, const Symmetric & b);
RankTwo operator*(const Symmetric & a, const Skew & b);

/// io for RankTwo tensors
std::ostream & operator<<(std::ostream & os, const RankTwo & v);

/// Symmetric Mandel rank 2 tensor
class Symmetric: public Tensor {
 public:
  Symmetric();
  Symmetric(const std::vector<double> v);
  Symmetric(double * v);
  Symmetric(const double * v);
  /// I guess take them seriously and symmetrize it
  Symmetric(const RankTwo & other);

  RankTwo to_full() const;

  Symmetric opposite() const;
  Symmetric operator-() const;

  Symmetric & operator+=(const Symmetric & other);
  Symmetric & operator-=(const Symmetric & other);
  
  static Symmetric id() { return
    Symmetric(std::vector<double>({1,1,1,0,0,0}));};
  Symmetric inverse() const;
  Symmetric transpose() const;

  double trace() const;
  Symmetric skew() const;

  // Various multiplications
  Vector dot(const Vector & other) const;
  Symmetric dot(const Symmetric & other) const;
  RankTwo dot(const RankTwo & other) const;
  RankTwo dot(const Skew & other) const;

  double contract(const RankTwo & other) const;
  double contract(const Symmetric & other) const;
  double contract(const Skew & other) const;
};

// Binary operators with scalars
Symmetric operator*(double s, const Symmetric & v);
Symmetric operator*(const Symmetric & v, double s);
Symmetric operator/(const Symmetric & v, double s);

// Various forms of addition
Symmetric operator+(const Symmetric & a, const Symmetric & b);
Symmetric operator-(const Symmetric & a, const Symmetric & b);

// Various forms of multiplication
Vector operator*(const Symmetric & a, const Vector & b);
Vector operator*(const Vector & a, const Symmetric & b);
Symmetric operator*(const Symmetric & a, const Symmetric & b);

/// io for symmetric tensors
std::ostream & operator<<(std::ostream & os, const Symmetric & v);

class Skew: public Tensor {
 public:
  Skew();
  Skew(const std::vector<double> v);
  Skew(double * v);
  Skew(const double * v);
  /// Skew a general tensor
  Skew(const RankTwo & other);

  RankTwo to_full() const;

  Skew opposite() const;
  Skew operator-() const;

  Skew & operator+=(const Skew & other);
  Skew & operator-=(const Skew & other);

  Skew transpose() const;

  // Various multiplication
  Vector dot(const Vector & other) const;
  Skew dot(const Skew & other) const;
  RankTwo dot(const RankTwo & other) const;
  RankTwo dot(const Symmetric & other) const;

  double contract(const RankTwo & other) const;
  double contract(const Symmetric & other) const;
  double contract(const Skew & other) const;
};

// Binary operators with scalars
Skew operator*(double s, const Skew & v);
Skew operator*(const Skew & v, double s);
Skew operator/(const Skew & v, double s);

// Various forms of addition
Skew operator+(const Skew & a, const Skew & b);
Skew operator-(const Skew & a, const Skew & b);

// Various forms of multiplication
Vector operator*(const Skew & a, const Vector & b);
Vector operator*(const Vector & a, const Skew & b);
Skew operator*(const Skew & a, const Skew & b);

/// io for skew tensors
std::ostream & operator<<(std::ostream & os, const Skew & v);

class RankFour: public Tensor {
 public:
  RankFour();
  RankFour(const std::vector<double> v);
  RankFour(const std::vector<std::vector<std::vector<std::vector<double>>>> A);
  RankFour(double * v);
  RankFour(const double * v);

  RankFour opposite() const;
  RankFour operator-() const;

  RankFour & operator+=(const RankFour & other);
  RankFour & operator-=(const RankFour & other);

  double & operator()(size_t i, size_t j, size_t k, size_t l);
  const double & operator()(size_t i, size_t j, size_t k, size_t l) const;

  SymSym to_sym() const;
  SymSkew to_symskew() const;
  SkewSym to_skewsym() const;

  // Various multiplications
  RankFour dot(const RankFour & other) const;
  RankFour dot(const SymSym & other) const;
  RankFour dot(const SymSkew & other) const;
  RankFour dot(const SkewSym & other) const;

  RankTwo dot(const RankTwo & other) const;
  RankTwo dot(const Symmetric & other) const;
  RankTwo dot(const Skew & other) const;
};

// Binary operators with scalars
RankFour operator*(double s, const RankFour & v);
RankFour operator*(const RankFour & v, double s);
RankFour operator/(const RankFour & v, double s);

// Various forms of addition
RankFour operator+(const RankFour & a, const RankFour & b);
RankFour operator-(const RankFour & a, const RankFour & b);

// Various forms of multiplication
RankFour operator*(const RankFour & a, const RankFour & b);
RankFour operator*(const RankFour & a, const SymSym & b);
RankFour operator*(const RankFour & a, const SymSkew & b);
RankFour operator*(const RankFour & a, const SkewSym & b);

RankTwo operator*(const RankFour & a, const RankTwo & b);
RankTwo operator*(const RankFour & a, const Symmetric & b);
RankTwo operator*(const RankFour & a, const Skew & b);

/// io for SymSym tensors
std::ostream & operator<<(std::ostream & os, const RankFour & v);

class SymSym: public Tensor {
 public:
  SymSym();
  SymSym(const std::vector<double> v);
  SymSym(const std::vector<std::vector<double>> A);
  SymSym(double * v);
  SymSym(const double * v);

  SymSym opposite() const;
  SymSym operator-() const;

  SymSym & operator+=(const SymSym & other);
  SymSym & operator-=(const SymSym & other);

  RankFour to_full() const;

  double & operator()(size_t i, size_t j);
  const double & operator()(size_t i, size_t j) const;

  // Various multiplication
  RankFour dot(const RankFour & other) const;
  SymSym dot(const SymSym & other) const;
  RankFour dot(const SymSkew & other) const;
  RankFour dot(const SkewSym & other) const;

  RankTwo dot(const RankTwo & other) const;
  RankTwo dot(const Skew & other) const;
  Symmetric dot(const Symmetric & other) const;
};

// Binary operators with scalars
SymSym operator*(double s, const SymSym & v);
SymSym operator*(const SymSym & v, double s);
SymSym operator/(const SymSym & v, double s);

// Various forms of addition
SymSym operator+(const SymSym & a, const SymSym & b);
SymSym operator-(const SymSym & a, const SymSym & b);

// Various forms of multiplication
RankFour operator*(const SymSym & a, const RankFour & b);
SymSym operator*(const SymSym & a, const SymSym & b);
RankFour operator*(const SymSym & a, const SymSkew & b);
RankFour operator*(const SymSym & a, const SkewSym & b);

RankTwo operator*(const SymSym & a, const RankTwo & b);
RankTwo operator*(const SymSym & a, const Skew & b);
Symmetric operator*(const SymSym & a, const Symmetric & b);

/// D1_ij D2_kl
SymSym douter(const Symmetric & a, const Symmetric & b);

/// io for SymSym tensors
std::ostream & operator<<(std::ostream & os, const SymSym & v);

class SymSkew: public Tensor {
 public:
  SymSkew();
  SymSkew(const std::vector<double> v);
  SymSkew(const std::vector<std::vector<double>> A);
  SymSkew(double * v);
  SymSkew(const double * v);

  SymSkew opposite() const;
  SymSkew operator-() const;

  SymSkew & operator+=(const SymSkew & other);
  SymSkew & operator-=(const SymSkew & other);

  RankFour to_full() const;

  double & operator()(size_t i, size_t j);
  const double & operator()(size_t i, size_t j) const;

  // Various multiplication
  RankFour dot(const RankFour & other) const;
  RankFour dot(const SymSym & other) const;
  RankFour dot(const SymSkew & other) const;
  RankFour dot(const SkewSym & other) const;

  RankTwo dot(const RankTwo & other) const;
  RankTwo dot(const Skew & other) const;
  RankTwo dot(const Symmetric & other) const;
};

// Binary operators with scalars
SymSkew operator*(double s, const SymSkew & v);
SymSkew operator*(const SymSkew & v, double s);
SymSkew operator/(const SymSkew & v, double s);

// Various forms of addition
SymSkew operator+(const SymSkew & a, const SymSkew & b);
SymSkew operator-(const SymSkew & a, const SymSkew & b);

// Various forms of multiplication
RankFour operator*(const SymSkew & a, const RankFour & b);
RankFour operator*(const SymSkew & a, const SymSym & b);
RankFour operator*(const SymSkew & a, const SymSkew & b);
RankFour operator*(const SymSkew & a, const SkewSym & b);

RankTwo operator*(const SymSkew & a, const RankTwo & b);
RankTwo operator*(const SymSkew & a, const Skew & b);
RankTwo operator*(const SymSkew & a, const Symmetric & b);

/// io for SymSkew tensors
std::ostream & operator<<(std::ostream & os, const SymSkew & v);

class SkewSym: public Tensor {
 public:
  SkewSym();
  SkewSym(const std::vector<double> v);
  SkewSym(const std::vector<std::vector<double>> A);
  SkewSym(double * v);
  SkewSym(const double * v);

  SkewSym opposite() const;
  SkewSym operator-() const;

  SkewSym & operator+=(const SkewSym & other);
  SkewSym & operator-=(const SkewSym & other);

  RankFour to_full() const;

  double & operator()(size_t i, size_t j);
  const double & operator()(size_t i, size_t j) const;

  // Various multiplication
  RankFour dot(const RankFour & other) const;
  RankFour dot(const SymSym & other) const;
  RankFour dot(const SkewSym & other) const;
  RankFour dot(const SymSkew & other) const;

  RankTwo dot(const RankTwo & other) const;
  RankTwo dot(const Skew & other) const;
  RankTwo dot(const Symmetric & other) const;
};

// Binary operators with scalars
SkewSym operator*(double s, const SkewSym & v);
SkewSym operator*(const SkewSym & v, double s);
SkewSym operator/(const SkewSym & v, double s);

// Various forms of addition
SkewSym operator+(const SkewSym & a, const SkewSym & b);
SkewSym operator-(const SkewSym & a, const SkewSym & b);

// Various forms of multiplication
RankFour operator*(const SkewSym & a, const RankFour & b);
RankFour operator*(const SkewSym & a, const SymSym & b);
RankFour operator*(const SkewSym & a, const SkewSym & b);
RankFour operator*(const SkewSym & a, const SymSkew & b);

RankTwo operator*(const SkewSym & a, const RankTwo & b);
RankTwo operator*(const SkewSym & a, const Skew & b);
RankTwo operator*(const SkewSym & a, const Symmetric & b);

// ij kl
SkewSym douter(const Skew & a, const Symmetric & b);

/// io for SkewSym tensors
std::ostream & operator<<(std::ostream & os, const SkewSym & v);


// Helper CP methods
/// Skmab*Wml - Wkm*Smlab
SymSym SymSymSkew_SkewSymSym(const SymSym & S, const Skew & W);

/// Dkm*Smlab - Skmab*Dml
SymSym SymSkewSym_SkewSymSym(const SkewSym & S, const Symmetric & D);

/// Specialty operator for Skew part C_ijkb e_ka - C_ijal e_bl
SymSkew SpecialSymSymSym(const SymSym & S, const Symmetric & D);

} // namespace neml

#endif
