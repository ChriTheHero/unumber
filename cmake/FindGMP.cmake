# Try to find the GNU Multiple Precision Arithmetic Library (GMP)
# See http://gmplib.org/

if (GMP_INCLUDES AND GMP_LIBRARIES)
    set(GMP_FIND_QUIETLY TRUE)
endif (GMP_INCLUDES AND GMP_LIBRARIES)

find_path(GMP_INCLUDES
        NAMES gmp.h
        PATHS $ENV{GMPDIR} /usr/include /usr/local/include
)
find_library(GMP_LIBRARIES gmp
        PATHS $ENV{GMPDIR} /usr/lib /usr/local/lib
)
find_library(GMPXX_LIBRARIES gmpxx
        PATHS $ENV{GMPDIR} /usr/lib /usr/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMP DEFAULT_MSG
        GMP_INCLUDES GMP_LIBRARIES)
mark_as_advanced(GMP_INCLUDES GMP_LIBRARIES)