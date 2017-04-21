
# BOARD_TAG     = mega2560
# BOARD_VARIANT = mega
BOARD_TAG         = mega
BOARD_SUB         = atmega2560

MONITOR_PORT  = /dev/ttyACM0
ARDUINO_LIBS = Wire SoftwareSerial

# check-syntax:
# 	$(CXX) -c -include Arduino.h   -x c++ $(CXXFLAGS)   $(CPPFLAGS)  -fsyntax-only $(CHK_SOURCES)

USER_LIB_PATH += .
#ARDUINO_LIBS += tone_library
#ARDUINO_LIBS += hmc5803l_library

include $(ARDMK_DIR)/Arduino.mk
