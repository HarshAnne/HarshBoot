.PHONY: all boot loader app clean

all: boot loader app

boot:
	@$(MAKE) -C boot/ all

loader:
	@$(MAKE) -C loader/ all

app: boot loader
	@$(MAKE) -C app/ all

clean:
	@$(MAKE) -C boot/ clean
	@$(MAKE) -C loader/ clean
	@$(MAKE) -C app/ clean
