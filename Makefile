# Configurations
# --------------

# compiler
CXX ?= g++
CXXFLAGS = -Wall -std=c++11
CXXFLAGS += -fPIC

# Contents
# --------

BINDIR = bin
BINARIES = insert_postgres

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
CACHED_SIGMC_PATHNAME = $(META_ENCODER_ROOT)/cached/sigmc_ml_assignment.csv
CACHED_GENERIC_PATHNAME = $(META_ENCODER_ROOT)/cached/generic_ml_assignment.csv
CACHED_EVENTWEIGHTS_PATHNAME = $(META_ENCODER_ROOT)/cached/event_weights.csv
CXXFLAGS += -D__CACHED_SIGMC_PATHNAME='"$(CACHED_SIGMC_PATHNAME)"'
CXXFLAGS += -D__CACHED_GENERIC_PATHNAME='"$(CACHED_GENERIC_PATHNAME)"'
CXXFLAGS += -D__CACHED_EVENTWEIGHTS_PATHNAME='"$(CACHED_EVENTWEIGHTS_PATHNAME)"'

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

# bta postgres
BTA_POSTGRES_ROOT = .
INCFLAGS += -I$(BTA_POSTGRES_ROOT)
LDFLAGS += -L$(BTA_POSTGRES_ROOT)/lib \
					 -lBtaPostgres \
					 -Wl,-rpath,$(BTA_POSTGRES_ROOT)/lib


# Build Rules
# -----------

.PHONY: all debug clean 

$@ : $@.cc
	$(CXX) $(CXXFLAGS) $< -o $%

all : CXXFLAGS += -O3
all : $(BINARIES)

debug : CXX += -DDEBUG -g
debug : $(BINARIES)

$(BINARIES) : % : %.cc
	$(CXX) $(CXXFLAGS) $(INCFLAGS) $(LDFLAGS) $< -o $(BINDIR)/$@

clean:
	rm -f *~ *.o $(addprefix $(BINDIR)/, $(BINARIES))
