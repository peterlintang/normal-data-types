
SRCS := \
	common/conv.c	\
	common/crc32.c	\
	common/custalloc.c	\
	common/stream.c	\
	common/unarr.c	\
	lzmasdk/7zArcIn.c  \
	lzmasdk/7zStream.c  \
	lzmasdk/Bra.c      \
	lzmasdk/Lzma2Dec.c  \
	lzmasdk/Ppmd7.c     \
	lzmasdk/Ppmd8Dec.c \
	lzmasdk/7zBuf.c    \
	lzmasdk/Bcj2.c      \
	lzmasdk/CpuArch.c  \
	lzmasdk/LzmaDec.c   \
	lzmasdk/Ppmd7Dec.c\
	lzmasdk/7zDec.c    \
	lzmasdk/Bra86.c     \
	lzmasdk/Delta.c    \
	lzmasdk/Ppmd7aDec.c  \
	lzmasdk/Ppmd8.c		\
	_7z/_7z.c	\
	rar/filter-rar.c  \
	rar/huffman-rar.c  \
	rar/parse-rar.c  \
	rar/rar.c  \
	rar/rarvm.c  \
	rar/uncompress-rar.c	\
	tar/parse-tar.c  \
	tar/tar.c		\
	zip/inflate.c  \
	zip/parse-zip.c  \
	zip/uncompress-zip.c  \
	zip/zip.c


all:
	gcc main.c $(SRCS) -I./ -I./_7z -I./common -I./lzmasdk
clean:
	rm a.out
