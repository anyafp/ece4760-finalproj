add_executable(digitrec digitrec.c)

# Add pico_multicore which is required for multicore functionality
target_link_libraries(digitrec pico_stdlib pico_multicore pico_bootsel_via_double_reset hardware_sync hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(digitrec)