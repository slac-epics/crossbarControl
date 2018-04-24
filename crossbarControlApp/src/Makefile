TOP=../..

include $(TOP)/configure/CONFIG

USR_CXXFLAGS += -std=c++0x
USR_CXXFLAGS += -DHAVE_YAML
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#========================================
INCLUDES += $(addprefix -I,$(YAML_INCLUDE))
INCLUDES += $(addprefix -I,$(BOOST_INCLUDE))

## this part is just for yaml loader driver
INCLUDES += $(addprefix -I,$(CPSW_FRAMEWORK_INCLUDE))



#INC += 
DBD += crossbarControl.dbd

LIBRARY_IOC = crossbarControl

crossbarControl_DBD  += crossbarControl.dbd
crossbarControl_SRCS += crossbarControl.cpp
crossbarControl_SRCS += crossbarControlYaml.cc

yaml-cpp_DIR += $(YAML_LIB)
crossbarControl_LIBS += yaml-cpp

cpsw_DIR = $(CPSW_FRAMEWORK_LIB)
crossbarControl_LIBS += cpsw


tprPattern_LIBS += asyn
tprPattern_LIBS += yamlLoader

tprPattern_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

