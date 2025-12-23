set(DEPENDENT_MP_BIN2HEXCanalysis_default_CIPhAfES "c:/Program Files/Microchip/xc32/v5.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFCanalysis_default_CIPhAfES ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Canalysis/default.elf)
set(DEPENDENT_TARGET_DIRCanalysis_default_CIPhAfES ${CMAKE_CURRENT_LIST_DIR}/../../../../out/Canalysis)
set(DEPENDENT_BYPRODUCTSCanalysis_default_CIPhAfES ${DEPENDENT_TARGET_DIRCanalysis_default_CIPhAfES}/${sourceFileNameCanalysis_default_CIPhAfES}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRCanalysis_default_CIPhAfES}/${sourceFileNameCanalysis_default_CIPhAfES}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXCanalysis_default_CIPhAfES} --image ${DEPENDENT_DEPENDENT_TARGET_ELFCanalysis_default_CIPhAfES} --image-generated-c ${sourceFileNameCanalysis_default_CIPhAfES}.c --image-generated-h ${sourceFileNameCanalysis_default_CIPhAfES}.h --image-copy-mode ${modeCanalysis_default_CIPhAfES} --image-offset ${addressCanalysis_default_CIPhAfES} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRCanalysis_default_CIPhAfES}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFCanalysis_default_CIPhAfES})
add_custom_target(
    dependent_produced_source_artifactCanalysis_default_CIPhAfES 
    DEPENDS ${DEPENDENT_TARGET_DIRCanalysis_default_CIPhAfES}/${sourceFileNameCanalysis_default_CIPhAfES}.c
    )
