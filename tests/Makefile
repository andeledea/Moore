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
	g++ axis_test.cpp ..\src\IkOptical.cpp ..\src\Scale.cpp ..\src\SimpleSerial.cpp ..\src\Asse.cpp ..\src\keysight.cpp -I../src/moore_inc -L../lib/gcc_dll -lIK220Dll64 -o bin\axis_test

.PHONY: moore
moore:
	g++ moore_test.cpp ../src/Moore.cpp ../src/Asse.cpp ../src/SimpleSerial.cpp ../src/Laser.cpp ../src/IkOptical.cpp ../src/Scale.cpp ../src/keysight.cpp ../src/pos.cpp ../src/cary.cpp -I../src/moore_inc -L../lib/gcc_dll -lni4882 -lIK220Dll64 -lE1735A -lE1735ACore -o bin\moore_test

.PHONY: cary
cary:
	g++ cary_test.cpp ..\src\cary.cpp ..\src\SimpleSerial.cpp -I../src/moore_inc -o bin\cary_test

.PHONY: ik220
ik220:
	g++ ik220_test.cpp ..\src\IkOptical.cpp ..\src\Scale.cpp -I../src/moore_inc -L../lib/gcc_dll -lIK220Dll64 -o bin\ik220_test

.PHONY: keysight
keysight:
	g++ keysight_test.cpp ..\src\keysight.cpp -I../src/moore_inc -L../lib/gcc_dll -lE1735A -lE1735ACore -o bin\keysight_test

.PHONY: STM
STM: 
	python STM_test.py

.PHONY: nilox
nilox:
	g++ nilox_test.cpp ..\src\nilox.cpp -I../src/moore_inc -IC:\Users\X\libs\opencv\build\include -LC:\Users\X\libs\opencv\mybuild\bin -lstdc++ -lopencv_calib3d4100 -lopencv_core4100 -lopencv_dnn4100 -lopencv_features2d4100 -lopencv_flann4100 -lopencv_gapi4100 -lopencv_highgui4100 -lopencv_imgcodecs4100 -lopencv_imgproc4100 -lopencv_ml4100 -lopencv_objdetect4100 -lopencv_photo4100 -lopencv_stitching4100 -lopencv_video4100 -lopencv_videoio4100 -o bin\nilox_test

.PHONY: marker_origin
marker_origin:
	g++ marker_origin_test.cpp ../src/Moore.cpp ../src/nilox.cpp ../src/Asse.cpp ../src/SimpleSerial.cpp ../src/Laser.cpp ../src/IkOptical.cpp ../src/Scale.cpp ../src/keysight.cpp ../src/pos.cpp ../src/cary.cpp -I../src/moore_inc -L../lib/gcc_dll -lni4882 -lIK220Dll64 -lE1735A -lE1735ACore  -IC:\Users\X\libs\opencv\build\include -LC:\Users\X\libs\opencv\mybuild\bin -lstdc++ -lopencv_calib3d4100 -lopencv_core4100 -lopencv_dnn4100 -lopencv_features2d4100 -lopencv_flann4100 -lopencv_gapi4100 -lopencv_highgui4100 -lopencv_imgcodecs4100 -lopencv_imgproc4100 -lopencv_ml4100 -lopencv_objdetect4100 -lopencv_photo4100 -lopencv_stitching4100 -lopencv_video4100 -lopencv_videoio4100 -o bin\marker_origin_test