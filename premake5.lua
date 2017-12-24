workspace "rle_array"

  configurations { "dbg", "dist" }

  flags { "fatalwarnings", "linktimeoptimization" }

  targetdir "bin/%{cfg.buildcfg}/"

  filter { "action:gmake*", "toolset:gcc" }
    buildoptions {
      "-Wall", "-Wextra", "-Wfloat-equal", "-Winline", "-Wundef", "-Werror",
      "-fverbose-asm", "-Wint-to-pointer-cast", "-Wshadow", "-Wpointer-arith",
      "-Wcast-align", "-Wcast-qual", "-Wunreachable-code", "-Wstrict-overflow=5",
      "-Wwrite-strings", "-Wconversion", "--pedantic-errors",
      "-Wredundant-decls", "-Werror=maybe-uninitialized",
      "-Wmissing-declarations", "-Wmissing-parameter-type",
      "-Wmissing-prototypes", "-Wnested-externs", "-Wold-style-declaration",
      "-Wold-style-definition", "-Wstrict-prototypes", "-Wpointer-sign"
    }

  filter "configurations:dbg"
    buildoptions { "-ggdb3" }
    symbols "on"
    optimize "off"

  project "interact"
    kind "consoleapp"

    files { "interact.c" }
    links { "rlearray" }

  project "rlearray"
    kind "staticlib"

    files { "rlep.c" }

  project "test"
    kind "consoleapp"

    files { "test.c" }
    links { "criterion", "rlearray" }

    targetname "test_rle"

  project "clobber"
    kind "makefile"

    -- not windows, clean like this
    filter "system:not windows"
      cleancommands {
        "({RMDIR} bin obj *.make Makefile *.o -r 2>/dev/null; echo)"
      }

    -- otherwise, clean like this
    filter "system:windows"
      cleancommands {
        "{DELETE} *.make Makefile *.o",
        "{RMDIR} bin obj"
      }
