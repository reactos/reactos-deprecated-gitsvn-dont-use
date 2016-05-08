
set(SARCH "" CACHE STRING
"Sub-architecture to build for.")

set(OARCH "athlon64" CACHE STRING
"Generate instructions for this CPU type. Specify one of:
 k8 opteron athlon64 athlon-fx")

set (OPTIMIZE "1" CACHE STRING
"What level of optimization to use.
 0 = off
 1 = Default option, optimize for size (-Os) with some additional options
 2 = Optimize for size (-Os)
 3 = Optimize debugging experience (-Og)
 4 = Optimize (-O1)
 5 = Optimize even more (-O2)
 6 = Optimize yet more (-O3)
 7 = Disregard strict standards compliance (-Ofast)")

set(LTCG FALSE CACHE BOOL
"Whether to build with link-time code generation")

set(DBG TRUE CACHE BOOL
"Whether to compile for debugging.")

set(KDBG FALSE CACHE BOOL
"Whether to compile in the integrated kernel debugger.")

set(GDB FALSE CACHE BOOL
"Whether to compile for debugging with GDB.
If you don't use GDB, don't enable this.")

set(_WINKD_ TRUE CACHE BOOL
"Whether to compile with the KD protocol.")

set(_ELF_ FALSE CACHE BOOL
"Whether to compile support for ELF files.
Do not enable unless you know what you're doing.")

set(NSWPAT FALSE CACHE BOOL
"Whether to compile apps/libs with features covered software patents or not.
If you live in a country where software patents are valid/apply, don't
enable this (except they/you purchased a license from the patent owner).
This settings is disabled (0) by default.")

set(USERMODE TRUE CACHE BOOL
"Whether to compile any usermode parts. This is while kernel mode is under
 heavy development and usermode part not relevant for bootcd.")

if(MSVC)
set(_PREFAST_ FALSE CACHE BOOL
"Whether to enable PREFAST while compiling.")
endif()
