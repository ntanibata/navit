include(LibFindMacros)

libfind_pkg_check_modules(X11_PKGCONFIG x11)

FIND_PATH(X11_INCLUDE_DIR X11/Xlib.h
   PATHS
      ${X11_PKGCONFIG_INCLUDE_DIRS}
      /usr/include
)

FIND_LIBRARY(X11_LIBRARY X11
   PATHS
      ${X11_PKGCONFIG_LIBRARY_DIRS}
      /usr/lib
)
