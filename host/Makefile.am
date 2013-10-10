## Thermostat: an example project using Automake

## Additional flags to pass to aclocal when it is invoked automatically at
## make time. The ${ACLOCAL_FLAGS} variable is picked up from the environment
## to provide a way for the user to supply additional arguments.
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

SUBDIRS = src

## The EXTRA_DIST directive tells Automake to place the file autogen.sh in the 
## distribution archive, so that that file is shipped to our users
EXTRA_DIST = autogen.sh
