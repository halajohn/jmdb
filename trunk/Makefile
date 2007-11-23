# Jmdb - A J2ME debugger with Emacs interface
# Copyright (C) <2007>  Wei Hu <wei.hu.tw@gmail.com>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

export BUILD_ROOT_DIR = $(PWD)
export BUILD_MAK_DIR  = $(BUILD_ROOT_DIR)/mak

include $(BUILD_MAK_DIR)/config.mak

all:
	@$(MAKE) build_stage1
	@$(MAKE) build_stage2
	@$(MAKE) build_stage3

build_stage1: check_dir produce_depend
build_stage2: produce_objfile produce_i18n_files
build_stage3: $(PACKAGE_EXEC_FILE)

###############################################################################
# Stage 1
###############################################################################

check_dir:
	@if [ ! -e $(BUILD_OBJ_DIR) ]; then \
		$(MKDIR) $(BUILD_OBJ_DIR); \
	fi
	@if [ ! -e $(BUILD_BIN_DIR) ]; then \
		$(MKDIR) $(BUILD_BIN_DIR); \
	fi
	@if [ ! -e $(BUILD_I18N_DIR) ]; then \
		$(MKDIR) -p $(BUILD_I18N_DIR); \
	fi

produce_depend:
	@for i in $(BUILD_SRC_DIR); do \
		cd $$i; \
		$(MAKE) depend || exit 1; \
		cd $(BUILD_ROOT_DIR); \
	done

###############################################################################
# Stage 2
###############################################################################

produce_objfile:
	@for i in $(BUILD_SRC_DIR); do \
		cd $$i; \
		$(MAKE) || exit 1; \
		cd $(BUILD_ROOT_DIR); \
	done

produce_i18n_files:
	@cd $(BUILD_I18N_DIR); \
	$(MAKE) || exit 1
	@cd $(BUILD_ROOT_DIR)

###############################################################################
# Stage 3 - Link Custom Executable
###############################################################################

$(PACKAGE_EXEC_FILE): $(OBJFILES)
	@$(ECHO) " --- Linking jmdb"
	@$(CXX) -o $(PACKAGE_EXEC_FILE) $(OBJFILES) $(LDFLAGS)

install:
	@$(ECHO) " --- Installing jmdb Binary... "
	@if [ ! -e $(INSTALL_BIN_DIR) ]; then \
		$(MKDIR) -p $(INSTALL_BIN_DIR); \
	fi
	@$(CP) $(PACKAGE_EXEC_FILE) $(INSTALL_BIN_DIR)
	@cd $(BUILD_I18N_DIR); $(MAKE) install; cd $(BUILD_ROOT_DIR)

clean:
	@for i in $(BUILD_SRC_DIR); do \
		cd $$i; \
		$(MAKE) clean || exit 1; \
		cd $(BUILD_ROOT_DIR); \
	done
	@cd $(BUILD_I18N_DIR); $(MAKE) clean || exit 1; cd $(BUILD_ROOT_DIR)
	@$(RM) -rf $(BUILD_OBJ_DIR) $(BUILD_BIN_DIR)

include $(BUILD_MAK_DIR)/rules.mak
