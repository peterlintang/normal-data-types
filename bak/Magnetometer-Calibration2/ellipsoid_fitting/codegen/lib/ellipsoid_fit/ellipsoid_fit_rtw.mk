###########################################################################
## Makefile generated for component 'ellipsoid_fit'. 
## 
## Makefile     : ellipsoid_fit_rtw.mk
## Generated on : Fri Aug 02 17:35:22 2024
## Final product: ./ellipsoid_fit.a
## Product type : static-library
## 
###########################################################################

###########################################################################
## MACROS
###########################################################################

# Macro Descriptions:
# PRODUCT_NAME            Name of the system to build
# MAKEFILE                Name of this makefile
# MODELLIB                Static library target

PRODUCT_NAME              = ellipsoid_fit
MAKEFILE                  = ellipsoid_fit_rtw.mk
MATLAB_ROOT               = /home/lxj/MATLAB
MATLAB_BIN                = /home/lxj/MATLAB/bin
MATLAB_ARCH_BIN           = $(MATLAB_BIN)/glnxa64
START_DIR                 = /home/lxj/Magnetometer-Calibration/ellipsoid_fitting
TGT_FCN_LIB               = ISO_C
SOLVER_OBJ                = 
CLASSIC_INTERFACE         = 0
MODEL_HAS_DYNAMICALLY_LOADED_SFCNS = 
RELATIVE_PATH_TO_ANCHOR   = ../../..
C_STANDARD_OPTS           = -fwrapv
CPP_STANDARD_OPTS         = -fwrapv
MODELLIB                  = ellipsoid_fit.a

###########################################################################
## TOOLCHAIN SPECIFICATIONS
###########################################################################

# Toolchain Name:          GNU gcc/g++ | gmake (64-bit Linux)
# Supported Version(s):    
# ToolchainInfo Version:   2021b
# Specification Revision:  1.0
# 
#-------------------------------------------
# Macros assumed to be defined elsewhere
#-------------------------------------------

# C_STANDARD_OPTS
# CPP_STANDARD_OPTS

#-----------
# MACROS
#-----------

WARN_FLAGS         = -Wall -W -Wwrite-strings -Winline -Wstrict-prototypes -Wnested-externs -Wpointer-arith -Wcast-align
WARN_FLAGS_MAX     = $(WARN_FLAGS) -Wcast-qual -Wshadow
CPP_WARN_FLAGS     = -Wall -W -Wwrite-strings -Winline -Wpointer-arith -Wcast-align
CPP_WARN_FLAGS_MAX = $(CPP_WARN_FLAGS) -Wcast-qual -Wshadow

TOOLCHAIN_SRCS = 
TOOLCHAIN_INCS = 
TOOLCHAIN_LIBS = 

#------------------------
# BUILD TOOL COMMANDS
#------------------------

# C Compiler: GNU C Compiler
CC = gcc

# Linker: GNU Linker
LD = g++

# C++ Compiler: GNU C++ Compiler
CPP = g++

# C++ Linker: GNU C++ Linker
CPP_LD = g++

# Archiver: GNU Archiver
AR = ar

# MEX Tool: MEX Tool
MEX_PATH = $(MATLAB_ARCH_BIN)
MEX = "$(MEX_PATH)/mex"

# Download: Download
DOWNLOAD =

# Execute: Execute
EXECUTE = $(PRODUCT)

# Builder: GMAKE Utility
MAKE_PATH = %MATLAB%/bin/glnxa64
MAKE = "$(MAKE_PATH)/gmake"


#-------------------------
# Directives/Utilities
#-------------------------

CDEBUG              = -g
C_OUTPUT_FLAG       = -o
LDDEBUG             = -g
OUTPUT_FLAG         = -o
CPPDEBUG            = -g
CPP_OUTPUT_FLAG     = -o
CPPLDDEBUG          = -g
OUTPUT_FLAG         = -o
ARDEBUG             =
STATICLIB_OUTPUT_FLAG =
MEX_DEBUG           = -g
RM                  = @rm -f
ECHO                = @echo
MV                  = @mv
RUN                 =

#--------------------------------------
# "Faster Runs" Build Configuration
#--------------------------------------

ARFLAGS              = ruvs
CFLAGS               = -c $(C_STANDARD_OPTS) -fPIC \
                       -O3 -fno-loop-optimize -fno-aggressive-loop-optimizations
CPPFLAGS             = -c $(CPP_STANDARD_OPTS) -fPIC \
                       -O3 -fno-loop-optimize -fno-aggressive-loop-optimizations
CPP_LDFLAGS          =
CPP_SHAREDLIB_LDFLAGS  = -shared -Wl,--no-undefined
DOWNLOAD_FLAGS       =
EXECUTE_FLAGS        =
LDFLAGS              =
MEX_CPPFLAGS         =
MEX_CPPLDFLAGS       =
MEX_CFLAGS           =
MEX_LDFLAGS          =
MAKE_FLAGS           = -f $(MAKEFILE)
SHAREDLIB_LDFLAGS    = -shared -Wl,--no-undefined



###########################################################################
## OUTPUT INFO
###########################################################################

PRODUCT = ./ellipsoid_fit.a
PRODUCT_TYPE = "static-library"
BUILD_TYPE = "Static Library"

###########################################################################
## INCLUDE PATHS
###########################################################################

INCLUDES_BUILDINFO = -I$(START_DIR)/codegen/lib/ellipsoid_fit -I$(START_DIR) -I$(MATLAB_ROOT)/extern/include

INCLUDES = $(INCLUDES_BUILDINFO)

###########################################################################
## DEFINES
###########################################################################

DEFINES_CUSTOM = 
DEFINES_STANDARD = -DMODEL=ellipsoid_fit

DEFINES = $(DEFINES_CUSTOM) $(DEFINES_STANDARD)

###########################################################################
## SOURCE FILES
###########################################################################

SRCS = $(START_DIR)/codegen/lib/ellipsoid_fit/ellipsoid_fit_data.c $(START_DIR)/codegen/lib/ellipsoid_fit/rt_nonfinite.c $(START_DIR)/codegen/lib/ellipsoid_fit/rtGetNaN.c $(START_DIR)/codegen/lib/ellipsoid_fit/rtGetInf.c $(START_DIR)/codegen/lib/ellipsoid_fit/ellipsoid_fit_initialize.c $(START_DIR)/codegen/lib/ellipsoid_fit/ellipsoid_fit_terminate.c $(START_DIR)/codegen/lib/ellipsoid_fit/ellipsoid_fit.c $(START_DIR)/codegen/lib/ellipsoid_fit/eig.c $(START_DIR)/codegen/lib/ellipsoid_fit/xnrm2.c $(START_DIR)/codegen/lib/ellipsoid_fit/xzlartg.c $(START_DIR)/codegen/lib/ellipsoid_fit/schur.c $(START_DIR)/codegen/lib/ellipsoid_fit/xzlarf.c $(START_DIR)/codegen/lib/ellipsoid_fit/xdhseqr.c $(START_DIR)/codegen/lib/ellipsoid_fit/xdlanv2.c $(START_DIR)/codegen/lib/ellipsoid_fit/xrot.c $(START_DIR)/codegen/lib/ellipsoid_fit/xzggev.c $(START_DIR)/codegen/lib/ellipsoid_fit/xzhgeqz.c $(START_DIR)/codegen/lib/ellipsoid_fit/sqrt.c $(START_DIR)/codegen/lib/ellipsoid_fit/xztgevc.c $(START_DIR)/codegen/lib/ellipsoid_fit/ellipsoid_fit_rtwutil.c

ALL_SRCS = $(SRCS)

###########################################################################
## OBJECTS
###########################################################################

OBJS = ellipsoid_fit_data.o rt_nonfinite.o rtGetNaN.o rtGetInf.o ellipsoid_fit_initialize.o ellipsoid_fit_terminate.o ellipsoid_fit.o eig.o xnrm2.o xzlartg.o schur.o xzlarf.o xdhseqr.o xdlanv2.o xrot.o xzggev.o xzhgeqz.o sqrt.o xztgevc.o ellipsoid_fit_rtwutil.o

ALL_OBJS = $(OBJS)

###########################################################################
## PREBUILT OBJECT FILES
###########################################################################

PREBUILT_OBJS = 

###########################################################################
## LIBRARIES
###########################################################################

LIBS = 

###########################################################################
## SYSTEM LIBRARIES
###########################################################################

SYSTEM_LIBS =  -lm

###########################################################################
## ADDITIONAL TOOLCHAIN FLAGS
###########################################################################

#---------------
# C Compiler
#---------------

CFLAGS_BASIC = $(DEFINES) $(INCLUDES)

CFLAGS += $(CFLAGS_BASIC)

#-----------------
# C++ Compiler
#-----------------

CPPFLAGS_BASIC = $(DEFINES) $(INCLUDES)

CPPFLAGS += $(CPPFLAGS_BASIC)

###########################################################################
## INLINED COMMANDS
###########################################################################

###########################################################################
## PHONY TARGETS
###########################################################################

.PHONY : all build clean info prebuild download execute


all : build
	@echo "### Successfully generated all binary outputs."


build : prebuild $(PRODUCT)


prebuild : 


download : $(PRODUCT)


execute : download


###########################################################################
## FINAL TARGET
###########################################################################

#---------------------------------
# Create a static library         
#---------------------------------

$(PRODUCT) : $(OBJS) $(PREBUILT_OBJS)
	@echo "### Creating static library "$(PRODUCT)" ..."
	$(AR) $(ARFLAGS)  $(PRODUCT) $(OBJS)
	@echo "### Created: $(PRODUCT)"


###########################################################################
## INTERMEDIATE TARGETS
###########################################################################

#---------------------
# SOURCE-TO-OBJECT
#---------------------

%.o : %.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.o : %.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.o : $(RELATIVE_PATH_TO_ANCHOR)/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.o : $(RELATIVE_PATH_TO_ANCHOR)/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.o : $(START_DIR)/codegen/lib/ellipsoid_fit/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.o : $(START_DIR)/codegen/lib/ellipsoid_fit/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


%.o : $(START_DIR)/%.c
	$(CC) $(CFLAGS) -o "$@" "$<"


%.o : $(START_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -o "$@" "$<"


ellipsoid_fit_data.o : $(START_DIR)/codegen/lib/ellipsoid_fit/ellipsoid_fit_data.c
	$(CC) $(CFLAGS) -o "$@" "$<"


rt_nonfinite.o : $(START_DIR)/codegen/lib/ellipsoid_fit/rt_nonfinite.c
	$(CC) $(CFLAGS) -o "$@" "$<"


rtGetNaN.o : $(START_DIR)/codegen/lib/ellipsoid_fit/rtGetNaN.c
	$(CC) $(CFLAGS) -o "$@" "$<"


rtGetInf.o : $(START_DIR)/codegen/lib/ellipsoid_fit/rtGetInf.c
	$(CC) $(CFLAGS) -o "$@" "$<"


ellipsoid_fit_initialize.o : $(START_DIR)/codegen/lib/ellipsoid_fit/ellipsoid_fit_initialize.c
	$(CC) $(CFLAGS) -o "$@" "$<"


ellipsoid_fit_terminate.o : $(START_DIR)/codegen/lib/ellipsoid_fit/ellipsoid_fit_terminate.c
	$(CC) $(CFLAGS) -o "$@" "$<"


ellipsoid_fit.o : $(START_DIR)/codegen/lib/ellipsoid_fit/ellipsoid_fit.c
	$(CC) $(CFLAGS) -o "$@" "$<"


eig.o : $(START_DIR)/codegen/lib/ellipsoid_fit/eig.c
	$(CC) $(CFLAGS) -o "$@" "$<"


xnrm2.o : $(START_DIR)/codegen/lib/ellipsoid_fit/xnrm2.c
	$(CC) $(CFLAGS) -o "$@" "$<"


xzlartg.o : $(START_DIR)/codegen/lib/ellipsoid_fit/xzlartg.c
	$(CC) $(CFLAGS) -o "$@" "$<"


schur.o : $(START_DIR)/codegen/lib/ellipsoid_fit/schur.c
	$(CC) $(CFLAGS) -o "$@" "$<"


xzlarf.o : $(START_DIR)/codegen/lib/ellipsoid_fit/xzlarf.c
	$(CC) $(CFLAGS) -o "$@" "$<"


xdhseqr.o : $(START_DIR)/codegen/lib/ellipsoid_fit/xdhseqr.c
	$(CC) $(CFLAGS) -o "$@" "$<"


xdlanv2.o : $(START_DIR)/codegen/lib/ellipsoid_fit/xdlanv2.c
	$(CC) $(CFLAGS) -o "$@" "$<"


xrot.o : $(START_DIR)/codegen/lib/ellipsoid_fit/xrot.c
	$(CC) $(CFLAGS) -o "$@" "$<"


xzggev.o : $(START_DIR)/codegen/lib/ellipsoid_fit/xzggev.c
	$(CC) $(CFLAGS) -o "$@" "$<"


xzhgeqz.o : $(START_DIR)/codegen/lib/ellipsoid_fit/xzhgeqz.c
	$(CC) $(CFLAGS) -o "$@" "$<"


sqrt.o : $(START_DIR)/codegen/lib/ellipsoid_fit/sqrt.c
	$(CC) $(CFLAGS) -o "$@" "$<"


xztgevc.o : $(START_DIR)/codegen/lib/ellipsoid_fit/xztgevc.c
	$(CC) $(CFLAGS) -o "$@" "$<"


ellipsoid_fit_rtwutil.o : $(START_DIR)/codegen/lib/ellipsoid_fit/ellipsoid_fit_rtwutil.c
	$(CC) $(CFLAGS) -o "$@" "$<"


###########################################################################
## DEPENDENCIES
###########################################################################

$(ALL_OBJS) : rtw_proj.tmw $(MAKEFILE)


###########################################################################
## MISCELLANEOUS TARGETS
###########################################################################

info : 
	@echo "### PRODUCT = $(PRODUCT)"
	@echo "### PRODUCT_TYPE = $(PRODUCT_TYPE)"
	@echo "### BUILD_TYPE = $(BUILD_TYPE)"
	@echo "### INCLUDES = $(INCLUDES)"
	@echo "### DEFINES = $(DEFINES)"
	@echo "### ALL_SRCS = $(ALL_SRCS)"
	@echo "### ALL_OBJS = $(ALL_OBJS)"
	@echo "### LIBS = $(LIBS)"
	@echo "### MODELREF_LIBS = $(MODELREF_LIBS)"
	@echo "### SYSTEM_LIBS = $(SYSTEM_LIBS)"
	@echo "### TOOLCHAIN_LIBS = $(TOOLCHAIN_LIBS)"
	@echo "### CFLAGS = $(CFLAGS)"
	@echo "### LDFLAGS = $(LDFLAGS)"
	@echo "### SHAREDLIB_LDFLAGS = $(SHAREDLIB_LDFLAGS)"
	@echo "### CPPFLAGS = $(CPPFLAGS)"
	@echo "### CPP_LDFLAGS = $(CPP_LDFLAGS)"
	@echo "### CPP_SHAREDLIB_LDFLAGS = $(CPP_SHAREDLIB_LDFLAGS)"
	@echo "### ARFLAGS = $(ARFLAGS)"
	@echo "### MEX_CFLAGS = $(MEX_CFLAGS)"
	@echo "### MEX_CPPFLAGS = $(MEX_CPPFLAGS)"
	@echo "### MEX_LDFLAGS = $(MEX_LDFLAGS)"
	@echo "### MEX_CPPLDFLAGS = $(MEX_CPPLDFLAGS)"
	@echo "### DOWNLOAD_FLAGS = $(DOWNLOAD_FLAGS)"
	@echo "### EXECUTE_FLAGS = $(EXECUTE_FLAGS)"
	@echo "### MAKE_FLAGS = $(MAKE_FLAGS)"


clean : 
	$(ECHO) "### Deleting all derived files..."
	$(RM) $(PRODUCT)
	$(RM) $(ALL_OBJS)
	$(ECHO) "### Deleted all derived files."


