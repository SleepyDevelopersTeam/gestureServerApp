#-------------------------------------------------
#
# Project created by QtCreator 2016-09-16T12:08:58
#
#-------------------------------------------------

QT       += core gui\
		 network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = server
TEMPLATE = app


SOURCES += \
    main.cpp \
    server.cpp \
    view.cpp \
    gesturedetector.cpp \
    foneaccumulator.cpp \
    accumulator.cpp

HEADERS  += \
    server.h \
    view.h \
    foneaccumulator.h \
    gesturedetector.h \
    accumulator.h

FORMS += \
    server.ui

LIBS += -L/usr/local/Cellar/opencv3/3.1.0_3/lib -lopencv_stitching -lopencv_superres -lopencv_sfm -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_cvv -lopencv_dnn -lopencv_dpm -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_plot -lopencv_reg -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_rgbd -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_face -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_ml -lopencv_xphoto -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_photo -lopencv_imgproc -lopencv_core
INCLUDEPATH += -I /usr/local/Cellar/opencv3/3.1.0_3/include
INCLUDEPATH += /usr/local/Cellar/eigen/3.2.8/include/eigen3




INCLUDEPATH += $$PWD/../../../usr/local/Cellar/ceres-solver/1.11.0_2/include
DEPENDPATH += $$PWD/../../../usr/local/Cellar/ceres-solver/1.11.0_2/include


INCLUDEPATH += $$PWD/../../../usr/local/Cellar/glog/0.3.4/include
DEPENDPATH += $$PWD/../../../usr/local/Cellar/glog/0.3.4/include



INCLUDEPATH += $$PWD/../../../usr/local/Cellar/gflags/2.1.2/include
DEPENDPATH += $$PWD/../../../usr/local/Cellar/gflags/2.1.2/include
