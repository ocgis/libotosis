CFLAGS = @CFLAGS@ -Wall

osisdir = @OSISINCLUDE@
osis_HEADERS = \
	biosbind.h gemdosbind.h mintbind.h xbiosbind.h \
	libotosis.h libotosis_option.h libotosis_optiondef.h \
	libotosis_sysvars.h lineabind.h

lib_LTLIBRARIES = libotosis.la

bios_sources = \
	bios.h bios_chardev.c bios_dispatch.c bios_misc.c bios_vt52.c \
	biosbind.c

gemdos_sources = \
	gemdos.h gemdos_chardev.c gemdos_dispatch.c gemdos_file.c \
	gemdos_mem.c gemdos_proc.c gemdosbind.c

hwreg_sources = \
	hwreg_dispatch.c

linea_sources = \
	linea.c

mint_sources = \
	mint.h mint_dispatch.c mint_file.c mint_proc.c mintbind.c

xbios_sources = \
	xbios_dispatch.c xbios_hw.c xbiosbind.c

xgemdos_sources = \
	xgemdos.h xgemdos.c xgemdos_dispatch.c xgemdosbind.c

common_sources = \
	file_emu.c init.c init.h malloc.c optfile.c optiondef.c \
	scancode_us.c tty.c strace.c trace.c

common_headers = \
	bindproto.h client_endian.h div.h fd.h memory.h prototypes.h \
	toserrors.h traps.h version.h vt52.h xbios.h

BUILT_SOURCES =  optiondef.c libotosis_optiondef.h scancode_us.c

libotosis_la_SOURCES = \
	${bios_sources} \
	${gemdos_sources} \
	${hwreg_sources} \
	${linea_sources} \
	${mint_sources} \
	${xbios_sources} \
	${common_sources} \
	${common_headers}

libotosis_la_LDFLAGS = @LT_VERSION_INFO@

optiondef.c: ../tools/options_to_c.awk ../options.def
	$(AWK) -f ../tools/options_to_c.awk < ../options.def > optiondef.c

libotosis_optiondef.h: ../tools/options_to_h.awk ../options.def
	$(AWK) -f ../tools/options_to_h.awk < ../options.def > libotosis_optiondef.h

scancode_us.c: ../tools/make_scancode scancode.us
	../tools/make_scancode scancode.us > scancode_us.c

EXTRA_DIST =\
	${xgemdos_sources} scancode.us
