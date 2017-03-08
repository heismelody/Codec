#-------------------------------------------------
#
# Project created by QtCreator 2017-01-05T19:06:33
#
#-------------------------------------------------

QT       += core gui\
            uitools

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MAplayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    ma_jpeg_codec.cpp \
    ma_jpeg_viewer.cpp \
    ma_mpeg_decoder.cpp \
    fileloader.cpp \
    bitstream/sequenceheader.cpp \
    bitstream/sequenceextension.cpp \
    bitstream/sequencedisplayextension.cpp \
    bitstream/sequencescalableextension.cpp \
    bitstream/gopheader.cpp \
    bitstream/pictureheader.cpp \
    bitstream/picturecodingextension.cpp \
    bitstream/quantmatrixextension.cpp \
    bitstream/picturedisplayextension.cpp \
    bitstream/picturetemporalscalableextension.cpp \
    bitstream/picturespatialscalableextension.cpp \
    mpegutil.cpp \
    streamreader.cpp \
    mpegconst.cpp \
    bitstream/basebitstream.cpp \
    mpegsequence.cpp \
    mpeggop.cpp \
    mpegpicture.cpp \
    mpegslice.cpp \
    vlc.cpp \
    mpegcontext.cpp \
    gltest.cpp

HEADERS  += mainwindow.h \
    ma_jpeg_codec.h \
    ma_jpeg_viewer.h \
    ma_mpeg_decoder.h \
    fileloader.h \
    bitstream/sequenceheader.h \
    bitstream/sequenceextension.h \
    bitstream/sequencedisplayextension.h \
    bitstream/sequencescalableextension.h \
    bitstream/gopheader.h \
    bitstream/pictureheader.h \
    bitstream/picturecodingextension.h \
    bitstream/quantmatrixextension.h \
    bitstream/picturedisplayextension.h \
    bitstream/picturetemporalscalableextension.h \
    bitstream/picturespatialscalableextension.h \
    mpegutil.h \
    streamreader.h \
    mpegconst.h \
    bitstream/basebitstream.h \
    mpegsequence.h \
    mpeggop.h \
    mpegpicture.h \
    mpegslice.h \
    vlc.h \
    mpegcontext.h \
    gltest.h

FORMS    += mainwindow.ui
