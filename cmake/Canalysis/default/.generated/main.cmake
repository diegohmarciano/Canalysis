# cmake files support debug production
include("${CMAKE_CURRENT_LIST_DIR}/rule.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/file.cmake")

set(Canalysis_default_library_list )

# Handle files with suffix s, for group default-XC32
if(Canalysis_default_default_XC32_FILE_TYPE_assemble)
add_library(Canalysis_default_default_XC32_assemble OBJECT ${Canalysis_default_default_XC32_FILE_TYPE_assemble})
    Canalysis_default_default_XC32_assemble_rule(Canalysis_default_default_XC32_assemble)
    list(APPEND Canalysis_default_library_list "$<TARGET_OBJECTS:Canalysis_default_default_XC32_assemble>")

endif()

# Handle files with suffix S, for group default-XC32
if(Canalysis_default_default_XC32_FILE_TYPE_assembleWithPreprocess)
add_library(Canalysis_default_default_XC32_assembleWithPreprocess OBJECT ${Canalysis_default_default_XC32_FILE_TYPE_assembleWithPreprocess})
    Canalysis_default_default_XC32_assembleWithPreprocess_rule(Canalysis_default_default_XC32_assembleWithPreprocess)
    list(APPEND Canalysis_default_library_list "$<TARGET_OBJECTS:Canalysis_default_default_XC32_assembleWithPreprocess>")

endif()

# Handle files with suffix [cC], for group default-XC32
if(Canalysis_default_default_XC32_FILE_TYPE_compile)
add_library(Canalysis_default_default_XC32_compile OBJECT ${Canalysis_default_default_XC32_FILE_TYPE_compile})
    Canalysis_default_default_XC32_compile_rule(Canalysis_default_default_XC32_compile)
    list(APPEND Canalysis_default_library_list "$<TARGET_OBJECTS:Canalysis_default_default_XC32_compile>")

endif()

# Handle files with suffix cpp, for group default-XC32
if(Canalysis_default_default_XC32_FILE_TYPE_compile_cpp)
add_library(Canalysis_default_default_XC32_compile_cpp OBJECT ${Canalysis_default_default_XC32_FILE_TYPE_compile_cpp})
    Canalysis_default_default_XC32_compile_cpp_rule(Canalysis_default_default_XC32_compile_cpp)
    list(APPEND Canalysis_default_library_list "$<TARGET_OBJECTS:Canalysis_default_default_XC32_compile_cpp>")

endif()

# Handle files with suffix [cC], for group default-XC32
if(Canalysis_default_default_XC32_FILE_TYPE_dependentObject)
add_library(Canalysis_default_default_XC32_dependentObject OBJECT ${Canalysis_default_default_XC32_FILE_TYPE_dependentObject})
    Canalysis_default_default_XC32_dependentObject_rule(Canalysis_default_default_XC32_dependentObject)
    list(APPEND Canalysis_default_library_list "$<TARGET_OBJECTS:Canalysis_default_default_XC32_dependentObject>")

endif()

# Handle files with suffix elf, for group default-XC32
if(Canalysis_default_default_XC32_FILE_TYPE_bin2hex)
add_library(Canalysis_default_default_XC32_bin2hex OBJECT ${Canalysis_default_default_XC32_FILE_TYPE_bin2hex})
    Canalysis_default_default_XC32_bin2hex_rule(Canalysis_default_default_XC32_bin2hex)
    list(APPEND Canalysis_default_library_list "$<TARGET_OBJECTS:Canalysis_default_default_XC32_bin2hex>")

endif()


# Main target for this project
add_executable(Canalysis_default_image_CIPhAfES ${Canalysis_default_library_list})

set_target_properties(Canalysis_default_image_CIPhAfES PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${Canalysis_default_output_dir})
set_target_properties(Canalysis_default_image_CIPhAfES PROPERTIES OUTPUT_NAME "default")
set_target_properties(Canalysis_default_image_CIPhAfES PROPERTIES SUFFIX ".elf")

target_link_libraries(Canalysis_default_image_CIPhAfES PRIVATE ${Canalysis_default_default_XC32_FILE_TYPE_link})


# Add the link options from the rule file.
Canalysis_default_link_rule(Canalysis_default_image_CIPhAfES)

# Call bin2hex function from the rule file
Canalysis_default_bin2hex_rule(Canalysis_default_image_CIPhAfES)


