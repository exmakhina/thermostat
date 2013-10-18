## Thermostat: an example project using Automake

## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_CXXFLAGS = -std=c++0x 

# this lists the binaries to produce, the (non-PHONY, binary) targets in
# the previous manual Makefile
bin_PROGRAMS = thermostat

thermostat_SOURCES = src/main.cpp \
                      src/application.cpp
                      
thermostat_SOURCES += src/model/peripherals/serial_if/serial_if.c \
                       src/model/peripherals/serial_if/crc8.c	\
                       src/model/peripherals/serial_if/serialif.cpp \
                       src/model/peripherals/relay/relay.cpp \
                       src/model/model.cpp
                       
thermostat_SOURCES += src/controller/controller.cpp


