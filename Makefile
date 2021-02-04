PORT ?=
SKETCH ?=

all:
	arduino-cli compile --fqbn arduino:avr:uno $(SKETCH)

clean:
	rm -vrf $(SKETCH)/build

flash: all
	arduino-cli upload --verify --port $(PORT) --fqbn arduino:avr:uno $(SKETCH)

.PHONY: clean flash
