.PHONY: all boot app

all: boot app clean

boot:
	@$(MAKE) -C boot/ all

app: boot
	@$(MAKE) -C app/ all

clean:
	@$(MAKE) -C boot/ clean
	@$(MAKE) -C app/ clean
