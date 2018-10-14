#-------------------------------------------------
#
# Project created by QtCreator 2018-03-17T10:25:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PillCount_Mid
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
    Snake/common.cpp \
    Snake/drlse_edge.cpp \
    Snake/gvfc.cpp \
    helper.cpp \
    detailwindow.cpp

HEADERS  += mainwindow.h \
    Snake/common.h \
    Snake/drlse_edge.h \
    Snake/gvfc.h \
    helper.h \
    detailwindow.h

FORMS    += mainwindow.ui \
    detailwindow.ui

OPENCV_VISION = 2.4.9
INCLUDEPATH += /usr/local/include \
                /usr/local/opencv/$$OPENCV_VISION/include \
                /usr/local/opencv/$$OPENCV_VISION/include/opencv \
                /usr/local/opencv/$$OPENCV_VISION/include/opencv2 \
                /home/dzqiu/anaconda2/include/python2.7 \


#LIBS += -L/home/dzqiu/anaconda2/lib/python2.7
LIBS += `pkg-config --libs opencv`
LIBS += -L/usr/local/cuda-8.0/lib64

DISTFILES += \
    imregionmax.cu \
    loadNet.py

##cuda setting

LIBS += -L/usr/local/lib
LIBS += -L/usr/local/cuda-8.0/lib64
LIBS += -lcudart
LIBS += -lcufft
LIBS += -lcublas
LIBS += -lpython2.7

DEPENDPATH += .
OTHER_FILES += imregionmax.cu

CUDA_SOURCES += imregionmax.cu

CUDA_SDK = "/usr/local/cuda-8.0"   # Path to cuda SDK install
CUDA_DIR = "/usr/local/cuda-8.0"           # Path to cuda toolkit install
SYSTEM_NAME = linux         # Depending on your system either 'Win32', 'x64', or 'Win64'
SYSTEM_TYPE = 64           # '32' or '64', depending on your system
CUDA_ARCH = sm_37         # Type of CUDA architecture, for example 'compute_10', 'compute_11', 'sm_10'
NVCC_OPTIONS = --use_fast_math
INCLUDEPATH += $$CUDA_DIR/include
QMAKE_LIBDIR += $$CUDA_DIR/lib64/

CUDA_OBJECTS_DIR = ./

CUDA_LIBS = cudart cufft
CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')
NVCC_LIBS = $$join(CUDA_LIBS,' -l','-l', '')

CONFIG(debug, debug|release) {
    # Debug mode
    cuda_d.input = CUDA_SOURCES
    cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda_d.commands = $$CUDA_DIR/bin/nvcc -D_DEBUG $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda_d.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda_d
}
else {
    # Release mode
    cuda.input = CUDA_SOURCES
    cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda.commands = $$CUDA_DIR/bin/nvcc $$NVCC_OPTIONS $$CUDA_INC $$NVCC_LIBS --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -O3 -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda
}
