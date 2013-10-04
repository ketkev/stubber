CC             = g++
INCLUDES       = -Iinclude -IC:/msys/include
CFLAGS         = $(INCLUDES) -O2 -c -Wall -pedantic -std=c++11 -fexec-charset=UTF-8 -finput-charset=UTF-8 -D_GLIBCXX_HAVE_BROKEN_VSWPRINTF

LIB_PATHS      = -LC:/msys/lib
LIBS           = 
LDFLAGS        = ${LIB_PATHS} ${LIBS}


find = $(foreach dir,$(1),$(foreach d,$(wildcard $(dir)/*),$(call find,$(d),$(2))) $(wildcard $(dir)/$(strip $(2))))

SOURCES_LIB       = $(call find, src, *.cpp)
OBJECTS_LIB       = $(SOURCES_LIB:.cpp=.o)
STATIC_TARGET     = lib/libstub.a


all: $(SOURCES) $(STATIC_TARGET)


$(STATIC_TARGET): $(OBJECTS_LIB)
	ar rcs $@ $(OBJECTS_LIB)


.cpp.o:
	$(CC) ${CFLAGS} $< -o $@


clean:
	rm -f $(OBJECTS_LIB)
	rm -f $(STATIC_TARGET)
	rm -f gmon.out

