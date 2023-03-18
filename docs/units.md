# ![Q-Logo](assets/images/q-logo-small.png) Audio DSP Library

# Units

## Table of Contents
- [Table of Contents](#table-of-contents)
- [Value](#value)
  - [Expressions](#expressions)
- [Value Models](#value-models)
  - [frequency](#frequency)
  - [duration](#duration)
  - [period](#period)
  - [phase](#phase)
  - [basic\_interval](#basic_interval)
- [NonLinearValue](#nonlinearvalue)
  - [Expressions](#expressions-5)
- [NonLinearValue Models](#nonlinearvalue-models)
  - [decibel](#decibel)
- [note](#note)

## Value

A representation of a linear scalar value as a concept.

### Expressions

#### Notation

| Symbol       | Description              |
| ------------ | ------------------------ |
| `V`          | Value type               |
| `v`          | Primitive scalar value.  |
| `a`, `b`     | Instances of `Value`.    |

#### Constructors and assignment

| Expression   |  Semantics                                                      |
| ------------ | --------------------------------------------------------------- |
| `V{}`        |  Empty brace initialization. Value initialize a `Value`         |
| `V()`        |  Default construction. Default construct initialize a `Value`.  |
| `V{v}`       |  Brace initialization from primitive scalar value `v`.          |
| `V(v)`       |  Construct a `Value` from primitive scalar value `v`.           |
| `V{a}`       |  Copy brace initialization from `Value` `a`.                    |
| `V(a)`       |  Copy construct from `Value` `a`.                               |
| `a = b`      |  Assignment. Assign `Value` `b`, to `Value` `a`.                |

#### Comparison

| Expression   |  Semantics                                    | Return Type     |
| ------------ | ----------------------------------------------|---------------- |
| `a == b`     | Equality.                                     | `bool`          |
| `a == v`     | Equality with a primitive scalar.             | `bool`          |
| `v == b`     | Equality with a primitive scalar.             | `bool`          |
| `a != b`     | Non-Equality.                                 | `bool`          |
| `a != v`     | Non-Equality with a primitive scalar.         | `bool`          |
| `v != b`     | Non-Equality with a primitive scalar.         | `bool`          |
| `a < b`      | Less than.                                    | `bool`          |
| `a < v`      | Less than with a primitive scalar.            | `bool`          |
| `v < b`      | Less than with a primitive scalar.            | `bool`          |
| `a <= b`     | Less than equal.                              | `bool`          |
| `a <= v`     | Less than equal with a primitive scalar.      | `bool`          |
| `v <= b`     | Less than equal with a primitive scalar.      | `bool`          |
| `a > b`      | Greater than.                                 | `bool`          |
| `a > v`      | Greater than with a primitive scalar.         | `bool`          |
| `v > b`      | Greater than with a primitive scalar.         | `bool`          |
| `a >= b`     | Greater than equal.                           | `bool`          |
| `a >= v`     | Greater than equal with a primitive scalar.   | `bool`          |
| `v >= b`     | Greater than equal with a primitive scalar.   | `bool`          |

#### Arithmetic

| Expression   |  Semantics                                    | Return Type     |
| ------------ | ----------------------------------------------|---------------- |
| `+a`         | Positive.                                     | `Value`         |
| `-a`         | Negative.                                     | `Value`         |
| `a += b`     | Add assign.                                   | `Value&`        |
| `a -= b`     | Subtract assign.                              | `Value&`        |
| `a *= b`     | Multiply assign.                              | `Value&`        |
| `a /= b`     | Divide assign.                                | `Value&`        |
| `a + b`      | Addition.                                     | `Value`         |
| `a + v`      | Addition with a scalar.                       | `Value`         |
| `v + b`      | Addition with a scalar.                       | `Value`         |
| `a - b`      | Subtraction.                                  | `Value`         |
| `a - v`      | Subtraction with a scalar.                    | `Value`         |
| `v - b`      | Subtraction with a scalar.                    | `Value`         |
| `a * b`      | Multiplication.                               | `Value`         |
| `a * v`      | Multiplication with a scalar.                 | `Value`         |
| `v * b`      | Multiplication with a scalar.                 | `Value`         |
| `a / b`      | Division.                                     | `Value`         |
| `a / v`      | Division with a scalar.                       | `Value`         |
| `v / b`      | Division with a scalar.                       | `Value`         |

## Value Models

### frequency

Type safe representation of frequency in Hertz.

```c++
struct frequency : value<double, frequency>
{
   constexpr                     frequency(double val);
   constexpr                     frequency(duration d);

   constexpr explicit operator   double() const ;
   constexpr explicit operator   float() const;
   constexpr q::period           period() const;
};
```

#### Expressions

In addition to valid expressions for Value, `frequency` allows these
expressions.

#### Notation

| `d`       | Instance of `duration` (see below.)  |
| `f`       | Instance of `frequency`              |

#### Construction

```c++
// Construct a phase given the period (duration)
phase{ d }
```

#### Conversions

```c++
float(f)       // Convert frequency to a scalar (float)
double(f)      // Convert frequency to a scalar (double)
```

#### Misc

```c++
f.period()     // get the period (1/f)
```

### duration

Type safe representation of duration.

```c++
struct duration : value<double, duration>
{
   constexpr                     duration(double val);

   constexpr explicit operator   double() const;
   constexpr explicit operator   float() const;
};
```

#### Expressions

In addition to valid expressions for Value, `duration` allows these
expressions.

#### Notation

| `d`    | Instance of `duration`            |

#### Conversions

```c++
float(d)       // Convert duration to a scalar (float)
double(d)      // Convert duration to a scalar (double)
```

### period

Type safe representation of period (reciprocal of frequency).

```c++
struct period : duration
{
   using duration::duration;

   constexpr                     period(duration d);
   constexpr                     period(frequency f);
};
```

#### Expressions

In addition to valid expressions for Value, `period` allows these
expressions.

#### Notation

| `d`       | Instance of `duration`      |
| `f`       | Instance of `frequency`     |
| `p`       | Instance of `period`        |

#### Construction

```c++
// Construct a phase given a duration
phase{ d }

// Construct a phase given a frequency
phase{ f }
```

#### Conversions

```c++
float(p)       // Convert period to a scalar (float)
double(p)      // Convert period to a scalar (double)
```

### phase

phase: The synthesizers use fixed point 1.31 format computations where 31
bits are fractional. phase represents phase values that run from 0 to
4294967295 (0 to 2π) suitable for oscillators.

The turn, also cycle, full circle, revolution, and rotation, is a complete
circular movement or measure (as to return to the same point) with circle or
ellipse. A turn is abbreviated τ, cyc, rev, or rot depending on the
application. The symbol τ can also be used as a mathematical constant to
represent 2π radians.

[https://en.wikipedia.org/wiki/Angular_unit](https://en.wikipedia.org/wiki/Angular_unit)

```c++
struct phase : _unspecified_base_type_
{
   using base_type = value<std::uint32_t, phase>;
   using base_type::base_type;

   constexpr static auto one_cyc = int_max<std::uint32_t>();
   constexpr static auto bits = sizeof(std::uint32_t) * 8;

   constexpr explicit            phase(value_type val = 0);
   constexpr explicit            phase(float frac);
   constexpr explicit            phase(double frac);
   constexpr explicit            phase(long double frac);
   constexpr                     phase(frequency freq, float sps);

   constexpr static phase        begin();
   constexpr static phase        end();
   constexpr static phase        middle();
};

// Free functions
constexpr double  as_double(phase d);
constexpr float   as_float(phase d);
```

#### Expressions

In addition to valid expressions for Value, `phase` allows these
expressions.

#### Notation

| Symbol       | Description              |
| ------------ | ------------------------ |
| `f`          | A `double` or `float`    |
| `freq`       | Instance of `frequency`  |
| `sps`        | Scalar value             |
| `p`          | Instance of `phase`      |

#### Construction

```c++
// Construct a phase given the a fractional number from 0.0 to 1.0 (0 to 2π)
phase{ f }

// Construct a phase given the frequency and samples/second (`sps`)
phase{ freq, sps }
```

#### Conversions

```c++
as_float(p)    // Convert a phase to a scalar (float)
as_double(p)   // Convert a phase to a scalar (double)
```

#### Min and Max

```c++
phase::begin()    // Get the minimum phase representing 0 degrees
phase::end()      // Get the maximum phase representing 360 degrees (2π)
phase::middle()   // Get the phase representing 180 degrees (π)
```

### basic_interval

## NonLinearValue

A representation of a non-linear scalar value as a concept. These values, unlike linear values, have restricted operations. Non-linear scalar values do not permit comparison with bare scalars (such as `float`), nor do they permit addition and subtraction with bare scalars.

### Expressions

#### Notation

| Symbol       | Description                    |
| ------------ | ------------------------------ |
| `V`          | NonLinearValue type            |
| `v`          | Scalar value.                  |
| `a`, `b`     | Instance of `NonLinearValue`   |

#### Constructors and assignment

```c++
// Default constructor [1].
V{}
V()

// Constructor [2].
V{v}
V(v)

// Copy constructor. [3]
V{a}
V(a)

// Assignment [4]
a = b
```

#### Semantics

1. Default construct a `NonLinearValue` with initial value `{ 0 }`
2. Construct a `NonLinearValue` given initial value `v`.
3. Copy construct a `NonLinearValue`, `a`.
4. Assign `b`, to `a`.

#### Comparison

```c++
a == b      // Equality
a != b      // Non-equality
a < b       // Less than
a <= b      // Less than equal
a > b       // Greater than
a >= b      // Greater than equal
```

#### Arithmetic
```c++
+a          // Positive
-a          // Negative

a += b      // Add assign
a -= b      // Subtract assign
a *= b      // Multiply assign
a /= b      // Divide assign

a + b       // Addition
a - b       // Subtraction

a * b       // Multiplication
a * v       // Multiplication with a scalar
v * b       // Multiplication with a scalar

a / b       // Division
a / v       // Division with a scalar
```

## NonLinearValue Models

### decibel

Decibel is non-linear and operates on the logarithmic domain. The `decibel` class is perfectly suitable for dynamics processing (e.g. compressors and limiters and envelopes). Q provides fast `decibel` computations using fast math functions and lookup tables for converting to and from scalars.

```c++
struct decibel
{
   constexpr            decibel();
   explicit             decibel(double val);

   constexpr decibel    operator+() const;
   constexpr decibel    operator-() const;

   constexpr decibel&   operator+=(decibel b);
   constexpr decibel&   operator-=(decibel b);
   constexpr decibel&   operator*=(decibel b);
   constexpr decibel&   operator/=(decibel b);

   double rep = 0.0f;
};

// Free functions
constexpr double  as_double(decibel db);
constexpr float   as_float(decibel db);

constexpr decibel operator-(decibel a, decibel b);
constexpr decibel operator+(decibel a, decibel b);

constexpr decibel operator*(decibel a, decibel b);
constexpr decibel operator*(decibel a, double b);
constexpr decibel operator*(decibel a, float b);
constexpr decibel operator*(decibel a, int b);
constexpr decibel operator*(double a, decibel b);
constexpr decibel operator*(float a, decibel b);
constexpr decibel operator*(int a, decibel b);

constexpr decibel operator/(decibel a, decibel b);
constexpr decibel operator/(decibel a, double b);
constexpr decibel operator/(decibel a, float b);
constexpr decibel operator/(decibel a, int b);

constexpr bool    operator==(decibel a, decibel b);
constexpr bool    operator!=(decibel a, decibel b);
constexpr bool    operator<(decibel a, decibel b);
constexpr bool    operator<=(decibel a, decibel b);
constexpr bool    operator>(decibel a, decibel b);
constexpr bool    operator>=(decibel a, decibel b);
```

#### Expressions

In addition to valid expressions for `NonLinearValue`, `decibel` allows these expressions.

#### Notation

| Symbol       | Description                    |
| ------------ | ------------------------------ |
| `a`          | Instance of `decibel`          |

#### Conversions

```c++
as_float(a)    // Convert a decibel to a scalar (float)
as_double(a)   // Convert a decibel to a scalar (double)
```

## note

There is also a complete set of tables for notes from `A[0]` (27.5Hz) to
`Ab[9]` (13289.75Hz). For example, to get the frequencies for each of the
open strings in a 6-string guitar:

```c++
// 6 string guitar frequencies:
constexpr auto low_e   = E[2];
constexpr auto a       = A[2];
constexpr auto d       = D[3];
constexpr auto g       = G[3];
constexpr auto b       = B[3];
constexpr auto high_e  = E[4];
```

To use these literals, include the `notes.hpp` header:

```c++
#include <q/support/notes.hpp>
```

then use the `notes` namespace somewhere in a scope where you need it:

```c++
using namespace q::notes;
```




