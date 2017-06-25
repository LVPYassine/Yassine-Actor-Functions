/**
 * Copyright (c) 2017 Yassine's Actor Functions (Include)
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

#include <a_actor>

/*
	(Pawn Syntax) 
	
		Set Functions:
			
			native SetActorSpawnInfo(Actor:Id, skin, Float:x, Float:y, Float:z, virtualworld = -1)
			native SetActorName(Actor:Id, name[])
			native SetActorColor(Actor:Id, color)
			native SetActorSkin(Actor:Id, skin)
			
		Get Functions:
		
			native GetActorName(Actor:Id)
			native GetActorColor(Actor:Id)
		
		Global Functions:
			
			native Actor:SpawnActor(name[], Float:x, Float:y, Float:z)
			native ToggleActorName(Actor:Id, bool:show)
			native RemoveActor(Actor:Id)



		Callbacks:
			
			forward OnActorSpawn(Actor:Id)
			forward OnActorSkinChange(Actor:Id, oldskin, newskin)
			forward OnActorNameChange(Actor:Id, oldnick[], newnick[])
			
	(Yassine's Syntax)
		
			Set Functions:
				
				native Actor::SetSpawnInfo(Actor:Id, skin, Float:x, Float:y, Float:z, virtualworld = -1)
				native Actor::SetName(Actor:Id, name[])
				native Actor::SetColor(Actor:Id, color)
				native Actor::SetSkin(Actor:Id, skin)
			
			Get Functions:
				
				native Actor::GetName(Actor:Id)
				native Actor::GetColor(Actor:Id)
			
			Callbacks:
				
				Actor->OnSpawn()
				Actor->OnSkinChange(oldskin, newskin)
				Actor->OnNameChange(oldnick[], newnick[])
*/

// Syntax creating

#define SetActorSpawnInfo Actor::SetSpawnInfo
#define SetActorName Actor::SetName
#define SetActorColor Actor::SetColor
#define SetActorSkin Actor::SetSkin

#define GetActorName Actor::GetName
#define GetActorColor Actor::GetColor

#define Actor->OnSpawn() public OnActorSpawn(Actor:Id)
#define Actor->OnSkinChange(oldskin, newskin) public OnActorSkinChange(Actor:Id, oldskin, newskin)
#define Actor->OnNameChange(oldnick[], newnick[]) public OnActorNameChange(Actor:Id, oldnick[], newnick[])

// Variables
new Actor:MAX_ACTORS = 100;

enum ENUM_ACTOR_INFO
{
	Id,
	Name[25],
	Text3D:NameText,
	Float:x,
	Float:z, 
	Float:y,
	Color = -1,
	Skin
}

new ActorInfo[MAX_ACTORS][ENUM_ACTOR_INFO], currentactor;

// Set Functions
static stock SetActorSpawnInfo(Actor:Id, skin, Float:x, Float:y, Float:z, virtualworld = -1)
{
	if(!IsValidActor(Id)) return printf("[Debug::Actors]: Actor:Id %d doesnt not exists", Id);
	SetActorSkin(Id, skin);
	ActorInfo[Id][x] = x, ActorInfo[Id][y] = y, ActorInfo[Id][z] = z;
	RespawnActor(Id);
	return 1;
}

static stock SetActorName(Actor:Id, name[])
{
	if(!IsValidActor(Id)) return printf("[Debug::Actors]: Actor:Id %d doesnt not exists", Id);
	CallRemoteFunction("OnActorSkinChange", "ii", ActorInfo[Id][Name], name);
	ActorInfo[Id][Name] = name;
	UpdateActorName(Id);
	return 
}

static stock SetActorColor(Actor:Id, color)
{
	if(!IsValidActor(Id)) return printf("[Debug::Actors]: Actor:Id %d doesnt not exists", Id);
	ActorInfo[Id][Color] = color;
	UpdateActorName(Id);
	return true;
}

static stock SetActorSkin(Actor:Id, skin)
{
	if(!IsValidActor(Id)) return printf("[Debug::Actors]: Actor:Id %d doesnt not exists", Id);
	SetActorSpawnInfo(Id, skin, ActorInfo[Id][x], c);
	CallRemoteFunction("OnActorSkinChange", "ii", ActorInfo[Id][Skin], skin);
	ActorInfo[Id][Skin] = skin;
	return true;
}

// Get Functions
static stock GetActorName(Actor:Id)
{
	if(!IsValidActor(Id)) return printf("[Debug::Actors]: Actor:Id %d doesnt not exists", Id);
	return ActorInfo[Id][Name];
}

static stock GetActorColor(Actor:Id)
{
	if(!IsValidActor(Id)) return printf("[Debug::Actors]: Actor:Id %d doesnt not exists", Id);
	return ActorInfo[Id][Color];
}

// Global Functions
static stock Actor:SpawnActor(name[], Float:x, Float:y, Float:z, Float:a)
{
	ActorInfo[currentactor][Id] = CreateActor(0, x, y, z, a);
	ActorInfo[Id][x] = x, ActorInfo[Id][y] = y, ActorInfo[Id][z] = z, ActorInfo[Id][Angle] = a;
	ActorInfo[currentactor][Name] = name;
	ActorInfo[currentactor][NameText] = Create3DTextLabel(name, ActorInfo[Id][Color], x, y, z, 40.0, -1, 0);
	currentactor++;
	
	new Actor:func = currentactor - 1;
	CallRemoteFunction("OnActorSpawn", "i", func);
	return ActorInfo[currentactor - 1][Id];
}

static stock ToggleActorName(Actor:Id, bool:show)
{
	if(!IsValidActor(Id)) return false;
	switch(show)
	{
		case false:
		{
			Delete3DTextLabel(ActorInfo[Id][NameText]);
		}
		case true:
		{
			ActorInfo[Id][NameText] = Create3DTextLabel(ActorInfo[Id][Name], ActorInfo[Id][Color], x, y, z, 40.0, -1, 0);
		}
	}
	return true;
}

static stock RemoveActor(Actor:Id)
{
	if(!IsValidActor(Id)) return printf("[Debug::Actors]: Actor:Id %d doesnt not exists", Id);
	RemoveActor(ActorInfo[Id][Id]);
	Delete3DTextLabel(ActorInfo[Id][NameText]);
	return true;
}

static stock UpdateActorName(Actor:Id)
{
	if(!IsValidActor(Id)) return printf("[Debug::Actors]: Actor:Id %d doesnt not exists", Id);
	ToggleActorName(Id, false);
	ToggleActorName(Id, true);
	return true;
}

static stock RespawnActor(Actor:Id)
{
	if(!IsValidActor(Id)) return printf("[Debug::Actors]: Actor:Id %d doesnt not exists", Id);
	RemoveActor(Id);
	Id = SpawnActor(Id, ActorInfo[Id][x], ActorInfo[Id][y], ActorInfo[Id][z], ActorInfo[Id][Angle]);
	return 1;
}

// Callbacks
forward OnActorSpawn(Actor:Id);
forward OnActorSkinChange(Actor:Id, oldskin, newskin);
forward OnActorNameChange(Actor:Id, oldnick[], newnick[]);
forward OnActorPosChange(Actor:Id, Float:oldx, Float:oldy, Float:oldz, Float:x, Float:y, Float:z);

// Name update & Init check
forward OnActorUpdate();
public OnActorUpdate()
{
	new Float:pX, Float:pY, Float:pZ;
	GetActorPos(i, pX, pY, pZ);
	if (pX != ActorInfo[Id][x] || pY != ActorInfo[Id][y] || pZ != ActorInfo[Id][z])
	{
		CallRemoteFunction("OnActorPosChange", "ffffff", ActorInfo[Id][x], ActorInfo[Id][y], ActorInfo[Id][z], pX, pY, pZ);
		ActorInfo[Id][x] = pX, ActorInfo[Id][y] = pY, ActorInfo[Id][z] = pZ;
		UpdateActorName(Id);
	}
	return true;
}

