CXX = clang++
CC = clang
GOLD = $(shell pwd)/ld
CFLAGS = -B${GOLD} -Weverything -Werror -pedantic -std=c99 -O0 -fvisibility=hidden -flto -fno-sanitize-trap=all 
CXXFLAGS = -B${GOLD} -Weverything -Werror -pedantic -Wno-c++98-compat -Wno-weak-vtables -std=c++11 -O0 -fvisibility=hidden -flto -fno-sanitize-trap=all 

CFI_TARGETS = check_clang_version cfi_icall cfi_vcall cfi_nvcall cfi_unrelated_cast cfi_derived_cast cfi_cast_strict
NO_CFI_TARGETS = $(addprefix no_, $(CFI_TARGETS))

TARGETS = $(CFI_TARGETS) $(NO_CFI_TARGETS)

all: $(TARGETS)

check_clang_version:
	dpkg --compare-versions "$(shell ${CC} -dumpversion)" "ge" "3.9"

cfi_icall: cfi_icall.c
	@echo Compiling $< to $@
	@$(CC) $(CFLAGS) -fsanitize=cfi-icall -o $@ $<

no_cfi_icall: cfi_icall.c
	@echo Compiling $< to $@
	@$(CC) $(CFLAGS) -o $@ $<

cfi_vcall: cfi_vcall.cpp
	@echo Compiling $< to $@
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-vcall -o $@ $<

no_cfi_vcall: cfi_vcall.cpp
	@echo Compiling $< to $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

cfi_nvcall: cfi_nvcall.cpp
	@echo Compiling $< to $@
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-nvcall -o $@ $<

no_cfi_nvcall: cfi_nvcall.cpp
	@echo Compiling $< to $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

cfi_unrelated_cast: cfi_unrelated_cast.cpp
	@echo Compiling $< to $@
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-unrelated-cast -o $@ $<

no_cfi_unrelated_cast: cfi_unrelated_cast.cpp
	@echo Compiling $< to $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

cfi_derived_cast: cfi_derived_cast.cpp
	@echo Compiling $< to $@
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-derived-cast -o $@ $<

no_cfi_derived_cast: cfi_derived_cast.cpp
	@echo Compiling $< to $@
	@$(CXX) $(CXXFLAGS) -o $@ $<

cfi_cast_strict: cfi_cast_strict.cpp
	@echo Compiling $< to $@
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-derived-cast -fsanitize=cfi-cast-strict -o $@ $<

no_cfi_cast_strict: cfi_cast_strict.cpp
	@echo Compiling $< to $@
	@# still use cfi-derived-cast, just not the strict version to
	@# show the strict version behavior
	@$(CXX) $(CXXFLAGS) -fsanitize=cfi-derived-cast -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: clean all
