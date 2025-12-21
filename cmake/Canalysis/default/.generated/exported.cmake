set(DEPENDENT_MP_BIN2HEXCanalystII_default_CIPhAfES "c:/Program Files/Microchip/xc32/v5.00/bin/xc32-bin2hex.exe")
set(DEPENDENT_DEPENDENT_TARGET_ELFCanalystII_default_CIPhAfES ${CMAKE_CURRENT_LIST_DIR}/../../../../out/CanalystII/default.elf)
set(DEPENDENT_TARGET_DIRCanalystII_default_CIPhAfES ${CMAKE_CURRENT_LIST_DIR}/../../../../out/CanalystII)
set(DEPENDENT_BYPRODUCTSCanalystII_default_CIPhAfES ${DEPENDENT_TARGET_DIRCanalystII_default_CIPhAfES}/${sourceFileNameCanalystII_default_CIPhAfES}.c)
add_custom_command(
    OUTPUT ${DEPENDENT_TARGET_DIRCanalystII_default_CIPhAfES}/${sourceFileNameCanalystII_default_CIPhAfES}.c
    COMMAND ${DEPENDENT_MP_BIN2HEXCanalystII_default_CIPhAfES} --image ${DEPENDENT_DEPENDENT_TARGET_ELFCanalystII_default_CIPhAfES} --image-generated-c ${sourceFileNameCanalystII_default_CIPhAfES}.c --image-generated-h ${sourceFileNameCanalystII_default_CIPhAfES}.h --image-copy-mode ${modeCanalystII_default_CIPhAfES} --image-offset ${addressCanalystII_default_CIPhAfES} 
    WORKING_DIRECTORY ${DEPENDENT_TARGET_DIRCanalystII_default_CIPhAfES}
    DEPENDS ${DEPENDENT_DEPENDENT_TARGET_ELFCanalystII_default_CIPhAfES})
add_custom_target(
    dependent_produced_source_artifactCanalystII_default_CIPhAfES 
    DEPENDS ${DEPENDENT_TARGET_DIRCanalystII_default_CIPhAfES}/${sourceFileNameCanalystII_default_CIPhAfES}.c
    )
