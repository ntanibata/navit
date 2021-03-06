include(LibFindMacros)

libfind_pkg_check_modules(OPENGL_PKGCONFIG gl)
libfind_pkg_check_modules(OPENGL_GLU_PKGCONFIG glu)
libfind_pkg_check_modules(OPENGL_GLUT_PKGCONFIG glut)
libfind_pkg_check_modules(OPENGLES_EGL_PKGCONFIG egl)
libfind_pkg_check_modules(OPENGLES_PKGCONFIG glesv1_cm)
libfind_pkg_check_modules(OPENGLES2_PKGCONFIG glesv2)

################
# Includes dirs
################
FIND_PATH(OPENGL_INCLUDE_DIR GL/gl.h
   PATHS
      ${OPENGL_PKGCONFIG_INCLUDE_DIRS}
      /home/user/usrfs/include
)

FIND_PATH(OPENGL_GLU_INCLUDE_DIR GL/glu.h
   PATHS
      ${OPENGL_GLU_PKGCONFIG_INCLUDE_DIRS}
      /home/user/usrfs/include
      /usr/include
)

FIND_PATH(OPENGL_GLUT_INCLUDE_DIR GL/glut.h
   PATHS
      ${OPENGL_GLUT_PKGCONFIG_INCLUDE_DIRS}
      /home/user/usrfs/include
      /usr/include
)

FIND_PATH(OPENGLES_EGL_INCLUDE_DIR EGL/egl.h
   PATHS
      ${OPENGLES_EGL_PKGCONFIG_INCLUDE_DIRS}
      /home/user/usrfs/include
)

FIND_PATH(OPENGLES_INCLUDE_DIR GLES/gl.h
   PATHS
      ${OPENGLES_PKGCONFIG_INCLUDE_DIRS}
      /home/user/usrfs/include
)

FIND_PATH(OPENGLES2_INCLUDE_DIR GLES2/gl2.h
   PATHS
      ${OPENGLES2_PKGCONFIG_INCLUDE_DIRS}
      /home/user/usrfs/include
)

#################
# Libraries dirs
#################
FIND_LIBRARY(OPENGL_gl_LIBRARY GL
   PATHS
      ${OPENGL_PKGCONFIG_LIBRARY_DIRS}
      /home/user/usrfs/lib
)

FIND_LIBRARY(OPENGL_glu_LIBRARY GLU
   PATHS
      ${OPENGL_GLU_PKGCONFIG_LIBRARY_DIRS}
      /usr/lib
)

FIND_LIBRARY(GLUT_glut_LIBRARY glut
   PATHS
      ${OPENGL_GLUT_PKGCONFIG_LIBRARY_DIRS}
      /usr/lib
)

FIND_LIBRARY(OPENGLES_EGL_LIBRARY EGL
   PATHS
      ${OPENGLES_EGL_PKGCONFIG_LIBRARY_DIRS}
      /home/user/usrfs/lib
)

FIND_LIBRARY(OPENGLES_glesv1_CM_LIBRARY GLESv1_CM
   PATHS
      ${OPENGLES_PKGCONFIG_LIBRARY_DIRS}
      /home/user/usrfs/lib
)

FIND_LIBRARY(OPENGLES2_glesv2_LIBRARY GLESv2
   PATHS
      ${OPENGLES2_PKGCONFIG_LIBRARY_DIRS}
      /home/user/usrfs/lib
)

IF (OPENGL_gl_LIBRARY)
   SET(OPENGL_FOUND TRUE)
ENDIF (OPENGL_gl_LIBRARY)

IF (OPENGL_glu_LIBRARY)
   SET(OPENGL_GLU_FOUND TRUE)
ENDIF (OPENGL_glu_LIBRARY)

IF (OPENGL_glut_LIBRARY)
   SET(GLUT_FOUND TRUE)
ENDIF (OPENGL_glut_LIBRARY)

IF (OPENGLES_EGL_LIBRARY)
   IF (OPENGLES_glesv1_CM_LIBRARY)
      SET(USE_OPENGLES TRUE)
   ENDIF (OPENGLES_glesv1_CM_LIBRARY)

   IF (OPENGLES2_glesv2_LIBRARY)
      SET(USE_OPENGLES2 TRUE)
   ENDIF (OPENGLES2_glesv2_LIBRARY)
ENDIF (OPENGLES_EGL_LIBRARY)

# Result
MESSAGE(STATUS "OPENGL_INCLUDE_DIR        : ${OPENGL_INCLUDE_DIR}")
MESSAGE(STATUS "OPENGL_gl_LIBRARY         : ${OPENGL_gl_LIBRARY}")
MESSAGE(STATUS "OPENGL_GLU_INCLUDE_DIR    : ${OPENGL_GLU_INCLUDE_DIR}")
MESSAGE(STATUS "OPENGL_glu_LIBRARY        : ${OPENGL_glu_LIBRARY}")
MESSAGE(STATUS "OPENGL_GLUT_INCLUDE_DIR   : ${OPENGL_GLUT_INCLUDE_DIR}")
MESSAGE(STATUS "GLUT_glut_LIBRARY         : ${GLUT_glut_LIBRARY}")
MESSAGE(STATUS "OPENGLES_EGL_INCLUDE_DIR  : ${OPENGLES_EGL_INCLUDE_DIR}")
MESSAGE(STATUS "OPENGLES_EGL_LIBRARY      : ${OPENGLES_EGL_LIBRARY}")
MESSAGE(STATUS "OPENGLES_INCLUDE_DIR      : ${OPENGLES_INCLUDE_DIR}")
MESSAGE(STATUS "OPENGLES_glesv1_CM_LIBRARY: ${OPENGLES_glesv1_CM_LIBRARY}")
MESSAGE(STATUS "OPENGLES2_INCLUDE_DIR     : ${OPENGLES2_INCLUDE_DIR}")
MESSAGE(STATUS "OPENGLES2_glesv2_LIBRARY  : ${OPENGLES2_glesv2_LIBRARY}")
