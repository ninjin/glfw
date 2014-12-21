# Find libOVR
#
# OVR_INCLUDE_DIR
# OVR_LIBRARY
# OVR_FOUND

find_path(OVR_INCLUDE_DIR NAMES OVR_CAPI.h)

if (WIN32 AND "${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
    set(OVR_NAMES libovr64)
else()
    set(OVR_NAMES libovr)
endif()

find_library(OVR_LIBRARY NAMES ${OVR_NAMES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OVR DEFAULT_MSG OVR_LIBRARY OVR_INCLUDE_DIR)

mark_as_advanced(OVR_INCLUDE_DIR OVR_LIBRARY)

