#pragma once
#ifndef CPUINFO_H
#define CPUINFO_H

#ifndef __cplusplus
	#include <stdbool.h>
#endif

#include <stdint.h>

/* Identify architecture and define corresponding macro */

#if defined(__i386__) || defined(__i486__) || defined(__i586__) || defined(__i686__) || defined(_M_IX86)
	#define CPUINFO_ARCH_X86 1
#endif

#if defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64)
	#define CPUINFO_ARCH_X86_64 1
#endif

#if defined(__arm__) || defined(_M_ARM)
	#define CPUINFO_ARCH_ARM 1
#endif

#if defined(__aarch64__) || defined(_M_ARM64)
	#define CPUINFO_ARCH_ARM64 1
#endif

#if defined(__PPC64__) || defined(__powerpc64__) || defined(_ARCH_PPC64)
	#define CPUINFO_ARCH_PPC64 1
#endif

#if defined(__pnacl__)
	#define CPUINFO_ARCH_PNACL 1
#endif

#if defined(EMSCRIPTEN)
	#define CPUINFO_ARCH_ASMJS 1
#endif

#if CPUINFO_ARCH_X86 && defined(_MSC_VER)
	#define CPUINFO_ABI __cdecl
#elif CPUINFO_ARCH_X86 && defined(__GNUC__)
	#define CPUINFO_ABI __attribute__((__cdecl__))
#else
	#define CPUINFO_ABI
#endif

/* Define other architecture-specific macros as 0 */

#ifndef CPUINFO_ARCH_X86
	#define CPUINFO_ARCH_X86 0
#endif

#ifndef CPUINFO_ARCH_X86_64
	#define CPUINFO_ARCH_X86_64 0
#endif

#ifndef CPUINFO_ARCH_ARM
	#define CPUINFO_ARCH_ARM 0
#endif

#ifndef CPUINFO_ARCH_ARM64
	#define CPUINFO_ARCH_ARM64 0
#endif

#ifndef CPUINFO_ARCH_PPC64
	#define CPUINFO_ARCH_PPC64 0
#endif

#ifndef CPUINFO_ARCH_PNACL
	#define CPUINFO_ARCH_PNACL 0
#endif

#ifndef CPUINFO_ARCH_ASMJS
	#define CPUINFO_ARCH_ASMJS 0
#endif

#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	struct cpuinfo_x86_isa {
		#if CPUINFO_ARCH_X86
			bool rdtsc;
		#endif
		bool sysenter;
		#if CPUINFO_ARCH_X86
			bool syscall;
		#endif
		bool msr;
		bool clzero;
		bool clflush;
		bool clflushopt;
		bool mwait;
		bool mwaitx;
		#if CPUINFO_ARCH_X86
			bool emmx;
		#endif
		bool fxsave;
		bool xsave;
		#if CPUINFO_ARCH_X86
			bool fpu;
			bool mmx;
			bool mmx_plus;
		#endif
		bool three_d_now;
		bool three_d_now_plus;
		#if CPUINFO_ARCH_X86
			bool three_d_now_geode;
		#endif
		bool three_d_now_prefetch;
		#if CPUINFO_ARCH_X86
			bool sse;
			bool sse2;
		#endif
		bool sse3;
		bool ssse3;
		bool sse4_1;
		bool sse4_2;
		bool sse4a;
		bool misaligned_sse;
		bool avx;
		bool fma3;
		bool fma4;
		bool xop;
		bool f16c;
		bool avx2;
		bool avx512f;
		bool avx512pf;
		bool avx512er;
		bool avx512cd;
		bool avx512bw;
		bool avx512vl;
		bool avx512vbmi;
		bool avx512vpopcntdq;
		bool avx512_4vnniw;
		bool avx512_4fmaps;
		bool hle;
		bool rtm;
		bool xtest;
		bool mpx;
		#if CPUINFO_ARCH_X86		
			bool cmov;
			bool cmpxchg8b;
		#endif
		bool cmpxchg16b;
		bool clwb;
		bool movbe;
		#if CPUINFO_ARCH_X86_64
			bool lahf_sahf;
		#endif
		bool fs_gs_base;
		bool lzcnt;
		bool popcnt;
		bool tbm;
		bool bmi;
		bool bmi2;
		bool adx;
		bool aes;
		bool pclmulqdq;
		bool rdrand;
		bool rdseed;
		bool sha;
		bool rng;
		bool ace;
		bool ace2;
		bool phe;
		bool pmm;
		bool lwp;
		bool rdtscp;
		bool rdpid;
	};

	struct cpuinfo_x86_model_info {
		uint32_t model;
		uint32_t family;

		uint32_t base_model;
		uint32_t base_family;
		uint32_t stepping;
		uint32_t extended_model;
		uint32_t extended_family;
		uint32_t processor_type;
	};
#endif

#define CPUINFO_CACHE_UNIFIED          0x00000001
#define CPUINFO_CACHE_INCLUSIVE        0x00000002
#define CPUINFO_CACHE_COMPLEX_INDEXING 0x00000004

struct cpuinfo_cache {
	uint32_t size;
	uint32_t associativity;
	uint32_t sets;
	uint32_t partitions;
	uint32_t line_size;
	uint32_t flags;
	uint32_t thread_start;
	uint32_t thread_count;
};

struct cpuinfo_trace_cache {
	uint32_t uops;
	uint32_t associativity;
};

#define CPUINFO_PAGE_SIZE_4KB  0x1000
#define CPUINFO_PAGE_SIZE_1MB  0x100000
#define CPUINFO_PAGE_SIZE_2MB  0x200000
#define CPUINFO_PAGE_SIZE_4MB  0x400000
#define CPUINFO_PAGE_SIZE_16MB 0x1000000
#define CPUINFO_PAGE_SIZE_1GB  0x40000000

struct cpuinfo_tlb {
	uint32_t entries;
	uint32_t associativity;
	uint64_t pages;
};

/** @name	Processor vendor information */
enum cpuinfo_vendor {
	/** @brief	Processor vendor is not known to the library, or the library failed to get vendor information from the OS. */
	cpuinfo_vendor_unknown = 0,
	
	/* Active vendors of modern CPUs */

	/** @brief	Intel Corporation. Vendor of x86, x86-64, IA64, and ARM processor microarchitectures. */
	/** @details	Sold its ARM design subsidiary in 2006. The last ARM processor design was released in 2004. */
	cpuinfo_vendor_intel    = 1,
	/** @brief	Advanced Micro Devices, Inc. Vendor of x86 and x86-64 processor microarchitectures. */
	cpuinfo_vendor_amd      = 2,
	/** @brief	ARM Holdings plc. Vendor of ARM and ARM64 processor microarchitectures. */
	cpuinfo_vendor_arm      = 3,
	/** @brief	Qualcomm Incorporated. Vendor of ARM and ARM64 processor microarchitectures. */
	cpuinfo_vendor_qualcomm = 4,
	/** @brief	Apple Inc. Vendor of ARM and ARM64 processor microarchitectures. */
	cpuinfo_vendor_apple    = 5,
	/** @brief	Samsung Electronics Co., Ltd. Vendir if ARM64 processor microarchitectures. */
	cpuinfo_vendor_samsung  = 6,
	/** @brief	nVidia Corporation. Vendor of ARM64-compatible processor microarchitectures. */
	cpuinfo_vendor_nvidia   = 7,
	/** @brief	MIPS Technologies, Inc. Vendor of MIPS processor microarchitectures. */
	cpuinfo_vendor_mips     = 8,
	/** @brief	International Business Machines Corporation. Vendor of PowerPC processor microarchitectures. */
	cpuinfo_vendor_ibm      = 9,
	/** @brief	Ingenic Semiconductor. Vendor of MIPS processor microarchitectures. */
	cpuinfo_vendor_ingenic  = 10,
	/** @brief	VIA Technologies, Inc. Vendor of x86 and x86-64 processor microarchitectures. */
	/** @details	Processors are designed by Centaur Technology, a subsidiary of VIA Technologies. */
	cpuinfo_vendor_via      = 11,

	/* Active vendors of embedded CPUs */

	/** @brief	Texas Instruments Inc. Vendor of ARM processor microarchitectures. */
	cpuinfo_vendor_texas_instruments = 30,
	/** @brief	Marvell Technology Group Ltd. Vendor of ARM processor microarchitectures. */
	cpuinfo_vendor_marvell           = 31,
	/** @brief	RDC Semiconductor Co., Ltd. Vendor of x86 processor microarchitectures. */
	/** @details	Designes embedded x86 CPUs. */
	cpuinfo_vendor_rdc               = 32,
	/** @brief	DM&P Electronics Inc. Vendor of x86 processor microarchitectures. */
	/** @details	Mostly embedded x86 designs. */
	cpuinfo_vendor_dmp               = 33,
	/** @brief	Motorola, Inc. Vendor of PowerPC and ARM processor microarchitectures. */
	cpuinfo_vendor_motorola          = 34,

	/* Defunct CPU vendors */
		
	/** @brief	Transmeta Corporation. Vendor of x86 processor microarchitectures. */
	/** @details	Now defunct. The last processor design was released in 2004. */
	/**         	Transmeta processors implemented VLIW ISA and used binary translation to execute x86 code. */
	cpuinfo_vendor_transmeta = 50,
	/** @brief	Cyrix Corporation. Vendor of x86 processor microarchitectures. */
	/** @details	Now defunct. The last processor design was released in 1996. */
	cpuinfo_vendor_cyrix     = 51,
	/** @brief	Rise Technology. Vendor of x86 processor microarchitectures. */
	/** @details	Now defunct. The last processor design was released in 1999. */
	cpuinfo_vendor_rise      = 52,
	/** @brief	National Semiconductor. Vendor of x86 processor microarchitectures. */
	/** @details	Sold its x86 design subsidiary in 1999. The last processor design was released in 1998. */
	cpuinfo_vendor_nsc       = 53,
	/** @brief	Silicon Integrated Systems. Vendor of x86 processor microarchitectures. */
	/** @details	Sold its x86 design subsidiary in 2001. The last processor design was released in 2001. */
	cpuinfo_vendor_sis       = 54,
	/** @brief	NexGen. Vendor of x86 processor microarchitectures. */
	/** @details	Now defunct. The last processor design was released in 1994. */
	/**         	NexGen designed the first x86 microarchitecture which decomposed x86 instructions into simple microoperations. */
	cpuinfo_vendor_nexgen    = 55,
	/** @brief	United Microelectronics Corporation. Vendor of x86 processor microarchitectures. */
	/** @details	Ceased x86 in the early 1990s. The last processor design was released in 1991. */
	/**         	Designed U5C and U5D processors. Both are 486 level. */
	cpuinfo_vendor_umc       = 56,
	/** @brief	Digital Equipment Corporation. Vendor of ARM processor microarchitecture. */
	/** @details	Sold its ARM designs in 1997. The last processor design was released in 1997. */
	cpuinfo_vendor_dec       = 57,
};

enum cpuinfo_uarch {
	/** @brief Microarchitecture is unknown, or the library failed to get information about the microarchitecture from OS */
	cpuinfo_uarch_unknown = 0,

	/** @brief Pentium and Pentium MMX microarchitecture. */
	cpuinfo_uarch_p5    = 0x00100100,
	/** @brief Intel Quark microarchitecture. */
	cpuinfo_uarch_quark = 0x00100101,

	/** @brief Pentium Pro, Pentium II, and Pentium III. */
	cpuinfo_uarch_p6           = 0x00100200,
	/** @brief Pentium M. */
	cpuinfo_uarch_dothan       = 0x00100201,
	/** @brief Intel Core microarchitecture. */
	cpuinfo_uarch_yonah        = 0x00100202,
	/** @brief Intel Core 2 microarchitecture on 65 nm process. */
	cpuinfo_uarch_conroe       = 0x00100203,
	/** @brief Intel Core 2 microarchitecture on 45 nm process. */
	cpuinfo_uarch_penryn       = 0x00100204,
	/** @brief Intel Nehalem and Westmere microarchitectures (Core i3/i5/i7 1st gen). */
	cpuinfo_uarch_nehalem      = 0x00100205,
	/** @brief Intel Sandy Bridge microarchitecture (Core i3/i5/i7 2nd gen). */
	cpuinfo_uarch_sandy_bridge = 0x00100206,
	/** @brief Intel Ivy Bridge microarchitecture (Core i3/i5/i7 3rd gen). */
	cpuinfo_uarch_ivy_bridge   = 0x00100207,
	/** @brief Intel Haswell microarchitecture (Core i3/i5/i7 4th gen). */
	cpuinfo_uarch_haswell      = 0x00100208,
	/** @brief Intel Broadwell microarchitecture. */
	cpuinfo_uarch_broadwell    = 0x00100209,
	/** @brief Intel Sky Lake microarchitecture. */
	cpuinfo_uarch_sky_lake     = 0x0010020A,
	/** @brief Intel Kaby Lake microarchitecture. */
	cpuinfo_uarch_kaby_lake    = 0x0010020B,

	/** @brief Pentium 4 with Willamette, Northwood, or Foster cores. */
	cpuinfo_uarch_willamette = 0x00100300,
	/** @brief Pentium 4 with Prescott and later cores. */
	cpuinfo_uarch_prescott   = 0x00100301,

	/** @brief Intel Atom on 45 nm process. */
	cpuinfo_uarch_bonnell    = 0x00100400,
	/** @brief Intel Atom on 32 nm process. */
	cpuinfo_uarch_saltwell   = 0x00100401,
	/** @brief Intel Silvermont microarchitecture (22 nm out-of-order Atom). */
	cpuinfo_uarch_silvermont = 0x00100402,
	/** @brief Intel Airmont microarchitecture (14 nm out-of-order Atom). */
	cpuinfo_uarch_airmont    = 0x00100403,

	/** @brief Intel Knights Ferry HPC boards. */
	cpuinfo_uarch_knights_ferry   = 0x00100500,
	/** @brief Intel Knights Corner HPC boards (aka Xeon Phi). */
	cpuinfo_uarch_knights_corner  = 0x00100501,
	/** @brief Intel Knights Landing microarchitecture (second-gen MIC). */
	cpuinfo_uarch_knights_landing = 0x00100502,
	/** @brief Intel Knights Hill microarchitecture (third-gen MIC). */
	cpuinfo_uarch_knights_hill    = 0x00100503,
	/** @brief Intel Knights Mill Xeon Phi. */
	cpuinfo_uarch_knights_mill    = 0x00100504,

	/** @brief AMD K5. */
	cpuinfo_uarch_k5        = 0x00200100,
	/** @brief AMD K6 and alike. */
	cpuinfo_uarch_k6        = 0x00200101,
	/** @brief AMD Athlon and Duron. */
	cpuinfo_uarch_k7        = 0x00200102,
	/** @brief AMD Athlon 64, Opteron 64. */
	cpuinfo_uarch_k8        = 0x00200103,
	/** @brief AMD Family 10h (Barcelona, Istambul, Magny-Cours). */
	cpuinfo_uarch_k10       = 0x00200104,
	/**
	 * @brief AMD Bulldozer microarchitecture
	 * @details Zambezi FX-series CPUs, Zurich, Valencia and Interlagos Opteron CPUs.
	 */
	cpuinfo_uarch_bulldozer = 0x00200105,
	/**
	 * @brief AMD Piledriver microarchitecture
	 * @details Vishera FX-series CPUs, Trinity and Richland APUs, Delhi, Seoul, Abu Dhabi Opteron CPUs.
	 */
	cpuinfo_uarch_piledriver  = 0x00200106,
	/** @brief AMD Steamroller microarchitecture (Kaveri APUs). */
	cpuinfo_uarch_steamroller = 0x00200107,
	/** @brief AMD Excavator microarchitecture (Carizzo APUs). */
	cpuinfo_uarch_excavator   = 0x00200108,
	/** @brief AMD Zen microarchitecture (Ryzen CPUs). */
	cpuinfo_uarch_zen         = 0x00200109,

	/** @brief NSC Geode and AMD Geode GX and LX. */
	cpuinco_uarch_geode  = 0x00200200,
	/** @brief AMD Bobcat mobile microarchitecture. */
	cpuinfo_uarch_bobcat = 0x00200201,
	/** @brief AMD Jaguar mobile microarchitecture. */
	cpuinfo_uarch_jaguar = 0x00200202,
	
	/** @brief ARM7 series. */
	cpuinfo_uarch_arm7  = 0x00300100,
	/** @brief ARM9 series. */
	cpuinfo_uarch_arm9  = 0x00300101,
	/** @brief ARM 1136, ARM 1156, ARM 1176, or ARM 11MPCore. */
	cpuinfo_uarch_arm11 = 0x00300102,

	/** @brief ARM Cortex-A5. */
	cpuinfo_uarch_cortex_a5  = 0x00300205,
	/** @brief ARM Cortex-A7. */
	cpuinfo_uarch_cortex_a7  = 0x00300207,
	/** @brief ARM Cortex-A8. */
	cpuinfo_uarch_cortex_a8  = 0x00300208,
	/** @brief ARM Cortex-A9. */
	cpuinfo_uarch_cortex_a9  = 0x00300209,
	/** @brief ARM Cortex-A12. */
	cpuinfo_uarch_cortex_a12 = 0x00300212,
	/** @brief ARM Cortex-A15. */
	cpuinfo_uarch_cortex_a15 = 0x00300215,
	/** @brief ARM Cortex-A15. */
	cpuinfo_uarch_cortex_a17 = 0x00300217,

	/** @brief ARM Cortex-A32. */
	cpuinfo_uarch_cortex_a32 = 0x00300332,
	/** @brief ARM Cortex-A35. */
	cpuinfo_uarch_cortex_a35 = 0x00300335,
	/** @brief ARM Cortex-A53. */
	cpuinfo_uarch_cortex_a53 = 0x00300353,
	/** @brief ARM Cortex-A57. */
	cpuinfo_uarch_cortex_a57 = 0x00300357,
	/** @brief ARM Cortex-A72. */
	cpuinfo_uarch_cortex_a72 = 0x00300372,
	/** @brief ARM Cortex-A73. */
	cpuinfo_uarch_cortex_a73 = 0x00300373,

	/** @brief Qualcomm Scorpion. */
	cpuinfo_uarch_scorpion = 0x00400100,
	/** @brief Qualcomm Krait. */
	cpuinfo_uarch_krait    = 0x00400101,
	/** @brief Qualcomm Kryo. */
	cpuinfo_uarch_kryo     = 0x00400102,

	/** @brief Samsung Mongoose. */
	cpuinfo_uarch_mongoose = 0x00500100,

	/** @brief Apple A6 and A6X processors. */
	cpuinfo_uarch_swift     = 0x00600100,
	/** @brief Apple A7 processor. */
	cpuinfo_uarch_cyclone   = 0x00600101,
	/** @brief Apple A8 processor. */
	cpuinfo_uarch_typhoon   = 0x00600102,
	/** @brief Apple A9 processor. */
	cpuinfo_uarch_twister   = 0x00600103,
	/** @brief Apple A10 processor. */
	cpuinfo_uarch_hurricane = 0x00600104,
};

struct cpuinfo_processor {
	enum cpuinfo_vendor vendor;
	enum cpuinfo_uarch uarch;
	uint64_t system_id;
	uint32_t thread_id;
	uint32_t core_id;
	uint32_t package_id;
	struct {
		const struct cpuinfo_trace_cache* trace;
		const struct cpuinfo_cache* l1i;
		const struct cpuinfo_cache* l1d;
		const struct cpuinfo_cache* l2;
		const struct cpuinfo_cache* l3;
		const struct cpuinfo_cache* l4;
	} cache;
};

struct cpuinfo_core {
	uint32_t thread_start;
	uint32_t thread_count;
};

struct cpuinfo_package {
	uint32_t thread_start;
	uint32_t thread_count;
	uint32_t core_count;
	#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
		struct cpuinfo_x86_model_info model_info;
	#endif
};

#ifdef __cplusplus
extern "C" {
#endif

void CPUINFO_ABI cpuinfo_initialize(void);

void CPUINFO_ABI cpuinfo_deinitialize(void);

#if CPUINFO_ARCH_X86 || CPUINFO_ARCH_X86_64
	extern struct cpuinfo_x86_isa cpuinfo_isa;
#endif

extern struct cpuinfo_processor* cpuinfo_processors;
extern struct cpuinfo_cores* cpuinfo_cores;
extern struct cpuinfo_package* cpuinfo_packages;
extern struct cpuinfo_cache* cpuinfo_l1i_cache;
extern struct cpuinfo_cache* cpuinfo_l1d_cache;
extern struct cpuinfo_cache* cpuinfo_l2_cache;
extern struct cpuinfo_cache* cpuinfo_l3_cache;
extern struct cpuinfo_cache* cpuinfo_l4_cache;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CPUINFO_H */