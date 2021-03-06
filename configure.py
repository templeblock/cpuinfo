#!/usr/bin/env python


import confu
parser = confu.standard_parser("cpuinfo configuration script")
parser.add_argument("--log", dest="log_level",
    choices=("none", "error", "warning", "info", "debug"), default="error")
parser.add_argument("--mock", dest="mock", action="store_true")


def main(args):
    options = parser.parse_args(args)
    build = confu.Build.from_options(options)

    macros = {
        "CPUINFO_LOG_LEVEL": {"none": 0, "error": 1, "warning": 2, "info": 3, "debug": 4}[options.log_level],
        "CPUINFO_MOCK": int(options.mock)
    }
    if build.target.is_linux:
        macros["_GNU_SOURCE"] = 1

    build.export_cpath("include", ["cpuinfo.h"])

    with build.options(source_dir="src", macros=macros, extra_include_dirs="src"):
        sources = ["init.c", "cache.c", "log.c"]
        if build.target.is_x86_64:
            sources += [
                "x86/init.c", "x86/info.c", "x86/vendor.c", "x86/uarch.c", "x86/topology.c",
                "x86/cache/init.c", "x86/cache/descriptor.c", "x86/cache/deterministic.c",
            ]
            if build.target.is_macos:
                sources += ["x86/mach/init.c"]
            elif build.target.is_linux:
                sources += ["x86/linux/init.c"]
            sources.append("x86/isa.c" if not build.target.is_nacl else "x86/nacl/isa.c")
        if build.target.is_arm or build.target.is_arm64:
            sources += ["arm/uarch.c", "arm/cache.c"]
            if build.target.is_linux:
                sources += [
                    "arm/linux/init.c",
                    "arm/linux/cpuinfo.c"
                ]
                if build.target.is_arm:
                    sources.append("arm/linux/arm32-isa.c")
                elif build.target.is_arm64:
                    sources.append("arm/linux/arm64-isa.c")

        if build.target.is_macos:
            sources += ["mach/topology.c"]
        if build.target.is_linux:
            sources += ["linux/cpuset.c"]
        build.static_library("cpuinfo", map(build.cc, sources))

    with build.options(source_dir="tools", deps=build):
        build.executable("cpu-info", build.cc("cpu-info.c"))
        build.executable("isa-info", build.cc("isa-info.c"))
        build.executable("cache-info", build.cc("cache-info.c"))

    with build.options(source_dir="test", deps=[build, build.deps.googletest]):
        build.smoketest("init-test", build.cxx("init.cc"))
        if options.mock:
            with build.options(macros={"CPUINFO_MOCK": int(options.mock)}):
                if build.target.is_arm and build.target.is_linux:
                    build.unittest("raspberry-pi-test", build.cxx("raspberry-pi.cc"))
                    build.unittest("raspberry-pi-zero-test", build.cxx("raspberry-pi-zero.cc"))
                    build.unittest("raspberry-pi2-test", build.cxx("raspberry-pi2.cc"))
                    build.unittest("raspberry-pi3-test", build.cxx("raspberry-pi3.cc"))
                    build.unittest("beagleboard-xm-test", build.cxx("beagleboard-xm.cc"))
                    build.unittest("pandaboard-es-test", build.cxx("pandaboard-es.cc"))
                    build.unittest("odroid-u2-test", build.cxx("odroid-u2.cc"))
                    build.unittest("arndaleboard-test", build.cxx("arndaleboard.cc"))

                    build.unittest("nexus-s-test", build.cxx("nexus-s.cc"))
                    build.unittest("galaxy-nexus-test", build.cxx("galaxy-nexus.cc"))
                    build.unittest("nexus4-test", build.cxx("nexus4.cc"))
                    build.unittest("nexus5-test", build.cxx("nexus5.cc"))

                if (build.target.is_arm or build.target.is_arm64) and build.target.is_linux:
                    build.unittest("jetson-tx1-test", build.cxx("jetson-tx1.cc"))
                    build.unittest("nexus9-test", build.cxx("nexus9.cc"))
                    build.unittest("scaleway-test", build.cxx("scaleway.cc"))

    return build


if __name__ == "__main__":
    import sys
    main(sys.argv[1:]).generate()
