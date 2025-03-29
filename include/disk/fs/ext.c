#include <disk/fs/ext.h>
#include <common/type.h>
#include <devices/drive/ide.h>
#include <mem_mgt.h>
#include <typeout.h>

ext2_superblock get_super_block(uint32 vol_start_lba){
    uint8 disk_buffer[512];//NOTE: There is a weird thing going on here with the stack, if the result is defined first, the mem_cpy clobbers the stack. Idk why. 
    ext2_superblock result;

    bool read_success = ide_read_sectors(IDE_PRIMARY, 0, 1, vol_start_lba + 2, (void*)disk_buffer);

    if (read_success){
        mem_cpy((void*)disk_buffer, &result, sizeof(ext2_superblock));
    } else {
        //Error value
        result.magic_sig = 0x0;
    }

    return result;
}
