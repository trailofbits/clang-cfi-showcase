# Clang CFI Showcase

[Control flow integrity](https://www.microsoft.com/en-us/research/publication/control-flow-integrity/) is an exploit mitigation, like stack cookies, DEP, and ASLR. Like other exploit mitigations, the goal of CFI is to prevent bugs from turning into exploits. CFI works by reducing the ability of an attacker to redirect program execution to an attacker controlled destination.

We have created samples with specially crafted bugs to showcase clang's control flow integrity implementation. These examples are an accompaniment to [our blog post describing CFI in clang](http://blog-post-URL).

All of the samples are designed to compile **cleanly** with the absolute maximum warning level* (`-Weverything`).

The bugs in these examples are not statically identified by the compiler, but are detected at runtime via CFI. Where possible, we simulate potential malicious behavior that occurs without CFI protections. 

Each example builds two binaries, one with CFI protection (e.g. `cfi_icall`) and one without CFI protections (e.g. `no_cfi_icall`).

*Ok, we lied, we had to disable two warnings, one about C++98 compatibility, and one about virtual functions being defined inline. The point is still valid since those warnings do not relate to potential bugs.

# CFI Examples

* **cfi_icall** demonstrates control flow integrity of indirect calls. The example binary accepts a single command line argument (valid values are 0-3, but try invalid values with both binaries!). The command line argument shows different aspects of indirect call CFI protection, or lack thereof. 
* **cfi_vcall** shows an example of CFI applied to virtual function calls. This example demonstrates how CFI would protect against a type confusion or similar attack.
* **cfi_nvcall** shows clang’s protections for calling non-virtual member functions via something that is not an object that has those functions defined.
* **cfi_unrelated_cast** shows how clang can prevent casts between objects of unrelated types.
* **cfi_derived_cast** expands on cfi_unrelated_cast and shows how clang can prevent casts from an object of a base class to an object of a derived class, if the object is not actually of the derived class.
* **cfi_cast_strict** is a stricter version of cfi_derived_cast that may be incompatible with some applications.

# Requirements

These examples assume a Linux build environment with clang-3.9 and the GNU gold linker.

They should be portable to other operating systems; the only strict requirements are clang 3.7+ and an LTO capable linker.
