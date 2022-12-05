add_executable(digitrec digitrec.c)

add_compile_options(-Ofast)

pico_generate_pio_header(digitrec ${CMAKE_CURRENT_LIST_DIR}/hsync.pio)
pico_generate_pio_header(digitrec ${CMAKE_CURRENT_LIST_DIR}/vsync.pio)
pico_generate_pio_header(digitrec ${CMAKE_CURRENT_LIST_DIR}/rgb.pio)

target_sources(digitrec PRIVATE digitrec.c vga_graphics.c)

# Add pico_multicore which is required for multicore functionality
target_link_libraries(digitrec PRIVATE pico_stdlib pico_divider pico_multicore pico_bootsel_via_double_reset hardware_pio hardware_dma hardware_adc hardware_irq hardware_clocks hardware_pll hardware_sync hardware_spi)

# create map/bin/hex file etc.
pico_add_extra_outputs(digitrec)