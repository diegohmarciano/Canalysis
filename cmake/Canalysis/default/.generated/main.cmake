# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(CanalystII_default_library_list )

# Handle files with suffix s, for group default-XC32
if(CanalystII_default_default_XC32_FILE_TYPE_assemble)
add_library(CanalystII_default_default_XC32_assemble OBJECT ${CanalystII_default_default_XC32_FILE_TYPE_assemble})
    CanalystII_default_default_XC32_assemble_rule(CanalystII_default_default_XC32_assemble)
    list(APPEND CanalystII_default_library_list "$<TARGET_OBJECTS:CanalystII_default_default_XC32_assemble>")

endif()

# Handle files with suffix S, for group default-XC32
if(CanalystII_default_default_XC32_FILE_TYPE_assembleWithPreprocess)
add_library(CanalystII_default_default_XC32_assembleWithPreprocess OBJECT ${CanalystII_default_default_XC32_FILE_TYPE_assembleWithPreprocess})
    CanalystII_default_default_XC32_assembleWithPreprocess_rule(CanalystII_default_default_XC32_assembleWithPreprocess)
    list(APPEND CanalystII_default_library_list "$<TARGET_OBJECTS:CanalystII_default_default_XC32_assembleWithPreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC32
if(CanalystII_default_default_XC32_FILE_TYPE_compile)
add_library(CanalystII_default_default_XC32_compile OBJECT ${CanalystII_default_default_XC32_FILE_TYPE_compile})
    CanalystII_default_default_XC32_compile_rule(CanalystII_default_default_XC32_compile)
    list(APPEND CanalystII_default_library_list "$<TARGET_OBJECTS:CanalystII_default_default_XC32_compile>")

endif()

# Handle files with suffix cpp, for group default-XC32
if(CanalystII_default_default_XC32_FILE_TYPE_compile_cpp)
add_library(CanalystII_default_default_XC32_compile_cpp OBJECT ${CanalystII_default_default_XC32_FILE_TYPE_compile_cpp})
    CanalystII_default_default_XC32_compile_cpp_rule(CanalystII_default_default_XC32_compile_cpp)
    list(APPEND CanalystII_default_library_list "$<TARGET_OBJECTS:CanalystII_default_default_XC32_compile_cpp>")

endif()

# Handle files with suffix [cC], for group default-XC32
if(CanalystII_default_default_XC32_FILE_TYPE_dependentObject)
add_library(CanalystII_default_default_XC32_dependentObject OBJECT ${CanalystII_default_default_XC32_FILE_TYPE_dependentObject})
    CanalystII_default_default_XC32_dependentObject_rule(CanalystII_default_default_XC32_dependentObject)
    list(APPEND CanalystII_default_library_list "$<TARGET_OBJECTS:CanalystII_default_default_XC32_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC32
if(CanalystII_default_default_XC32_FILE_TYPE_bin2hex)
add_library(CanalystII_default_default_XC32_bin2hex OBJECT ${CanalystII_default_default_XC32_FILE_TYPE_bin2hex})
    CanalystII_default_default_XC32_bin2hex_rule(CanalystII_default_default_XC32_bin2hex)
    list(APPEND CanalystII_default_library_list "$<TARGET_OBJECTS:CanalystII_default_default_XC32_bin2hex>")

endif()


# Main target for this project
add_executable(CanalystII_default_image_CIPhAfES ${CanalystII_default_library_list})

set_target_properties(CanalystII_default_image_CIPhAfES PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CanalystII_default_output_dir})
set_target_properties(CanalystII_default_image_CIPhAfES PROPERTIES OUTPUT_NAME "default")
set_target_properties(CanalystII_default_image_CIPhAfES PROPERTIES SUFFIX ".elf")

target_link_libraries(CanalystII_default_image_CIPhAfES PRIVATE ${CanalystII_default_default_XC32_FILE_TYPE_link})


# Add the link options from the rule file.
CanalystII_default_link_rule(CanalystII_default_image_CIPhAfES)

# Call bin2hex function from the rule file
CanalystII_default_bin2hex_rule(CanalystII_default_image_CIPhAfES)


