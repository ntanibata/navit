include(LibFindMacros)
message(STATUS "FindWayland.cmake: Start")

if (USE_WAYLAND AND WAYLAND_LIBRARIES AND WAYLAND_INCLUDE_DIRS)
  message(STATUS "  [WAYLAND_LIBRARIES]    Found: ${WAYLAND_LIBRARIES}")
  message(STATUS "  [WAYLAND_INCLUDE_DIRS] Found: ${WAYLAND_INCLUDE_DIRS}")
else (USE_WAYLAND AND WAYLAND_LIBRARIES AND WAYLAND_INCLUDE_DIRS)

  libfind_pkg_check_modules(WAYLAND_WLCLIENT_PKGCONFIG wayland-client)
  libfind_pkg_check_modules(WAYLAND_WLEGL_PKGCONFIG wayland-egl)
  libfind_pkg_check_modules(WAYLAND_CURSOR_PKGCONFIG wayland-cursor)

  #################################################################
  #
  # Finding Include
  #
  #################################################################

  if (NOT WAYLAND_WLCLIENT_INCLUDE_DIR)
    find_path(WAYLAND_WLCLIENT_INCLUDE_DIR
      NAMES
        wayland-client.h
      PATHS
        ${WAYLAND_WLCLIENT_PKGCONFIG_INCLUDE_DIRS}
        /home/user/usrfs/include
    )
  endif (NOT WAYLAND_WLCLIENT_INCLUDE_DIR)

  if (NOT WAYLAND_WLEGL_INCLUDE_DIR)
    find_path(WAYLAND_WLEGL_INCLUDE_DIR
      NAMES
        wayland-egl.h
      PATHS
        ${WAYLAND_WLEGL_PKGCONFIG_INCLUDE_DIRS}
        /home/user/usrfs/include
    )
  endif (NOT WAYLAND_WLEGL_INCLUDE_DIR)

  ################################################################
  #
  # Finding Result of Include
  #
  ################################################################

  if (WAYLAND_WLCLIENT_INCLUDE_DIR)
    message(STATUS "  [WAYLAND_WLCLIENT_INCLUDE_DIR]    Found: ${WAYLAND_WLCLIENT_INCLUDE_DIR}")
  else (WAYLAND_WLCLIENT_INCLUDE_DIR)
    message(STATUS "  [WAYLAND_WLCLIENT_INCLUDE_DIR]    Could not find")
  endif (WAYLAND_WLCLIENT_INCLUDE_DIR)

  if (WAYLAND_WLEGL_INCLUDE_DIR)
    message(STATUS "  [WAYLAND_WLEGL_INCLUDE_DIR]       Found: ${WAYLAND_WLEGL_INCLUDE_DIR}")
  else (WAYLAND_WLEGL_INCLUDE_DIR)
    message(STATUS "  [WAYLAND_WLEGL_INCLUDE_DIR]       Could not find")
  endif (WAYLAND_WLEGL_INCLUDE_DIR)

  if (WAYLAND_WLCLIENT_INCLUDE_DIR AND WAYLAND_WLEGL_INCLUDE_DIR)
    if (${WAYLAND_WLCLIENT_INCLUDE_DIR} STREQUAL ${WAYLAND_WLEGL_INCLUDE_DIR})
      set (WAYLAND_INCLUDE_DIRS
        ${WAYLAND_WLCLIENT_INCLUDE_DIR}
      )
    else (${WAYLAND_WLCLIENT_INCLUDE_DIR} STREQUAL ${WAYLAND_WLEGL_INCLUDE_DIR})
      set (WAYLAND_INCLUDE_DIRS
        ${WAYLAND_WLCLIENT_INCLUDE_DIR}
        ${WAYLAND_WLEGL_INCLUDE_DIR}
      )
    endif (${WAYLAND_WLCLIENT_INCLUDE_DIR} STREQUAL ${WAYLAND_WLEGL_INCLUDE_DIR})
    message(STATUS "  [WAYLAND_INCLUDE_DIRS]            Found: ${WAYLAND_INCLUDE_DIRS}")
  else (WAYLAND_WLCLIENT_INCLUDE_DIR AND WAYLAND_WLEGL_INCLUDE_DIR)
    message(STATUS "  [WAYLAND_INCLUDE_DIRS]            Could not find")
  endif (WAYLAND_WLCLIENT_INCLUDE_DIR AND WAYLAND_WLEGL_INCLUDE_DIR)

  #################################################################
  #
  # Finding Library
  #
  #################################################################

  if (NOT WAYLAND_WLCLIENT_LIBRARY)
    find_library(WAYLAND_WLCLIENT_LIBRARY
      NAMES
        wayland-client
      PATHS
        ${WAYLAND_WLCLIENT_PKG_CONFIG_LIBRARY_DIRS}
        /home/user/usrfs/lib
    )
  endif (WAYLAND_WLCLIENT_LIBRARY)

  if (NOT WAYLAND_WLEGL_LIBRARY)
    find_library(WAYLAND_WLEGL_LIBRARY
      NAMES
        wayland-egl
      PATHS
        ${WAYLAND_WLEGL_PKG_CONFIG_LIBRARY_DIRS}
        /home/user/usrfs/lib
    )
  endif (NOT WAYLAND_WLCLIENT_LIBRARY)

  if (NOT WAYLAND_WLCURSOR_LIBRARY)
    find_library(WAYLAND_WLCURSOR_LIBRARY
      NAMES
        wayland-cursor
      PATHS
        ${WAYLAND_CURSOR_PKG_CONFIG_LIBRARY_DIRS}
        /home/user/usrfs/lib
    )
  endif (NOT WAYLAND_WLCURSOR_LIBRARY)

  ################################################################
  #
  # Finding Result of Library
  #
  ################################################################

  if (WAYLAND_WLCLIENT_LIBRARY)
    message(STATUS "  [WAYLAND_WLCLIENT_LIBRARY]        Found: ${WAYLAND_WLCLIENT_LIBRARY}")
  else (WAYLAND_WLCLIENT_LIBRARY)
    message(STATUS "  [WAYLAND_WLCLIENT_LIBRARY]        Could not find")
  endif (WAYLAND_WLCLIENT_LIBRARY)

  if (WAYLAND_WLEGL_LIBRARY)
    message(STATUS "  [WAYLAND_WLEGL_LIBRARY]           Found: ${WAYLAND_WLEGL_LIBRARY}")
  else (WAYLAND_WLEGL_LIBRARY)
    message(STATUS "  [WAYLAND_WLEGL_LIBRARY]           Could not find")
  endif (WAYLAND_WLEGL_LIBRARY)

  if (WAYLAND_WLCURSOR_LIBRARY)
    message(STATUS "  [WAYLAND_WLCURSOR_LIBRARY]        Found: ${WAYLAND_WLCURSOR_LIBRARY}")
  else (WAYLAND_WLCURSOR_LIBRARY)
    message(STATUS "  [WAYLAND_WLCURSOR_LIBRARY]        Could not find")
  endif (WAYLAND_WLCURSOR_LIBRARY)

  if (WAYLAND_WLCLIENT_LIBRARY AND WAYLAND_WLEGL_LIBRARY AND WAYLAND_WLCURSOR_LIBRARY)
    set (WAYLAND_LIBRARIES
      ${WAYLAND_WLCLIENT_LIBRARY}
      ${WAYLAND_WLEGL_LIBRARY}
      ${WAYLAND_WLCURSOR_LIBRARY}
    )
    message(STATUS "  [WAYLAND_LIBRARIES]               Found: ${WAYLAND_LIBRARIES}")
  else (WAYLAND_WLCLIENT_LIBRARY AND WAYLAND_WLEGL_LIBRARY AND WAYLAND_WLCURSOR_LIBRARY)
    message(STATUS "  [WAYLAND_LIBRARIES]               Could not find")
  endif (WAYLAND_WLCLIENT_LIBRARY AND WAYLAND_WLEGL_LIBRARY AND WAYLAND_WLCURSOR_LIBRARY)


  if (WAYLAND_LIBRARIES AND WAYLAND_INCLUDE_DIRS)
    set (USE_WAYLAND TRUE)
  endif (WAYLAND_LIBRARIES AND WAYLAND_INCLUDE_DIRS)

endif (USE_WAYLAND AND WAYLAND_LIBRARIES AND WAYLAND_INCLUDE_DIRS)

message(STATUS "FindWayland.cmake: End")

