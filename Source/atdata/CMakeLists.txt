SET(target atExplorerData)


SET(src
atATData
atATDataUtils
atChannels
atFile
atFileFolders
atATIFData
atRibbons
atSections
atSessions
atTiles.cpp
atChannel
atFileFolder
atFileSystemObject
atRibbon
atSection
atSession
atTile
)

#===== Shared library
if(BUILD_SHARED_LIBS)
    message("Building shared ATData lib")
    ADD_LIBRARY(${target} SHARED
    ${src}
    )

    SET_PROPERTY(TARGET ${target}  PROPERTY COMPILE_DEFINITIONS 
    EXPORT_ATE_DATA
    )

    TARGET_LINK_LIBRARIES(${target} 
        atExplorerCore
        mkjson
#        ${CMAKE_INSTALL_PREFIX}/lib/dslFoundation.lib
#        ${CMAKE_INSTALL_PREFIX}/lib/poco_foundation.lib
    )

    INSTALL(    
    TARGETS ${target} 
    RUNTIME DESTINATION bin COMPONENT core
    LIBRARY DESTINATION lib COMPONENT core
    ARCHIVE DESTINATION lib COMPONENT core
    )            
endif()

#===== General Installs
if(INSTALL_CXX_LIBS)
FILE (GLOB headers *.h)
INSTALL (   FILES ${headers}			 	
            DESTINATION include/atexplorer 
            COMPONENT cxx_headers)

endif()

