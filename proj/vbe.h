#pragma once

#define VBE_INDEXED_MODE        0x105
#define VBE_DIRECT_MODE1        0x115
#define VBE_DIRECT_MODE2        0x11A
#define VBE_DIRECT_MODE3        0x11B
#define VBE_DIRECT_MODE4        0x14C

#define VBE_INT_VECTOR          0x10

#define VBE_GET_CONTR_INFO      0x4f00
#define VBE_GET_MODE_INFO       0x4f01
#define VBE_SET_MODE            0x4f02

#define VBE_FUNC_FAIL           0x01
#define VBE_FUNC_UNSUPPORTED    0x02
#define VBE_FUNC_INVALID        0x03
#define VBE_INT_VECTOR          0x10

#define VBE_SET_LIN_BUF         1 << 14

#pragma pack(1)
typedef struct {
	uint8_t VbeSignature[4];
	uint16_t VbeVersion;
	phys_bytes OemStringPtr;
	uint8_t Capabilities[4];
	phys_bytes VideoModePtr;
	uint16_t TotalMemory;

	uint16_t OemSoftwareRev;
	phys_bytes OemVendorNamePtr;
	phys_bytes OemProductNamePtr;
	phys_bytes OemProductRevPtr;
	uint8_t Reserved[222];
	uint8_t OemData[256];
} vbe_info_block;
#pragma options align=reset

unsigned long get_address(phys_bytes far_address, void* initial_address);

int vbe_get_controllerInfo(vg_vbe_contr_info_t *vci_p, void* initial_address);
int vbe_get_modeInfo(uint16_t mode, vbe_mode_info_t * vmi_p);
