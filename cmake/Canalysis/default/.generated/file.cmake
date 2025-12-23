# The following variables contains the files used by the different stages of the build process.
set(Canalysis_default_default_XC32_FILE_TYPE_assemble)
set_source_files_properties(${Canalysis_default_default_XC32_FILE_TYPE_assemble} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Canalysis_default_default_XC32_FILE_TYPE_assemble})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Canalysis_default_default_XC32_FILE_TYPE_assembleWithPreprocess)
set_source_files_properties(${Canalysis_default_default_XC32_FILE_TYPE_assembleWithPreprocess} PROPERTIES LANGUAGE ASM)

# For assembly files, add "." to the include path for each file so that .include with a relative path works
foreach(source_file ${Canalysis_default_default_XC32_FILE_TYPE_assembleWithPreprocess})
        set_source_files_properties(${source_file} PROPERTIES INCLUDE_DIRECTORIES "$<PATH:NORMAL_PATH,$<PATH:REMOVE_FILENAME,${source_file}>>")
endforeach()

set(Canalysis_default_default_XC32_FILE_TYPE_compile
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Canalysis.X/main.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Canalysis.X/usb/usb.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Canalysis.X/usb/usb_cdc.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../Canalysis.X/usb_descriptors.c")
set_source_files_properties(${Canalysis_default_default_XC32_FILE_TYPE_compile} PROPERTIES LANGUAGE C)
set(Canalysis_default_default_XC32_FILE_TYPE_compile_cpp)
set_source_files_properties(${Canalysis_default_default_XC32_FILE_TYPE_compile_cpp} PROPERTIES LANGUAGE CXX)
set(Canalysis_default_default_XC32_FILE_TYPE_link)
set(Canalysis_default_default_XC32_FILE_TYPE_bin2hex)
set(Canalysis_default_image_name "default.elf")
set(Canalysis_default_image_base_name "default")

# The output directory of the final image.
set(Canalysis_default_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/../../../out/Canalysis")

# The full path to the final image.
set(Canalysis_default_full_path_to_image ${Canalysis_default_output_dir}/${Canalysis_default_image_name})
