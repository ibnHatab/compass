
##

ALTERNATE_CORE    = wemos
BOARD_TAG   = xi

ALTERNATE_VAR_PATH = /local/tools/arduino-1.8.2/hardware/wemos/XI/variants
ALTERNATE_CORE_PATH = /local/tools/arduino-1.8.2/hardware/wemos/XI/

ISP_PROG    = arduino
ISP_PORT    = /dev/ttyUSB0
AVRDUDE_ISP_BAUDRATE = 57600

MONITOR_PORT  = /dev/ttyUSB0
ARDUINO_LIBS = Wire

USER_LIB_PATH += .

#ARDUINO_LIBS += tone_library
#ARDUINO_LIBS += hmc5803l_library

include $(ARDMK_DIR)/Arduino.mk
