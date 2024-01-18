# This makefile is for cross-compiling on student computers, i.e. login.student.lth.se


COMMON_DIR?=/usr/local/cs/rtp

COMPILER_DIR=${COMMON_DIR}/tools/comptools-mips-r12_1.2-0/comptools-mips-r12_1.2-0_amd64/mipsisa32r2el/r12
# Absolute path to the Axis Software Development Kit containing camera specific libs and headers, used for capture.h     and its corresponding object file (i.e. libcapture)
AXIS_PATH=${COMMON_DIR}/tools/emb-app-sdk_1_4/target/mipsisa32r2el-axis-linux-gnu

CC=${COMPILER_DIR}/bin/mipsisa32r2el-axis-linux-gnu-gcc
LD=${COMPILER_DIR}/bin/mipsisa32r2el-axis-linux-gnu-ld
#CC=mipsisa32r2el-axis-linux-gnu-gcc
#LD=mipsisa32r2el-axis-linux-gnu-gcc

# Set PATH to ensure J2C finds the right compiler.
#export PATH=""$COMPILER_DIR"/bin:"$PATH""
# Set to ensure compiler can find platform specific standard libs
#export LIBRARY_PATH=""$COMPILER_DIR"/lib:"$LIBRARY_PATH""

# Tell the compiler where to look for header files used in the auxilliary native methods for the camera classes
#CPPFLAGS+=-I${AXIS_PATH}/include -I${AXIS_PATH}/usr/include
CPPFLAGS+=-I${AXIS_PATH}/usr/include -I${COMPILER_DIR}/mipsisa32r2el-axis-linux-gnu/sys-include
AXIS_LIBCAP=-lcapture # This is needed for the native camera classes to fetch images
AXIS_LIBCAP_DEP=-ldbus-1 -lgobject-2.0 -ldbus-glib-1 -lgthread-2.0 -lglib-2.0 -lrapp -lpthread # Dependencies of libcapture
LDFLAGS+=${AXIS_LIBCAP} ${AXIS_LIBCAP_DEP} ${AXIS_LIBCAP} -L${AXIS_PATH}/lib -L${AXIS_PATH}/usr/lib # Specify which libs are used and set search path for them
CFLAGS+=-O3 -g -Wall
LDLIBS+=-lpthread
OBJS = camera.o socket.o webb_listener.o http_server.o mode_handler.o socket_listener.o socket_writer.o motion_listener.o

all: http_server

http_server: $(OBJS)
http_server.o: http_server.c motion_listener.h socket.h webb_listener.h socket_writer.h mode_handler.h
camera.o: camera.c camera.h fakecapture.h
socket.o: socket.c socket.h
webb_listener.o: webb_listener.c webb_listener.h socket.h logger.h
mode_handler.o: mode_handler.c mode_handler.h logger.h
socket_listener.o: socket_listener.c socket_listener.h mode_handler.h socket.h
socket_writer.o: socket_writer.c socket_writer.h socket.h socket_listener.h mode_handler.h
motion_listener.o: motion_listener.c motion_listener.h mode_handler.h camera.h


clean:
	-rm http_server.o camera.o

distclean: clean
	-rm http_server


.PHONY: all clean

