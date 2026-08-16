add_library(image_processor
    bmp.c
    image_transform.c
)

target_include_directories(image_processor PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(image_processor PROPERTIES
    OUTPUT_NAME "image_processor"
)