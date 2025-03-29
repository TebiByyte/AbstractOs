#include <disk/part.h>
#include <devices/drive/ide.h>
#include <mem_mgt.h>

//Assume this exists on a single IDE drive
part_tbl_entry get_entry(uint32 index, part_tbl_head *gpt_header){
    uint32 gpt_start_lba = gpt_header->part_tbl_start;
    uint32 entry_lba = (index * gpt_header->part_entry_size) / 512;
    uint32 offset = index % (512 / gpt_header->part_entry_size);

    uint8 read_buffer[512];
    
    bool read_success = ide_read_sectors(IDE_PRIMARY, 0, 1, gpt_start_lba + entry_lba, (void*)read_buffer);
    part_tbl_entry result;

    if (read_success){
        part_tbl_entry *read_ptr = (part_tbl_entry*)(read_buffer) + offset;

        mem_cpy((void*)read_ptr, &result, sizeof(part_tbl_entry));
    } else {
        result.end_lba = 0;
        result.start_lba = 0;
        //Error condition
    }

    return result;
}
