# Clang CFI Showcase

[Control flow integrity](https://www.microsoft.com/en-us/research/publication/control-flow-integrity/) is an exploit mitigation, like stack cookies, DEP, and ASLR. Like other exploit mitigations, the goal of CFI is to prevent bugs from turning into exploits. CFI works by reducing the ability of an attacker to redirect program execution to an attacker controlled destination.

We have created samples with specially crafted bugs to showcase clang's control flow integrity implementation. These examples are an accompaniment to [our blog post describing CFI in clang](https://blog.trailofbits.com/2016/10/17/lets-talk-about-cfi-clang-edition/).

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
* **cfi_cast_strict** showcases the very specific instance where the default level of base-to-derived cast protection, like in cfi_derived_cast, would not catch an illegal cast.

# Requirements

These examples assume a Linux build environment with clang-3.9 and the GNU gold linker.

They should be portable to other operating systems; the only strict requirements are clang 3.7+ and an LTO capable linker.

## Installing clang 3.9 on Linux

The following commands should install clang 3.9 on Ubuntu-based Linux distriutions.

    UBUNTU_RELEASE=`lsb_release -sc`
    sudo apt-get install -y software-properties-common build-essential
    wget -qO - http://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
    sudo add-apt-repository -y "deb http://apt.llvm.org/${UBUNTU_RELEASE}/ llvm-toolchain-${UBUNTU_RELEASE}-3.9 main"
    sudo apt-get update
    sudo apt-get install -y clang-3.9

## Installing clang 3.9 on MacOS
The following instructions work to install clang 3.9 on MacOS 10.11 (El Capitan) and MacOS 10.12 (Sierra). These instructions are adapted from [this stack overflow post](https://github.com/explosion/spaCy/issues/267).

    curl -O http://llvm.org/releases/3.9.0/clang+llvm-3.9.0-x86_64-apple-darwin.tar.xz
    tar xJf clang+llvm-3.9.0-x86_64-apple-darwin.tar.xz
    sudo mkdir -p /opt
    sudo mv clang+llvm-3.9.0-x86_64-apple-darwin /opt/llvm39

    export CC=/opt/llvm39/bin/clang
    export CXX=/opt/llvm39/bin/clang++
    export PATH=/opt/llvm39/bin:$PATH
    export C_INCLUDE_PATH=/opt/llvm39/include:$C_INCLUDE_PATH
    export CPLUS_INCLUDE_PATH=/opt/llvm39/include:$CPLUS_INCLUDE_PATH
    export LIBRARY_PATH=/opt/llvm39/lib:$LIBRARY_PATH
    export DYLD_LIBRARY_PATH=/opt/llvm39/lib:$DYLD_LIBRARY_PATH

Makefile changes are needed to build these samples on MacOS:

* The `CC` and `CXX` variables should say `clang`, and not `clang-3.9`
* The default linker on MacOS supports LTO; the `-B${GOLD}` flag should be removed.

After these changes, the header of the Makefile should look similar to the following:

    CXX = /opt/llvm39/bin/clang++
    CC = /opt/llvm39/bin/clang
    CFLAGS = -Weverything -Werror -pedantic -std=c99 -O0 -fvisibility=hidden -flto -fno-sanitize-trap=all
    CXXFLAGS = -Weverything -Werror -pedantic -Wno-c++98-compat -Wno-weak-vtables -std=c++11 -O0 -fvisibility=hidden -flto -fno-sanitize-trap=all
