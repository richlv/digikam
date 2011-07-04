# - Try to find libpgf
# Once done this will define
#
#  PGF_FOUND            - system has libgf
#  PGF_INCLUDE_DIRS     - the libpgf include directory
#  PGF_LIBRARIES        - Link these to use libpgf
#  PGF_CODEC_VERSION_ID - PGF codec version ID.

# PKG-CONFIG is required.
INCLUDE(FindPkgConfig REQUIRED)

IF(PKG_CONFIG_FOUND)

    INCLUDE(FindPkgConfig)

    PKG_CHECK_MODULES(PGF libpgf)

    MESSAGE(STATUS "PGF_INCLUDE_DIRS     = ${PGF_INCLUDE_DIRS}")
    MESSAGE(STATUS "PGF_INCLUDEDIR       = ${PGF_INCLUDEDIR}")
    MESSAGE(STATUS "PGF_LIBRARIES        = ${PGF_LIBRARIES}")
    MESSAGE(STATUS "PGF_LDFLAGS          = ${PGF_LDFLAGS}")
    MESSAGE(STATUS "PGF_CFLAGS           = ${PGF_CFLAGS}")
    MESSAGE(STATUS "PGF_VERSION          = ${PGF_VERSION}")

    STRING(REPLACE "." "" PGF_CODEC_VERSION_ID "${PGF_VERSION}")
    MESSAGE(STATUS "PGF_CODEC_VERSION_ID = ${PGF_CODEC_VERSION_ID}")

ENDIF(PKG_CONFIG_FOUND)
