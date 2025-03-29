#ifndef __PART__H__
#define __PART__H__

#include <common/type.h>

typedef struct __attribute__((__packed__)) part_tbl_head{
    uint8 sig[8]; // "EFI PART"
    uint32 gpt_rev;
    uint32 header_size;
    uint32 ck_sum;
    uint32 reserved;
    uint64 header_lba;
    uint64 alt_header_lba;
    uint64 first_gpt_lba;
    uint64 last_gpt_lba;
    uint8 disk_guid[16];
    uint64 part_tbl_start;
    uint32 num_entries;
    uint32 part_entry_size;
    uint32 crc32_part_tbl;
} part_tbl_head;

typedef struct __attribute__((__packed__)) part_tbl_entry{
    uint8 part_type_guid[16];
    uint8 part_guid[16];
    uint64 start_lba;
    uint64 end_lba;
    uint64 attrs;
    uint8 part_name[72];
} part_tbl_entry;

part_tbl_entry get_entry(uint32 index, part_tbl_head *gpt_header);
#endif
