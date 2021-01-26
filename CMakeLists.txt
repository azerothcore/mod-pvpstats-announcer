AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/PvPstatsAnnouncer.cpp")
AC_ADD_SCRIPT_LOADER("PvPstatsAnnouncer" "${CMAKE_CURRENT_LIST_DIR}/src/loader.h")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/pvpstats-announcer.conf.dist")