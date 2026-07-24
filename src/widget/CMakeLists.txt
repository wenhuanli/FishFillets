add_library(widget STATIC
    HBox.cpp HBox.h
    IWidget.cpp IWidget.h
    Slider.cpp Slider.h
    VBox.cpp VBox.h
    WiBox.cpp WiBox.h
    WiPicture.cpp WiPicture.h
    WiSpace.h
    WiContainer.cpp WiContainer.h
    WiButton.cpp WiButton.h
    WiLabel.cpp WiLabel.h
    Labels.cpp Labels.h
    WiStatusBar.cpp WiStatusBar.h
    WiPara.cpp WiPara.h
)
target_include_directories(widget PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(widget PUBLIC gengine effect)
