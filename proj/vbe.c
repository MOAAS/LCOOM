#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>

#include "vbe.h"

unsigned long get_address(phys_bytes far_address, void* initial_address) {
    unsigned long address = far_address;
    return ((address >> 16) * 0x10) + (address & 0x0FFFF) + (unsigned long)initial_address;
}

int vbe_get_modeInfo(uint16_t mode, vbe_mode_info_t * vmi_p) {
    phys_bytes buf;
    mmap_t map;

    if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL) {
        printf("lm_alloc failed!\n");
		return 1;
    }
    buf = map.phys;

    struct reg86u reg;
    memset(&reg, 0, sizeof(reg));	// zero the structure
    reg.u.w.ax = VBE_GET_MODE_INFO;
    reg.u.w.es = PB2BASE(buf); /* set a segment base */
    reg.u.w.di = PB2OFF(buf); /* set the offset accordingly */
    reg.u.w.cx = mode;
    reg.u.b.intno = VBE_INT_VECTOR;
    if( sys_int86(&reg) != OK ) {
        printf("set_vbe_mode: sys_int86() failed \n");
        lm_free(&map);
        return 1;
    }
    if (reg.u.b.ah == VBE_FUNC_FAIL) {
		printf("Function call failed!\n");
		lm_free(&map);
		return 1;
	}
	else if (reg.u.b.ah == VBE_FUNC_UNSUPPORTED) {
		printf("Function is not supported in current HW configuration!\n");
		lm_free(&map);
		return 1;
	}
	else if (reg.u.b.ah == VBE_FUNC_INVALID) {
		printf("Function is invalid in current video mode!\n");
		lm_free(&map);
		return 1;
	}
    *vmi_p = *(vbe_mode_info_t*) map.virt;
    lm_free(&map);
    return 0;
}

int vbe_get_controllerInfo(vg_vbe_contr_info_t *vci_p, void* init_address){
    phys_bytes buf;
	mmap_t map;
	if (lm_alloc(sizeof(vbe_info_block), &map) == NULL) {
        printf("lm_alloc failed!\n");
		return 1;
	}
    buf = map.phys;
	struct reg86u reg;
    memset(&reg, 0, sizeof(reg));	// zero the structure

	vbe_info_block VBE_Info;
	memcpy(VBE_Info.VbeSignature, "VBE2", 4);

	reg.u.w.ax = VBE_GET_CONTR_INFO;
	reg.u.w.es = PB2BASE(buf); 
	reg.u.w.di = PB2OFF(buf);
	reg.u.b.intno = VBE_INT_VECTOR;
	if( sys_int86(&reg) != OK ) { 
        printf("set_vbe_mode: sys_int86() failed \n");
		lm_free(&map); //free memory
		return 1;
	}
    if (reg.u.b.ah == VBE_FUNC_FAIL) {
		printf("Function call failed!\n");
		lm_free(&map);
		return 1;
	}
	else if (reg.u.b.ah == VBE_FUNC_UNSUPPORTED) {
		printf("Function is not supported in current HW configuration!\n");
		lm_free(&map);
		return 1;
	}
	else if (reg.u.b.ah == VBE_FUNC_INVALID) {
		printf("Function is invalid in current video mode!\n");
		lm_free(&map);
		return 1;
	}
	VBE_Info = *(vbe_info_block*) map.virt; //virtual memory

	memcpy(vci_p->VBESignature, VBE_Info.VbeSignature, 4);
    vci_p->VBEVersion[0] = VBE_Info.VbeVersion;
    vci_p->VBEVersion[1] = VBE_Info.VbeVersion >> 8;
    vci_p->TotalMemory = VBE_Info.TotalMemory * 64; // VBE_Info.TotalMemory = Number of 64 KB blocks
    vci_p->OEMString = (char*)get_address(VBE_Info.OemStringPtr, init_address);
    vci_p->VideoModeList = (uint16_t*)get_address(VBE_Info.VideoModePtr, init_address);
    vci_p->OEMVendorNamePtr = (char*)get_address(VBE_Info.OemVendorNamePtr, init_address);
    vci_p->OEMProductNamePtr = (char*)get_address(VBE_Info.OemProductNamePtr, init_address);
    vci_p->OEMProductRevPtr = (char*)get_address(VBE_Info.OemProductRevPtr, init_address);
	lm_free(&map); //free memory
	return 0;
}
