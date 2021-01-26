PORT ?=
TASK1 = ./task-1/master

all:
	arduino-cli compile --fqbn arduino:avr:uno $(TASK1)

clean:
	rm -vrf $(TASK1)/build

flash: all
	arduino-cli upload --verify --port $(PORT) --fqbn arduino:avr:uno $(TASK1)

.PHONY: clean flash
