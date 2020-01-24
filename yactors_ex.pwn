/**
 * Copyright (c) 2017 Yassine's Actor Functions (FilterScript Ex)
 *
 * This program is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
*/

 /*AUTHOR ***********************
  @Yassine - SA-MP Scripter
 ********************************/

#include <a_samp>
#include <yactors>
#include <zcmd>

static Actor:Testing;

public OnFilterScriptInit()
{
	Testing = SpawnActor("Testing", 1249.7258, -2047.9263, 59.9209, 90.2055);
	print("[Debug::Actor]: FilterScript has been loaded.");
	return 1;
}

public OnFilterScriptExit()
{
	RemoveActor(Testing);
	print("[Debug::Actor]: FilterScript has been unloaded.");
	return 1;
}

public OnActorSpawn(Actor:Id)
{
	printf("[Debug::Actor]: Actor Id: %d has been spawned.", Id);
	return 1;
}

public OnActorSkinChange(Actor:Id, oldskin, newskin)
{
	printf("[Debug::Actor]: Actor Id: %d skin has been changed from %d to %d.", Id, oldskin, newskin);
	return 1;
}

public OnActorNameChange(Actor:Id, oldnick[], newnick[])
{
	printf("[Debug::Actor]: Actor Id: %d name has been changed from %s to %s.", Id, oldnick, newnick);
	return 1;
}

CMD:actorsay(playerid, params[])
{
	new string[129], Id; // SA-MP Chat max text.
	if(sscanf(params, "is[129]", Id, string)) return SendClientMessage(playerid, -1, "Usage: /actorsay [actorid] [text]");
	format(string, sizeof(string), "[Actor]%s: %s", GetActorName(Id), string);
	SendClientMessageToAll(-1, string);
	return 1;
}
