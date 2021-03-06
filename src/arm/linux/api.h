#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <cpuinfo.h>


#define PROC_CPUINFO_ARCH_T UINT32_C(0x00000001)
#define PROC_CPUINFO_ARCH_E UINT32_C(0x00000002)
#define PROC_CPUINFO_ARCH_J UINT32_C(0x00000004)

struct proc_cpuinfo_arch {
	uint32_t version;
	uint32_t flags;
};

struct proc_cpuinfo_cache {
	uint32_t i_size;
	uint32_t i_assoc;
	uint32_t i_line_length;
	uint32_t i_sets;
	uint32_t d_size;
	uint32_t d_assoc;
	uint32_t d_line_length;
	uint32_t d_sets;
};

#if CPUINFO_ARCH_ARM
	/* arch/arm/include/uapi/asm/hwcap.h */

	#define PROC_CPUINFO_FEATURE_SWP      UINT32_C(0x00000001)
	#define PROC_CPUINFO_FEATURE_HALF     UINT32_C(0x00000002)
	#define PROC_CPUINFO_FEATURE_THUMB    UINT32_C(0x00000004)
	#define PROC_CPUINFO_FEATURE_26BIT    UINT32_C(0x00000008)
	#define PROC_CPUINFO_FEATURE_FASTMULT UINT32_C(0x00000010)
	#define PROC_CPUINFO_FEATURE_FPA      UINT32_C(0x00000020)
	#define PROC_CPUINFO_FEATURE_VFP      UINT32_C(0x00000040)
	#define PROC_CPUINFO_FEATURE_EDSP     UINT32_C(0x00000080)
	#define PROC_CPUINFO_FEATURE_JAVA     UINT32_C(0x00000100)
	#define PROC_CPUINFO_FEATURE_IWMMXT   UINT32_C(0x00000200)
	#define PROC_CPUINFO_FEATURE_CRUNCH   UINT32_C(0x00000400)
	#define PROC_CPUINFO_FEATURE_THUMBEE  UINT32_C(0x00000800)
	#define PROC_CPUINFO_FEATURE_NEON     UINT32_C(0x00001000)
	#define PROC_CPUINFO_FEATURE_VFPV3    UINT32_C(0x00002000)
	#define PROC_CPUINFO_FEATURE_VFPV3D16 UINT32_C(0x00004000) /* Also set for VFPv4 with 16 double-precision registers */
	#define PROC_CPUINFO_FEATURE_TLS      UINT32_C(0x00008000)
	#define PROC_CPUINFO_FEATURE_VFPV4    UINT32_C(0x00010000)
	#define PROC_CPUINFO_FEATURE_IDIVA    UINT32_C(0x00020000)
	#define PROC_CPUINFO_FEATURE_IDIVT    UINT32_C(0x00040000)
	#define PROC_CPUINFO_FEATURE_IDIV     UINT32_C(0x00060000)
	#define PROC_CPUINFO_FEATURE_VFPD32   UINT32_C(0x00080000)
	#define PROC_CPUINFO_FEATURE_LPAE     UINT32_C(0x00100000)
	#define PROC_CPUINFO_FEATURE_EVTSTRM  UINT32_C(0x00200000)

	#define PROC_CPUINFO_FEATURE2_AES   UINT32_C(0x00000001)
	#define PROC_CPUINFO_FEATURE2_PMULL UINT32_C(0x00000002)
	#define PROC_CPUINFO_FEATURE2_SHA1  UINT32_C(0x00000004)
	#define PROC_CPUINFO_FEATURE2_SHA2  UINT32_C(0x00000008)
	#define PROC_CPUINFO_FEATURE2_CRC32 UINT32_C(0x00000010)
#elif CPUINFO_ARCH_ARM64
	/* arch/arm64/include/uapi/asm/hwcap.h */
	#define PROC_CPUINFO_FEATURE_FP       UINT32_C(0x00000001)
	#define PROC_CPUINFO_FEATURE_ASIMD    UINT32_C(0x00000002)
	#define PROC_CPUINFO_FEATURE_EVTSTRM  UINT32_C(0x00000004)
	#define PROC_CPUINFO_FEATURE_AES      UINT32_C(0x00000008)
	#define PROC_CPUINFO_FEATURE_PMULL    UINT32_C(0x00000010)
	#define PROC_CPUINFO_FEATURE_SHA1     UINT32_C(0x00000020)
	#define PROC_CPUINFO_FEATURE_SHA2     UINT32_C(0x00000040)
	#define PROC_CPUINFO_FEATURE_CRC32    UINT32_C(0x00000080)
	#define PROC_CPUINFO_FEATURE_ATOMICS  UINT32_C(0x00000100)
	#define PROC_CPUINFO_FEATURE_FPHP     UINT32_C(0x00000200)
	#define PROC_CPUINFO_FEATURE_ASIMDHP  UINT32_C(0x00000400)
	#define PROC_CPUINFO_FEATURE_CPUID    UINT32_C(0x00000800)
	#define PROC_CPUINFO_FEATURE_ASIMDRDM UINT32_C(0x00001000)
	#define PROC_CPUINFO_FEATURE_JSCVT    UINT32_C(0x00002000)
	#define PROC_CPUINFO_FEATURE_FCMA     UINT32_C(0x00004000)
	#define PROC_CPUINFO_FEATURE_LRCPC    UINT32_C(0x00008000)
#endif


#define PROC_CPUINFO_VALID_ARCHITECTURE UINT32_C(0x00000001)
#define PROC_CPUINFO_VALID_IMPLEMENTER  UINT32_C(0x00000002)
#define PROC_CPUINFO_VALID_VARIANT      UINT32_C(0x00000004)
#define PROC_CPUINFO_VALID_PART         UINT32_C(0x00000008)
#define PROC_CPUINFO_VALID_REVISION     UINT32_C(0x00000010)
#define PROC_CPUINFO_VALID_FEATURES     UINT32_C(0x00000020)
#if CPUINFO_ARCH_ARM
	#define PROC_CPUINFO_VALID_ICACHE_SIZE UINT32_C(0x00000100)
	#define PROC_CPUINFO_VALID_ICACHE_SETS UINT32_C(0x00000200)
	#define PROC_CPUINFO_VALID_ICACHE_WAYS UINT32_C(0x00000400)
	#define PROC_CPUINFO_VALID_ICACHE_LINE UINT32_C(0x00000800)
	#define PROC_CPUINFO_VALID_DCACHE_SIZE UINT32_C(0x00001000)
	#define PROC_CPUINFO_VALID_DCACHE_SETS UINT32_C(0x00002000)
	#define PROC_CPUINFO_VALID_DCACHE_WAYS UINT32_C(0x00004000)
	#define PROC_CPUINFO_VALID_DCACHE_LINE UINT32_C(0x00008000)
#endif

#define PROC_CPUINFO_VALID_INFO UINT32_C(0x0000003F)
#if CPUINFO_ARCH_ARM
	#define PROC_CPUINFO_VALID_ICACHE     UINT32_C(0x00000F00)
	#define PROC_CPUINFO_VALID_DCACHE     UINT32_C(0x0000F000)
	#define PROC_CPUINFO_VALID_CACHE_LINE UINT32_C(0x00008800)
#endif

struct proc_cpuinfo {
	struct proc_cpuinfo_arch architecture;
#if CPUINFO_ARCH_ARM
	struct proc_cpuinfo_cache cache;
#endif
	uint32_t features;
#if CPUINFO_ARCH_ARM
	uint32_t features2;
#endif
	uint32_t cpuid;
	uint32_t implementer;
	uint32_t variant;
	uint32_t part;
	uint32_t revision;
	uint32_t valid_mask;
};

struct proc_cpuinfo* cpuinfo_arm_linux_parse_proc_cpuinfo(
	uint32_t processors_count[restrict static 1]);

#if CPUINFO_ARCH_ARM
	void cpuinfo_arm_linux_decode_isa_from_proc_cpuinfo(
		const struct proc_cpuinfo proc_cpuinfo[restrict static 1],
		uint32_t proc_cpuinfo_count,
		struct cpuinfo_arm_isa isa[restrict static 1]);
#elif CPUINFO_ARCH_ARM64
	void cpuinfo_arm64_linux_decode_isa_from_proc_cpuinfo(
		const struct proc_cpuinfo proc_cpuinfo[restrict static 1],
		struct cpuinfo_arm_isa isa[restrict static 1]);
#endif
