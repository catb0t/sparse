workspace "rle_array"

  configurations { "dbg", "dist" }

  flags { "fatalwarnings", "linktimeoptimization" }

  targetdir "bin/%{cfg.buildcfg}/"

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
