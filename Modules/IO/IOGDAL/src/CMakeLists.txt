set(OTBIOGDAL_SRC
  otbGDALDatasetWrapper.cxx
  otbGDALDriverManagerWrapper.cxx
  otbGDALImageIO.cxx
  otbGDALImageIOFactory.cxx
  otbGDALOverviewsBuilder.cxx
  otbOGRIOHelper.cxx
  otbOGRVectorDataIO.cxx
  otbOGRVectorDataIOFactory.cxx
  )

add_library(OTBIOGDAL ${OTBIOGDAL_SRC})
target_link_libraries(OTBIOGDAL 
  ${OTBVectorDataBase_LIBRARIES}
  ${OTBGdalAdapters_LIBRARIES}
  ${OTBTinyXML_LIBRARIES}
  ${OTBImageBase_LIBRARIES}
  ${OTBCommon_LIBRARIES}
  ${OTBGDAL_LIBRARIES}
  ${OTBBoost_LIBRARIES}
  ${OTBOSSIMAdapters_LIBRARIES}
  )

otb_module_target(OTBIOGDAL)
