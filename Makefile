###############################################################################
# EDIT
###############################################################################

CEU_DIR ?= $(error set absolute path to "<ceu>" repository)
C_FLAGS += -llua5.3		# samples/fs-03.ceu

###############################################################################
# DO NOT EDIT
###############################################################################

UV_DIR ?= .
ARCH_DIR ?= $(UV_DIR)/arch
include $(CEU_DIR)/Makefile

ifneq ($(MAKECMDGOALS),link)
ifeq ("$(wildcard $(UV_DIR)/arch/up)","")
$(error run "make link")
endif
endif

link:
	rm -f arch/up
	ln -s `readlink -f $(CEU_DIR)/arch` $(UV_DIR)/arch/up
