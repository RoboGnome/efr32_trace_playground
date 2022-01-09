project(minimal_orbstat)

# Create the executable target
add_executable(${PROJECT_NAME}
                    main.c
                    add.c
                    itm_trace_function.c)

# Link the libraries
target_link_libraries (${PROJECT_NAME} 
                        PUBLIC 
                            gcc
                            c
                            efr32_device
                            emlib
                            halconfig
                            kit_common_bsp)

# Add some more project specific compile commands if neccessary
# Can be also added through cmake
target_compile_options(${PROJECT_NAME} 
                        PUBLIC
                        --specs=nosys.specs                        
                        -finstrument-functions
                        -finstrument-functions-exclude-file-list=CMSIS)

# Link with the nano C std library
target_link_options(${PROJECT_NAME} 
                            PUBLIC
                            --specs=nosys.specs
                            -finstrument-functions
                            -finstrument-functions-exclude-file-list=CMSIS)

# Add the binary for flashing
create_bin_output(${PROJECT_NAME})

# Show the section sizes of the elf file
print_sizes(${PROJECT_NAME})