# Configurations
# --------------

# compiler
CXX ?= g++
CXXFLAGS = -Wall -std=c++11 
CXXFLAGS += -fPIC

# library name
LIBNAME = libBtaPostgres.so

# path to compiled libaries
PKG_LIBPATH = ../lib

# package contents
SOURCES = pq_wrappers.cc TableInserterPQ.cc \
          EventMetaInserterPQ.cc EventStatusInserterPQ.cc \
					EventInserterPQ.cc McEventInserterPQ.cc \
					CandidateMetaInserterPQ.cc \
					CandidateInserterPQ.cc McCandidateInserterPQ.cc 

# Dependencies
# ------------

# libpq
INCFLAGS += -I$(shell pg_config --includedir)
LDFLAGS += -L$(shell pg_config --libdir) -lpq

# boost
INCFLAGS += -I$(BOOST_ROOT)

# cern root
INCFLAGS += $(shell root-config --cflags)
LDFLAGS += $(shell root-config --libs)

# meta data encoder
META_ENCODER_ROOT = /Users/dchao/bdtaunu/bdtaunu_meta_encoder
INCFLAGS += -I$(META_ENCODER_ROOT)
LDFLAGS += -L$(META_ENCODER_ROOT)/lib \
					 -lMetaEncoder \
					 -Wl,-rpath,$(META_ENCODER_ROOT)/lib

# bta tuple reader
TUPLE_READER_ROOT = /Users/dchao/bdtaunu/bta_tuple_reader
INCFLAGS += -I$(TUPLE_READER_ROOT)
LDFLAGS += -L$(TUPLE_READER_ROOT)/lib \
					 -lTupleReader \
					 -Wl,-rpath,$(TUPLE_READER_ROOT)/lib

# custom cpp utilities
CUSTOM_CPP_UTIL_ROOT = /Users/dchao/bdtaunu/custom_cpp_utilities
INCFLAGS += -I$(CUSTOM_CPP_UTIL_ROOT)
LDFLAGS += -L$(CUSTOM_CPP_UTIL_ROOT)/lib \
					 -lCustomCPPUtils \
					 -Wl,-rpath,$(CUSTOM_CPP_UTIL_ROOT)/lib

# Build Rules
# -----------

.PHONY: all debug lib clean distclean

OBJECTS = $(addprefix $(PKG_LIBPATH)/, $(patsubst %.cc, %.o, $(SOURCES)))
DEPENDENCIES = $(addprefix $(PKG_LIBPATH)/, $(patsubst %.cc, %.d, $(SOURCES)))

all : CXXFLAGS += -O3
all : lib

debug : CXX += -DDEBUG -g
debug : CXXFLAGS += -O0
debug : lib

lib : $(PKG_LIBPATH)/$(LIBNAME)

$(PKG_LIBPATH)/$(LIBNAME) : $(OBJECTS)
	if [ "$(shell uname)" = "Darwin" ]; then \
		SHARED_LIB_FLAG="-dynamiclib -Wl,-install_name,@rpath/$(LIBNAME)"; \
	else \
		SHARED_LIB_FLAG="-shared -Wl,-soname,$(LIBNAME)"; \
	fi; \
	$(CXX) $${SHARED_LIB_FLAG} $(OBJECTS) $(LDFLAGS) -o $@

$(OBJECTS) : $(PKG_LIBPATH)/%.o : %.cc %.h $(PKG_LIBPATH)/%.d
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -c $< -o $@

$(DEPENDENCIES) : $(PKG_LIBPATH)/%.d : %.cc %.h Makefile
	$(CXX) $(CXXFLAGS) $(INCFLAGS) -MM -MT $(PKG_LIBPATH)/$*.o -c $< -o $@

clean:
	rm -f *~ $(PKG_LIBPATH)/$(LIBNAME) $(OBJECTS)

distclean:
	rm -f *~ $(PKG_LIBPATH)/$(LIBNAME) $(OBJECTS) $(DEPENDENCIES)

-include $(DEPENDENCIES)

