#include <stdint.h>

#include <cpuinfo.h>
#include <log.h>
#include <arm/api.h>


void cpuinfo_arm_decode_cache(
	enum cpuinfo_uarch uarch,
	uint32_t uarch_cores,
	uint32_t cpu_part,
	uint32_t arch_version,
	struct cpuinfo_cache l1i[restrict static 1],
	struct cpuinfo_cache l1d[restrict static 1],
	struct cpuinfo_cache l2[restrict static 1])
{
	switch (uarch) {
		case cpuinfo_uarch_xscale:
			switch (cpu_part >> 8) {
				case 2:
					/*
					 * PXA 210/25X/26X
					 *
					 * See "Computer Organization and Design, Revised Printing: The Hardware/Software Interface"
					 *     by David A. Patterson, John L. Hennessy
					 */
					*l1i = (struct cpuinfo_cache) {
						.size = 16 * 1024,
						.associativity = 32,
						.line_size = 32
					};
					*l1d = (struct cpuinfo_cache) {
						.size = 16 * 1024,
						.associativity = 4,
						.line_size = 64
					};
					break;
				case 4:
					/* PXA 27X */
					*l1i = (struct cpuinfo_cache) {
						.size = 32 * 1024,
						.associativity = 32,
						.line_size = 32
					};
					*l1d = (struct cpuinfo_cache) {
						.size = 32 * 1024,
						.associativity = 32,
						.line_size = 32
					};
					break;
				case 6:
					/*
					 * PXA 3XX
					 *
					 * See http://download.intel.com/design/intelxscale/31628302.pdf
					 */
					*l1i = (struct cpuinfo_cache) {
						.size = 32 * 1024,
						.associativity = 4,
						.line_size = 32
					};
					*l1d = (struct cpuinfo_cache) {
						.size = 32 * 1024,
						.associativity = 4,
						.line_size = 32
					};
					*l2 = (struct cpuinfo_cache) {
						.size = 256 * 1024,
						.associativity = 8,
						.line_size = 32
					};
					break;
			}
			break;
		case cpuinfo_uarch_arm11:
			*l1i = (struct cpuinfo_cache) {
				.size = 16 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 16 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			break;
		case cpuinfo_uarch_cortex_a5:
			/*
			 * Cortex-A5 Technical Reference Manual:
			 * 7.1.1. Memory system
			 *   The Cortex-A5 processor has separate instruction and data caches.
			 *   The caches have the following features:
			 *    - Data cache is 4-way set-associative.
			 *    - Instruction cache is 2-way set-associative.
			 *    - The cache line length is eight words.
			 *    - You can configure the instruction and data caches independently during implementation
			 *      to sizes of 4KB, 8KB, 16KB, 32KB, or 64KB.
			 * 1.1.3. System design components
			 *    PrimeCell Level 2 Cache Controller (PL310)
			 *      The addition of an on-chip secondary cache, also referred to as a Level 2 or L2 cache, is a
			 *      recognized method of improving the performance of ARM-based systems when significant memory traffic
			 *      is generated by the processor. The PrimeCell Level 2 Cache Controller reduces the number of external
			 *      memory accesses and has been optimized for use with the Cortex-A5 processor.
			 * 8.1.7. Exclusive L2 cache
			 *    The Cortex-A5 processor can be connected to an L2 cache that supports an exclusive cache mode.
			 *    This mode must be activated both in the Cortex-A5 processor and in the L2 cache controller. 
			 *
			 *  +--------------------+-----------+-----------+----------+-----------+
			 *  | Processor model    | L1D cache | L1I cache | L2 cache | Reference |
			 *  +--------------------+-----------+-----------+----------+-----------+
			 *  | Qualcomm MSM7225A  |           |           |          |           |
			 *  | Qualcomm MSM7625A  |           |           |          |           |
			 *  | Qualcomm MSM7227A  |           |           |          |           |
			 *  | Qualcomm MSM7627A  |    32K    |    32K    |   256K   | Wiki [1]  |
			 *  | Qualcomm MSM7225AB |           |           |          |           |
			 *  | Qualcomm MSM7225AB |           |           |          |           |
			 *  | Qualcomm QSD8250   |           |           |          |           |
			 *  | Qualcomm QSD8650   |           |           |          |           |
			 *  +--------------------+-----------+-----------+----------+-----------+
			 *  | Spreadtrum SC6821  |    32K    |    32K    |    ?     |           |
			 *  | Spreadtrum SC6825  |    32K    |    32K    |   256K   | Wiki [2]  |
			 *  | Spreadtrum SC8810  |     ?     |     ?     |    ?     |           |
			 *  | Spreadtrum SC8825  |    32K    |    32K    |    ?     |           |
			 *  +--------------------+-----------+-----------+----------+-----------+
			 *
			 * [1] https://en.wikipedia.org/wiki/List_of_Qualcomm_Snapdragon_systems-on-chip#Snapdragon_S1
			 * [2] https://en.wikipedia.org/wiki/Spreadtrum
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 2,
				.line_size = 32
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 256 * 1024,
				/*
				 * Follow NXP specification: "Eight-way set-associative 512 kB L2 cache with 32B line size"
				 * Reference: http://www.nxp.com/assets/documents/data/en/application-notes/AN4947.pdf
				 */
				.associativity = 8,
				.line_size = 32
			};
			break;
		case cpuinfo_uarch_cortex_a7:
			/*
			 * Cortex-A7 MPCore Technical Reference Manual:
			 * 6.1. About the L1 memory system
			 *   The L1 memory system consists of separate instruction and data caches. You can configure the
			 *   instruction and data caches independently during implementation to sizes of 8KB, 16KB, 32KB, or 64KB.
			 *
			 *   The L1 instruction memory system has the following features:
			 *    - Instruction side cache line length of 32-bytes.
			 *    - 2-way set-associative instruction cache.
			 *
			 *   The L1 data memory system has the following features:
			 *    - Data side cache line length of 64-bytes.
			 *    - 4-way set-associative data cache.
			 *
			 * 7.1. About the L2 Memory system
			 *   The L2 memory system consists of an:
			 *    - Optional tightly-coupled L2 cache that includes:
			 *      - Configurable L2 cache size of 128KB, 256KB, 512KB, and 1MB.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Allwinner A20      |   2   |    32K    |    32K    |   256K    |    [1]    |
			 *  | Allwinner A23      |   2   |    32K    |    32K    |   256K    |    [2]    |
			 *  | Allwinner A31      |   4   |    32K    |    32K    |    1M     |    [3]    |
			 *  | Allwinner A31s     |   4   |    32K    |    32K    |    1M     |    [4]    |
			 *  | Allwinner A33      |   4   |    32K    |    32K    |   512K    |    [5]    |
			 *  | Allwinner A80 Octa | 4(+4) |    32K    |    32K    | 512K(+2M) |    [6]    |
			 *  | Allwinner A81T     |   8   |    32K    |    32K    |    1M     |    [7]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Broadcom BCM2836   |   4   |    32K    |    32K    |    512K   |    [8]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] https://linux-sunxi.org/A20
			 * [2] https://linux-sunxi.org/A23
			 * [3] http://dl.linux-sunxi.org/A31/A3x_release_document/A31/IC/A31%20datasheet%20V1.3%2020131106.pdf
			 * [4] https://github.com/allwinner-zh/documents/blob/master/A31s/A31s_Datasheet_v1.5_20150510.pdf
			 * [5] http://dl.linux-sunxi.org/A33/A33_Datasheet_release1.0.pdf
			 * [6] https://linux-sunxi.org/images/1/10/A80_Datasheet_Revision_1.0_0404.pdf
			 * [7] http://dl.linux-sunxi.org/A83T/A83T_datasheet_Revision_1.1.pdf
			 * [8] https://www.raspberrypi.org/forums/viewtopic.php?t=98428
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 2,
				.line_size = 32
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 128 * 1024 * uarch_cores,
				.associativity = 8,
				.line_size = 64
			};
			break;
		case cpuinfo_uarch_cortex_a8:
			/*
			 * Cortex-A8 Technical Reference Manual:
			 * 7.1. About the L1 memory system
			 *    The L1 memory system consists of separate instruction and data caches in a Harvard arrangement.
			 *    The L1 memory system provides the core with:
			 *     - fixed line length of 64 bytes
			 *     - support for 16KB or 32KB caches
			 *     - 4-way set associative cache structure
			 * 8.1. About the L2 memory system
			 *    The L2 memory system is tightly coupled to the L1 data cache and L1 instruction cache.
			 *    The key features of the L2 memory system include:
			 *     - configurable cache size of 0KB, 128KB, 256KB, 512KB, and 1MB
			 *     - fixed line length of 64 bytes
			 *     - 8-way set associative cache structure
			 *
			 *  +----------------------+-----------+-----------+-----------+-----------+
			 *  | Processor model      | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +----------------------+-----------+-----------+-----------+-----------+
			 *  | Exynos 3 Single 3110 |    32K    |    32K    |   512K    |    [1]    |
			 *  +----------------------+-----------+-----------+-----------+-----------+
			 *  | TI DM 3730           |    32K    |    32K    |   256K    |    [2]    |
			 *  +----------------------+-----------+-----------+-----------+-----------+
			 *
			 * [1] https://en.wikichip.org/w/images/0/04/Exynos_3110.pdf
			 * [2] https://www.ti.com/lit/ds/symlink/dm3725.pdf
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 256 * 1024,
				.associativity = 8,
				.line_size = 64
			};
			break;
		case cpuinfo_uarch_cortex_a9:
			/*
			 * ARM Cortex‑A9 Technical Reference Manual:
			 * 7.1.1 Memory system
			 *    The Cortex‑A9 processor has separate instruction and data caches.
			 *    The caches have the following features:
			 *     - Both caches are 4-way set-associative.
			 *     - The cache line length is eight words.
			 *     - You can configure the instruction and data caches independently during implementation
			 *       to sizes of 16KB, 32KB, or 64KB.
			 * 8.1.5 Exclusive L2 cache
			 *    The Cortex‑A9 processor can be connected to an L2 cache that supports an exclusive cache mode.
			 *    This mode must be activated both in the Cortex‑A9 processor and in the L2 cache controller.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Exynos 4 Dual 4210 |   2   |    32K    |    32K    |    1M     |    [1]    |
			 *  | Exynos 4 Dual 4212 |   2   |    32K    |    32K    |    1M     |    [2]    |
			 *  | Exynos 4 Quad 4412 |   4   |    32K    |    32K    |    1M     |    [3]    |
			 *  | Exynos 4 Quad 4415 |   4   |    32K    |    32K    |    1M     |           |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] http://www.samsung.com/global/business/semiconductor/file/product/Exynos_4_Dual_45nm_User_Manaul_Public_REV1.00-0.pdf
			 * [2] http://www.samsung.com/global/business/semiconductor/file/product/Exynos_4_Dual_32nm_User_Manaul_Public_REV100-0.pdf
			 * [3] http://www.samsung.com/global/business/semiconductor/file/product/Exynos_4_Quad_User_Manaul_Public_REV1.00-0.pdf
			 */

			/* Use Exynos 4 specs */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 1024 * 1024,
				/* OMAP4460 in Pandaboard ES has 16-way set-associative L2 cache */
				.associativity = 16,
				.line_size = 32
			};
			break;
		case cpuinfo_uarch_cortex_a15:
			/*
			 * 6.1. About the L1 memory system
			 *    The L1 memory system consists of separate instruction and data caches.
			 *    The L1 instruction memory system has the following features:
			 *     - 32KB 2-way set-associative instruction cache.
			 *     - Fixed line length of 64 bytes.
			 *    The L1 data memory system has the following features:
			 *     - 32KB 2-way set-associative data cache.
			 *     - Fixed line length of 64 bytes.
			 * 7.1. About the L2 memory system
			 *    The features of the L2 memory system include:
			 *     - Configurable L2 cache size of 512KB, 1MB, 2MB and 4MB.
			 *     - Fixed line length of 64 bytes.
			 *     - 16-way set-associative cache structure.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Exynos 5 Dual 5250 |   2   |    32K    |    32K    |    1M     |    [1]    |
			 *  | Exynos 5 Hexa 5260 | 2(+4) |    32K    |    32K    | 1M(+512K) |    [2]    |
			 *  | Exynos 5 Octa 5410 | 4(+4) |    32K    |    32K    | 2M(+512K) |    [3]    |
			 *  | Exynos 5 Octa 5420 | 4(+4) |    32K    |    32K    | 2M(+512K) |    [3]    |
			 *  | Exynos 5 Octa 5422 | 4(+4) |    32K    |    32K    | 2M(+512K) |    [3]    |
			 *  | Exynos 5 Octa 5430 | 4(+4) |    32K    |    32K    | 2M(+512K) |    [3]    |
			 *  | Exynos 5 Octa 5800 | 4(+4) |    32K    |    32K    | 2M(+512K) |    [3]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] http://www.arndaleboard.org/wiki/downloads/supports/Exynos_5_Dual_User_Manaul_Public_REV1.00.pdf
			 * [2] http://www.yicsystem.com/wp-content/uploads/2014/08/Espresso5260P-Guide-Book.pdf
			 * [3] http://www.anandtech.com/show/6768/samsung-details-exynos-5-octa-architecture-power-at-isscc-13
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 2,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 2,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = uarch_cores * 512 * 1024,
				.associativity = 16,
				.line_size = 64
			};
			break;
		case cpuinfo_uarch_cortex_a53:
			/*
			 * ARM Cortex-A53 MPCore Processor Technical Reference Manual:
			 * 6.1. About the L1 memory system
			 *   The L1 memory system consists of separate instruction and data caches. The implementer configures the
			 *   instruction and data caches independently during implementation, to sizes of 8KB, 16KB, 32KB, or 64KB.
			 *
			 *   The L1 Instruction memory system has the following key features:
			 *    - Instruction side cache line length of 64 bytes.
			 *    - 2-way set associative L1 Instruction cache.
			 *
			 *   The L1 Data memory system has the following features:
			 *    - Data side cache line length of 64 bytes.
			 *    - 4-way set associative L1 Data cache.
			 *
			 * 7.1. About the L2 memory system
			 *   The L2 memory system consists of an:
			 *    - Optional tightly-coupled L2 cache that includes:
			 *      - Configurable L2 cache size of 128KB, 256KB, 512KB, 1MB and 2MB.
			 *      - Fixed line length of 64 bytes.
			 *      - 16-way set-associative cache structure.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Broadcom BCM2837   |   4   |    16K    |    16K    |    512K   |    [1]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] https://www.raspberrypi.org/forums/viewtopic.php?f=91&t=145766
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 16 * 1024,
				.associativity = 2,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 16 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = uarch_cores * 128 * 1024,
				.associativity = 16,
				.line_size = 64
			};
			break;
		case cpuinfo_uarch_cortex_a57:
			/*
			 * ARM Cortex-A57 MPCore Processor Technical Reference Manual:
			 * 6.1. About the L1 memory system
			 *   The L1 memory system consists of separate instruction and data caches.
			 *
			 *   The L1 instruction memory system has the following features:
			 *    - 48KB 3-way set-associative instruction cache.
			 *    - Fixed line length of 64 bytes.
			 *
			 *   The L1 data memory system has the following features:
			 *    - 32KB 2-way set-associative data cache.
			 *    - Fixed line length of 64 bytes.
			 *
			 * 7.1 About the L2 memory system
			 *   The features of the L2 memory system include:
			 *    - Configurable L2 cache size of 512KB, 1MB, and 2MB.
			 *    - Fixed line length of 64 bytes.
			 *    - 16-way set-associative cache structure.
			 *    - Inclusion property with L1 data caches.
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Jetson TX1         |   4   |    32K    |    48K    |    2M     |    [1]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] https://devblogs.nvidia.com/parallelforall/jetson-tx2-delivers-twice-intelligence-edge/
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 48 * 1024,
				.associativity = 3,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 2,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = uarch_cores * 512 * 1024,
				.associativity = 16,
				.line_size = 64,
				.flags = CPUINFO_CACHE_INCLUSIVE
			};
			break;
		case cpuinfo_uarch_scorpion:
			/*
			 * - "The CPU includes 32KB instruction and data caches as
			 *    well as a complete memory-management unit (MMU) suitable
			 *    for high-level operating systems. The CPU also has
			 *    256KB of SRAM that can be allocated in 64KB increments
			 *    to level-two (L2) cache or tightly coupled memory (TCM)." [1]
			 *    We interpret it as L2 cache being 4-way set-associative on single-core Scorpion.
			 * - L1 Data Cache = 32 KB. 32 B/line. [2]
             * - L2 Cache = 256 KB. 128 B/line. [2]
			 * - 256 KB (single-core) or 512 KB (dual-core) L2 cache [3]
			 * - Single or dual-core configuration [3]
			 * - For L1 cache assume the same associativity as Krait
			 *
			 * [1] https://www.qualcomm.com/media/documents/files/linley-report-on-dual-core-snapdragon.pdf
			 * [2] http://www.7-cpu.com/cpu/Snapdragon.html
			 * [3] https://en.wikipedia.org/wiki/Scorpion_(CPU)
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4,
				.line_size = 32
			};
			*l2 = (struct cpuinfo_cache) {
				.size = uarch_cores * 256 * 1024,
				.associativity = 4,
				.line_size = 128
			};
			break;
		case cpuinfo_uarch_krait:
			/*
			 * - L0 Data cache = 4 KB. 64 B/line, direct mapped [1]
			 * - L0 Instruction cache = 4 KB. [1]
			 * - L1 Data cache = 16 KB. 64 B/line, 4-way [1]
			 * - L1 Instruction cache = 16 KB, 4-way [1]
			 * - L2 Cache = 1 MB, 128 B/line, 8-way. Each core has fast access only to 512 KB of L2 cache. [1]
			 * - L2	= 1MB (dual core) or 2MB (quad core), 8-way set associative [2]
			 *
			 * [1] http://www.7-cpu.com/cpu/Krait.html
			 * [2] http://www.anandtech.com/show/4940/qualcomm-new-snapdragon-s4-msm8960-krait-architecture/2
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 16 * 1024,
				.associativity = 4,
				.line_size = 64 /* assume same as L1D */
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 16 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = uarch_cores * 512 * 1024,
				.associativity = 8,
				.line_size = 128
			};
			break;
		case cpuinfo_uarch_kryo:
			/*
			 *  +-----------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +-----------------+-------+-----------+-----------+-----------+-----------+
			 *  | Snapdragon 820  |  2+2  |    32K    |    32K    |  1M+512K  |    [1]    |
			 *  | Snapdragon 821  |  2+2  |    32K    |    32K    |  1M+512K  |    [1]    |
			 *  | Snapdragon 835  | 4(+4) |  64K+32K  |  64K+32K  |  2M(+1M)  |   sysfs   |
			 *  +-----------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] http://www.anandtech.com/show/9837/snapdragon-820-preview/2
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4 /* assume same as Krait */,
				.line_size = 64 /* assume same as Krait */
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4 /* assume same as Krait */,
				.line_size = 64 /* assume same as Krait */
			};
			*l2 = (struct cpuinfo_cache) {
				.size = uarch_cores * 512 * 1024,
				.associativity = 16 /* sysfs-reported on Snapdragon 835 */,
				.line_size = 64 /* assume same as Krait */
			};
			break;
		case cpuinfo_uarch_denver:
			/*
			 * The Denver chip includes a 128KB, 4-way level 1 instruction cache, a 64KB, 4-way level 2 data cache,
			 * and a 2MB, 16-way level 2 cache, all of which can service both cores. [1]
			 *
			 * All the caches have 64-byte lines. [2]
			 *
			 * [1] http://www.pcworld.com/article/2463900/nvidia-reveals-pc-like-performance-for-denver-tegra-k1.html
			 * [2] http://linleygroup.com/newsletters/newsletter_detail.php?num=5205&year=2014
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 128 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 2 * 1024 * 1024,
				.associativity = 16,
				.line_size = 64
			};
			break;
		case cpuinfo_uarch_mongoose:
			/*
			 * - "Moving past branch prediction we can see some elements of how the cache is set up for the L1 I$,
			 *    namely 64 KB split into four sets with 128-byte line sizes for 128 cache lines per set" [1]
			 * - "For loads and stores, a 32 KB, 8-way set associative cache with 64 byte line size is used" [1]
			 * - "The L2 cache here is 2MB shared across all cores split into 16 sets. This memory is also split
			 *    into 4 banks and has a 22 cycle latency" [1]
			 *
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Processor model    | Cores | L1D cache | L1I cache | L2 cache  | Reference |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *  | Exynos 8 Octa 8890 | 4(+4) |    64K    |    32K    |    2M     |    [1]    |
			 *  | Exynos 8 Octa 8895 | 4(+4) |    64K    |    32K    |    2M     |    [2]    |
			 *  +--------------------+-------+-----------+-----------+-----------+-----------+
			 *
			 * [1] http://www.anandtech.com/show/10590/hot-chips-2016-exynos-m1-architecture-disclosed
			 * [2] https://www.extremetech.com/mobile/244949-samsungs-exynos-8895-features-custom-cpu-cores-first-10nm-chip-market
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 64 * 1024,
				.associativity = 4,
				.line_size = 128
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 8,
				.line_size = 64
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 2 * 1024 * 1024,
				.associativity = 16,
				.line_size = 64
			};
			break;
		case cpuinfo_uarch_thunderx:
			/*
			 * "78K-Icache and 32K-D cache per core, 16 MB shared L2 cache" [1]
			 *
			 * [1] https://www.cavium.com/pdfFiles/ThunderX_CP_PB_Rev1.pdf
			 */
			*l1i = (struct cpuinfo_cache) {
				.size = 78 * 1024,
				.associativity = 4 /* assumption */,
				.line_size = 64 /* assumption */
			};
			*l1d = (struct cpuinfo_cache) {
				.size = 32 * 1024,
				.associativity = 4 /* assumption */,
				.line_size = 64 /* assumption */
			};
			*l2 = (struct cpuinfo_cache) {
				.size = 16 * 1024 * 1024,
				.associativity = 8 /* assumption */,
				.line_size = 64 /* assumption */
			};
			break;
		case cpuinfo_uarch_cortex_a12:
		case cpuinfo_uarch_cortex_a17:
		case cpuinfo_uarch_cortex_a32:
		case cpuinfo_uarch_cortex_a35:
		case cpuinfo_uarch_cortex_a72:
		case cpuinfo_uarch_cortex_a73:
		default:
			cpuinfo_log_warning("target uarch not recognized; using generic cache parameters");
			/* Follow OpenBLAS */
			if (arch_version >= 8) {
				*l1i = (struct cpuinfo_cache) {
					.size = 32 * 1024,
					.associativity = 4,
					.line_size = 64
				};
				*l1d = (struct cpuinfo_cache) {
					.size = 32 * 1024,
					.associativity = 4,
					.line_size = 64
				};
				*l2 = (struct cpuinfo_cache) {
					.size = uarch_cores * 256 * 1024,
					.associativity = 8,
					.line_size = 64
				};
			} else {
				*l1i = (struct cpuinfo_cache) {
					.size = 16 * 1024,
					.associativity = 4,
					.line_size = 32
				};
				*l1d = (struct cpuinfo_cache) {
					.size = 16 * 1024,
					.associativity = 4,
					.line_size = 32
				};
				if (arch_version >= 7) {
					*l2 = (struct cpuinfo_cache) {
						.size = uarch_cores * 128 * 1024,
						.associativity = 8,
						.line_size = 32
					};
				}
			}
			break;
	}
	l1i->sets = l1i->size / (l1i->associativity * l1i->line_size);
	l1i->partitions = 1;
	l1d->sets = l1d->size / (l1d->associativity * l1d->line_size);
	l1d->partitions = 1;
	if (l2->size != 0) {
		l2->sets = l2->size / (l2->associativity * l2->line_size);
		l2->partitions = 1;
	}
}
