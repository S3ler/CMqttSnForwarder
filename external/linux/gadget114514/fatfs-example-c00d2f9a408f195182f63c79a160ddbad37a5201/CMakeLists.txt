cmake_minimum_required(VERSION 3.10)

set(FATFS_FILES
        fatfs/source/diskio.c fatfs/source/diskio.h
        fatfs/source/ff.c fatfs/source/ff.h
        fatfs/source/ffconf.h
        fatfs/source/ffsystem.c
        fatfs/source/ffunicode.c
        fatfs/source/integer.h)

add_executable(gadget114514_cf cf.c ${FATFS_FILES})
add_executable(gadget114514_cf2 cf2.c ${FATFS_FILES})
add_executable(gadget114514_fmkfs fmkfs.c ${FATFS_FILES})
add_executable(gadget114514_mkd mkd.c ${FATFS_FILES})
add_executable(gadget114514_rd rd.c ${FATFS_FILES})
add_executable(gadget114514_rf rf.c ${FATFS_FILES})
add_executable(gadget114514_rf2 rf2.c ${FATFS_FILES})
add_executable(gadget114514_xf xf.c ${FATFS_FILES})
add_executable(gadget114514_df df.c ${FATFS_FILES})


