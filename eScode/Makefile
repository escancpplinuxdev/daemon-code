# Makefile for MBR Crypto Kernel Module

obj-m += escanmbr.o

# Kernel build directory (adjust for your system)
KDIR := /lib/modules/$(shell uname -r)/build
#KDIR := /lib/modules/6.8.0-86-generic/build
PWD := $(shell pwd)

# Default target
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Clean target
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f *.o *.ko *.mod.* *.symvers *.order

# Install target
install: all
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
	depmod -a

# Load module
load:
	sudo insmod escanmbr.ko encryption_key="$(KEY)"

# Unload module
unload:
	sudo rmmod escanmbr

# Show module info
info:
	modinfo escanmbr.ko

# Check kernel messages
log:
	sudo dmesg | tail -50 | grep escanmbr

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build the kernel module"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install module to system"
	@echo "  load     - Load module (use KEY=<hex> for custom key)"
	@echo "  unload   - Unload module"
	@echo "  info     - Show module information"
	@echo "  log      - Show kernel log messages"
	@echo ""
	@echo "Example usage:"
	@echo "  make"
	@echo "  sudo make load KEY=0123456789abcdef0123456789abcdef"
	@echo "  sudo make unload"

.PHONY: all clean install load unload info log help
