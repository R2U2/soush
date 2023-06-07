include ../config.mk

CSRC = $(shell find * -name "*.c")
COUT = $(patsubst %.c,../obj/%.o,$(CSRC))

$(COUT): ../obj/%.o: %.c 
	@mkdir -p $(dir $@)
	@echo "CC\t$(patsubst ../obj/%.o,%,$@)"
	@$(CC) $(CFLAGS) -c $(patsubst ../obj/%.o,%.c,$@) -o $@
build: $(COUT)
	@echo "CCLD\tsoush"
	@$(CC) $(CFLAGS) $(LDFLAGS) $(COUT) -o ../soush