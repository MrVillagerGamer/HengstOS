#include <arch/i386/pci.h>
#include <arch/i386/port.h>

#define PCI_COMMAND_PORT 0xCF8
#define PCI_DATA_PORT 0xCFC

pcidev_t pci_devices[256*32*8];
uint32_t pci_device_count = 0;

uint32_t pci_inl(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset) {
	uint32_t id = 0x1 << 31
		| ((bus & 0xff) << 16)
		| ((device & 0x1F) << 11)
		| ((function & 0x07) << 8)
		| (offset & 0xFC);
	
	outl(PCI_COMMAND_PORT, id);
	uint32_t result = inl(PCI_DATA_PORT);
	return result >> (8 * (offset % 4));
}

uint16_t pci_inw(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset) {
	return (uint16_t)pci_inl(bus, device, function, offset);
}

uint8_t pci_inb(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset) {
	return (uint8_t)pci_inl(bus, device, function, offset);
}

void pci_outl(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset, uint32_t value) {
	uint32_t id = 0x1 << 31
		| ((bus & 0xff) << 16)
		| ((device & 0x1F) << 11)
		| ((function & 0x07) << 8)
		| (offset & 0xFC);
	
	outl(PCI_COMMAND_PORT, id);
	outl(PCI_DATA_PORT, value);
}

void pci_outw(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset, uint16_t value) {
	pci_outl(bus, device, function, offset, value);
}

void pci_outb(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset, uint8_t value) {
	pci_outl(bus, device, function, offset, value);
}

void pci_chk_func(uint16_t bus, uint16_t device, uint16_t function) {
	pcidev_t dev = get_pcidev(bus, device, function);
	if(dev.class_id == 0x01) {
		if(dev.subclass_id == 0x01) {
			
		}
	}
}

void pci_chk_dev(uint16_t bus, uint16_t device) {
	uint16_t function = 0;
	pcidev_t dev1 = get_pcidev(bus, device, function);
	if(dev1.vendor_id == 0xFFFF) {
		return;
	}
	pci_chk_func(bus, device, function);
	if((dev1.header_type & 0x80) != 0) {
		for(function = 1; function < 8; function++) {
			dev1 = get_pcidev(bus, device, function);
			if(dev1.vendor_id != 0xFFFF) {
				pci_chk_func(bus, device, function);
			}
		}
	}
}

void pci_scan() {
	for(uint16_t bus = 0; bus < 256; bus++) {
		for(uint16_t device = 0; device < 32; device++) {
			pci_chk_dev(bus, device);
		}
	}
}

pcidev_t get_pcidev(uint16_t bus, uint16_t device, uint16_t function)
{
	pcidev_t result;
	result.bus = bus;
	result.device = device;
	result.function = function;
	
	result.vendor_id = pci_inl(bus, device, function, 0x00);
	result.device_id = pci_inl(bus, device, function, 0x02);
	
	result.class_id = pci_inl(bus, device, function, 0x0b);
	result.subclass_id = pci_inl(bus, device, function, 0x0a);
	result.interface_id = pci_inl(bus, device, function, 0x09);
	
	result.revision = pci_inl(bus, device, function, 0x08);
	result.interrupt = pci_inl(bus, device, function, 0x3C);
	result.header_type = pci_inl(bus, device, function, 0x0E);
	
	return result;
}