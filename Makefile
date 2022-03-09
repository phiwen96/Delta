############### C++ compiler flags ###################
CXX := /usr/bin/clang++-12
CXX_FLAGS = -std=c++2a -fmodules-ts -fmodules -fbuiltin-module-map -fimplicit-modules -fimplicit-module-maps -fprebuilt-module-path=.
# -stdlib=libc++
############### External C++ libraries  ###################
LIB_NLOHMANN := -I/opt/homebrew/Cellar/nlohmann-json/3.10.5

# LIB_OPENSSL := /opt/homebrew/Cellar/openssl@3/3.0.1
# LIB_OPENSSL := -Lopenssl/openssl-0.9.8k/

################## Project dirs ##################
PROJ_DIR := $(CURDIR)
BUILD_DIR := $(PROJ_DIR)/build
MODULES_DIR := $(BUILD_DIR)/modules
SOURCES_DIR := $(PROJ_DIR)/modules
TARGETS_DIR := $(PROJ_DIR)/targets
OBJECTS_DIR := $(BUILD_DIR)/objects
APPS_DIR := $(BUILD_DIR)/apps

#################### Targets ###############
TEST_0 := $(BUILD_DIR)/test0
TEST_1 := $(BUILD_DIR)/test1

######################################
SOURCES := $(wildcard $(SOURCES_DIR)/*.cpp)
TARGETS := $(wildcard $(TARGETS_DIR)/*.cpp)

__MODULES := $(subst .cpp,.pcm,$(SOURCES))
_MODULES := $(foreach F,$(__MODULES),$(word $(words $(subst /, ,$F)),$(subst /, ,$F)))
MODULES := $(foreach name, $(_MODULES), $(addprefix $(MODULES_DIR)/, $(name)))

# __APPS := $(subst .cpp,,$(TARGETS))
__APPS = $(basename $(TARGETS))
_APPS := $(foreach F,$(__APPS),$(word $(words $(subst /, ,$F)),$(subst /, ,$F)))
APPS := $(foreach name, $(_APPS), $(addprefix $(APPS_DIR)/, $(name)))

__OBJECTS := $(subst .cpp,.o,$(TARGETS))
_OBJECTS := $(foreach F,$(__OBJECTS),$(word $(words $(subst /, ,$F)),$(subst /, ,$F)))
OBJECTS := $(foreach name, $(_OBJECTS), $(addprefix $(OBJECTS_DIR)/, $(name)))

_BUILD_DIRS := apps modules targets objects docs tests
BUILD_DIRS := $(foreach dir, $(_BUILD_DIRS), $(addprefix $(BUILD_DIR)/, $(dir)))

######################################
all: $(APPS)

######## TEST0 ###########
$(APPS_DIR)/test0: $(OBJECTS_DIR)/test0.o 
	$(CXX) $(CXX_FLAGS) $(OBJECTS_DIR)/test0.o -o $@ -L/usr/lib -lssl -lcrypto

$(OBJECTS_DIR)/test0.o: $(TARGETS_DIR)/test0.cpp $(MODULES)
	$(CXX) $(CXX_FLAGS) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -o $@ $(LIB_NLOHMANN)/include

######## TEST1 ###########
$(APPS_DIR)/test1: $(OBJECTS_DIR)/test1.o
	$(CXX) $(CXX_FLAGS) $(OBJECTS_DIR)/test1.o -o $@

$(OBJECTS_DIR)/test1.o: $(TARGETS_DIR)/test1.cpp $(MODULES)
	$(CXX) $(CXX_FLAGS) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -o $@ $(LIB_NLOHMANN)/include

# $(info $$NAMES is [${NAMES}])


######## Modules ###########
$(MODULES_DIR)/Delta.pcm: $(SOURCES_DIR)/Delta.cpp $(MODULES_DIR)/Array.pcm $(MODULES_DIR)/AIO.pcm $(MODULES_DIR)/Future.pcm $(MODULES_DIR)/String.pcm $(MODULES_DIR)/Char.pcm $(MODULES_DIR)/Convertible_to.pcm $(MODULES_DIR)/Same_as.pcm
	$(CXX) $(CXX_FLAGS) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(LIB_NLOHMANN)/include -I/$(LIB_OPENSSL)/include

$(MODULES_DIR)/Array.pcm: $(SOURCES_DIR)/Array.cpp
	$(CXX) $(CXX_FLAGS) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(LIB_NLOHMANN)/include -I/$(LIB_OPENSSL)/include

$(MODULES_DIR)/AIO.pcm: $(SOURCES_DIR)/AIO.cpp $(MODULES_DIR)/String.pcm
	$(CXX) $(CXX_FLAGS) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(LIB_NLOHMANN)/include -I/$(LIB_OPENSSL)/include

$(MODULES_DIR)/String.pcm: $(SOURCES_DIR)/String.cpp $(MODULES_DIR)/Char.pcm
	$(CXX) $(CXX_FLAGS) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(LIB_NLOHMANN)/include -I/$(LIB_OPENSSL)/include

$(MODULES_DIR)/Future.pcm: $(SOURCES_DIR)/Future.cpp
	$(CXX) $(CXX_FLAGS) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(LIB_NLOHMANN)/include -I/$(LIB_OPENSSL)/include

$(MODULES_DIR)/Char.pcm: $(SOURCES_DIR)/Char.cpp $(MODULES_DIR)/Same_as.pcm
	$(CXX) $(CXX_FLAGS) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(LIB_NLOHMANN)/include -I/$(LIB_OPENSSL)/include

$(MODULES_DIR)/Same_as.pcm: $(SOURCES_DIR)/Same_as.cpp
	$(CXX) $(CXX_FLAGS) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(LIB_NLOHMANN)/include -I/$(LIB_OPENSSL)/include

$(MODULES_DIR)/Convertible_to.pcm: $(SOURCES_DIR)/Convertible_to.cpp
	$(CXX) $(CXX_FLAGS) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(LIB_NLOHMANN)/include -I/$(LIB_OPENSSL)/include

######################################
directories := $(foreach dir, $(BUILD_DIRS), $(shell [ -d $(dir) ] || mkdir -p $(dir)))

clean:
	rm -rf $(BUILD_DIR)/*