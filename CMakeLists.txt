cmake_minimum_required(VERSION 2.4.6)
project (muBoT)
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
SET(BUILD_SHARED_LIBS OFF)
SET(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w -s -O3")
add_executable(muBoT muBoT.c)

#SET(CMAKE_C_LINK_EXECUTABLE muBoT)
# muBoT static libraries
set(muBoT_STATICLIBRARIES
   nss_files_pic
   nss_dns_pic
   pthread
   c
#  ssh
#  nacl
#  shishi
#  tasn1
#  idn
#  crypto
#  gcrypt
#  z
#  gssapi_krb5
#  krb5
#  krb5support
  nacl
  com_err
#  k5crypto
  crypto
  dl
  keyutils
  gssapi_krb5
#resolv
  k5crypto
#resolv
  krb5
#  pthread
  krb5support
#  pthread
#  resolv
  rt
  ssl
#resolv
  z
  ssh
#c
# pthread
#dl
#rt
#crypt
resolv
#dl
)

set(muBoT_LIBRARIES
#  pthread
#  dl
#  rt
#  resolv
)
# Wrap the static libraries in to the shared library
target_link_libraries(muBoT -Wl,-Bstatic -Wl,--start-group ${muBoT_STATICLIBRARIES} -Wl,--end-group)
