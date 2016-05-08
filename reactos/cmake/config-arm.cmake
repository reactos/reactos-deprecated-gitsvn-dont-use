
set(SARCH "omap3-zoom2" CACHE STRING
"Sub-architecture (board) to build for. Specify one of:
 kurobox versatile omap3-zoom2 omap3-beagle")

set(OARCH "armv7-a" CACHE STRING
"Generate instructions for this CPU type. Specify one of:
 armv5te armv7-a")

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

set(BUILD_MP TRUE CACHE BOOL
"Whether to compile the multi processor versions for ntoskrnl and hal.")

set(NEWSPRINTF FALSE CACHE BOOL
"Whether to compile the new sprintf.")
