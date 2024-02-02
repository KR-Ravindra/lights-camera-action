INCFLAGS  = -I /usr/include/GL
INCFLAGS += -I include/vecmath

LINKFLAGS  = -lglut -lGL -lGLU
LINKFLAGS  += -L lib/vecmath -l:libvecmath.a

CFLAGS    = -O2
CC        = g++
SRCS      = main.cpp
OBJS      = $(SRCS:.cpp=.o)
PROG      = assignment_0

all: $(SRCS) $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(INCFLAGS) $(LINKFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@ $(INCFLAGS)

depend:
	makedepend $(INCFLAGS) -Y $(SRCS)

clean:
	rm $(OBJS) $(PROG)

