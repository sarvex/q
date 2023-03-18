# Fundamentals

## Table of Contents
- [Table of Contents](#table-of-contents)
- [File Structure](#file-structure)
- [Namespace](#namespace)
- [Function Objects](#function-objects)
- [Data Types](#data-types)
- [Literals](#literals)


## File Structure

The library is organized with this simplified directory structure:

* **docs**: Where this documentation resides.
* **example**: Self-contained and easy to understand c++ programs that demonstrate various features of the library.
* **q_io**:
   * **external**: 3rd party libraries used by the `q_io` module.
   * **include**: q_io header files.
   * **src**: q_io source files.
* **q_lib**:
   * include: Header-only core q_lib DSP library.
* **test**: Contains a comprehensive set of c++ files for testing the library.

Sub-directories not listed here can be ignored.

## Namespace

All entities in the Q library are placed in namespace `cycfi::q`. Everywhere
in this documentation, we will be using a namespace alias to make the code
less verbose:

```c++
namespace q = cycfi::q;
```

## Function Objects

In the realm of electronic music, there are *processors* and *synthesizers*, whose definitions overlap somewhat and differ primarily in how they handle input and output. The processor receives one or more input samples and generates one or more output samples according to a specific processing algorithm. A synthesizer, on the other hand, generates sound from scratch without using any samples.

In the Q world, both *processors* and *synthesizers* are just *function objects* — C++ function objects, which are basic building blocks that can be composed to form more complex functions. A function object can accept zero or more input values and generate zero or more output values.

Syntactically, you can use these functions just like any other function. For a single input function object, for example:

```c++
float r = f(s);
```
where `s` is the input value, and `f(s)` returns a result and stores it in the variable `r`.

## Data Types

Typical audio processors in the Q DSP library work on floating point input samples with the normal -1.0 to 1.0 range. However, values are not limited to sampled signals. For instance, signal envelopes are best represented as decibels that are processed in the logarithmic domain, so dynamic-range processors such as compressors and expanders accept `decibel` as input and return `decibel` as output. For example:

```c++
decibel gain = comp(env);
```

Synthesizer oscillators, as another example, operate on phase-angle inputs and return output samples:

```c++
float out = sin(phase++);
```

The Q DSP library has a rich set of such types:

* `float` and `double`: Typical sample data type -1.0 to 1.0.
* `frequency`: Cycles per second (Hz).
* `duration` : A time span (seconds, milliseconds, etc.)
* `period` : The inverse of frequency
* `phase`: Fixed point 1.31 format where 31 bits are fractional. `phase`
  represents 0 to 2π phase values suitable for oscillators.
* `decibel`: Ratio of one value to another on a logarithmic scale (dB)

The Q DSP library is typeful and typesafe. You can not mismatch values of different types such as `frequency` and `decibel`, for example. Such potentially disastrous mistakes can happen if all values are just raw floating point types.

There are conversions to and from these data types where it is reasonable to do so. 'decibel' can, for example, be converted to 'float' or 'double' using the 'as float' or 'as double' conversion functions.

Relational operations are allowed. For example:

```c++
if (gain > 3_dB) // 3_dB is a decibel literal (see below)
   s = lp(s);
```

Arithmetic operations are allowed. For example:
```c++
auto  = 3_ms + 5_ms; // 3_ms and 5_ms are duration literals (see below)
```

Where appropriate, arithmetic with raw types are allowed. For example:

```c++
auto harmonic = 440_Hz * 4; // 440_Hz is a frequency literal (see below)
```

## Literals

To augment the wealth of value types, the Q DSP library makes abundant use of
[C++ user-defined literals][1].

We take advantage of C++ (from c++11) type safe [user-defined literals][1], instead of the usual `int`, `float` or `double` which can be unsafe when values from different units (e.g. frequency vs. duration) are mismatched. The library makes abundant use of user-defined literals for units such as time, frequency and decibels (e.g. 24_dB, instead of a unit-less 24 or worse,
a non-intuitive, unit-less 15.8 —the gain equivalent of 24_dB). Such constants also make the code very readable, another objective of this library.

Q Literals are placed in the namespace `q::literals`. The namespace is sparse enough to be hoisted into your namespace using `using namespace`.

To use these literals, include the `literals.hpp` header:

```c++
#include <q/support/literals.hpp>
```

then use the `literals` namespace somewhere in a scope where you need it:

```c++
using namespace q::literals;
```

Here are some example expressions:

#### Frequencies

```c++
82.4069_Hz
440_Hz
1.5_KHz
1.5_kHz
1_KHz
1_kHz
0.5_MHz
3_MHz
```

#### Durations

```c++
10.3_s
1_s
20.5_ms
1_ms
10.5_us
500_us
```

#### Decibels

```c++
-3.5_dB
10_dB
```

#### Pi

```c++
2_pi
0.5_pi
```

[1]: http://tinyurl.com/yafvvb6b