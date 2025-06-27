/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_F2FS_H
#define _LINUX_F2FS_H

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/atomic.h>
#include <linux/mutex.h>
#include <linux/spinlock.h>
#include <linux/rwsem.h>
#include <linux/list.h>
#include <linux/wait.h>
#include <linux/pagevec.h>
#include <linux/slab.h>
#include <linux/uio.h>
#include <linux/time.h>

struct f2fs_sb_info;
struct extent_info;
struct inode;

/* ======================== */
/* F2FS Statistic Structure */
/* ======================== */
struct f2fs_stat_info {
	atomic_t inode_count;
	atomic_t dirty_inode;
	atomic_t dirty_node;
	atomic_t dirty_meta;
	atomic_t total_hit_ext;
	atomic_t read_hit_rbtree;
	atomic_t read_hit_largest;
	atomic_t read_hit_cached;
	atomic_t inline_inode;
	atomic_t inline_dir;
	atomic_t inline_xattr;
	atomic_t aw_cnt;
	atomic_t volatile_write;  /* 🔧 Tambahan untuk volatile support */
};

/* ======================== */
/* FI_* Flags untuk inode   */
/* ======================== */
enum {
	FI_NEW_INODE,
	FI_DIRTY_FILE,
	FI_INLINE_DATA,
	FI_APPEND_WRITE,
	FI_UPDATE_WRITE,
	FI_DIRTY_DIR,
	FI_INC_LINK,
	FI_ACL_MODE,
	FI_NO_ALLOC,
	FI_FREE_NID,
	FI_DIRTY_INODE,
	FI_NO_EXTENT,
	FI_NO_COMPR_FILE,
	FI_COMPRESSED_FILE,
	FI_COMPRESS_CORRUPT,
	FI_MMAP_FILE,
	FI_VOLATILE_FILE, /* 🔧 Diperlukan untuk volatile file */
	FI_ENABLE_COMPRESS,
	FI_COMPRESS_RELEASED,
	FI_ALIGNED_WRITE,
	FI_MAX,
};

/* Dummy struktur inode info */
struct f2fs_inode_info {
	unsigned long i_flags;
	/* Tambahan field internal jika perlu */
};

#define F2FS_I(inode)		((struct f2fs_inode_info *)(inode))
#define F2FS_I_SB(inode)	((struct f2fs_sb_info *)((inode)->i_sb->s_fs_info))

/* ======================== */
/* Flag Checking Macro      */
/* ======================== */
static inline int is_inode_flag_set(struct f2fs_inode_info *fi, int flag)
{
	return test_bit(flag, &fi->i_flags);
}

/* ======================== */
/* Volatile File Helpers    */
/* ======================== */
static inline bool f2fs_is_volatile_file(struct inode *inode)
{
	return is_inode_flag_set(F2FS_I(inode), FI_VOLATILE_FILE);
}

static inline void drop_inmem_pages(struct inode *inode)
{
	/* Kosongkan jika tidak digunakan */
}

static inline void stat_dec_volatile_write(struct inode *inode)
{
	atomic_dec(&F2FS_I_SB(inode)->stat_info.volatile_write);
}

#endif /* _LINUX_F2FS_H */
