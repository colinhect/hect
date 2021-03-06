project(PhysFS C)

add_library(PhysFS STATIC
    "physfs.c"
    "physfs.h"
    "physfs_byteorder.c"
    "physfs_casefolding.h"
    "physfs_internal.h"
    "physfs_platforms.h"
    "physfs_unicode.c"
    "archivers/dir.c"
    "archivers/grp.c"
    "archivers/hog.c"
    "archivers/lzma.c"
    "archivers/mvl.c"
    "archivers/qpak.c"
    "archivers/wad.c"
    "archivers/zip.c"
    "platform/windows.c"
    "platform/unix.c"
    "platform/posix.c"
    "platform/pocketpc.c"
    "platform/os2.c"
    "platform/macosx.c"
    "platform/beos.cpp"
    "zlib123/adler32.c"
    "zlib123/compress.c"
    "zlib123/crc32.c"
    "zlib123/crc32.h"
    "zlib123/deflate.c"
    "zlib123/deflate.h"
    "zlib123/gzio.c"
    "zlib123/infback.c"
    "zlib123/inffast.c"
    "zlib123/inffast.h"
    "zlib123/inffixed.h"
    "zlib123/inflate.c"
    "zlib123/inflate.h"
    "zlib123/inftrees.c"
    "zlib123/inftrees.h"
    "zlib123/trees.c"
    "zlib123/trees.h"
    "zlib123/uncompr.c"
    "zlib123/zconf.h"
    "zlib123/zlib.h"
    "zlib123/zutil.c"
    "zlib123/zutil.h"
    )

target_include_directories(PhysFS PUBLIC ${PROJECT_SOURCE_DIR})

if(MSVC)
    target_compile_options(PhysFS PRIVATE /W0)
else()
    target_compile_options(PhysFS PRIVATE -w -fPIC)
endif()

target_compile_definitions(PhysFS PRIVATE PHYSFS_NO_CDROM_SUPPORT)

set_target_properties(PhysFS PROPERTIES
    PROJECT_LABEL PhysFS
    FOLDER "/Engine/Dependencies"
    LINKER_LANGUAGE C
    )
