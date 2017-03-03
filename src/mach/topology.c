#include <string.h>
#include <alloca.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/api.h>

#include <log.h>
#include <mach/api.h>


struct cpuinfo_mach_topology cpuinfo_mach_detect_topology(void) {
	int packages = 1;
	size_t sizeof_packages = sizeof(packages);
	if (sysctlbyname("hw.packages", &packages, &sizeof_packages, NULL, 0) != 0) {
		cpuinfo_log_error("sysctlbyname(\"hw.packages\") failed: %s", strerror(errno));
	} else if (packages <= 0) {
		cpuinfo_log_error("invalid hw.packages value %d", packages);
	}

	int cores = 1;
	size_t sizeof_cores = sizeof(cores);
	if (sysctlbyname("hw.physicalcpu_max", &cores, &sizeof_cores, NULL, 0) != 0) {
		cpuinfo_log_error("sysctlbyname(\"hw.physicalcpu_max\") failed: %s", strerror(errno));
	} else if (cores <= 0) {
		cpuinfo_log_error("invalid hw.physicalcpu_max value %d", cores);
	}

	int threads = cores;
	size_t sizeof_threads = sizeof(threads);
	if (sysctlbyname("hw.logicalcpu_max", &threads, &sizeof_threads, NULL, 0) != 0) {
		cpuinfo_log_error("sysctlbyname(\"hw.logicalcpu_max\") failed: %s", strerror(errno));
	} else if (threads < cores) {
		cpuinfo_log_error("invalid hw.logicalcpu_max value %d (hw.physicalcpu_max = %d)", threads, cores);
	}

	cpuinfo_log_info("mach topology: packages = %d, cores = %d, threads = %d", packages, cores, threads);
	struct cpuinfo_mach_topology topology = {
		.packages = (uint32_t) packages,
		.cores = (uint32_t) cores,
		.threads = (uint32_t) threads
	};

	size_t cacheconfig_size = 0;
	if (sysctlbyname("hw.cacheconfig", NULL, &cacheconfig_size, NULL, 0) != 0) {
		cpuinfo_log_error("sysctlbyname(\"hw.cacheconfig\") failed: %s", strerror(errno));
	} else {
		uint64_t* cacheconfig = alloca(cacheconfig_size);
		if (sysctlbyname("hw.cacheconfig", cacheconfig, &cacheconfig_size, NULL, 0) != 0) {
			cpuinfo_log_error("sysctlbyname(\"hw.cacheconfig\") failed: %s", strerror(errno));
		} else {
			size_t cache_configs = cacheconfig_size / sizeof(uint64_t);
			cpuinfo_log_debug("mach hw.cacheconfig count: %zu", cache_configs);
			if (cache_configs > CPUINFO_MACH_MAX_CACHE_LEVELS) {
				cache_configs = CPUINFO_MACH_MAX_CACHE_LEVELS;
			}
			for (size_t i = 0; i < cache_configs; i++) {
				cpuinfo_log_debug("mach hw.cacheconfig[%zu]: %"PRIu64, i, cacheconfig[i]);
				topology.threads_per_cache[i] = cacheconfig[i];
			}
		}
	}
	return topology;
}