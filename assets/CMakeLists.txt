macro(convert_to_header INPUT NAME)
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${INPUT}.h"
        COMMAND
            "${Python_EXECUTABLE}"
            "${CMAKE_CURRENT_SOURCE_DIR}/hex_converter.py"
            "${CMAKE_CURRENT_SOURCE_DIR}/${INPUT}"
            "${NAME}"
            "${CMAKE_CURRENT_BINARY_DIR}/${INPUT}.h"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${INPUT}")
endmacro()

convert_to_header(blib.wav blib)
convert_to_header(font.png test_font)
convert_to_header(test.txt test)
convert_to_header(test_background.png test_background)
convert_to_header(test_background2.png test_background2)
convert_to_header(test_gradient.png test_gradient)
convert_to_header(test_music.mp3 test_music)
convert_to_header(test_particle.png test_particle)
convert_to_header(test_sprite.png test_sprite)
convert_to_header(test_spritemap.png test_spritemap)
convert_to_header(blib_afx1.wav blib_afx1)
convert_to_header(blib_afx2.wav blib_afx2)
convert_to_header(snd_laugh_child1.wav snd_laugh_child1)
convert_to_header(snd_laugh_female1.wav snd_laugh_female1)
convert_to_header(snd_laugh_female2.wav snd_laugh_female2)
convert_to_header(snd_laugh_female3.wav snd_laugh_female3)
convert_to_header(snd_laugh_male1.wav snd_laugh_male1)
convert_to_header(snd_laugh_male2.wav snd_laugh_male2)
convert_to_header(snd_laugh_male3.wav snd_laugh_male3)
convert_to_header(snd_laugh_male4.wav snd_laugh_male4)
convert_to_header(snd_laugh_male5.wav snd_laugh_male5)
convert_to_header(snd_steps.wav snd_steps)
convert_to_header(snd_steps2.wav snd_steps2)

add_library(test_assets INTERFACE
    "${CMAKE_CURRENT_SOURCE_DIR}/blib.wav"
    "${CMAKE_CURRENT_BINARY_DIR}/blib.wav.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/font.png"
    "${CMAKE_CURRENT_BINARY_DIR}/font.png.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/test.txt"
    "${CMAKE_CURRENT_BINARY_DIR}/test.txt.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_background.png"
    "${CMAKE_CURRENT_BINARY_DIR}/test_background.png.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_background2.png"
    "${CMAKE_CURRENT_BINARY_DIR}/test_background2.png.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_gradient.png"
    "${CMAKE_CURRENT_BINARY_DIR}/test_gradient.png.h"
    "${CMAKE_CURRENT_BINARY_DIR}/test_music.mp3"
    "${CMAKE_CURRENT_BINARY_DIR}/test_music.mp3.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_particle.png"
    "${CMAKE_CURRENT_BINARY_DIR}/test_particle.png.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_sprite.png"
    "${CMAKE_CURRENT_BINARY_DIR}/test_sprite.png.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/test_spritemap.png"
    "${CMAKE_CURRENT_BINARY_DIR}/test_spritemap.png.h"
    "${CMAKE_CURRENT_BINARY_DIR}/blib_afx1.wav"
    "${CMAKE_CURRENT_BINARY_DIR}/blib_afx1.wav.h"
    "${CMAKE_CURRENT_BINARY_DIR}/blib_afx2.wav"
    "${CMAKE_CURRENT_BINARY_DIR}/blib_afx2.wav.h"
)
target_include_directories(test_assets INTERFACE "${CMAKE_CURRENT_BINARY_DIR}")

convert_to_header(box.vert box_vert)
convert_to_header(box.frag box_frag)
convert_to_header(destination.vert destination_vert)
convert_to_header(destination.frag destination_frag)
convert_to_header(logo.png logo)
convert_to_header(sprite.vert sprite_vert)
convert_to_header(sprite.frag sprite_frag)

add_library(engine_assets INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/box.frag"
        "${CMAKE_CURRENT_BINARY_DIR}/box.frag.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/box.vert"
        "${CMAKE_CURRENT_BINARY_DIR}/box.vert.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/destination.frag"
        "${CMAKE_CURRENT_BINARY_DIR}/destination.frag.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/destination.vert"
        "${CMAKE_CURRENT_BINARY_DIR}/destination.vert.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/logo.png"
        "${CMAKE_CURRENT_BINARY_DIR}/logo.png.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/sprite.frag"
        "${CMAKE_CURRENT_BINARY_DIR}/sprite.frag.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/sprite.vert"
        "${CMAKE_CURRENT_BINARY_DIR}/sprite.vert.h"
)
target_include_directories(engine_assets INTERFACE "${CMAKE_CURRENT_BINARY_DIR}")

convert_to_header(boxyfont.png boxyfont)
add_library(boxyfont STATIC
        boxyfont.cpp
        boxyfont.h
        ${CMAKE_CURRENT_SOURCE_DIR}/boxyfont.png
        ${CMAKE_CURRENT_BINARY_DIR}/boxyfont.png.h)
target_include_directories(boxyfont PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(boxyfont INTERFACE ${CMAKE_CURRENT_BINARY_DIR})

convert_to_header(arena.png arena)
convert_to_header(family.png family)
convert_to_header(shop.png shop)

add_library(game_assets INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/arena.png"
        "${CMAKE_CURRENT_BINARY_DIR}/arena.png.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/family.png"
        "${CMAKE_CURRENT_BINARY_DIR}/family.png.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/shop.png"
        "${CMAKE_CURRENT_BINARY_DIR}/shop.png.h"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_child1.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_child1.wav.h"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_female1.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_female1.wav.h"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_female2.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_female2.wav.h"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_female3.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_female3.wav.h"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_male1.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_male1.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_male1.wav.h"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_male2.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_male2.wav.h"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_male3.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_male3.wav.h"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_male4.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_male4.wav.h"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_male5.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_laugh_male5.wav.h"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_steps.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_steps.wav.h"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_steps2.wav"
        "${CMAKE_CURRENT_BINARY_DIR}/snd_steps2.wav.h"
)
target_include_directories(game_assets INTERFACE ${CMAKE_CURRENT_BINARY_DIR})
