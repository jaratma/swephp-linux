dnl config.m4
PHP_ARG_WITH(swephp)

if test "$PHP_SWEPHP" != "no"; then

  dnl Check for required libcurl library
  AC_MSG_CHECKING([for libswe])
  for i in $PHP_SWEPHP  /usr; do
    if test -f "$i/include/swe/swephexp.h"; then
      PHP_SWEPHP_DIR=$i
      break
    fi
  done
  if test -z "$PHP_SWEPHP_DIR"; then
    AC_MSG_ERROR([not found])
  fi

  dnl Found libcurl's headers
  AC_MSG_RESULT([found in $PHP_SWEPHP_DIR])

  dnl Update library list and include paths for libcurl
  PHP_ADD_INCLUDE($PHP_SWEPHP_DIR/include)
  PHP_ADD_LIBRARY_WITH_PATH(swe, $PHP_SWEPHP_DIR/$PHP_LIB_DIR, SWEPHP_SHARED_LIBADD)
  PHP_SUBST(SWEPHP_SHARED_LIBADD)

  PHP_NEW_EXTENSION(swephp, swephp.c, $ext_shared)
fi
