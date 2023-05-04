# phony
.PHONY: all clean

# global utils
ECHO = @echo
MKDIR_P = mkdir -p
RM_RF = rm -rf

# global objects path (please, DO NOT use "./" in path, f.e "./obj/app", but use ONLY "obj/app")
OBJ_PATH = obj
LIB_PATH = lib

# app compiling
APP_CXX      = g++
APP_CXXFLAGS = -g -Wall -std=c++17
APP_DEFINES  = -D CL_TARGET_OPENCL_VERSION=200
APP_INCLUDES = -I ./deps/OpenCL-Headers
# app linking
APP_LD        = g++
APP_LDFLAGS   = -mconsole
APP_LIBRARIES = -L ./libs/x64 -l OpenCL
# app qt mocing
APP_MOC = moc
# targets
APP_TARGET_PATH = .
APP_TARGET_NAME = $(APP_TARGET_PATH)/main.exe
# app sources
APP_SOURCES_PATH = app
APP_SOURCES := $(wildcard $(APP_SOURCES_PATH)/*.cpp)
APP_HEADERS := $(wildcard $(APP_SOURCES_PATH)/*.hpp)
# app objects
APP_SOURCES_OBJ  := $(foreach file,$(APP_SOURCES),$(OBJ_PATH)/$(file).o)

# create app
all: $(APP_TARGET_NAME)

# link application
$(APP_TARGET_NAME): $(APP_SOURCES_OBJ)
	$(MKDIR_P) $(@D)
	$(APP_LD) $(APP_LDFLAGS) $^ $(APP_LIBRARIES) -o $@

# compile source code
$(APP_SOURCES_OBJ): $(APP_SOURCES) $(APP_HEADERS)
	$(MKDIR_P) $(@D)
	$(APP_CXX) $(APP_CXXFLAGS) $(APP_INCLUDES) $(APP_DEFINES) -c $(@:$(OBJ_PATH)/%.o=%) -o $@

# clean all
clean:
	$(RM_RF) $(APP_TARGET_NAME)
	$(RM_RF) $(OBJ_PATH)
	$(RM_RF) $(LIB_PATH)