#ifndef __EXT2H__
#define __EXT2H__

#include <common/type.h>

#define EXT2_SIGNATURE 0xEF53

typedef struct __attribute__((__packed__)) ext2_superblock {
    uint32 inodes_count;
    uint32 blocks_count;
    uint32 r_blocks_count;
    uint32 free_blocks_count;
    uint32 free_inodes_count;
    uint32 first_data_block_size;
    uint32 log_block_size;
    uint32 log_frag_size;
    uint32 blocks_per_group;
    uint32 frags_per_group;
    uint32 inodes_per_group;
    uint32 last_mount_time;
    uint32 last_write_time;
    uint16 mnt_count;
    uint16 max_mnt_count;
    uint16 magic_sig;
    uint16 state;
    uint16 errors;
    uint16 mnr_rev_lvl;
    uint32 last_check;
    uint32 check_interval;
    uint32 creator_os;
    uint32 rev_lvl;
    uint16 def_resuid;
    uint16 def_resgid;
} ext2_superblock;

ext2_superblock get_super_block(uint32 vol_start_lba);

#endif
