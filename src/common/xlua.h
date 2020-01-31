/*****************************************************************************\
 *  xlua.h - Lua integration common functions
 *****************************************************************************
 *  Copyright (C) 2015 SchedMD LLC.
 *  Written by Tim Wickberg <tim@schedmd.com>
 *
 *  This file is part of Slurm, a resource management program.
 *  For details, see <https://slurm.schedmd.com/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  Slurm is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  Slurm is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Slurm; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#ifndef _SLURM_XLUA_H
#define _SLURM_XLUA_H

#include <dlfcn.h>
#include <stdio.h>
#include "config.h"

#ifdef HAVE_LUA
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#endif

#include "slurm/slurm.h"
#include "slurm/slurm_errno.h"
#include "src/common/log.h"
#include "src/slurmctld/slurmctld.h"

int xlua_dlopen();

#ifdef HAVE_LUA
/*
 * function for create a new lua state object, loading a lua script, setting up
 * basic slurm/lua integration and returning the lua state to the caller.
 * If the script mtime is greater than *load_time, a new lua state will be
 * allocated and configured, the caller should close the old one after
 * completing any remaining setup.
 *
 * Parameters:
 * curr (in)   - current lua state object, should be NULL on first call
 * plugin (in) - string identifying the calling plugin, e.g. "job_submit/lua"
 * script_path (in) - path to script file
 * req_fxns (in) - NULL terminated array of functions that must exist in the
 *                 script
 * load_time (in/out) - mtime of script from the curr lua state object
 *
 * Returns:
 * pointer to new lua_State object - the caller should complete setup of the
 *                                   new environment, and possibly free any
 *                                   existing.
 * NULL -- an error occured, the caller should continue using the current object
 * same value as curr - no error, or a strong suggestion that the caller should
 *                      continue using the same state obj, with no further setup
 */
lua_State *xlua_loadscript(lua_State *curr, const char *plugin,
			   const char *script_path, const char **req_fxns,
			   time_t *load_time);
void xlua_register_slurm_output_functions(lua_State *L);
void xlua_table_register(lua_State *L, const char *libname,
				const luaL_Reg *l);
int xlua_job_record_field(lua_State *L, const job_record_t *job_ptr,
			  const char *name);
void xlua_stack_dump (const char *plugin, char *header, lua_State *L);
#endif

#endif
