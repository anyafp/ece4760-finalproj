add_executable(digitrec digitrec.c)

# add_compile_options(-lpng)
# add_library(libpng STATIC /Users/anya/Pico/ece4760-finalproj/project/digitrec/libpng-1.6.37/png.c)

# set(ZLIB_INCLUDE_DIR /usr/local/Cellar/zlib/1.2.13)
# set(PNG_INCLUDE_LIB /usr/local/Cellar/libpng/1.6.38)
# set(PNG_INCLUDE_DIR /Users/anya/Pico/ece4760-finalproj/project/digitrec/libpng-1.6.37)
# set(ZLIB_LIBRARY zlib)
# set(PNG_LIBRARY png)

# #set_target_properties(digitrec
# #    PROPERTIES
# #        INTERFACE_LINK_LIBRARIES "PNG::PNG"
# #)

# find_package(PNG REQUIRED)
# find_package(ZLIB REQUIRED)
# include_directories(${PNG_INCLUDE_LIB})
# include_directories(${ZLIB_INCLUDE_DIR})
# link_directories(${PNG_INCLUDE_DIR})

# #include_directories(/Users/anya/Pico/ece4760-finalproj/digitrec)

# target_include_directories(libpng PUBLIC ${PNG_INCLUDE_DIR})



#target_link_directories(digitrec PUBLIC /Users/anya/Pico/ece4760-finalproj/project/digitrec)

# Add pico_multicore which is required for multicore functionality
target_link_libraries(digitrec pico_stdlib pico_multicore pico_bootsel_via_double_reset hardware_sync hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(digitrec)