add_library(effect STATIC
    Color.h
    DrawPrimitives.cpp DrawPrimitives.h
    EffectDisintegrate.cpp EffectDisintegrate.h
    EffectInvisible.h
    EffectMirror.cpp EffectMirror.h
    EffectNone.cpp EffectNone.h
    EffectReverse.cpp EffectReverse.h
    EffectZx.cpp EffectZx.h
    Font.cpp Font.h
    LayeredPicture.cpp LayeredPicture.h
    Outline.cpp Outline.h
    Picture.cpp Picture.h
    PixelTool.cpp PixelTool.h
    ResColorPack.h
    SurfaceLock.cpp SurfaceLock.h
    TTFException.cpp TTFException.h
    ViewEffect.h
    WavyPicture.cpp WavyPicture.h
    SurfaceTool.cpp SurfaceTool.h
    PixelIterator.cpp PixelIterator.h
)
target_include_directories(effect PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(effect PUBLIC gengine SDL3_ttf::SDL3_ttf)
