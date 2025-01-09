CXX := gcc

INC_PATH_LIN := -I/usr/local/include/opencv4/
INC_PATH_WIN := -IC:/Users/X/libs/opencv/build/include

LIB_PATH_LIN := -L/usr/local/lib64/
LIB_PATH_WIN := -LC:/Users/X/libs/opencv/mybuild/bin

MOORE_LIBS := -lni4882 -lIK220Dll64 -lE1735A -lE1735ACore

OPENCV_LIBS_WIN := -lopencv_calib3d4100 -lopencv_core4100 -lopencv_dnn4100 -lopencv_features2d4100 -lopencv_flann4100 -lopencv_gapi4100 -lopencv_highgui4100 -lopencv_imgcodecs4100 -lopencv_imgproc4100 -lopencv_ml4100 -lopencv_objdetect4100 -lopencv_photo4100 -lopencv_stitching4100 -lopencv_video4100 -lopencv_videoio4100

OPENCV_LIBS_LIN := -lopencv_calib3d -lopencv_core -lopencv_dnn -lopencv_features2d -lopencv_flann -lopencv_gapi -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_stitching -lopencv_video -lopencv_videoio

ADD_LIBS := -lm

INC_PATH := $(INC_PATH_LIN)
LIB_PATH := $(LIB_PATH_LIN)
OPENCV_LIBS := $(OPENCV_LIBS_LIN)


default: help
help:
	@echo "Makefile Help:"
	@echo "  make axis 		- Build the axis target."
	@echo "  make moore 		- Build the moore target."
	@echo "  make cary 		- Build the cary target."
	@echo "  make ik220 		- Build the ik220 target."
	@echo "  make keysight 	- Build the keysight target."
	@echo "  make STM 		- Build the STM target."
	@echo "  make nilox 		- Build the nilox target."
	@echo "  make help 		- Display this help message."

.PHONY: axis
axis: 
	$(CXX) axis_test.cpp ../src/IkOptical.cpp ../src/Scale.cpp ../src/SimpleSerial.cpp ../src/Asse.cpp ../src/keysight.cpp -I../src/moore_inc -L../lib/gcc_dll $(MOORE_LIBS) $(ADD_LIBS) -o bin/axis_test

.PHONY: moore
moore:
	$(CXX) moore_test.cpp ../src/Moore.cpp ../src/Asse.cpp ../src/SimpleSerial.cpp ../src/Laser.cpp ../src/IkOptical.cpp ../src/Scale.cpp ../src/keysight.cpp ../src/pos.cpp ../src/cary.cpp -I../src/moore_inc -L../lib/gcc_dll $(MOORE_LIBS) $(ADD_LIBS) -o bin/moore_test

.PHONY: cary
cary:
	$(CXX) cary_test.cpp ../src/cary.cpp ../src/SimpleSerial.cpp -I../src/moore_inc $(ADD_LIBS) -o bin/cary_test

.PHONY: ik220
ik220:
	$(CXX) ik220_test.cpp ../src/IkOptical.cpp ../src/Scale.cpp -I../src/moore_inc -L../lib/gcc_dll $(MOORE_LIBS) $(ADD_LIBS) -o bin/ik220_test

.PHONY: keysight
keysight:
	$(CXX) keysight_test.cpp ../src/keysight.cpp -I../src/moore_inc -L../lib/gcc_dll $(MOORE_LIBS) $(ADD_LIBS) -o bin/keysight_test

.PHONY: STM
STM: 
	python STM_test.py

.PHONY: nilox
nilox:
	$(CXX) nilox_test.cpp ../src/Nilox.cpp $(INC_PATH) -I../src/moore_inc $(LIB_PATH) -lstdc++ $(OPENCV_LIBS) $(ADD_LIBS) -o bin/nilox_test

.PHONY: marker_origin
marker_origin:
	$(CXX) marker_origin_test.cpp ../src/Moore.cpp ../src/Nilox.cpp ../src/Asse.cpp ../src/SimpleSerial.cpp ../src/Laser.cpp ../src/IkOptical.cpp ../src/Scale.cpp ../src/keysight.cpp ../src/pos.cpp ../src/cary.cpp -I../src/moore_inc -L../lib/gcc_dll $(MOORE_LIBS) $(INC_PATH) $(LIB_PATH) -lstdc++ $(OPENCV_LIBS) $(ADD_LIBS) -o bin/marker_origin_test

.PHONY: mkdir
mkdir:
	@mkdir bin