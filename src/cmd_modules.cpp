/*       +------------------------------------+
 *       | Inspire Internet Relay Chat Daemon |
 *       +------------------------------------+
 *
 *  Inspire is copyright (C) 2002-2005 ChatSpike-Dev.
 *                       E-mail:
 *                <brain.net>
 *                <Craig.net>
 *
 * Written by Craig Edwards, Craig McLure, and others.
 * This program is free but copyrighted software; see
 *            the file COPYING for details.
 *
 * ---------------------------------------------------
 */

using namespace std;

#include "inspircd_config.h"
#include "inspircd.h"
#include "inspircd_io.h"
#include <time.h>
#include <string>
#ifdef GCC3
#include <ext/hash_map>
#else
#include <hash_map>
#endif
#include <map>
#include <sstream>
#include <vector>
#include <deque>
#include "users.h"
#include "ctables.h"
#include "globals.h"
#include "modules.h"
#include "dynamic.h"
#include "wildcard.h"
#include "message.h"
#include "commands.h"
#include "mode.h"
#include "xline.h"
#include "inspstring.h"
#include "dnsqueue.h"
#include "helperfuncs.h"
#include "hashcomp.h"
#include "socketengine.h"
#include "typedefs.h"
#include "command_parse.h"
#include "cmd_modules.h"

extern ServerConfig* Config;
extern InspIRCd* ServerInstance;
extern int MODCOUNT;
extern std::vector<Module*> modules;
extern std::vector<ircd_module*> factory;
extern time_t TIME;
extern user_hash clientlist;
extern chan_hash chanlist;
extern whowas_hash whowas;
extern std::vector<userrec*> all_opers;
extern std::vector<userrec*> local_users;
extern userrec* fd_ref_table[65536];

void cmd_modules::Handle (char **parameters, int pcnt, userrec *user)
{
  	for (unsigned int i = 0; i < Config->module_names.size(); i++)
	{
		Version V = modules[i]->GetVersion();
		char modulename[MAXBUF];
		char flagstate[MAXBUF];
		*flagstate = 0;
		if (V.Flags & VF_STATIC)
			strlcat(flagstate,", static",MAXBUF);
		if (V.Flags & VF_VENDOR)
			strlcat(flagstate,", vendor",MAXBUF);
		if (V.Flags & VF_COMMON)
			strlcat(flagstate,", common",MAXBUF);
		if (V.Flags & VF_SERVICEPROVIDER)
			strlcat(flagstate,", service provider",MAXBUF);
		if (!flagstate[0])
			strcpy(flagstate,"  <no flags>");
		strlcpy(modulename,Config->module_names[i].c_str(),256);
		if (strchr(user->modes,'o'))
		{
			WriteServ(user->fd,"900 %s :0x%08lx %d.%d.%d.%d %s (%s)",user->nick,modules[i],V.Major,V.Minor,V.Revision,V.Build,CleanFilename(modulename),flagstate+2);
		}
		else
		{
			WriteServ(user->fd,"900 %s :%s",user->nick,CleanFilename(modulename));
		}
	}
	WriteServ(user->fd,"901 %s :End of MODULES list",user->nick);
}


