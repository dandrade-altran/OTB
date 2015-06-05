find_package(Mercurial)
if(Mercurial_FOUND)
  message(STATUS "Repository status :")
  Mercurial_WC_IDENTIFY(${PROJECT_SOURCE_DIR} OTB)
  message(STATUS "  Mercurial version is ${Mercurial_VERSION_HG}")
  message(STATUS "  Repository revision is ${OTB_WC_REVISION}")
  set(OTB_WC_REVISION ${OTB_WC_REVISION} CACHE STRING "Repository version" FORCE)
  mark_as_advanced(OTB_WC_REVISION)
  if(OTB_WC_STATUS)
    message(STATUS "  Local file modifications:")
    string(REPLACE "\n" "\n--     " OTB_WC_STATUS_PRETTYPRINT "    ${OTB_WC_STATUS}")
    message(STATUS "${OTB_WC_STATUS_PRETTYPRINT}")
  else()
    message(STATUS "  No files modified locally")
  endif()
  string(REGEX REPLACE "\n" ";" OTB_WC_STATUS "${OTB_WC_STATUS}")
  set(OTB_WC_STATUS ${OTB_WC_STATUS} CACHE STRING "Repository status" FORCE)
  mark_as_advanced(OTB_WC_STATUS)

  if(OTB_DATA_ROOT)
    Mercurial_WC_IDENTIFY(${OTB_DATA_ROOT} OTB_DATA)
    set(OTB_DATA_WC_REVISION ${OTB_DATA_WC_REVISION} CACHE STRING "Repository version" FORCE)
    mark_as_advanced(OTB_DATA_WC_REVISION)
  endif()

endif()