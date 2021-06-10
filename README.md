# Ranluxpp - portable (not using assemly language) standalone version of Ranlux++ implemented in C

This is an improved version of the RANLUX generator, called Ranlux++ (or RANLUXPP) as originally proposed by Alexei Sibidanov.

There are two papers on it:
* [A revision of the subtract-with-borrow random number generators](https://arxiv.org/pdf/1705.03123.pdf)
* [Review of High-Quality Random Number Generators](https://arxiv.org/pdf/1903.01247.pdf)

The [original RANLUX RNG](https://www.math.utah.edu/software/gsl/gsl-ref_252.html) is a well-established generator in particle physics for Monte Carlo simulations. It's theoretically backed up by the theory of dynamical systems. The original RANLUX's main drawback is that it is very very slow. The reason is that many values (typically 389) were computed but skipped. The new advancement is that RANLUX is equivalent to LCG with constant `A` having 576 bits: X_{i + 1} = A * X_{i} mod m. 

This can be efficiently computed using long arithmetic with the help of SSE or AVX2 instructions. More importantly, skipping can be easily realized in one step - one just needs to precompute `A^skip mod m`. This new version is called Ranlux++ or RANLUXPP. With these improvements, RANLUX++ is running at speed 1.4 GiB/s on my laptop with Core i7-8650U CPU @ 1.90GHz. The original implementation using assembly by Alexei Sibidanov can be checked [here](https://github.com/sibidanov/ranluxpp).

The Alexei Sibidanov's implementation relies heavily on assembly language to implement multiplication of two integers with length of 576 bits each and is avaialble only for X86+64 CPUs with AVX2 support. 

The next development is a portable (not using assembly language) implementation of Ranlux++ for the [ROOT-Computing library developed by CERN](https://en.wikipedia.org/wiki/ROOT). This is advancement was presented by Jonas Hahnfeld and Lorenzo Moneta [in this article.](https://arxiv.org/abs/2106.02504)  The portable implementation reaches performance very close to the original assembler version.

My contribution is a standalone version of Ranlux++ written in C. It's combines [Alexei Sibidanov's code](https://github.com/sibidanov/ranluxpp) with the [implementation for ROOT](https://github.com/root-project/root/tree/master/math/mathcore/src/ranluxpp) converted from C++ to C.
