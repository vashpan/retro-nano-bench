# Retro Nano Benchmark

CPU benchmark aimed at maximum portability across multitple platforms and independence on any system-specific code.

You may ask: **why?**

I was wondering - how fast current computers are? How long a 386 would work to finish the job that modern Ryzen, Intel Core or Apple Silicon can make in seconds?

Properly measuring performance is not a simple task, there's a lot of factors. Modern CPUs are very different than old ones, even if they have the same architecture. We have many cores, powerful floating-point units, SIMD instructions, vectorization - and more.

To fairly compare processors accross different eras - we need to find some common denominator - ALU. This unit is still crucial part of the CPU, and it is shared even with the oldest CPUs.

## Approach

Benchmark is written in portable C, using C89 standard, which should be compiled even with the older compilers.

Benchmark code is not using any standard library functions. It's more fair this way, as standard library implementations or optimizations vary from platform to platform. It would also allow to port it to platforms without C library, like microcontrollers.

To compare performance in a more sensible way, possibly oldest version of compiler for given platform is preffered to be used for that purpuse. For example: a MS VC++ 2.0 from 1994 can generate code that still runs on modern version of Windows. This way we can compare a broad spectrum of CPUs: from 386 (minimum requirements for early Windows NTs) up to most modern x86 CPUs that are still capable of running 32 bit programs. It'll run the same code on all those CPUs. Of course we will loose some degree of optimizations for modern CPUs but at least modern compiler optimizations won't affect the results.

Most sensible results could be get if we use the same binary and OS across multiple CPUs. Comparing accross platforms and compilers could tell us some story, but remember it won't be 100% accurate.

## Porting

I want Retro Nano Benchmark to be my "toy project" for various development platforms - both modern and retro. For this purpuse, I want the most "native" ports for many platforms, that would provide actual use and could be used as tests for streaming or retro videos. For now only simply barebone text interface for POSIX systems, DOS & Windows is available.

## Results

I plan to create a dedicated repository/website that will host results. For now you can submit me your results via [Twitter](https://twitter.com/vashpan) or [Mastodon](https://mastodon.social/@kkolakowski). You can also post them here, or send me an e-mail.



Submit following informations: 

CPU type
L1, L2, L3 cache (if possible)
Machine type (PC, Microcontroller, SBC...)
Operating system (with version)
Compiler (with version)
Results (all benchmarks + total score)

## Benchmarks

Don't expect anything complex, they are mostly here just to make "some" non-trivial work done by the CPU. Score is calculated against i486DX4 CPU running on Windows NT 3.5.

### Random numbers

It's using classic linear-congruential pseudorandom number generator. It's code is also used in some other benchmarks to generate data.

### Word count

Simple algorithm to calculate number of words in a given string.

### CRC32 checksum

Another classic - a 32bit checksum for given data, using this implementation: http://home.thep.lu.se/~bjorn/crc/

### RLE compression algorithm

Very simple, basic compression algorithm

### QuickSort sorting

And yet another immortal classic. Sorting using quick sort algorithm.