
TARGETS = control_station soldier

SOURCES = $(TARGETS:=.c)

all: $(TARGETS)

$(TARGETS): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $@.c

clean:
	rm -f $(TARGETS)

