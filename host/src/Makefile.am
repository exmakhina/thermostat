## Define an executable target "thermostat", which will be installed into the
## directory named by the predefined variable $(bindir).
bin_PROGRAMS = thermostat

## Define the list of source files for the "thermostat" target. The file extension
## .cc is recognized by Automake, and causes it to produce rules which invoke
## the C++ compiler to produce an object file (.o) from each source file. The
## header files (.h) do not result in object files by themselves, but will be
## included in distribution archives of the project.
thermostat_SOURCES = crc8.c main.c serial_if.c
                
