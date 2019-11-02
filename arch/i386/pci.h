#pragma once

#include <stdint.h>

typedef struct {
	uint32_t port_base;
	uint32_t interrupt;
	uint16_t bus;
	uint16_t device;
	uint16_t function;
	uint16_t vendor_id;
	uint16_t device_id;
	uint8_t class_id;
	uint8_t subclass_id;
	uint8_t interface_id;
	uint8_t revision;
	uint8_t header_type;
} pcidev_t;

extern uint32_t pci_inl(uint16_t, uint16_t, uint16_t, uint32_t);
extern uint16_t pci_inw(uint16_t, uint16_t, uint16_t, uint32_t);
extern uint8_t pci_inb(uint16_t, uint16_t, uint16_t, uint32_t);
extern void pci_outl(uint16_t, uint16_t, uint16_t, uint32_t, uint32_t);
extern void pci_outw(uint16_t, uint16_t, uint16_t, uint32_t, uint16_t);
extern void pci_outb(uint16_t, uint16_t, uint16_t, uint32_t, uint8_t);
extern void pci_chk_func(uint16_t,uint16_t,uint16_t);
extern void pci_chk_dev(uint16_t,uint16_t);
extern void pci_scan();
extern pcidev_t get_pcidev(uint16_t, uint16_t, uint16_t);
