#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"

using namespace sqf;
void sqf::commandmap::initunimplemented()
{
	add(binary(4, "lnbsetcurselrow", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetcurselrow)." << std::endl; return {}; }));
	add(binary(4, "removemenuitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removemenuitem)." << std::endl; return {}; }));
	add(binary(4, "curatorcoef", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (curatorcoef)." << std::endl; return {}; }));
	add(binary(4, "getreldir", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getreldir)." << std::endl; return {}; }));
	add(binary(4, "menucollapse", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menucollapse)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfontheighth6", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfontheighth6)." << std::endl; return {}; }));
	add(binary(4, "findemptyposition", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (findemptyposition)." << std::endl; return {}; }));
	add(binary(4, "menusetdata", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusetdata)." << std::endl; return {}; }));
	add(binary(4, "setwaypointtype", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointtype)." << std::endl; return {}; }));
	add(binary(4, "lbsetcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetcolor)." << std::endl; return {}; }));
	add(binary(4, "lbsetcolorright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetcolorright)." << std::endl; return {}; }));
	add(binary(4, "say3d", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (say3d)." << std::endl; return {}; }));
	add(binary(4, "forceflagtexture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (forceflagtexture)." << std::endl; return {}; }));
	add(binary(4, "addvehicle", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addvehicle)." << std::endl; return {}; }));
	add(binary(4, "enablereload", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablereload)." << std::endl; return {}; }));
	add(binary(4, "setpilotlight", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setpilotlight)." << std::endl; return {}; }));
	add(binary(4, "setvisibleiftreecollapsed", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvisibleiftreecollapsed)." << std::endl; return {}; }));
	add(binary(4, "isequaltypeall", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isequaltypeall)." << std::endl; return {}; }));
	add(binary(4, "camconstuctionsetparams", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camconstuctionsetparams)." << std::endl; return {}; }));
	add(binary(4, "commandsuppressivefire", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandsuppressivefire)." << std::endl; return {}; }));
	add(binary(4, "tvtext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvtext)." << std::endl; return {}; }));
	add(binary(4, "switchlight", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (switchlight)." << std::endl; return {}; }));
	add(binary(4, "camsettarget", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camsettarget)." << std::endl; return {}; }));
	add(binary(4, "setanimspeedcoef", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setanimspeedcoef)." << std::endl; return {}; }));
	add(binary(4, "setposaslw", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setposaslw)." << std::endl; return {}; }));
	add(binary(4, "camsetfocus", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camsetfocus)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfontp", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfontp)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfont", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfont)." << std::endl; return {}; }));
	add(binary(4, "allowgetin", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allowgetin)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth4b", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth4b)." << std::endl; return {}; }));
	add(binary(4, "tvdata", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvdata)." << std::endl; return {}; }));
	add(binary(4, "setsimpletaskdescription", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setsimpletaskdescription)." << std::endl; return {}; }));
	add(binary(4, "sendtaskresult", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sendtaskresult)." << std::endl; return {}; }));
	add(binary(4, "directsay", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (directsay)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetforegroundcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetforegroundcolor)." << std::endl; return {}; }));
	add(binary(4, "setforcegeneratorrtd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setforcegeneratorrtd)." << std::endl; return {}; }));
	add(binary(4, "menusetvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusetvalue)." << std::endl; return {}; }));
	add(binary(4, "worldtomodel", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (worldtomodel)." << std::endl; return {}; }));
	add(binary(4, "sethitindex", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sethitindex)." << std::endl; return {}; }));
	add(binary(4, "hideobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hideobject)." << std::endl; return {}; }));
	add(binary(4, "get3denattribute", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (get3denattribute)." << std::endl; return {}; }));
	add(binary(4, "dofollow", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dofollow)." << std::endl; return {}; }));
	add(binary(4, "disableconversation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (disableconversation)." << std::endl; return {}; }));
	add(binary(4, "camsetdive", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camsetdive)." << std::endl; return {}; }));
	add(binary(4, "tvsetselectcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetselectcolor)." << std::endl; return {}; }));
	add(binary(4, "setweaponreloadingtime", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setweaponreloadingtime)." << std::endl; return {}; }));
	add(binary(4, "enableai", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableai)." << std::endl; return {}; }));
	add(binary(4, "removemagazine", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removemagazine)." << std::endl; return {}; }));
	add(binary(4, "remotecontrol", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (remotecontrol)." << std::endl; return {}; }));
	add(binary(4, "additem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (additem)." << std::endl; return {}; }));
	add(binary(4, "addmpeventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmpeventhandler)." << std::endl; return {}; }));
	add(binary(4, "actionparams", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (actionparams)." << std::endl; return {}; }));
	add(binary(4, "setfromeditor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setfromeditor)." << std::endl; return {}; }));
	add(binary(4, "selectweaponturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selectweaponturret)." << std::endl; return {}; }));
	add(binary(4, "menuurl", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuurl)." << std::endl; return {}; }));
	add(binary(4, "getfriend", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getfriend)." << std::endl; return {}; }));
	add(binary(4, "deleteeditorobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deleteeditorobject)." << std::endl; return {}; }));
	add(binary(4, "registertask", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (registertask)." << std::endl; return {}; }));
	add(binary(4, "synchronizeobjectsadd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (synchronizeobjectsadd)." << std::endl; return {}; }));
	add(binary(4, "disabletiequipment", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (disabletiequipment)." << std::endl; return {}; }));
	add(binary(4, "setparticlerandom", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setparticlerandom)." << std::endl; return {}; }));
	add(binary(4, "targetsquery", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (targetsquery)." << std::endl; return {}; }));
	add(binary(4, "lbsetselectcolorright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetselectcolorright)." << std::endl; return {}; }));
	add(binary(4, "animatepylon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (animatepylon)." << std::endl; return {}; }));
	add(binary(4, "enableuavwaypoints", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableuavwaypoints)." << std::endl; return {}; }));
	add(binary(4, "distancesqr", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (distancesqr)." << std::endl; return {}; }));
	add(binary(4, "setspeaker", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setspeaker)." << std::endl; return {}; }));
	add(binary(4, "setammoonpylon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setammoonpylon)." << std::endl; return {}; }));
	add(binary(4, "setcustomweightrtd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcustomweightrtd)." << std::endl; return {}; }));
	add(binary(4, "isuavconnectable", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isuavconnectable)." << std::endl; return {}; }));
	add(binary(4, "playaction", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playaction)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetangle", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetangle)." << std::endl; return {}; }));
	add(binary(4, "setsimpletasktype", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setsimpletasktype)." << std::endl; return {}; }));
	add(binary(4, "triggerattachobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggerattachobject)." << std::endl; return {}; }));
	add(binary(4, "animationsourcephase", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (animationsourcephase)." << std::endl; return {}; }));
	add(binary(4, "lnbsetvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetvalue)." << std::endl; return {}; }));
	add(binary(4, "loadmagazine", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (loadmagazine)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetmodel", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetmodel)." << std::endl; return {}; }));
	add(binary(4, "loadstatus", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (loadstatus)." << std::endl; return {}; }));
	add(binary(4, "removempeventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removempeventhandler)." << std::endl; return {}; }));
	add(binary(4, "setlightbrightness", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setlightbrightness)." << std::endl; return {}; }));
	add(binary(4, "swimindepth", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (swimindepth)." << std::endl; return {}; }));
	add(binary(4, "attachobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (attachobject)." << std::endl; return {}; }));
	add(binary(4, "addrating", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addrating)." << std::endl; return {}; }));
	add(binary(4, "nearobjectsready", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearobjectsready)." << std::endl; return {}; }));
	add(binary(4, "setwaypointposition", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointposition)." << std::endl; return {}; }));
	add(binary(4, "controlsgroupctrl", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (controlsgroupctrl)." << std::endl; return {}; }));
	add(binary(4, "tvpictureright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvpictureright)." << std::endl; return {}; }));
	add(binary(4, "ctrlsettooltipcolorshade", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsettooltipcolorshade)." << std::endl; return {}; }));
	add(binary(4, "saveidentity", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (saveidentity)." << std::endl; return {}; }));
	add(binary(4, "enablestamina", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablestamina)." << std::endl; return {}; }));
	add(binary(4, "settype", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settype)." << std::endl; return {}; }));
	add(binary(4, "setammo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setammo)." << std::endl; return {}; }));
	add(binary(4, "removeownedmine", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeownedmine)." << std::endl; return {}; }));
	add(binary(4, "setunitability", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setunitability)." << std::endl; return {}; }));
	add(binary(4, "additemtouniform", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (additemtouniform)." << std::endl; return {}; }));
	add(binary(4, "htmlload", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (htmlload)." << std::endl; return {}; }));
	add(binary(4, "setvehiclereceiveremotetargets", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehiclereceiveremotetargets)." << std::endl; return {}; }));
	add(binary(4, "enableweapondisassembly", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableweapondisassembly)." << std::endl; return {}; }));
	add(binary(4, "assignasgunner", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignasgunner)." << std::endl; return {}; }));
	add(binary(4, "setunitrecoilcoefficient", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setunitrecoilcoefficient)." << std::endl; return {}; }));
	add(binary(4, "menuchecked", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuchecked)." << std::endl; return {}; }));
	add(binary(4, "sethitpointdamage", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sethitpointdamage)." << std::endl; return {}; }));
	add(binary(4, "allow3dmode", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allow3dmode)." << std::endl; return {}; }));
	add(binary(4, "limitspeed", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (limitspeed)." << std::endl; return {}; }));
	add(binary(4, "loadoverlay", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (loadoverlay)." << std::endl; return {}; }));
	add(binary(4, "campreload", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (campreload)." << std::endl; return {}; }));
	add(binary(4, "removeitemfrombackpack", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeitemfrombackpack)." << std::endl; return {}; }));
	add(binary(4, "settriggertype", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settriggertype)." << std::endl; return {}; }));
	add(binary(4, "ppeffectcommit", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ppeffectcommit)." << std::endl; return {}; }));
	add(binary(4, "updatedrawicon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (updatedrawicon)." << std::endl; return {}; }));
	add(binary(4, "setwinddir", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwinddir)." << std::endl; return {}; }));
	add(binary(4, "ctrlremovealleventhandlers", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlremovealleventhandlers)." << std::endl; return {}; }));
	add(binary(4, "assignascargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignascargo)." << std::endl; return {}; }));
	add(binary(4, "setvehicleid", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehicleid)." << std::endl; return {}; }));
	add(binary(4, "groupradio", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (groupradio)." << std::endl; return {}; }));
	add(binary(4, "slidersetrange", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (slidersetrange)." << std::endl; return {}; }));
	add(binary(4, "lnbsettext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsettext)." << std::endl; return {}; }));
	add(binary(4, "setfog", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setfog)." << std::endl; return {}; }));
	add(binary(4, "setvectorup", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvectorup)." << std::endl; return {}; }));
	add(binary(4, "setunittrait", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setunittrait)." << std::endl; return {}; }));
	add(binary(4, "forcewalk", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (forcewalk)." << std::endl; return {}; }));
	add(binary(4, "addweaponcargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addweaponcargo)." << std::endl; return {}; }));
	add(binary(4, "addweaponcargoglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addweaponcargoglobal)." << std::endl; return {}; }));
	add(binary(4, "tvsetpictureright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpictureright)." << std::endl; return {}; }));
	add(binary(4, "setlightintensity", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setlightintensity)." << std::endl; return {}; }));
	add(binary(4, "customradio", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (customradio)." << std::endl; return {}; }));
	add(binary(4, "currentmagazinedetailturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentmagazinedetailturret)." << std::endl; return {}; }));
	add(binary(4, "settitleeffect", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settitleeffect)." << std::endl; return {}; }));
	add(binary(4, "lnbsettextright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsettextright)." << std::endl; return {}; }));
	add(binary(4, "commandfollow", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandfollow)." << std::endl; return {}; }));
	add(binary(4, "allowfileoperations", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allowfileoperations)." << std::endl; return {}; }));
	add(binary(4, "lbsetpicturecolordisabled", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpicturecolordisabled)." << std::endl; return {}; }));
	add(binary(4, "setcurrentwaypoint", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcurrentwaypoint)." << std::endl; return {}; }));
	add(binary(4, "removeitemfromuniform", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeitemfromuniform)." << std::endl; return {}; }));
	add(binary(4, "vectormodeltoworld", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vectormodeltoworld)." << std::endl; return {}; }));
	add(binary(4, "setlightnings", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setlightnings)." << std::endl; return {}; }));
	add(binary(4, "lnbsortbyvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsortbyvalue)." << std::endl; return {}; }));
	add(binary(4, "tvsetpicture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpicture)." << std::endl; return {}; }));
	add(binary(4, "lnbsetdata", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetdata)." << std::endl; return {}; }));
	add(binary(4, "setpylonloadout", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setpylonloadout)." << std::endl; return {}; }));
	add(binary(4, "removeprimaryweaponitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeprimaryweaponitem)." << std::endl; return {}; }));
	add(binary(4, "selecteditorobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selecteditorobject)." << std::endl; return {}; }));
	add(binary(4, "setobjectarguments", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setobjectarguments)." << std::endl; return {}; }));
	add(binary(4, "setflagtexture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setflagtexture)." << std::endl; return {}; }));
	add(binary(4, "removeitemfromvest", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeitemfromvest)." << std::endl; return {}; }));
	add(binary(4, "gethitindex", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (gethitindex)." << std::endl; return {}; }));
	add(binary(4, "forcefollowroad", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (forcefollowroad)." << std::endl; return {}; }));
	add(binary(4, "removeaction", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeaction)." << std::endl; return {}; }));
	add(binary(4, "addplayerscores", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addplayerscores)." << std::endl; return {}; }));
	add(binary(4, "setposworld", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setposworld)." << std::endl; return {}; }));
	add(binary(4, "creatediarysubject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (creatediarysubject)." << std::endl; return {}; }));
	add(binary(4, "knowsabout", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (knowsabout)." << std::endl; return {}; }));
	add(binary(4, "sendtask", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sendtask)." << std::endl; return {}; }));
	add(binary(4, "in", type::ARRAY, type::LOCATION, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (in)." << std::endl; return {}; }));
	add(binary(4, "lnbsetpicturecolorselectedright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpicturecolorselectedright)." << std::endl; return {}; }));
	add(binary(4, "enablepersonturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablepersonturret)." << std::endl; return {}; }));
	add(binary(4, "ctsetvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctsetvalue)." << std::endl; return {}; }));
	add(binary(4, "setwaypointscript", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointscript)." << std::endl; return {}; }));
	add(binary(4, "lbsetvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetvalue)." << std::endl; return {}; }));
	add(binary(4, "playmove", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playmove)." << std::endl; return {}; }));
	add(binary(4, "currentweaponturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentweaponturret)." << std::endl; return {}; }));
	add(binary(4, "ammoonpylon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ammoonpylon)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfontpb", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfontpb)." << std::endl; return {}; }));
	add(binary(4, "lockcamerato", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lockcamerato)." << std::endl; return {}; }));
	add(binary(4, "commandradio", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandradio)." << std::endl; return {}; }));
	add(binary(4, "removeweaponturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeweaponturret)." << std::endl; return {}; }));
	add(binary(4, "cutfadeout", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cutfadeout)." << std::endl; return {}; }));
	add(binary(4, "removeallmpeventhandlers", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallmpeventhandlers)." << std::endl; return {}; }));
	add(binary(4, "inpolygon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (inpolygon)." << std::endl; return {}; }));
	add(binary(4, "tvsettext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsettext)." << std::endl; return {}; }));
	add(binary(4, "set3denobjecttype", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3denobjecttype)." << std::endl; return {}; }));
	add(binary(4, "triggerdynamicsimulation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggerdynamicsimulation)." << std::endl; return {}; }));
	add(binary(4, "addmagazinecargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmagazinecargo)." << std::endl; return {}; }));
	add(binary(4, "addmagazinecargoglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmagazinecargoglobal)." << std::endl; return {}; }));
	add(binary(4, "minedetectedby", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (minedetectedby)." << std::endl; return {}; }));
	add(binary(4, "lnbtext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbtext)." << std::endl; return {}; }));
	add(binary(4, "isvehiclesensorenabled", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isvehiclesensorenabled)." << std::endl; return {}; }));
	add(binary(4, "tvsetdata", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetdata)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetbackgroundcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetbackgroundcolor)." << std::endl; return {}; }));
	add(binary(4, "canadditemtobackpack", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (canadditemtobackpack)." << std::endl; return {}; }));
	add(binary(4, "setposasl", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setposasl)." << std::endl; return {}; }));
	add(binary(4, "reveal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (reveal)." << std::endl; return {}; }));
	add(binary(4, "lbsettooltip", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsettooltip)." << std::endl; return {}; }));
	add(binary(4, "vehicleradio", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vehicleradio)." << std::endl; return {}; }));
	add(binary(4, "show3dicons", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (show3dicons)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetdisabledcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetdisabledcolor)." << std::endl; return {}; }));
	add(binary(4, "enablevehiclecargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablevehiclecargo)." << std::endl; return {}; }));
	add(binary(4, "lnbdata", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbdata)." << std::endl; return {}; }));
	add(binary(4, "lbadd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbadd)." << std::endl; return {}; }));
	add(binary(4, "get3denmissionattribute", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (get3denmissionattribute)." << std::endl; return {}; }));
	add(binary(4, "isflashlighton", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isflashlighton)." << std::endl; return {}; }));
	add(binary(4, "execfsm", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (execfsm)." << std::endl; return {}; }));
	add(binary(4, "setcollisionlight", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcollisionlight)." << std::endl; return {}; }));
	add(binary(4, "setvectordir", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvectordir)." << std::endl; return {}; }));
	add(binary(4, "setobjectmaterialglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setobjectmaterialglobal)." << std::endl; return {}; }));
	add(binary(4, "setdropinterval", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdropinterval)." << std::endl; return {}; }));
	add(binary(4, "ctrlsettooltipcolortext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsettooltipcolortext)." << std::endl; return {}; }));
	add(binary(4, "lbtext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbtext)." << std::endl; return {}; }));
	add(binary(4, "switchmove", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (switchmove)." << std::endl; return {}; }));
	add(binary(4, "additemcargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (additemcargo)." << std::endl; return {}; }));
	add(binary(4, "neartargets", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (neartargets)." << std::endl; return {}; }));
	add(binary(4, "addgoggles", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addgoggles)." << std::endl; return {}; }));
	add(binary(4, "lbpicture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbpicture)." << std::endl; return {}; }));
	add(binary(4, "tvsetpicturecolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpicturecolor)." << std::endl; return {}; }));
	add(binary(4, "allowcuratorlogicignoreareas", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allowcuratorlogicignoreareas)." << std::endl; return {}; }));
	add(binary(4, "assignascommander", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignascommander)." << std::endl; return {}; }));
	add(binary(4, "showwaypoint", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showwaypoint)." << std::endl; return {}; }));
	add(binary(4, "getdir", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getdir)." << std::endl; return {}; }));
	add(binary(4, "lbsettextright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsettextright)." << std::endl; return {}; }));
	add(binary(4, "ctrlremoveeventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlremoveeventhandler)." << std::endl; return {}; }));
	add(binary(4, "addmagazine", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmagazine)." << std::endl; return {}; }));
	add(binary(4, "disablenvgequipment", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (disablenvgequipment)." << std::endl; return {}; }));
	add(binary(4, "targetknowledge", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (targetknowledge)." << std::endl; return {}; }));
	add(binary(4, "lbdata", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbdata)." << std::endl; return {}; }));
	add(binary(4, "lbdelete", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbdelete)." << std::endl; return {}; }));
	add(binary(4, "campreparefov", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (campreparefov)." << std::endl; return {}; }));
	add(binary(4, "newoverlay", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (newoverlay)." << std::endl; return {}; }));
	add(binary(4, "respawnvehicle", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (respawnvehicle)." << std::endl; return {}; }));
	add(binary(4, "camsetfov", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camsetfov)." << std::endl; return {}; }));
	add(binary(4, "cutobj", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cutobj)." << std::endl; return {}; }));
	add(binary(4, "setsimpletaskalwaysvisible", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setsimpletaskalwaysvisible)." << std::endl; return {}; }));
	add(binary(4, "turretunit", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (turretunit)." << std::endl; return {}; }));
	add(binary(4, "kbaddtopic", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (kbaddtopic)." << std::endl; return {}; }));
	add(binary(4, "setposition", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setposition)." << std::endl; return {}; }));
	add(binary(4, "setformationtask", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setformationtask)." << std::endl; return {}; }));
	add(binary(4, "campreparebank", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (campreparebank)." << std::endl; return {}; }));
	add(binary(4, "setcurrenttask", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcurrenttask)." << std::endl; return {}; }));
	add(binary(4, "cbsetchecked", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cbsetchecked)." << std::endl; return {}; }));
	add(binary(4, "tvsort", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsort)." << std::endl; return {}; }));
	add(binary(4, "setuseractiontext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setuseractiontext)." << std::endl; return {}; }));
	add(binary(4, "setsuppression", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setsuppression)." << std::endl; return {}; }));
	add(binary(4, "lbsetpicturerightcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpicturerightcolor)." << std::endl; return {}; }));
	add(binary(4, "weaponsturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weaponsturret)." << std::endl; return {}; }));
	add(binary(4, "removeitems", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeitems)." << std::endl; return {}; }));
	add(binary(4, "radiochannelsetcallsign", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (radiochannelsetcallsign)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetchecked", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetchecked)." << std::endl; return {}; }));
	add(binary(4, "setgroupid", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setgroupid)." << std::endl; return {}; }));
	add(binary(4, "moveto", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (moveto)." << std::endl; return {}; }));
	add(binary(4, "tvexpand", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvexpand)." << std::endl; return {}; }));
	add(binary(4, "vectorworldtomodelvisual", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vectorworldtomodelvisual)." << std::endl; return {}; }));
	add(binary(4, "tvadd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvadd)." << std::endl; return {}; }));
	add(binary(4, "moveinturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (moveinturret)." << std::endl; return {}; }));
	add(binary(4, "kbadddatabase", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (kbadddatabase)." << std::endl; return {}; }));
	add(binary(4, "infopanelcomponentenabled", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (infopanelcomponentenabled)." << std::endl; return {}; }));
	add(binary(4, "create3denentity", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (create3denentity)." << std::endl; return {}; }));
	add(binary(4, "lbsetpicturerightcolorselected", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpicturerightcolorselected)." << std::endl; return {}; }));
	add(binary(4, "deleteresources", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deleteresources)." << std::endl; return {}; }));
	add(binary(4, "ropedetach", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropedetach)." << std::endl; return {}; }));
	add(binary(4, "enablesimulationglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablesimulationglobal)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth1b", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth1b)." << std::endl; return {}; }));
	add(binary(4, "setwantedrpmrtd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwantedrpmrtd)." << std::endl; return {}; }));
	add(binary(4, "tvcount", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvcount)." << std::endl; return {}; }));
	add(binary(4, "unregistertask", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unregistertask)." << std::endl; return {}; }));
	add(binary(4, "camcreate", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camcreate)." << std::endl; return {}; }));
	add(binary(4, "say", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (say)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetscale", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetscale)." << std::endl; return {}; }));
	add(binary(4, "ppeffectenable", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ppeffectenable)." << std::endl; return {}; }));
	add(binary(4, "intersect", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (intersect)." << std::endl; return {}; }));
	add(binary(4, "removehandgunitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removehandgunitem)." << std::endl; return {}; }));
	add(binary(4, "dofire", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dofire)." << std::endl; return {}; }));
	add(binary(4, "set3denattribute", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3denattribute)." << std::endl; return {}; }));
	add(binary(4, "lbsetselected", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetselected)." << std::endl; return {}; }));
	add(binary(4, "setdebriefingtext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdebriefingtext)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfade", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfade)." << std::endl; return {}; }));
	add(binary(4, "hcgroupparams", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hcgroupparams)." << std::endl; return {}; }));
	add(binary(4, "sideradio", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sideradio)." << std::endl; return {}; }));
	add(binary(4, "countenemy", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (countenemy)." << std::endl; return {}; }));
	add(binary(4, "progresssetposition", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (progresssetposition)." << std::endl; return {}; }));
	add(binary(4, "setdammage", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdammage)." << std::endl; return {}; }));
	add(binary(4, "moveinany", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (moveinany)." << std::endl; return {}; }));
	add(binary(4, "lnbdeletecolumn", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbdeletecolumn)." << std::endl; return {}; }));
	add(binary(4, "displayaddeventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (displayaddeventhandler)." << std::endl; return {}; }));
	add(binary(4, "ctrlsettooltip", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsettooltip)." << std::endl; return {}; }));
	add(binary(4, "camcommit", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camcommit)." << std::endl; return {}; }));
	add(binary(4, "setshotparents", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setshotparents)." << std::endl; return {}; }));
	add(binary(4, "modeltoworldvisual", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (modeltoworldvisual)." << std::endl; return {}; }));
	add(binary(4, "setattributes", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setattributes)." << std::endl; return {}; }));
	add(binary(4, "getgroupicon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getgroupicon)." << std::endl; return {}; }));
	add(binary(4, "tvdelete", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvdelete)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth5b", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth5b)." << std::endl; return {}; }));
	add(binary(4, "enableaifeature", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableaifeature)." << std::endl; return {}; }));
	add(binary(4, "addmenuitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmenuitem)." << std::endl; return {}; }));
	add(binary(4, "enablesimulation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablesimulation)." << std::endl; return {}; }));
	add(binary(4, "allowdamage", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allowdamage)." << std::endl; return {}; }));
	add(binary(4, "setlightcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setlightcolor)." << std::endl; return {}; }));
	add(binary(4, "setwaypointhouseposition", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointhouseposition)." << std::endl; return {}; }));
	add(binary(4, "setfriend", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setfriend)." << std::endl; return {}; }));
	add(binary(4, "addcuratoreditingarea", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addcuratoreditingarea)." << std::endl; return {}; }));
	add(binary(4, "attachto", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (attachto)." << std::endl; return {}; }));
	add(binary(4, "cutrsc", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cutrsc)." << std::endl; return {}; }));
	add(binary(4, "setcaptive", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcaptive)." << std::endl; return {}; }));
	add(binary(4, "editorseteventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (editorseteventhandler)." << std::endl; return {}; }));
	add(binary(4, "radiochannelsetlabel", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (radiochannelsetlabel)." << std::endl; return {}; }));
	add(binary(4, "lockedcargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lockedcargo)." << std::endl; return {}; }));
	add(binary(4, "turretlocal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (turretlocal)." << std::endl; return {}; }));
	add(binary(4, "setgroupidglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setgroupidglobal)." << std::endl; return {}; }));
	add(binary(4, "kbtell", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (kbtell)." << std::endl; return {}; }));
	add(binary(4, "setidentity", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setidentity)." << std::endl; return {}; }));
	add(binary(4, "menuenabled", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuenabled)." << std::endl; return {}; }));
	add(binary(4, "doorphase", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (doorphase)." << std::endl; return {}; }));
	add(binary(4, "lookat", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lookat)." << std::endl; return {}; }));
	add(binary(4, "selectleader", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selectleader)." << std::endl; return {}; }));
	add(binary(4, "backpackspacefor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (backpackspacefor)." << std::endl; return {}; }));
	add(binary(4, "animatesource", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (animatesource)." << std::endl; return {}; }));
	add(binary(4, "drawicon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drawicon)." << std::endl; return {}; }));
	add(binary(4, "setunitloadout", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setunitloadout)." << std::endl; return {}; }));
	add(binary(4, "ctdata", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctdata)." << std::endl; return {}; }));
	add(binary(4, "removecuratoraddons", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removecuratoraddons)." << std::endl; return {}; }));
	add(binary(4, "playgesture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playgesture)." << std::endl; return {}; }));
	add(binary(4, "campreparetarget", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (campreparetarget)." << std::endl; return {}; }));
	add(binary(4, "ctsetheadertemplate", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctsetheadertemplate)." << std::endl; return {}; }));
	add(binary(4, "setvehiclereportownposition", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehiclereportownposition)." << std::endl; return {}; }));
	add(binary(4, "animationphase", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (animationphase)." << std::endl; return {}; }));
	add(binary(4, "lockturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lockturret)." << std::endl; return {}; }));
	add(binary(4, "remoteexec", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (remoteexec)." << std::endl; return {}; }));
	add(binary(4, "removeweaponglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeweaponglobal)." << std::endl; return {}; }));
	add(binary(4, "createsite", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createsite)." << std::endl; return {}; }));
	add(binary(4, "setrainbow", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setrainbow)." << std::endl; return {}; }));
	add(binary(4, "setfatigue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setfatigue)." << std::endl; return {}; }));
	add(binary(4, "addmagazineturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmagazineturret)." << std::endl; return {}; }));
	add(binary(4, "countside", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (countside)." << std::endl; return {}; }));
	add(binary(4, "ctfindrowheader", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctfindrowheader)." << std::endl; return {}; }));
	add(binary(4, "setparticleparams", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setparticleparams)." << std::endl; return {}; }));
	add(binary(4, "drawrectangle", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drawrectangle)." << std::endl; return {}; }));
	add(binary(4, "lnbaddrow", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbaddrow)." << std::endl; return {}; }));
	add(binary(4, "tvsetpicturecolorselected", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpicturecolorselected)." << std::endl; return {}; }));
	add(binary(4, "drawline", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drawline)." << std::endl; return {}; }));
	add(binary(4, "posscreentoworld", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (posscreentoworld)." << std::endl; return {}; }));
	add(binary(4, "sethidebehind", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sethidebehind)." << std::endl; return {}; }));
	add(binary(4, "turretowner", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (turretowner)." << std::endl; return {}; }));
	add(binary(4, "setface", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setface)." << std::endl; return {}; }));
	add(binary(4, "fadesound", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (fadesound)." << std::endl; return {}; }));
	add(binary(4, "setwaypointformation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointformation)." << std::endl; return {}; }));
	add(binary(4, "disableuavconnectability", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (disableuavconnectability)." << std::endl; return {}; }));
	add(binary(4, "nearroads", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearroads)." << std::endl; return {}; }));
	add(binary(4, "displayctrl", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (displayctrl)." << std::endl; return {}; }));
	add(binary(4, "set3denmissionattribute", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3denmissionattribute)." << std::endl; return {}; }));
	add(binary(4, "setformdir", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setformdir)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfontsecondary", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfontsecondary)." << std::endl; return {}; }));
	add(binary(4, "lockcargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lockcargo)." << std::endl; return {}; }));
	add(binary(4, "camcommitprepared", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camcommitprepared)." << std::endl; return {}; }));
	add(binary(4, "setnamesound", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setnamesound)." << std::endl; return {}; }));
	add(binary(4, "modeltoworldvisualworld", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (modeltoworldvisualworld)." << std::endl; return {}; }));
	add(binary(4, "doartilleryfire", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (doartilleryfire)." << std::endl; return {}; }));
	add(binary(4, "isuniformallowed", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isuniformallowed)." << std::endl; return {}; }));
	add(binary(4, "skill", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (skill)." << std::endl; return {}; }));
	add(binary(4, "creatediaryrecord", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (creatediaryrecord)." << std::endl; return {}; }));
	add(binary(4, "addweapon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addweapon)." << std::endl; return {}; }));
	add(binary(4, "waypointattachvehicle", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointattachvehicle)." << std::endl; return {}; }));
	add(binary(4, "lockwp", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lockwp)." << std::endl; return {}; }));
	add(binary(4, "addmagazineammocargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmagazineammocargo)." << std::endl; return {}; }));
	add(binary(4, "displayseteventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (displayseteventhandler)." << std::endl; return {}; }));
	add(binary(4, "addmagazineglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmagazineglobal)." << std::endl; return {}; }));
	add(binary(4, "setunitposweak", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setunitposweak)." << std::endl; return {}; }));
	add(binary(4, "ctrlsettextcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsettextcolor)." << std::endl; return {}; }));
	add(binary(4, "addlivestats", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addlivestats)." << std::endl; return {}; }));
	add(binary(4, "forceadduniform", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (forceadduniform)." << std::endl; return {}; }));
	add(binary(4, "canvehiclecargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (canvehiclecargo)." << std::endl; return {}; }));
	add(binary(4, "removeitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeitem)." << std::endl; return {}; }));
	add(binary(4, "vectorworldtomodel", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vectorworldtomodel)." << std::endl; return {}; }));
	add(binary(4, "setrandomlip", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setrandomlip)." << std::endl; return {}; }));
	add(binary(4, "setwppos", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwppos)." << std::endl; return {}; }));
	add(binary(4, "findeditorobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (findeditorobject)." << std::endl; return {}; }));
	add(binary(4, "addforce", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addforce)." << std::endl; return {}; }));
	add(binary(4, "lockedturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lockedturret)." << std::endl; return {}; }));
	add(binary(4, "lbpictureright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbpictureright)." << std::endl; return {}; }));
	add(binary(4, "ctsetdata", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctsetdata)." << std::endl; return {}; }));
	add(binary(4, "ctrlmapcursor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmapcursor)." << std::endl; return {}; }));
	add(binary(4, "drawlink", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drawlink)." << std::endl; return {}; }));
	add(binary(4, "addcuratorpoints", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addcuratorpoints)." << std::endl; return {}; }));
	add(binary(4, "assignasturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignasturret)." << std::endl; return {}; }));
	add(binary(4, "sendsimplecommand", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sendsimplecommand)." << std::endl; return {}; }));
	add(binary(4, "lnbsort", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsort)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetposition", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetposition)." << std::endl; return {}; }));
	add(binary(4, "removecuratoreditingarea", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removecuratoreditingarea)." << std::endl; return {}; }));
	add(binary(4, "setwaypointspeed", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointspeed)." << std::endl; return {}; }));
	add(binary(4, "setdynamicsimulationdistance", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdynamicsimulationdistance)." << std::endl; return {}; }));
	add(binary(4, "setcuratorcameraareaceiling", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcuratorcameraareaceiling)." << std::endl; return {}; }));
	add(binary(4, "addtorque", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addtorque)." << std::endl; return {}; }));
	add(binary(4, "onmapsingleclick", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onmapsingleclick)." << std::endl; return {}; }));
	add(binary(4, "setmass", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setmass)." << std::endl; return {}; }));
	add(binary(4, "kbadddatabasetargets", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (kbadddatabasetargets)." << std::endl; return {}; }));
	add(binary(4, "posworldtoscreen", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (posworldtoscreen)." << std::endl; return {}; }));
	add(binary(4, "selectionposition", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selectionposition)." << std::endl; return {}; }));
	add(binary(4, "updatemenuitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (updatemenuitem)." << std::endl; return {}; }));
	add(binary(4, "addweaponturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addweaponturret)." << std::endl; return {}; }));
	add(binary(4, "setwindforce", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwindforce)." << std::endl; return {}; }));
	add(binary(4, "getfsmvariable", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getfsmvariable)." << std::endl; return {}; }));
	add(binary(4, "addresources", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addresources)." << std::endl; return {}; }));
	add(binary(4, "addaction", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addaction)." << std::endl; return {}; }));
	add(binary(4, "setsimpletaskcustomdata", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setsimpletaskcustomdata)." << std::endl; return {}; }));
	add(binary(4, "drawpolygon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drawpolygon)." << std::endl; return {}; }));
	add(binary(4, "menushortcut", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menushortcut)." << std::endl; return {}; }));
	add(binary(4, "camsetrelpos", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camsetrelpos)." << std::endl; return {}; }));
	add(binary(4, "ctrlcreate", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlcreate)." << std::endl; return {}; }));
	add(binary(4, "useaudiotimeformoves", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (useaudiotimeformoves)." << std::endl; return {}; }));
	add(binary(4, "addscore", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addscore)." << std::endl; return {}; }));
	add(binary(4, "enablevehiclesensor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablevehiclesensor)." << std::endl; return {}; }));
	add(binary(4, "assigncurator", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assigncurator)." << std::endl; return {}; }));
	add(binary(4, "buildingexit", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (buildingexit)." << std::endl; return {}; }));
	add(binary(4, "setfuel", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setfuel)." << std::endl; return {}; }));
	add(binary(4, "ppeffectadjust", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ppeffectadjust)." << std::endl; return {}; }));
	add(binary(4, "magazineturretammo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazineturretammo)." << std::endl; return {}; }));
	add(binary(4, "nmenuitems", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nmenuitems)." << std::endl; return {}; }));
	add(binary(4, "preloadobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (preloadobject)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetmodeldirandup", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetmodeldirandup)." << std::endl; return {}; }));
	add(binary(4, "lnbpictureright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbpictureright)." << std::endl; return {}; }));
	add(binary(4, "setwaypointcombatmode", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointcombatmode)." << std::endl; return {}; }));
	add(binary(4, "flyinheightasl", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (flyinheightasl)." << std::endl; return {}; }));
	add(binary(4, "creatempcampaigndisplay", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (creatempcampaigndisplay)." << std::endl; return {}; }));
	add(binary(4, "enabledynamicsimulation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enabledynamicsimulation)." << std::endl; return {}; }));
	add(binary(4, "setposatl", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setposatl)." << std::endl; return {}; }));
	add(binary(4, "setdrawicon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdrawicon)." << std::endl; return {}; }));
	add(binary(4, "globalradio", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (globalradio)." << std::endl; return {}; }));
	add(binary(4, "seteditormode", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (seteditormode)." << std::endl; return {}; }));
	add(binary(4, "tvtooltip", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvtooltip)." << std::endl; return {}; }));
	add(binary(4, "ctrlsettext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsettext)." << std::endl; return {}; }));
	add(binary(4, "sidechat", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sidechat)." << std::endl; return {}; }));
	add(binary(4, "enableuavconnectability", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableuavconnectability)." << std::endl; return {}; }));
	add(binary(4, "addweaponglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addweaponglobal)." << std::endl; return {}; }));
	add(binary(4, "ctrlcommit", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlcommit)." << std::endl; return {}; }));
	add(binary(4, "nearestobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearestobject)." << std::endl; return {}; }));
	add(binary(4, "setlightdaylight", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setlightdaylight)." << std::endl; return {}; }));
	add(binary(4, "lightattachobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lightattachobject)." << std::endl; return {}; }));
	add(binary(4, "setcenterofmass", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcenterofmass)." << std::endl; return {}; }));
	add(binary(4, "hcsetgroup", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hcsetgroup)." << std::endl; return {}; }));
	add(binary(4, "setairportside", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setairportside)." << std::endl; return {}; }));
	add(binary(4, "setunitrank", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setunitrank)." << std::endl; return {}; }));
	add(binary(4, "setobjecttextureglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setobjecttextureglobal)." << std::endl; return {}; }));
	add(binary(4, "getrelpos", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getrelpos)." << std::endl; return {}; }));
	add(binary(4, "setsimpletasktarget", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setsimpletasktarget)." << std::endl; return {}; }));
	add(binary(4, "synchronizewaypoint", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (synchronizewaypoint)." << std::endl; return {}; }));
	add(binary(4, "lbsettext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsettext)." << std::endl; return {}; }));
	add(binary(4, "lnbdeleterow", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbdeleterow)." << std::endl; return {}; }));
	add(binary(4, "nearsupplies", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearsupplies)." << std::endl; return {}; }));
	add(binary(4, "copywaypoints", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (copywaypoints)." << std::endl; return {}; }));
	add(binary(4, "setleader", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setleader)." << std::endl; return {}; }));
	add(binary(4, "tvvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvvalue)." << std::endl; return {}; }));
	add(binary(4, "unitsbelowheight", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unitsbelowheight)." << std::endl; return {}; }));
	add(binary(4, "disableai", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (disableai)." << std::endl; return {}; }));
	add(binary(4, "setdirection", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdirection)." << std::endl; return {}; }));
	add(binary(4, "servercommand", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (servercommand)." << std::endl; return {}; }));
	add(binary(4, "settriggertimeout", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settriggertimeout)." << std::endl; return {}; }));
	add(binary(4, "lbisselected", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbisselected)." << std::endl; return {}; }));
	add(binary(4, "campreparedir", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (campreparedir)." << std::endl; return {}; }));
	add(binary(4, "lbsetdata", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetdata)." << std::endl; return {}; }));
	add(binary(4, "camsetdir", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camsetdir)." << std::endl; return {}; }));
	add(binary(4, "setposasl2", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setposasl2)." << std::endl; return {}; }));
	add(binary(4, "camsetbank", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camsetbank)." << std::endl; return {}; }));
	add(binary(4, "action", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (action)." << std::endl; return {}; }));
	add(binary(4, "removemagazineturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removemagazineturret)." << std::endl; return {}; }));
	add(binary(4, "selectweapon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selectweapon)." << std::endl; return {}; }));
	add(binary(4, "createtask", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createtask)." << std::endl; return {}; }));
	add(binary(4, "addeditorobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addeditorobject)." << std::endl; return {}; }));
	add(binary(4, "enableinfopanelcomponent", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableinfopanelcomponent)." << std::endl; return {}; }));
	add(binary(4, "linkitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (linkitem)." << std::endl; return {}; }));
	add(binary(4, "seteditorobjectscope", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (seteditorobjectscope)." << std::endl; return {}; }));
	add(binary(4, "tvsetpicturecolordisabled", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpicturecolordisabled)." << std::endl; return {}; }));
	add(binary(4, "showlegend", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showlegend)." << std::endl; return {}; }));
	add(binary(4, "setdir", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdir)." << std::endl; return {}; }));
	add(binary(4, "setobjectproxy", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setobjectproxy)." << std::endl; return {}; }));
	add(binary(4, "ordergetin", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ordergetin)." << std::endl; return {}; }));
	add(binary(4, "ctremoverows", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctremoverows)." << std::endl; return {}; }));
	add(binary(4, "getunittrait", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getunittrait)." << std::endl; return {}; }));
	add(binary(4, "hcremovegroup", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hcremovegroup)." << std::endl; return {}; }));
	add(binary(4, "lbsetpictureright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpictureright)." << std::endl; return {}; }));
	add(binary(4, "menusize", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusize)." << std::endl; return {}; }));
	add(binary(4, "issensortargetconfirmed", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (issensortargetconfirmed)." << std::endl; return {}; }));
	add(binary(4, "usermfdvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setusermfdvalue)." << std::endl; return {}; }));
	add(binary(4, "getsoundcontroller", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getsoundcontroller)." << std::endl; return {}; }));
	add(binary(4, "enablemimics", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablemimics)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth2b", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth2b)." << std::endl; return {}; }));
	add(binary(4, "setcustomaimcoef", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcustomaimcoef)." << std::endl; return {}; }));
	add(binary(4, "setairplanethrottle", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setairplanethrottle)." << std::endl; return {}; }));
	add(binary(4, "setsize", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setsize)." << std::endl; return {}; }));
	add(binary(4, "setunitpos", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setunitpos)." << std::endl; return {}; }));
	add(binary(4, "animatedoor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (animatedoor)." << std::endl; return {}; }));
	add(binary(4, "isequaltypeparams", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isequaltypeparams)." << std::endl; return {}; }));
	add(binary(4, "ctfindheaderrows", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctfindheaderrows)." << std::endl; return {}; }));
	add(binary(4, "gethit", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (gethit)." << std::endl; return {}; }));
	add(binary(4, "drawarrow", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drawarrow)." << std::endl; return {}; }));
	add(binary(4, "setvehicleposition", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehicleposition)." << std::endl; return {}; }));
	add(binary(4, "ctrlshow", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlshow)." << std::endl; return {}; }));
	add(binary(4, "removemagazineglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removemagazineglobal)." << std::endl; return {}; }));
	add(binary(4, "setlightflaremaxdistance", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setlightflaremaxdistance)." << std::endl; return {}; }));
	add(binary(4, "playmovenow", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playmovenow)." << std::endl; return {}; }));
	add(binary(4, "commandfire", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandfire)." << std::endl; return {}; }));
	add(binary(4, "forcespeed", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (forcespeed)." << std::endl; return {}; }));
	add(binary(4, "lbsetpicturerightcolordisabled", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpicturerightcolordisabled)." << std::endl; return {}; }));
	add(binary(4, "setvelocitytransformation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvelocitytransformation)." << std::endl; return {}; }));
	add(binary(4, "enablecollisionwith", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablecollisionwith)." << std::endl; return {}; }));
	add(binary(4, "displayremoveeventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (displayremoveeventhandler)." << std::endl; return {}; }));
	add(binary(4, "addmagazines", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmagazines)." << std::endl; return {}; }));
	add(binary(4, "menuaction", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuaction)." << std::endl; return {}; }));
	add(binary(4, "deletegroupwhenempty", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deletegroupwhenempty)." << std::endl; return {}; }));
	add(binary(4, "isflatempty", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isflatempty)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetstructuredtext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetstructuredtext)." << std::endl; return {}; }));
	add(binary(4, "addteammember", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addteammember)." << std::endl; return {}; }));
	add(binary(4, "revealmine", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (revealmine)." << std::endl; return {}; }));
	add(binary(4, "reportremotetarget", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (reportremotetarget)." << std::endl; return {}; }));
	add(binary(4, "setmusiceffect", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setmusiceffect)." << std::endl; return {}; }));
	add(binary(4, "switchgesture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (switchgesture)." << std::endl; return {}; }));
	add(binary(4, "setformation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setformation)." << std::endl; return {}; }));
	add(binary(4, "commandwatch", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandwatch)." << std::endl; return {}; }));
	add(binary(4, "lbcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbcolor)." << std::endl; return {}; }));
	add(binary(4, "targets", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (targets)." << std::endl; return {}; }));
	add(binary(4, "addcuratorcameraarea", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addcuratorcameraarea)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth6b", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth6b)." << std::endl; return {}; }));
	add(binary(4, "setpilotcamerarotation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setpilotcamerarotation)." << std::endl; return {}; }));
	add(binary(4, "fire", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (fire)." << std::endl; return {}; }));
	add(binary(4, "setautonomous", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setautonomous)." << std::endl; return {}; }));
	add(binary(4, "getsoundcontrollerresult", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getsoundcontrollerresult)." << std::endl; return {}; }));
	add(binary(4, "setvehiclevarname", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehiclevarname)." << std::endl; return {}; }));
	add(binary(4, "ctvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctvalue)." << std::endl; return {}; }));
	add(binary(4, "hasweapon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hasweapon)." << std::endl; return {}; }));
	add(binary(4, "ctrlsettextsecondary", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsettextsecondary)." << std::endl; return {}; }));
	add(binary(4, "setvehicletipars", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehicletipars)." << std::endl; return {}; }));
	add(binary(4, "setflagside", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setflagside)." << std::endl; return {}; }));
	add(binary(4, "ctrowcontrols", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrowcontrols)." << std::endl; return {}; }));
	add(binary(4, "setspeedmode", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setspeedmode)." << std::endl; return {}; }));
	add(binary(4, "removeeventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeeventhandler)." << std::endl; return {}; }));
	add(binary(4, "waypointattachobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointattachobject)." << std::endl; return {}; }));
	add(binary(4, "joinsilent", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (joinsilent)." << std::endl; return {}; }));
	add(binary(4, "tvsortbyvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsortbyvalue)." << std::endl; return {}; }));
	add(binary(4, "deletevehiclecrew", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deletevehiclecrew)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetautoscrollrewind", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetautoscrollrewind)." << std::endl; return {}; }));
	add(binary(4, "lbsetpicturecolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpicturecolor)." << std::endl; return {}; }));
	add(binary(4, "buildingpos", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (buildingpos)." << std::endl; return {}; }));
	add(binary(4, "setwaypointstatements", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointstatements)." << std::endl; return {}; }));
	add(binary(4, "setwaypointforcebehaviour", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointforcebehaviour)." << std::endl; return {}; }));
	add(binary(4, "setmimic", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setmimic)." << std::endl; return {}; }));
	add(binary(4, "engineon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (engineon)." << std::endl; return {}; }));
	add(binary(4, "setwindstr", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwindstr)." << std::endl; return {}; }));
	add(binary(4, "lbtextright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbtextright)." << std::endl; return {}; }));
	add(binary(4, "allowsprint", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allowsprint)." << std::endl; return {}; }));
	add(binary(4, "additemtobackpack", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (additemtobackpack)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetautoscrollspeed", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetautoscrollspeed)." << std::endl; return {}; }));
	add(binary(4, "hideselection", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hideselection)." << std::endl; return {}; }));
	add(binary(4, "removedrawlinks", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removedrawlinks)." << std::endl; return {}; }));
	add(binary(4, "setwaypointcompletionradius", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointcompletionradius)." << std::endl; return {}; }));
	add(binary(4, "setgroupiconparams", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setgroupiconparams)." << std::endl; return {}; }));
	add(binary(4, "lbcolorright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbcolorright)." << std::endl; return {}; }));
	add(binary(4, "setcuratoreditingareatype", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcuratoreditingareatype)." << std::endl; return {}; }));
	add(binary(4, "confirmsensortarget", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (confirmsensortarget)." << std::endl; return {}; }));
	add(binary(4, "forceweaponfire", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (forceweaponfire)." << std::endl; return {}; }));
	add(binary(4, "setlightambient", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setlightambient)." << std::endl; return {}; }));
	add(binary(4, "removesimpletask", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removesimpletask)." << std::endl; return {}; }));
	add(binary(4, "setdestination", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdestination)." << std::endl; return {}; }));
	add(binary(4, "menupicture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menupicture)." << std::endl; return {}; }));
	add(binary(4, "ctrlenable", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlenable)." << std::endl; return {}; }));
	add(binary(4, "groupchat", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (groupchat)." << std::endl; return {}; }));
	add(binary(4, "setrain", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setrain)." << std::endl; return {}; }));
	add(binary(4, "add3denlayer", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (add3denlayer)." << std::endl; return {}; }));
	add(binary(4, "getspeed", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getspeed)." << std::endl; return {}; }));
	add(binary(4, "addweaponitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addweaponitem)." << std::endl; return {}; }));
	add(binary(4, "cuttext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cuttext)." << std::endl; return {}; }));
	add(binary(4, "settaskstate", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settaskstate)." << std::endl; return {}; }));
	add(binary(4, "triggerattachvehicle", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggerattachvehicle)." << std::endl; return {}; }));
	add(binary(4, "setcamuseti", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcamuseti)." << std::endl; return {}; }));
	add(binary(4, "addcuratoraddons", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addcuratoraddons)." << std::endl; return {}; }));
	add(binary(4, "addbackpackcargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addbackpackcargo)." << std::endl; return {}; }));
	add(binary(4, "join", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (join)." << std::endl; return {}; }));
	add(binary(4, "glanceat", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (glanceat)." << std::endl; return {}; }));
	add(binary(4, "displayremovealleventhandlers", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (displayremovealleventhandlers)." << std::endl; return {}; }));
	add(binary(4, "ctrlmapscreentoworld", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmapscreentoworld)." << std::endl; return {}; }));
	add(binary(4, "setwaypointname", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointname)." << std::endl; return {}; }));
	add(binary(4, "canadd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (canadd)." << std::endl; return {}; }));
	add(binary(4, "counttype", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (counttype)." << std::endl; return {}; }));
	add(binary(4, "moveobjecttoend", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (moveobjecttoend)." << std::endl; return {}; }));
	add(binary(4, "removemagazinesturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removemagazinesturret)." << std::endl; return {}; }));
	add(binary(4, "tvsetcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetcolor)." << std::endl; return {}; }));
	add(binary(4, "setrectangular", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setrectangular)." << std::endl; return {}; }));
	add(binary(4, "menuenable", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuenable)." << std::endl; return {}; }));
	add(binary(4, "ctheadercontrols", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctheadercontrols)." << std::endl; return {}; }));
	add(binary(4, "suppressfor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (suppressfor)." << std::endl; return {}; }));
	add(binary(4, "ctrlsettextcolorsecondary", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsettextcolorsecondary)." << std::endl; return {}; }));
	add(binary(4, "inserteditorobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (inserteditorobject)." << std::endl; return {}; }));
	add(binary(4, "set3denlogictype", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3denlogictype)." << std::endl; return {}; }));
	add(binary(4, "setdynamicsimulationdistancecoef", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdynamicsimulationdistancecoef)." << std::endl; return {}; }));
	add(binary(4, "fadespeech", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (fadespeech)." << std::endl; return {}; }));
	add(binary(4, "setpylonspriority", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setpylonspriority)." << std::endl; return {}; }));
	add(binary(4, "closedisplay", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (closedisplay)." << std::endl; return {}; }));
	add(binary(4, "menuvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuvalue)." << std::endl; return {}; }));
	add(binary(4, "settaskresult", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settaskresult)." << std::endl; return {}; }));
	add(binary(4, "setlightuseflare", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setlightuseflare)." << std::endl; return {}; }));
	add(binary(4, "breakout", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (breakout)." << std::endl; return {}; }));
	add(binary(4, "isequaltypeany", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isequaltypeany)." << std::endl; return {}; }));
	add(binary(4, "drawtriangle", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drawtriangle)." << std::endl; return {}; }));
	add(binary(4, "lbsetselectcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetselectcolor)." << std::endl; return {}; }));
	add(binary(4, "mapcenteroncamera", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (mapcenteroncamera)." << std::endl; return {}; }));
	add(binary(4, "lbsetpicture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpicture)." << std::endl; return {}; }));
	add(binary(4, "setammocargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setammocargo)." << std::endl; return {}; }));
	add(binary(4, "addbackpackcargoglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addbackpackcargoglobal)." << std::endl; return {}; }));
	add(binary(4, "createsimpletask", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createsimpletask)." << std::endl; return {}; }));
	add(binary(4, "commandfsm", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandfsm)." << std::endl; return {}; }));
	add(binary(4, "setcamerainterest", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcamerainterest)." << std::endl; return {}; }));
	add(binary(4, "switchaction", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (switchaction)." << std::endl; return {}; }));
	add(binary(4, "animate", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (animate)." << std::endl; return {}; }));
	add(binary(4, "setcuratorwaypointcost", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcuratorwaypointcost)." << std::endl; return {}; }));
	add(binary(4, "diarysubjectexists", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (diarysubjectexists)." << std::endl; return {}; }));
	add(binary(4, "selectrandomweighted", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selectrandomweighted)." << std::endl; return {}; }));
	add(binary(4, "setslingload", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setslingload)." << std::endl; return {}; }));
	add(binary(4, "random", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (random)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfontheight", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfontheight)." << std::endl; return {}; }));
	add(binary(4, "ctrlchecked", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlchecked)." << std::endl; return {}; }));
	add(binary(4, "tvsettooltip", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsettooltip)." << std::endl; return {}; }));
	add(binary(4, "setfaceanimation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setfaceanimation)." << std::endl; return {}; }));
	add(binary(4, "setflaganimationphase", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setflaganimationphase)." << std::endl; return {}; }));
	add(binary(4, "addhandgunitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addhandgunitem)." << std::endl; return {}; }));
	add(binary(4, "setwaypointloiterradius", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointloiterradius)." << std::endl; return {}; }));
	add(binary(4, "setpipeffect", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setpipeffect)." << std::endl; return {}; }));
	add(binary(4, "playactionnow", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playactionnow)." << std::endl; return {}; }));
	add(binary(4, "weapondirection", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weapondirection)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetautoscrolldelay", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetautoscrolldelay)." << std::endl; return {}; }));
	add(binary(4, "countunknown", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (countunknown)." << std::endl; return {}; }));
	add(binary(4, "ctrlmapworldtoscreen", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmapworldtoscreen)." << std::endl; return {}; }));
	add(binary(4, "ropeattachto", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropeattachto)." << std::endl; return {}; }));
	add(binary(4, "addpublicvariableeventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addpublicvariableeventhandler)." << std::endl; return {}; }));
	add(binary(4, "getpos", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getpos)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetactivecolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetactivecolor)." << std::endl; return {}; }));
	add(binary(4, "moveincargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (moveincargo)." << std::endl; return {}; }));
	add(binary(4, "canadditemtouniform", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (canadditemtouniform)." << std::endl; return {}; }));
	add(binary(4, "setbleedingremaining", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setbleedingremaining)." << std::endl; return {}; }));
	add(binary(4, "setlightflaresize", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setlightflaresize)." << std::endl; return {}; }));
	add(binary(4, "removemagazines", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removemagazines)." << std::endl; return {}; }));
	add(binary(4, "vectormodeltoworldvisual", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vectormodeltoworldvisual)." << std::endl; return {}; }));
	add(binary(4, "execeditorscript", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (execeditorscript)." << std::endl; return {}; }));
	add(binary(4, "setparticleclass", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setparticleclass)." << std::endl; return {}; }));
	add(binary(4, "setparticlecircle", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setparticlecircle)." << std::endl; return {}; }));
	add(binary(4, "foreachmember", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (foreachmember)." << std::endl; return {}; }));
	add(binary(4, "setvelocitymodelspace", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvelocitymodelspace)." << std::endl; return {}; }));
	add(binary(4, "animatebay", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (animatebay)." << std::endl; return {}; }));
	add(binary(4, "findcover", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (findcover)." << std::endl; return {}; }));
	add(binary(4, "vectorfromto", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vectorfromto)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth1", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth1)." << std::endl; return {}; }));
	add(binary(4, "ctrlsettooltipcolorbox", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsettooltipcolorbox)." << std::endl; return {}; }));
	add(binary(4, "drawlocation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drawlocation)." << std::endl; return {}; }));
	add(binary(4, "setwaypointdescription", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointdescription)." << std::endl; return {}; }));
	add(binary(4, "menusetpicture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusetpicture)." << std::endl; return {}; }));
	add(binary(4, "additemcargoglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (additemcargoglobal)." << std::endl; return {}; }));
	add(binary(4, "radiochannelremove", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (radiochannelremove)." << std::endl; return {}; }));
	add(binary(4, "removedrawicon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removedrawicon)." << std::endl; return {}; }));
	add(binary(4, "setlightattenuation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setlightattenuation)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth2", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth2)." << std::endl; return {}; }));
	add(binary(4, "setname", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setname)." << std::endl; return {}; }));
	add(binary(4, "lbvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbvalue)." << std::endl; return {}; }));
	add(binary(4, "addgroupicon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addgroupicon)." << std::endl; return {}; }));
	add(binary(4, "menuexpand", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuexpand)." << std::endl; return {}; }));
	add(binary(4, "ppeffectforceinnvg", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ppeffectforceinnvg)." << std::endl; return {}; }));
	add(binary(4, "setgroupowner", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setgroupowner)." << std::endl; return {}; }));
	add(binary(4, "setvehiclearmor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehiclearmor)." << std::endl; return {}; }));
	add(binary(4, "addmenu", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmenu)." << std::endl; return {}; }));
	add(binary(4, "savestatus", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (savestatus)." << std::endl; return {}; }));
	add(binary(4, "ctrlmapanimadd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmapanimadd)." << std::endl; return {}; }));
	add(binary(4, "setstamina", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setstamina)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth3", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth3)." << std::endl; return {}; }));
	add(binary(4, "setwaves", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaves)." << std::endl; return {}; }));
	add(binary(4, "slidersetposition", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (slidersetposition)." << std::endl; return {}; }));
	add(binary(4, "campreparerelpos", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (campreparerelpos)." << std::endl; return {}; }));
	add(binary(4, "setconvoyseparation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setconvoyseparation)." << std::endl; return {}; }));
	add(binary(4, "enablefatigue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablefatigue)." << std::endl; return {}; }));
	add(binary(4, "moveindriver", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (moveindriver)." << std::endl; return {}; }));
	add(binary(4, "selectdiarysubject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selectdiarysubject)." << std::endl; return {}; }));
	add(binary(4, "canslingload", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (canslingload)." << std::endl; return {}; }));
	add(binary(4, "createmissiondisplay", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createmissiondisplay)." << std::endl; return {}; }));
	add(binary(4, "setfuelcargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setfuelcargo)." << std::endl; return {}; }));
	add(binary(4, "dotarget", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dotarget)." << std::endl; return {}; }));
	add(binary(4, "addvest", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addvest)." << std::endl; return {}; }));
	add(binary(4, "remoteexeccall", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (remoteexeccall)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth4", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth4)." << std::endl; return {}; }));
	add(binary(4, "kbreact", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (kbreact)." << std::endl; return {}; }));
	add(binary(4, "land", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (land)." << std::endl; return {}; }));
	add(binary(4, "enablegunlights", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablegunlights)." << std::endl; return {}; }));
	add(binary(4, "setowner", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setowner)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth5", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth5)." << std::endl; return {}; }));
	add(binary(4, "lnbsetpictureright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpictureright)." << std::endl; return {}; }));
	add(binary(4, "ctremoveheaders", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctremoveheaders)." << std::endl; return {}; }));
	add(binary(4, "switchcamera", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (switchcamera)." << std::endl; return {}; }));
	add(binary(4, "setunloadincombat", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setunloadincombat)." << std::endl; return {}; }));
	add(binary(4, "inarea", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (inarea)." << std::endl; return {}; }));
	add(binary(4, "skillfinal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (skillfinal)." << std::endl; return {}; }));
	add(binary(4, "loadidentity", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (loadidentity)." << std::endl; return {}; }));
	add(binary(4, "addsecondaryweaponitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addsecondaryweaponitem)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth6", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth6)." << std::endl; return {}; }));
	add(binary(4, "removecuratorcameraarea", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removecuratorcameraarea)." << std::endl; return {}; }));
	add(binary(4, "removeteammember", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeteammember)." << std::endl; return {}; }));
	add(binary(4, "buttonsetaction", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (buttonsetaction)." << std::endl; return {}; }));
	add(binary(4, "menudelete", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menudelete)." << std::endl; return {}; }));
	add(binary(4, "joinassilent", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (joinassilent)." << std::endl; return {}; }));
	add(binary(4, "modeltoworldworld", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (modeltoworldworld)." << std::endl; return {}; }));
	add(binary(4, "lnbcolorright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbcolorright)." << std::endl; return {}; }));
	add(binary(4, "tvcollapse", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvcollapse)." << std::endl; return {}; }));
	add(binary(4, "inflame", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (inflame)." << std::endl; return {}; }));
	add(binary(4, "lnbsetcolumnspos", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetcolumnspos)." << std::endl; return {}; }));
	add(binary(4, "say2d", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (say2d)." << std::endl; return {}; }));
	add(binary(4, "commandchat", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandchat)." << std::endl; return {}; }));
	add(binary(4, "hintc", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hintc)." << std::endl; return {}; }));
	add(binary(4, "tvsetcursel", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetcursel)." << std::endl; return {}; }));
	add(binary(4, "setwaypointvisible", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointvisible)." << std::endl; return {}; }));
	add(binary(4, "nearobjects", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearobjects)." << std::endl; return {}; }));
	add(binary(4, "hideobjectglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hideobjectglobal)." << std::endl; return {}; }));
	add(binary(4, "lnbcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbcolor)." << std::endl; return {}; }));
	add(binary(4, "setsimpletaskdestination", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setsimpletaskdestination)." << std::endl; return {}; }));
	add(binary(4, "commandartilleryfire", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandartilleryfire)." << std::endl; return {}; }));
	add(binary(4, "setvehicleradar", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehicleradar)." << std::endl; return {}; }));
	add(binary(4, "dosuppressivefire", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dosuppressivefire)." << std::endl; return {}; }));
	add(binary(4, "assignteam", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignteam)." << std::endl; return {}; }));
	add(binary(4, "kbhastopic", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (kbhastopic)." << std::endl; return {}; }));
	add(binary(4, "menutext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menutext)." << std::endl; return {}; }));
	add(binary(4, "lnbaddcolumn", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbaddcolumn)." << std::endl; return {}; }));
	add(binary(4, "settriggeractivation", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settriggeractivation)." << std::endl; return {}; }));
	add(binary(4, "additemtovest", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (additemtovest)." << std::endl; return {}; }));
	add(binary(4, "assignitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignitem)." << std::endl; return {}; }));
	add(binary(4, "adduniform", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (adduniform)." << std::endl; return {}; }));
	add(binary(4, "globalchat", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (globalchat)." << std::endl; return {}; }));
	add(binary(4, "menushortcuttext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menushortcuttext)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfonth3b", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfonth3b)." << std::endl; return {}; }));
	add(binary(4, "stop", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (stop)." << std::endl; return {}; }));
	add(binary(4, "setvehicleammo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehicleammo)." << std::endl; return {}; }));
	add(binary(4, "lnbsetpicture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpicture)." << std::endl; return {}; }));
	add(binary(4, "setcombatmode", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcombatmode)." << std::endl; return {}; }));
	add(binary(4, "tvsetvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetvalue)." << std::endl; return {}; }));
	add(binary(4, "setobjecttexture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setobjecttexture)." << std::endl; return {}; }));
	add(binary(4, "assigntoairport", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assigntoairport)." << std::endl; return {}; }));
	add(binary(4, "enableattack", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableattack)." << std::endl; return {}; }));
	add(binary(4, "createmenu", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createmenu)." << std::endl; return {}; }));
	add(binary(4, "debugfsm", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (debugfsm)." << std::endl; return {}; }));
	add(binary(4, "settext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settext)." << std::endl; return {}; }));
	add(binary(4, "camsetfovrange", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camsetfovrange)." << std::endl; return {}; }));
	add(binary(4, "synchronizeobjectsremove", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (synchronizeobjectsremove)." << std::endl; return {}; }));
	add(binary(4, "menudata", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menudata)." << std::endl; return {}; }));
	add(binary(4, "modeltoworld", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (modeltoworld)." << std::endl; return {}; }));
	add(binary(4, "checkvisibility", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (checkvisibility)." << std::endl; return {}; }));
	add(binary(4, "kbremovetopic", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (kbremovetopic)." << std::endl; return {}; }));
	add(binary(4, "settriggerarea", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settriggerarea)." << std::endl; return {}; }));
	add(binary(4, "campreparedive", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (campreparedive)." << std::endl; return {}; }));
	add(binary(4, "setwaypointtimeout", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointtimeout)." << std::endl; return {}; }));
	add(binary(4, "enableaimprecision", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableaimprecision)." << std::endl; return {}; }));
	add(binary(4, "assignascargoindex", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignascargoindex)." << std::endl; return {}; }));
	add(binary(4, "emptypositions", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (emptypositions)." << std::endl; return {}; }));
	add(binary(4, "commandmove", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandmove)." << std::endl; return {}; }));
	add(binary(4, "ammo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ammo)." << std::endl; return {}; }));
	add(binary(4, "ctsetrowtemplate", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctsetrowtemplate)." << std::endl; return {}; }));
	add(binary(4, "allowfleeing", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allowfleeing)." << std::endl; return {}; }));
	add(binary(4, "setflagowner", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setflagowner)." << std::endl; return {}; }));
	add(binary(4, "targetsaggregate", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (targetsaggregate)." << std::endl; return {}; }));
	add(binary(4, "gethitpointdamage", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (gethitpointdamage)." << std::endl; return {}; }));
	add(binary(4, "leavevehicle", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (leavevehicle)." << std::endl; return {}; }));
	add(binary(4, "unassignitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unassignitem)." << std::endl; return {}; }));
	add(binary(4, "setspeech", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setspeech)." << std::endl; return {}; }));
	add(binary(4, "addheadgear", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addheadgear)." << std::endl; return {}; }));
	add(binary(4, "setfeaturetype", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setfeaturetype)." << std::endl; return {}; }));
	add(binary(4, "set3denlayer", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3denlayer)." << std::endl; return {}; }));
	add(binary(4, "setvectordirandup", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvectordirandup)." << std::endl; return {}; }));
	add(binary(4, "lnbsetpicturecolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpicturecolor)." << std::endl; return {}; }));
	add(binary(4, "commandtarget", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandtarget)." << std::endl; return {}; }));
	add(binary(4, "flyinheight", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (flyinheight)." << std::endl; return {}; }));
	add(binary(4, "setside", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setside)." << std::endl; return {}; }));
	add(binary(4, "lockdriver", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lockdriver)." << std::endl; return {}; }));
	add(binary(4, "addwaypoint", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addwaypoint)." << std::endl; return {}; }));
	add(binary(4, "settriggertext", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settriggertext)." << std::endl; return {}; }));
	add(binary(4, "synchronizetrigger", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (synchronizetrigger)." << std::endl; return {}; }));
	add(binary(4, "getobjectargument", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getobjectargument)." << std::endl; return {}; }));
	add(binary(4, "addeventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addeventhandler)." << std::endl; return {}; }));
	add(binary(4, "sethit", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sethit)." << std::endl; return {}; }));
	add(binary(4, "tvsetpicturerightcolorselected", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpicturerightcolorselected)." << std::endl; return {}; }));
	add(binary(4, "exec", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (exec)." << std::endl; return {}; }));
	add(binary(4, "setovercast", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setovercast)." << std::endl; return {}; }));
	add(binary(4, "getcargoindex", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getcargoindex)." << std::endl; return {}; }));
	add(binary(4, "setradiomsg", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setradiomsg)." << std::endl; return {}; }));
	add(binary(4, "setvehiclereportremotetargets", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehiclereportremotetargets)." << std::endl; return {}; }));
	add(binary(4, "menusetaction", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusetaction)." << std::endl; return {}; }));
	add(binary(4, "setsoundeffect", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setsoundeffect)." << std::endl; return {}; }));
	add(binary(4, "objstatus", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (objstatus)." << std::endl; return {}; }));
	add(binary(4, "geteditorobjectscope", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (geteditorobjectscope)." << std::endl; return {}; }));
	add(binary(4, "customchat", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (customchat)." << std::endl; return {}; }));
	add(binary(4, "move", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (move)." << std::endl; return {}; }));
	add(binary(4, "setrotorbrakertd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setrotorbrakertd)." << std::endl; return {}; }));
	add(binary(4, "ctsetcursel", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctsetcursel)." << std::endl; return {}; }));
	add(binary(4, "inrangeofartillery", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (inrangeofartillery)." << std::endl; return {}; }));
	add(binary(4, "listobjects", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (listobjects)." << std::endl; return {}; }));
	add(binary(4, "connectterminaltouav", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (connectterminaltouav)." << std::endl; return {}; }));
	add(binary(4, "setwingforcescalertd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwingforcescalertd)." << std::endl; return {}; }));
	add(binary(4, "ctrladdeventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrladdeventhandler)." << std::endl; return {}; }));
	add(binary(4, "setparticlefire", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setparticlefire)." << std::endl; return {}; }));
	add(binary(4, "lbsetpicturecolorselected", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpicturecolorselected)." << std::endl; return {}; }));
	add(binary(4, "setskill", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setskill)." << std::endl; return {}; }));
	add(binary(4, "removeweaponcargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeweaponcargo)." << std::endl; return {}; }));
	add(binary(4, "unlinkitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unlinkitem)." << std::endl; return {}; }));
	add(binary(4, "currentmagazineturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentmagazineturret)." << std::endl; return {}; }));
	add(binary(4, "forgettarget", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (forgettarget)." << std::endl; return {}; }));
	add(binary(4, "setbrakesrtd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setbrakesrtd)." << std::endl; return {}; }));
	add(binary(4, "worldtomodelvisual", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (worldtomodelvisual)." << std::endl; return {}; }));
	add(binary(4, "faderadio", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (faderadio)." << std::endl; return {}; }));
	add(binary(4, "setoxygenremaining", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setoxygenremaining)." << std::endl; return {}; }));
	add(binary(4, "getobjectchildren", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getobjectchildren)." << std::endl; return {}; }));
	add(binary(4, "setimportance", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setimportance)." << std::endl; return {}; }));
	add(binary(4, "countfriendly", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (countfriendly)." << std::endl; return {}; }));
	add(binary(4, "camcommand", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camcommand)." << std::endl; return {}; }));
	add(binary(4, "removegroupicon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removegroupicon)." << std::endl; return {}; }));
	add(binary(4, "publicvariableclient", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (publicvariableclient)." << std::endl; return {}; }));
	add(binary(4, "vehiclechat", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vehiclechat)." << std::endl; return {}; }));
	add(binary(4, "radiochanneladd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (radiochanneladd)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetmodelscale", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetmodelscale)." << std::endl; return {}; }));
	add(binary(4, "lnbpicture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbpicture)." << std::endl; return {}; }));
	add(binary(4, "findemptypositionready", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (findemptypositionready)." << std::endl; return {}; }));
	add(binary(4, "enableautotrimrtd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableautotrimrtd)." << std::endl; return {}; }));
	add(binary(4, "assignasdriver", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignasdriver)." << std::endl; return {}; }));
	add(binary(4, "enablecopilot", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablecopilot)." << std::endl; return {}; }));
	add(binary(4, "menuadd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuadd)." << std::endl; return {}; }));
	add(binary(4, "landat", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (landat)." << std::endl; return {}; }));
	add(binary(4, "onshownewobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onshownewobject)." << std::endl; return {}; }));
	add(binary(4, "setwaypointloitertype", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointloitertype)." << std::endl; return {}; }));
	add(binary(4, "allowcrewinimmobile", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allowcrewinimmobile)." << std::endl; return {}; }));
	add(binary(4, "settriggerstatements", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settriggerstatements)." << std::endl; return {}; }));
	add(binary(4, "setgusts", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setgusts)." << std::endl; return {}; }));
	add(binary(4, "setrank", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setrank)." << std::endl; return {}; }));
	add(binary(4, "clear3denattribute", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clear3denattribute)." << std::endl; return {}; }));
	add(binary(4, "enableirlasers", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableirlasers)." << std::endl; return {}; }));
	add(binary(4, "enablechannel", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablechannel)." << std::endl; return {}; }));
	add(binary(4, "getenvsoundcontroller", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getenvsoundcontroller)." << std::endl; return {}; }));
	add(binary(4, "lnbtextright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbtextright)." << std::endl; return {}; }));
	add(binary(4, "canadditemtovest", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (canadditemtovest)." << std::endl; return {}; }));
	add(binary(4, "tvsetpicturerightcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpicturerightcolor)." << std::endl; return {}; }));
	add(binary(4, "addscoreside", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addscoreside)." << std::endl; return {}; }));
	add(binary(4, "enableropeattach", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableropeattach)." << std::endl; return {}; }));
	add(binary(4, "setvehicleammodef", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehicleammodef)." << std::endl; return {}; }));
	add(binary(4, "enableautostartuprtd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableautostartuprtd)." << std::endl; return {}; }));
	add(binary(4, "setmagazineturretammo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setmagazineturretammo)." << std::endl; return {}; }));
	add(binary(4, "gethidefrom", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (gethidefrom)." << std::endl; return {}; }));
	add(binary(4, "ondoubleclick", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ondoubleclick)." << std::endl; return {}; }));
	add(binary(4, "removeweaponattachmentcargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeweaponattachmentcargo)." << std::endl; return {}; }));
	add(binary(4, "setrepaircargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setrepaircargo)." << std::endl; return {}; }));
	add(binary(4, "setunconscious", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setunconscious)." << std::endl; return {}; }));
	add(binary(4, "setgroupicon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setgroupicon)." << std::endl; return {}; }));
	add(binary(4, "setwaypointbehaviour", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwaypointbehaviour)." << std::endl; return {}; }));
	add(binary(4, "slidersetspeed", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (slidersetspeed)." << std::endl; return {}; }));
	add(binary(4, "inareaarray", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (inareaarray)." << std::endl; return {}; }));
	add(binary(4, "createdisplay", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createdisplay)." << std::endl; return {}; }));
	add(binary(4, "lnbvalue", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbvalue)." << std::endl; return {}; }));
	add(binary(4, "tvpicture", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvpicture)." << std::endl; return {}; }));
	add(binary(4, "setcuratorcoef", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcuratorcoef)." << std::endl; return {}; }));
	add(binary(4, "lnbsetpicturecolorselected", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpicturecolorselected)." << std::endl; return {}; }));
	add(binary(4, "addcuratoreditableobjects", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addcuratoreditableobjects)." << std::endl; return {}; }));
	add(binary(4, "infopanelcomponents", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (infopanelcomponents)." << std::endl; return {}; }));
	add(binary(4, "addbackpackglobal", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addbackpackglobal)." << std::endl; return {}; }));
	add(binary(4, "kbwassaid", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (kbwassaid)." << std::endl; return {}; }));
	add(binary(4, "getcompatiblepylonmagazines", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getcompatiblepylonmagazines)." << std::endl; return {}; }));
	add(binary(4, "evalobjectargument", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (evalobjectargument)." << std::endl; return {}; }));
	add(binary(4, "hcselectgroup", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hcselectgroup)." << std::endl; return {}; }));
	add(binary(4, "menusetcheck", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusetcheck)." << std::endl; return {}; }));
	add(binary(4, "removecuratoreditableobjects", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removecuratoreditableobjects)." << std::endl; return {}; }));
	add(binary(4, "lnbsetcolorright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetcolorright)." << std::endl; return {}; }));
	add(binary(4, "addbackpack", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addbackpack)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfontheightsecondary", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfontheightsecondary)." << std::endl; return {}; }));
	add(binary(4, "setfsmvariable", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setfsmvariable)." << std::endl; return {}; }));
	add(binary(4, "campreparefovrange", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (campreparefovrange)." << std::endl; return {}; }));
	add(binary(4, "moveincommander", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (moveincommander)." << std::endl; return {}; }));
	add(binary(4, "groupselectunit", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (groupselectunit)." << std::endl; return {}; }));
	add(binary(4, "foreachmemberagent", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (foreachmemberagent)." << std::endl; return {}; }));
	add(binary(4, "lnbsetcolor", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetcolor)." << std::endl; return {}; }));
	add(binary(4, "fireattarget", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (fireattarget)." << std::endl; return {}; }));
	add(binary(4, "ctrlseteventhandler", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlseteventhandler)." << std::endl; return {}; }));
	add(binary(4, "setdriveonpath", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdriveonpath)." << std::endl; return {}; }));
	add(binary(4, "setenginerpmrtd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setenginerpmrtd)." << std::endl; return {}; }));
	add(binary(4, "foreachmemberteam", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (foreachmemberteam)." << std::endl; return {}; }));
	add(binary(4, "addownedmine", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addownedmine)." << std::endl; return {}; }));
	add(binary(4, "showneweditorobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showneweditorobject)." << std::endl; return {}; }));
	add(binary(4, "cameraeffect", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cameraeffect)." << std::endl; return {}; }));
	add(binary(4, "setvehiclecargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehiclecargo)." << std::endl; return {}; }));
	add(binary(4, "getartilleryeta", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getartilleryeta)." << std::endl; return {}; }));
	add(binary(4, "lookatpos", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lookatpos)." << std::endl; return {}; }));
	add(binary(4, "dofsm", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dofsm)." << std::endl; return {}; }));
	add(binary(4, "setobjectmaterial", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setobjectmaterial)." << std::endl; return {}; }));
	add(binary(4, "fademusic", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (fademusic)." << std::endl; return {}; }));
	add(binary(4, "removealleventhandlers", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removealleventhandlers)." << std::endl; return {}; }));
	add(binary(4, "camsetpos", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camsetpos)." << std::endl; return {}; }));
	add(binary(4, "joinas", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (joinas)." << std::endl; return {}; }));
	add(binary(4, "moveingunner", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (moveingunner)." << std::endl; return {}; }));
	add(binary(4, "campreparepos", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (campreparepos)." << std::endl; return {}; }));
	add(binary(4, "disablecollisionwith", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (disablecollisionwith)." << std::endl; return {}; }));
	add(binary(4, "isequaltypearray", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isequaltypearray)." << std::endl; return {}; }));
	add(binary(4, "setvehiclelock", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setvehiclelock)." << std::endl; return {}; }));
	add(binary(4, "addprimaryweaponitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addprimaryweaponitem)." << std::endl; return {}; }));
	add(binary(4, "lbsetcursel", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetcursel)." << std::endl; return {}; }));
	add(binary(4, "seteffectcondition", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (seteffectcondition)." << std::endl; return {}; }));
	add(binary(4, "aimedattarget", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (aimedattarget)." << std::endl; return {}; }));
	add(binary(4, "drawellipse", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drawellipse)." << std::endl; return {}; }));
	add(binary(4, "setactualcollectivertd", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setactualcollectivertd)." << std::endl; return {}; }));
	add(binary(4, "editobject", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (editobject)." << std::endl; return {}; }));
	add(binary(4, "lnbsetpicturecolorright", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpicturecolorright)." << std::endl; return {}; }));
	add(binary(4, "allowdammage", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allowdammage)." << std::endl; return {}; }));
	add(binary(4, "setbehaviour", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setbehaviour)." << std::endl; return {}; }));
	add(binary(4, "findnearestenemy", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (findnearestenemy)." << std::endl; return {}; }));
	add(binary(4, "dowatch", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dowatch)." << std::endl; return {}; }));
	add(binary(4, "isirlaseron", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isirlaseron)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfontheighth1", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfontheighth1)." << std::endl; return {}; }));
	add(binary(4, "nearentities", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearentities)." << std::endl; return {}; }));
	add(binary(4, "tvsetpicturerightcolordisabled", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpicturerightcolordisabled)." << std::endl; return {}; }));
	add(binary(4, "lock", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lock)." << std::endl; return {}; }));
	add(binary(4, "weaponaccessories", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weaponaccessories)." << std::endl; return {}; }));
	add(binary(4, "magazinesturret", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazinesturret)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfontheighth2", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfontheighth2)." << std::endl; return {}; }));
	add(binary(4, "campreparefocus", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (campreparefocus)." << std::endl; return {}; }));
	add(binary(4, "setpitch", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setpitch)." << std::endl; return {}; }));
	add(binary(4, "getobjectproxy", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getobjectproxy)." << std::endl; return {}; }));
	add(binary(4, "settaskmarkeroffset", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settaskmarkeroffset)." << std::endl; return {}; }));
	add(binary(4, "setpilotcameradirection", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setpilotcameradirection)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfontheighth3", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfontheighth3)." << std::endl; return {}; }));
	add(binary(4, "settargetage", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settargetage)." << std::endl; return {}; }));
	add(binary(4, "setpilotcameratarget", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setpilotcameratarget)." << std::endl; return {}; }));
	add(binary(4, "removesecondaryweaponitem", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removesecondaryweaponitem)." << std::endl; return {}; }));
	add(binary(4, "weaponaccessoriescargo", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weaponaccessoriescargo)." << std::endl; return {}; }));
	add(binary(4, "removeweapon", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeweapon)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfontheighth4", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfontheighth4)." << std::endl; return {}; }));
	add(binary(4, "menusort", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusort)." << std::endl; return {}; }));
	add(binary(4, "ctrlsetfontheighth5", type::ANY, type::ANY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfontheighth5)." << std::endl; return {}; }));



	add(unary("showradio", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showradio)." << std::endl; return {}; })); /* BOOL */
	add(unary("vectorup", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vectorup)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lnbsetcurselrow", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetcurselrow)." << std::endl; return {}; })); /* ARRAY */
	add(unary("animationnames", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (animationnames)." << std::endl; return {}; })); /* OBJECT */
	add(unary("currentzeroing", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentzeroing)." << std::endl; return {}; })); /* OBJECT */
	add(unary("isonroad", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isonroad)." << std::endl; return {}; })); /* ARRAY,OBJECT */
	add(unary("triggerarea", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggerarea)." << std::endl; return {}; })); /* OBJECT */
	add(unary("get3dengrid", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (get3dengrid)." << std::endl; return {}; })); /* STRING */
	add(unary("settrafficspeed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settrafficspeed)." << std::endl; return {}; })); /* ARRAY */
	add(unary("supportinfo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (supportinfo)." << std::endl; return {}; })); /* STRING */
	add(unary("menucollapse", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menucollapse)." << std::endl; return {}; })); /* ARRAY */
	add(unary("taskdescription", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (taskdescription)." << std::endl; return {}; })); /* TASK */
	add(unary("isautohoveron", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isautohoveron)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlangle", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlangle)." << std::endl; return {}; })); /* CONTROL */
	add(unary("menusetdata", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusetdata)." << std::endl; return {}; })); /* ARRAY */
	add(unary("moonphase", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (moonphase)." << std::endl; return {}; })); /* ARRAY */
	add(unary("onplayerdisconnected", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onplayerdisconnected)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("isshowing3dicons", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isshowing3dicons)." << std::endl; return {}; })); /* CONTROL */
	add(unary("lbsetcolor", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetcolor)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getallhitpointsdamage", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getallhitpointsdamage)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbsetcolorright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetcolorright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("triggertext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggertext)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getfatigue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getfatigue)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlshown", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlshown)." << std::endl; return {}; })); /* CONTROL */
	add(unary("eyedirection", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (eyedirection)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlmodel", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmodel)." << std::endl; return {}; })); /* CONTROL */
	add(unary("tvtext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvtext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("ropeunwind", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropeunwind)." << std::endl; return {}; })); /* ARRAY */
	add(unary("fillweaponsfrompool", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (fillweaponsfrompool)." << std::endl; return {}; })); /* OBJECT */
	add(unary("hcselected", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hcselected)." << std::endl; return {}; })); /* OBJECT */
	add(unary("asltoagl", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (asltoagl)." << std::endl; return {}; })); /* ARRAY */
	add(unary("isobjectrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isobjectrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("enabletraffic", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enabletraffic)." << std::endl; return {}; })); /* BOOL */
	add(unary("roledescription", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (roledescription)." << std::endl; return {}; })); /* OBJECT */
	add(unary("simpletasks", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (simpletasks)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getammocargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getammocargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("flagowner", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (flagowner)." << std::endl; return {}; })); /* OBJECT */
	add(unary("tvdata", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvdata)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getenginetargetrpmrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getenginetargetrpmrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("waypointattachedvehicle", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointattachedvehicle)." << std::endl; return {}; })); /* ARRAY */
	add(unary("speed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (speed)." << std::endl; return {}; })); /* OBJECT */
	add(unary("menusetvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusetvalue)." << std::endl; return {}; })); /* ARRAY */
	add(unary("reloadenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (reloadenabled)." << std::endl; return {}; })); /* OBJECT */
	add(unary("rating", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (rating)." << std::endl; return {}; })); /* OBJECT */
	add(unary("hideobject", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hideobject)." << std::endl; return {}; })); /* OBJECT */
	add(unary("useaiopermapobstructiontest", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (useaiopermapobstructiontest)." << std::endl; return {}; })); /* BOOL */
	add(unary("primaryweapon", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (primaryweapon)." << std::endl; return {}; })); /* OBJECT */
	add(unary("addtoremainscollector", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addtoremainscollector)." << std::endl; return {}; })); /* ARRAY */
	add(unary("uisleep", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (uisleep)." << std::endl; return {}; })); /* SCALAR */
	add(unary("ctrlmodelscale", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmodelscale)." << std::endl; return {}; })); /* CONTROL */
	add(unary("curatorcameraareaceiling", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (curatorcameraareaceiling)." << std::endl; return {}; })); /* OBJECT */
	add(unary("removevest", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removevest)." << std::endl; return {}; })); /* OBJECT */
	add(unary("gearslotdata", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (gearslotdata)." << std::endl; return {}; })); /* CONTROL */
	add(unary("formationposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (formationposition)." << std::endl; return {}; })); /* OBJECT */
	add(unary("gettrimoffsetrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (gettrimoffsetrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getmissionconfigvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getmissionconfigvalue)." << std::endl; return {}; })); /* ARRAY,STRING */
	add(unary("simulcloudocclusion", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (simulcloudocclusion)." << std::endl; return {}; })); /* ARRAY */
	add(unary("movetofailed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (movetofailed)." << std::endl; return {}; })); /* OBJECT */
	add(unary("playmission", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playmission)." << std::endl; return {}; })); /* ARRAY */
	add(unary("rankid", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (rankid)." << std::endl; return {}; })); /* OBJECT */
	add(unary("showgps", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showgps)." << std::endl; return {}; })); /* BOOL */
	add(unary("synchronizedtriggers", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (synchronizedtriggers)." << std::endl; return {}; })); /* ARRAY */
	add(unary("removeallitemswithmagazines", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallitemswithmagazines)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setaperturenew", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setaperturenew)." << std::endl; return {}; })); /* ARRAY */
	add(unary("simulinclouds", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (simulinclouds)." << std::endl; return {}; })); /* ARRAY */
	add(unary("menuurl", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuurl)." << std::endl; return {}; })); /* ARRAY */
	add(unary("leaderboardsrequestuploadscorekeepbest", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (leaderboardsrequestuploadscorekeepbest)." << std::endl; return {}; })); /* ARRAY */
	add(unary("removebackpackglobal", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removebackpackglobal)." << std::endl; return {}; })); /* OBJECT */
	add(unary("exportjipmessages", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (exportjipmessages)." << std::endl; return {}; })); /* STRING */
	add(unary("ctrlhtmlloaded", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlhtmlloaded)." << std::endl; return {}; })); /* CONTROL */
	add(unary("face", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (face)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbsetselectcolorright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetselectcolorright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getvehiclecargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getvehiclecargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("deletelocation", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deletelocation)." << std::endl; return {}; })); /* LOCATION */
	add(unary("waypointvisible", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointvisible)." << std::endl; return {}; })); /* ARRAY */
	add(unary("enablesatnormalondetail", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablesatnormalondetail)." << std::endl; return {}; })); /* BOOL */
	add(unary("getposatl", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getposatl)." << std::endl; return {}; })); /* OBJECT */
	add(unary("objectcurators", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (objectcurators)." << std::endl; return {}; })); /* OBJECT */
	add(unary("activatekey", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (activatekey)." << std::endl; return {}; })); /* STRING */
	add(unary("disableremotesensors", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (disableremotesensors)." << std::endl; return {}; })); /* BOOL */
	add(unary("attackenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (attackenabled)." << std::endl; return {}; })); /* OBJECT,GROUP */
	add(unary("curatoreditingarea", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (curatoreditingarea)." << std::endl; return {}; })); /* OBJECT */
	add(unary("playmusic", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playmusic)." << std::endl; return {}; })); /* STRING, ARRAY */
	add(unary("assignedteam", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignedteam)." << std::endl; return {}; })); /* OBJECT */
	add(unary("vectordirvisual", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vectordirvisual)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lnbsetvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetvalue)." << std::endl; return {}; })); /* ARRAY */
	add(unary("triggeractivated", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggeractivated)." << std::endl; return {}; })); /* OBJECT */
	add(unary("set3denmissionattributes", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3denmissionattributes)." << std::endl; return {}; })); /* ARRAY */
	add(unary("unassignvehicle", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unassignvehicle)." << std::endl; return {}; })); /* OBJECT */
	add(unary("boundingbox", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (boundingbox)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ppeffectcreate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ppeffectcreate)." << std::endl; return {}; })); /* ARRAY */
	add(unary("movetime", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (movetime)." << std::endl; return {}; })); /* OBJECT */
	add(unary("waypointspeed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointspeed)." << std::endl; return {}; })); /* ARRAY */
	add(unary("tvpictureright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvpictureright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("ctrltext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrltext)." << std::endl; return {}; })); /* CONTROL, SCALAR */
	add(unary("ctrlclassname", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlclassname)." << std::endl; return {}; })); /* CONTROL */
	add(unary("actionname", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (actionname)." << std::endl; return {}; })); /* STRING */
	add(unary("animationstate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (animationstate)." << std::endl; return {}; })); /* OBJECT */
	add(unary("menuchecked", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuchecked)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lineintersectsobjs", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lineintersectsobjs)." << std::endl; return {}; })); /* ARRAY */
	add(unary("weaponcargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weaponcargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("buttonaction", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (buttonaction)." << std::endl; return {}; })); /* CONTROL, SCALAR */
	add(unary("set3deniconsvisible", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3deniconsvisible)." << std::endl; return {}; })); /* ARRAY */
	add(unary("hcshowbar", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hcshowbar)." << std::endl; return {}; })); /* BOOL */
	add(unary("drop", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drop)." << std::endl; return {}; })); /* ARRAY */
	add(unary("isturnedout", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isturnedout)." << std::endl; return {}; })); /* OBJECT */
	add(unary("addweaponpool", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addweaponpool)." << std::endl; return {}; })); /* ARRAY */
	add(unary("camtarget", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camtarget)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setwinddir", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwinddir)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getcameraviewdirection", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getcameraviewdirection)." << std::endl; return {}; })); /* OBJECT */
	add(unary("set3denattributes", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3denattributes)." << std::endl; return {}; })); /* ARRAY */
	add(unary("slidersetrange", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (slidersetrange)." << std::endl; return {}; })); /* ARRAY */
	add(unary("textlogformat", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (textlogformat)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbsettext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsettext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("waypointstatements", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointstatements)." << std::endl; return {}; })); /* ARRAY */
	add(unary("scudstate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (scudstate)." << std::endl; return {}; })); /* OBJECT */
	add(unary("sliderrange", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sliderrange)." << std::endl; return {}; })); /* CONTROL, SCALAR */
	add(unary("isautotrimonrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isautotrimonrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("boundingboxreal", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (boundingboxreal)." << std::endl; return {}; })); /* OBJECT */
	add(unary("terrainintersect", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (terrainintersect)." << std::endl; return {}; })); /* ARRAY */
	add(unary("ropedestroy", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropedestroy)." << std::endl; return {}; })); /* OBJECT */
	add(unary("tvsetpictureright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpictureright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("createteam", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createteam)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbsettextright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsettextright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lbsetpicturecolordisabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpicturecolordisabled)." << std::endl; return {}; })); /* ARRAY */
	add(unary("setcamshakeparams", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcamshakeparams)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lognetwork", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lognetwork)." << std::endl; return {}; })); /* ARRAY */
	add(unary("debriefingtext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (debriefingtext)." << std::endl; return {}; })); /* STRING */
	add(unary("lnbsortbyvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsortbyvalue)." << std::endl; return {}; })); /* ARRAY */
	add(unary("istouchingground", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (istouchingground)." << std::endl; return {}; })); /* OBJECT */
	add(unary("taskmarkeroffset", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (taskmarkeroffset)." << std::endl; return {}; })); /* OBJECT */
	add(unary("aisfinishheal", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (aisfinishheal)." << std::endl; return {}; })); /* ARRAY */
	add(unary("tvsetpicture", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpicture)." << std::endl; return {}; })); /* ARRAY */
	add(unary("vest", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vest)." << std::endl; return {}; })); /* OBJECT */
	add(unary("headgear", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (headgear)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lnbsetdata", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetdata)." << std::endl; return {}; })); /* ARRAY */
	add(unary("fuel", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (fuel)." << std::endl; return {}; })); /* OBJECT */
	add(unary("agltoasl", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (agltoasl)." << std::endl; return {}; })); /* ARRAY */
	add(binary(4, "setdamage", type::OBJECT, type::ARRAY, "", [](virtualmachine* vm, value::cref l, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdamage OBJECT ARRAY)." << std::endl; return {}; }));
	add(unary("weaponsitemscargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weaponsitemscargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("linearconversion", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (linearconversion)." << std::endl; return {}; })); /* ARRAY */
	add(unary("removemissioneventhandler", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removemissioneventhandler)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getmodelinfo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getmodelinfo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("dogetout", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dogetout)." << std::endl; return {}; })); /* ARRAY,OBJECT */
	add(unary("getbleedingremaining", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getbleedingremaining)." << std::endl; return {}; })); /* OBJECT */
	add(unary("squadparams", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (squadparams)." << std::endl; return {}; })); /* OBJECT */
	add(unary("groupfromnetid", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (groupfromnetid)." << std::endl; return {}; })); /* STRING */
	add(unary("leader", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (leader)." << std::endl; return {}; })); /* OBJECT, GROUP,TEAM_MEMBER  */
	add(unary("settrafficdistance", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settrafficdistance)." << std::endl; return {}; })); /* SCALAR */
	add(unary("currentthrowable", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentthrowable)." << std::endl; return {}; })); /* OBJECT */
	add(unary("enableengineartillery", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableengineartillery)." << std::endl; return {}; })); /* BOOL */
	add(unary("terrainintersectasl", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (terrainintersectasl)." << std::endl; return {}; })); /* ARRAY */
	add(unary("debuglog", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (debuglog)." << std::endl; return {}; })); /* ANY */
	add(unary("lnbsetpicturecolorselectedright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpicturecolorselectedright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbaddarray", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbaddarray)." << std::endl; return {}; })); /* ARRAY */
	add(unary("savevar", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (savevar)." << std::endl; return {}; })); /* STRING */
	add(unary("onbriefingteamswitch", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onbriefingteamswitch)." << std::endl; return {}; })); /* STRING */
	add(unary("lbsetvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetvalue)." << std::endl; return {}; })); /* ARRAY */
	add(unary("edit3denmissionattributes", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (edit3denmissionattributes)." << std::endl; return {}; })); /* STRING */
	add(unary("uniformitems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (uniformitems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getcustomaimcoef", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getcustomaimcoef)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ingameuiseteventhandler", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ingameuiseteventhandler)." << std::endl; return {}; })); /* ARRAY */
	add(unary("leaderboardrequestrowsfriends", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (leaderboardrequestrowsfriends)." << std::endl; return {}; })); /* STRING */
	add(unary("showcommandingmenu", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showcommandingmenu)." << std::endl; return {}; })); /* STRING */
	add(unary("unitrecoilcoefficient", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unitrecoilcoefficient)." << std::endl; return {}; })); /* OBJECT */
	add(unary("unassigncurator", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unassigncurator)." << std::endl; return {}; })); /* OBJECT */
	add(unary("actionkeysnames", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (actionkeysnames)." << std::endl; return {}; })); /* ARRAY,STRING */
	add(unary("clearmagazinecargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clearmagazinecargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("hostmission", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hostmission)." << std::endl; return {}; })); /* ARRAY */
	add(unary("canmove", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (canmove)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getstatvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getstatvalue)." << std::endl; return {}; })); /* STRING */
	add(unary("tvsettext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsettext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("positioncameratoworld", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (positioncameratoworld)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getwingsorientationrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getwingsorientationrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("isstaminaenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isstaminaenabled)." << std::endl; return {}; })); /* OBJECT */
	add(unary("weaponsitems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weaponsitems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("unitbackpack", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unitbackpack)." << std::endl; return {}; })); /* OBJECT */
	add(unary("finite", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (finite)." << std::endl; return {}; })); /* SCALAR,NaN */
	add(unary("lnbtext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbtext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("teamname", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (teamname)." << std::endl; return {}; })); /* TEAM_MEMBER */
	add(unary("pickweaponpool", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (pickweaponpool)." << std::endl; return {}; })); /* OBJECT */
	add(unary("surfaceiswater", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (surfaceiswater)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getslingload", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getslingload)." << std::endl; return {}; })); /* OBJECT */
	add(unary("reload", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (reload)." << std::endl; return {}; })); /* OBJECT */
	add(unary("tvsetdata", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetdata)." << std::endl; return {}; })); /* ARRAY */
	add(unary("setterraingrid", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setterraingrid)." << std::endl; return {}; })); /* SCALAR */
	add(unary("speaker", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (speaker)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbsettooltip", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsettooltip)." << std::endl; return {}; })); /* ARRAY */
	add(unary("weapons", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weapons)." << std::endl; return {}; })); /* OBJECT */
	add(unary("selectededitorobjects", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selectededitorobjects)." << std::endl; return {}; })); /* CONTROL */
	add(unary("removeall3deneventhandlers", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeall3deneventhandlers)." << std::endl; return {}; })); /* STRING */
	add(unary("leaderboardsrequestuploadscore", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (leaderboardsrequestuploadscore)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbdata", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbdata)." << std::endl; return {}; })); /* ARRAY */
	add(unary("unitisuav", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unitisuav)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbadd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbadd)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbclear", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbclear)." << std::endl; return {}; })); /* CONTROL, SCALAR */
	add(unary("assignedtarget", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignedtarget)." << std::endl; return {}; })); /* OBJECT */
	add(unary("cameraeffectenablehud", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cameraeffectenablehud)." << std::endl; return {}; })); /* BOOL */
	add(unary("execfsm", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (execfsm)." << std::endl; return {}; })); /* STRING */
	add(unary("attachedto", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (attachedto)." << std::endl; return {}; })); /* OBJECT */
	add(unary("showuavfeed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showuavfeed)." << std::endl; return {}; })); /* BOOL */
	add(unary("querymagazinepool", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (querymagazinepool)." << std::endl; return {}; })); /* STRING */
	add(unary("sizeof", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sizeof)." << std::endl; return {}; })); /* STRING */
	add(unary("landresult", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (landresult)." << std::endl; return {}; })); /* OBJECT */
	add(unary("cbchecked", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cbchecked)." << std::endl; return {}; })); /* CONTROL */
	add(unary("onhcgroupselectionchanged", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onhcgroupselectionchanged)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("ctrlscale", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlscale)." << std::endl; return {}; })); /* CONTROL */
	add(unary("lbtext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbtext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("clearweaponcargoglobal", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clearweaponcargoglobal)." << std::endl; return {}; })); /* OBJECT */
	add(unary("assigneddriver", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assigneddriver)." << std::endl; return {}; })); /* OBJECT */
	add(unary("allcontrols", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allcontrols)." << std::endl; return {}; })); /* DISPLAY */
	add(unary("taskresult", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (taskresult)." << std::endl; return {}; })); /* TASK */
	add(unary("lbpicture", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbpicture)." << std::endl; return {}; })); /* ARRAY */
	add(unary("do3denaction", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (do3denaction)." << std::endl; return {}; })); /* STRING */
	add(unary("numbertodate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (numbertodate)." << std::endl; return {}; })); /* ARRAY */
	add(unary("leaderboardstate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (leaderboardstate)." << std::endl; return {}; })); /* STRING */
	add(unary("tvsetpicturecolor", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpicturecolor)." << std::endl; return {}; })); /* ARRAY */
	add(unary("secondaryweaponmagazine", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (secondaryweaponmagazine)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setaperture", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setaperture)." << std::endl; return {}; })); /* SCALAR */
	add(unary("showpad", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showpad)." << std::endl; return {}; })); /* BOOL */
	add(unary("ctrlidc", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlidc)." << std::endl; return {}; })); /* CONTROL */
	add(unary("getdir", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getdir)." << std::endl; return {}; })); /* OBJECT */
	add(unary("uniformcontainer", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (uniformcontainer)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbsortbyvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsortbyvalue)." << std::endl; return {}; })); /* CONTROL */
	add(unary("isplayer", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isplayer)." << std::endl; return {}; })); /* OBJECT */
	add(unary("buldozer_enableroaddiag", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (buldozer_enableroaddiag)." << std::endl; return {}; })); /* BOOL */
	add(unary("campreloaded", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (campreloaded)." << std::endl; return {}; })); /* OBJECT */
	add(unary("local", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (local)." << std::endl; return {}; })); /* OBJECT, GROUP */
	add(unary("drawicon3d", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drawicon3d)." << std::endl; return {}; })); /* ARRAY */
	add(unary("surfacetype", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (surfacetype)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lbdata", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbdata)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lbdelete", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbdelete)." << std::endl; return {}; })); /* ARRAY */
	add(unary("boundingcenter", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (boundingcenter)." << std::endl; return {}; })); /* OBJECT */
	add(unary("enablediaglegend", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablediaglegend)." << std::endl; return {}; })); /* BOOL */
	add(unary("ctrlidd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlidd)." << std::endl; return {}; })); /* DISPLAY */
	add(unary("fleeing", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (fleeing)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getgroupiconparams", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getgroupiconparams)." << std::endl; return {}; })); /* GROUP */
	add(unary("cutobj", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cutobj)." << std::endl; return {}; })); /* ARRAY */
	add(unary("iscopilotenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (iscopilotenabled)." << std::endl; return {}; })); /* OBJECT */
	add(unary("uniform", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (uniform)." << std::endl; return {}; })); /* OBJECT */
	add(unary("delete3denentities", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (delete3denentities)." << std::endl; return {}; })); /* ARRAY */
	add(unary("commandgetout", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandgetout)." << std::endl; return {}; })); /* ARRAY,OBJECT */
	add(unary("wfsidetext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (wfsidetext)." << std::endl; return {}; })); /* SIDE, OBJECT, GROUP */
	add(unary("tvexpandall", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvexpandall)." << std::endl; return {}; })); /* SCALAR, CONTROL */
	add(unary("velocitymodelspace", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (velocitymodelspace)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getallownedmines", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getallownedmines)." << std::endl; return {}; })); /* OBJECT */
	add(unary("onpreloadstarted", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onpreloadstarted)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("modparams", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (modparams)." << std::endl; return {}; })); /* ARRAY */
	add(unary("flagtexture", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (flagtexture)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getfuelcargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getfuelcargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("rotorsrpmrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (rotorsrpmrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("waypointloiterradius", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointloiterradius)." << std::endl; return {}; })); /* ARRAY */
	add(unary("size", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (size)." << std::endl; return {}; })); /* LOCATION */
	add(unary("captivenum", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (captivenum)." << std::endl; return {}; })); /* OBJECT */
	add(unary("waypointtimeout", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointtimeout)." << std::endl; return {}; })); /* ARRAY */
	add(unary("tvsort", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsort)." << std::endl; return {}; })); /* ARRAY */
	add(unary("selectionnames", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selectionnames)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbselection", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbselection)." << std::endl; return {}; })); /* CONTROL */
	add(unary("position", type::LOCATION, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (position)." << std::endl; return {}; })); /* LOCATION */
	add(unary("canunloadincombat", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (canunloadincombat)." << std::endl; return {}; })); /* OBJECT */
	add(unary("attachedobjects", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (attachedobjects)." << std::endl; return {}; })); /* OBJECT */
	add(unary("netid", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (netid)." << std::endl; return {}; })); /* OBJECT, GROUP */
	add(unary("waypointcompletionradius", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointcompletionradius)." << std::endl; return {}; })); /* ARRAY */
	add(unary("removeallassigneditems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallassigneditems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ropeunwound", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropeunwound)." << std::endl; return {}; })); /* OBJECT */
	add(unary("waypointposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointposition)." << std::endl; return {}; })); /* ARRAY */
	add(unary("isdlcavailable", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isdlcavailable)." << std::endl; return {}; })); /* SCALAR */
	add(unary("waypointtype", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointtype)." << std::endl; return {}; })); /* ARRAY */
	add(unary("addswitchableunit", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addswitchableunit)." << std::endl; return {}; })); /* OBJECT */
	add(unary("closeoverlay", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (closeoverlay)." << std::endl; return {}; })); /* CONTROL */
	add(unary("tvexpand", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvexpand)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getartilleryammo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getartilleryammo)." << std::endl; return {}; })); /* ARRAY */
	add(unary("tvadd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvadd)." << std::endl; return {}; })); /* ARRAY */
	add(unary("owner", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (owner)." << std::endl; return {}; })); /* OBJECT */
	add(unary("progressloadingscreen", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (progressloadingscreen)." << std::endl; return {}; })); /* SCALAR */
	add(unary("estimatedtimeleft", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (estimatedtimeleft)." << std::endl; return {}; })); /* SCALAR */
	add(unary("displayparent", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (displayparent)." << std::endl; return {}; })); /* DISPLAY */
	add(unary("create3denentity", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (create3denentity)." << std::endl; return {}; })); /* ARRAY */
	add(unary("primaryweaponmagazine", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (primaryweaponmagazine)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlparentcontrolsgroup", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlparentcontrolsgroup)." << std::endl; return {}; })); /* CONTROL */
	add(unary("actionkeysimages", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (actionkeysimages)." << std::endl; return {}; })); /* ARRAY,STRING */
	add(unary("enablesentences", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablesentences)." << std::endl; return {}; })); /* BOOL */
	add(unary("curatoreditableobjects", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (curatoreditableobjects)." << std::endl; return {}; })); /* OBJECT */
	add(unary("entities", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (entities)." << std::endl; return {}; })); /* STRING, ARRAY */
	add(unary("tvcount", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvcount)." << std::endl; return {}; })); /* ARRAY */
	add(unary("setgroupiconsselectable", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setgroupiconsselectable)." << std::endl; return {}; })); /* BOOL */
	add(unary("showwarrant", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showwarrant)." << std::endl; return {}; })); /* BOOL */
	add(unary("assigneditems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assigneditems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("groupselectedunits", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (groupselectedunits)." << std::endl; return {}; })); /* OBJECT */
	add(unary("titlefadeout", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (titlefadeout)." << std::endl; return {}; })); /* SCALAR */
	add(unary("atltoasl", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (atltoasl)." << std::endl; return {}; })); /* ARRAY */
	add(unary("loaduniform", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (loaduniform)." << std::endl; return {}; })); /* OBJECT */
	add(unary("someammo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (someammo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setwind", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setwind)." << std::endl; return {}; })); /* ARRAY */
	add(unary("groupowner", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (groupowner)." << std::endl; return {}; })); /* GROUP */
	add(unary("isweaponrested", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isweaponrested)." << std::endl; return {}; })); /* OBJECT */
	add(unary("isaimprecisionenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isaimprecisionenabled)." << std::endl; return {}; })); /* OBJECT */
	add(unary("isagent", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isagent)." << std::endl; return {}; })); /* TEAM_MEMBER */
	add(unary("leaderboardinit", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (leaderboardinit)." << std::endl; return {}; })); /* STRING */
	add(unary("taskcustomdata", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (taskcustomdata)." << std::endl; return {}; })); /* TASK */
	add(unary("lnbdeletecolumn", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbdeletecolumn)." << std::endl; return {}; })); /* ARRAY */
	add(unary("incapacitatedstate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (incapacitatedstate)." << std::endl; return {}; })); /* OBJECT */
	add(unary("saveoverlay", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (saveoverlay)." << std::endl; return {}; })); /* CONTROL */
	add(unary("magazinesallturrets", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazinesallturrets)." << std::endl; return {}; })); /* OBJECT */
	add(unary("collectivertd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (collectivertd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("tvdelete", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvdelete)." << std::endl; return {}; })); /* ARRAY */
	add(unary("taskcompleted", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (taskcompleted)." << std::endl; return {}; })); /* TASK */
	add(unary("playsound3d", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playsound3d)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getanimaimprecision", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getanimaimprecision)." << std::endl; return {}; })); /* OBJECT */
	add(unary("waypointformation", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointformation)." << std::endl; return {}; })); /* ARRAY */
	add(unary("cantriggerdynamicsimulation", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cantriggerdynamicsimulation)." << std::endl; return {}; })); /* OBJECT */
	add(unary("get3denconnections", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (get3denconnections)." << std::endl; return {}; })); /* ANY */
	add(unary("ongroupiconoverenter", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ongroupiconoverenter)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("backpackmagazines", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (backpackmagazines)." << std::endl; return {}; })); /* OBJECT */
	add(unary("cutrsc", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cutrsc)." << std::endl; return {}; })); /* ARRAY */
	add(unary("weaponlowered", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weaponlowered)." << std::endl; return {}; })); /* OBJECT */
	add(unary("handshit", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (handshit)." << std::endl; return {}; })); /* OBJECT */
	add(unary("selectbestplaces", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selectbestplaces)." << std::endl; return {}; })); /* ARRAY */
	add(unary("removeallcuratorcameraareas", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallcuratorcameraareas)." << std::endl; return {}; })); /* OBJECT */
	add(unary("taskparent", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (taskparent)." << std::endl; return {}; })); /* TASK */
	add(unary("triggertype", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggertype)." << std::endl; return {}; })); /* OBJECT */
	add(unary("hidebody", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hidebody)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getpilotcameratarget", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getpilotcameratarget)." << std::endl; return {}; })); /* OBJECT */
	add(unary("menuenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuenabled)." << std::endl; return {}; })); /* ARRAY */
	add(unary("showchat", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showchat)." << std::endl; return {}; })); /* BOOL */
	add(unary("scoreside", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (scoreside)." << std::endl; return {}; })); /* SIDE */
	add(unary("behaviour", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (behaviour)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getmagazinecargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getmagazinecargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("nearestterrainobjects", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearestterrainobjects)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lifestate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lifestate)." << std::endl; return {}; })); /* OBJECT */
	add(unary("issprintallowed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (issprintallowed)." << std::endl; return {}; })); /* OBJECT */
	add(unary("classname", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (classname)." << std::endl; return {}; })); /* LOCATION */
	add(unary("difficultyoption", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (difficultyoption)." << std::endl; return {}; })); /* STRING */
	add(unary("getfieldmanualstartpage", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getfieldmanualstartpage)." << std::endl; return {}; })); /* DISPLAY */
	add(unary("islocalized", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (islocalized)." << std::endl; return {}; })); /* STRING */
	add(unary("triggertimeout", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggertimeout)." << std::endl; return {}; })); /* OBJECT */
	add(unary("remoteexec", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (remoteexec)." << std::endl; return {}; })); /* ARRAY */
	add(unary("removeallcontainers", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallcontainers)." << std::endl; return {}; })); /* OBJECT */
	add(unary("hcleader", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hcleader)." << std::endl; return {}; })); /* GROUP */
	add(unary("detectedmines", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (detectedmines)." << std::endl; return {}; })); /* SIDE */
	add(unary("getobjecttype", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getobjecttype)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ropeattachedto", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropeattachedto)." << std::endl; return {}; })); /* OBJECT */
	add(unary("diag_codeperformance", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (diag_codeperformance)." << std::endl; return {}; })); /* ARRAY */
	add(unary("inputaction", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (inputaction)." << std::endl; return {}; })); /* STRING */
	add(unary("oncommandmodechanged", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (oncommandmodechanged)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("movetocompleted", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (movetocompleted)." << std::endl; return {}; })); /* OBJECT */
	add(unary("requiredversion", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (requiredversion)." << std::endl; return {}; })); /* STRING */
	add(unary("lnbaddrow", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbaddrow)." << std::endl; return {}; })); /* ARRAY */
	add(unary("textlog", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (textlog)." << std::endl; return {}; })); /* ANY */
	add(unary("openyoutubevideo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (openyoutubevideo)." << std::endl; return {}; })); /* STRING */
	add(unary("combatmode", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (combatmode)." << std::endl; return {}; })); /* OBJECT,GROUP */
	add(unary("ppeffectdestroy", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ppeffectdestroy)." << std::endl; return {}; })); /* SCALAR, ARRAY */
	add(unary("add3deneventhandler", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (add3deneventhandler)." << std::endl; return {}; })); /* ARRAY */
	add(unary("canstand", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (canstand)." << std::endl; return {}; })); /* OBJECT */
	add(unary("rotorsforcesrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (rotorsforcesrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("formationleader", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (formationleader)." << std::endl; return {}; })); /* OBJECT */
	add(unary("radiochannelcreate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (radiochannelcreate)." << std::endl; return {}; })); /* ARRAY */
	add(unary("enginestorquertd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enginestorquertd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("isengineon", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isengineon)." << std::endl; return {}; })); /* OBJECT */
	add(unary("add3denconnection", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (add3denconnection)." << std::endl; return {}; })); /* ARRAY */
	add(unary("collapseobjecttree", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (collapseobjecttree)." << std::endl; return {}; })); /* CONTROL */
	add(unary("waypointhouseposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointhouseposition)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getplayerscores", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getplayerscores)." << std::endl; return {}; })); /* OBJECT */
	add(unary("enableradio", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableradio)." << std::endl; return {}; })); /* BOOL */
	add(unary("skill", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (skill)." << std::endl; return {}; })); /* OBJECT */
	add(unary("settimemultiplier", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settimemultiplier)." << std::endl; return {}; })); /* SCALAR */
	add(unary("waypointtimeoutcurrent", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointtimeoutcurrent)." << std::endl; return {}; })); /* GROUP */
	add(unary("magazinecargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazinecargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ropecut", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropecut)." << std::endl; return {}; })); /* ARRAY */
	add(unary("creatediarylink", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (creatediarylink)." << std::endl; return {}; })); /* ARRAY */
	add(unary("backpack", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (backpack)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlmapanimcommit", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmapanimcommit)." << std::endl; return {}; })); /* CONTROL */
	add(unary("mapanimadd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (mapanimadd)." << std::endl; return {}; })); /* ARRAY */
	add(unary("surfacenormal", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (surfacenormal)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lineintersectswith", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lineintersectswith)." << std::endl; return {}; })); /* ARRAY */
	add(unary("hcremoveallgroups", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hcremoveallgroups)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getposworld", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getposworld)." << std::endl; return {}; })); /* OBJECT */
	add(unary("showwatch", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showwatch)." << std::endl; return {}; })); /* BOOL */
	add(unary("configsourcemodlist", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (configsourcemodlist)." << std::endl; return {}; })); /* CONFIG */
	add(unary("createtrigger", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createtrigger)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getstamina", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getstamina)." << std::endl; return {}; })); /* OBJECT */
	add(unary("waypointshow", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointshow)." << std::endl; return {}; })); /* ARRAY */
	add(unary("ctrltype", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrltype)." << std::endl; return {}; })); /* CONTROL */
	add(unary("getmass", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getmass)." << std::endl; return {}; })); /* OBJECT */
	add(unary("weaponstate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weaponstate)." << std::endl; return {}; })); /* ARRAY,OBJECT */
	add(unary("lbpictureright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbpictureright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("load", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (load)." << std::endl; return {}; })); /* OBJECT */
	add(unary("loadabs", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (loadabs)." << std::endl; return {}; })); /* OBJECT */
	add(unary("removeswitchableunit", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeswitchableunit)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lnbsort", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsort)." << std::endl; return {}; })); /* ARRAY */
	add(unary("islighton", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (islighton)." << std::endl; return {}; })); /* OBJECT */
	add(unary("simulationenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (simulationenabled)." << std::endl; return {}; })); /* OBJECT */
	add(unary("currentmagazinedetail", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentmagazinedetail)." << std::endl; return {}; })); /* OBJECT */
	add(unary("onmapsingleclick", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onmapsingleclick)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("screenshot", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (screenshot)." << std::endl; return {}; })); /* STRING */
	add(unary("unitaimpositionvisual", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unitaimpositionvisual)." << std::endl; return {}; })); /* OBJECT */
	add(unary("actionids", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (actionids)." << std::endl; return {}; })); /* OBJECT */
	add(unary("everybackpack", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (everybackpack)." << std::endl; return {}; })); /* OBJECT */
	add(unary("asltoatl", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (asltoatl)." << std::endl; return {}; })); /* ARRAY */
	add(unary("sethudmovementlevels", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sethudmovementlevels)." << std::endl; return {}; })); /* ARRAY */
	add(unary("set3denmodelsvisible", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3denmodelsvisible)." << std::endl; return {}; })); /* ARRAY */
	add(unary("currentmuzzle", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentmuzzle)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlautoscrollspeed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlautoscrollspeed)." << std::endl; return {}; })); /* CONTROL */
	add(unary("currentweaponmode", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentweaponmode)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getwingspositionrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getwingspositionrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("waypointloitertype", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointloitertype)." << std::endl; return {}; })); /* ARRAY */
	add(unary("name", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (name)." << std::endl; return {}; })); /* OBJEC, LOCATIONT */
	add(unary("onbriefinggroup", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onbriefinggroup)." << std::endl; return {}; })); /* STRING */
	add(unary("locationposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (locationposition)." << std::endl; return {}; })); /* LOCATION */
	add(unary("importance", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (importance)." << std::endl; return {}; })); /* LOCATION */
	add(unary("captive", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (captive)." << std::endl; return {}; })); /* OBJECT */
	add(unary("isweapondeployed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isweapondeployed)." << std::endl; return {}; })); /* OBJECT */
	add(unary("menushortcut", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menushortcut)." << std::endl; return {}; })); /* ARRAY */
	add(unary("keyimage", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (keyimage)." << std::endl; return {}; })); /* SCALAR */
	add(unary("removeallweapons", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallweapons)." << std::endl; return {}; })); /* OBJECT */
	add(unary("titleobj", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (titleobj)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lbsort", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsort)." << std::endl; return {}; })); /* CONTROL, ARRAY */
	add(unary("vehiclevarname", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vehiclevarname)." << std::endl; return {}; })); /* OBJECT */
	add(unary("triggertimeoutcurrent", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggertimeoutcurrent)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlmodeldirandup", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmodeldirandup)." << std::endl; return {}; })); /* CONTROL */
	add(unary("assignedgunner", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignedgunner)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setmouseposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setmouseposition)." << std::endl; return {}; })); /* ARRAY */
	add(unary("soldiermagazines", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (soldiermagazines)." << std::endl; return {}; })); /* OBJECT */
	add(unary("endmission", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (endmission)." << std::endl; return {}; })); /* STRING */
	add(unary("leaderboardrequestrowsglobal", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (leaderboardrequestrowsglobal)." << std::endl; return {}; })); /* ARRAY */
	add(unary("magazinesammo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazinesammo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("removeuniform", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeuniform)." << std::endl; return {}; })); /* OBJECT */
	add(unary("faction", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (faction)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrltextsecondary", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrltextsecondary)." << std::endl; return {}; })); /* CONTROL */
	add(unary("clear3deninventory", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clear3deninventory)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbpictureright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbpictureright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("servercommandavailable", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (servercommandavailable)." << std::endl; return {}; })); /* STRING */
	add(unary("geteditormode", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (geteditormode)." << std::endl; return {}; })); /* CONTROL */
	add(unary("removeallprimaryweaponitems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallprimaryweaponitems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("menuhover", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuhover)." << std::endl; return {}; })); /* CONTROL, SCALAR */
	add(unary("verifysignature", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (verifysignature)." << std::endl; return {}; })); /* STRING */
	add(unary("group", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (group)." << std::endl; return {}; })); /* OBJECT */
	add(unary("allturrets", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allturrets)." << std::endl; return {}; })); /* ARRAY, OBJECT */
	add(unary("restarteditorcamera", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (restarteditorcamera)." << std::endl; return {}; })); /* CONTROL */
	add(unary("camcommitted", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camcommitted)." << std::endl; return {}; })); /* OBJECT */
	add(unary("tvtooltip", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvtooltip)." << std::endl; return {}; })); /* ARRAY */
	add(unary("startloadingscreen", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (startloadingscreen)." << std::endl; return {}; })); /* ARRAY */
	add(unary("ctrlsettext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsettext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("enabledynamicsimulation", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enabledynamicsimulation)." << std::endl; return {}; })); /* BOOL */
	add(unary("currenttask", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currenttask)." << std::endl; return {}; })); /* OBJECT */
	add(unary("flagside", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (flagside)." << std::endl; return {}; })); /* OBJECT */
	add(unary("isinremainscollector", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isinremainscollector)." << std::endl; return {}; })); /* OBJECT */
	add(unary("nearestobject", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearestobject)." << std::endl; return {}; })); /* ARRAY */
	add(unary("magazinesammocargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazinesammocargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setplayable", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setplayable)." << std::endl; return {}; })); /* OBJECT */
	add(unary("unlockachievement", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unlockachievement)." << std::endl; return {}; })); /* STRING */
	add(unary("lbsettext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsettext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("isautonomous", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isautonomous)." << std::endl; return {}; })); /* OBJECT */
	add(unary("additempool", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (additempool)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getmissionlayerentities", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getmissionlayerentities)." << std::endl; return {}; })); /* SCALAR,STRING */
	add(unary("lnbdeleterow", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbdeleterow)." << std::endl; return {}; })); /* ARRAY */
	add(unary("dissolveteam", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dissolveteam)." << std::endl; return {}; })); /* STRING */
	add(unary("publicvariableserver", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (publicvariableserver)." << std::endl; return {}; })); /* STRING */
	add(unary("handgunmagazine", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (handgunmagazine)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getoxygenremaining", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getoxygenremaining)." << std::endl; return {}; })); /* OBJECT */
	add(unary("progressposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (progressposition)." << std::endl; return {}; })); /* CONTROL */
	add(unary("tvvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvvalue)." << std::endl; return {}; })); /* ARRAY */
	add(unary("buldozer_loadnewroads", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (buldozer_loadnewroads)." << std::endl; return {}; })); /* STRING */
	add(unary("removeallactions", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallactions)." << std::endl; return {}; })); /* OBJECT */
	add(unary("servercommand", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (servercommand)." << std::endl; return {}; })); /* STRING */
	add(unary("attachedobject", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (attachedobject)." << std::endl; return {}; })); /* LOCATION */
	add(unary("everycontainer", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (everycontainer)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlautoscrollrewind", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlautoscrollrewind)." << std::endl; return {}; })); /* CONTROL */
	add(unary("stopenginertd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (stopenginertd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("effectivecommander", type::OBJECT, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (effectivecommander)." << std::endl; return {}; }));
	add(unary("lbsetdata", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetdata)." << std::endl; return {}; })); /* ARRAY */
	add(unary("isvehiclecargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isvehiclecargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("agent", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (agent)." << std::endl; return {}; })); /* TEAM_MEMBER */
	add(unary("openmap", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (openmap)." << std::endl; return {}; })); /* ARRAY, BOOL */
	add(unary("playsound", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playsound)." << std::endl; return {}; })); /* STRING, ARRAY */
	add(unary("dostop", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dostop)." << std::endl; return {}; })); /* ARRAY,OBJECT */
	add(unary("oneachframe", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (oneachframe)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("lightdetachobject", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lightdetachobject)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getpersonuseddlcs", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getpersonuseddlcs)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getgroupicons", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getgroupicons)." << std::endl; return {}; })); /* GROUP */
	add(unary("getwppos", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getwppos)." << std::endl; return {}; })); /* ARRAY */
	add(unary("setsimulweatherlayers", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setsimulweatherlayers)." << std::endl; return {}; })); /* SCALAR */
	add(unary("getdescription", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getdescription)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ropeendposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropeendposition)." << std::endl; return {}; })); /* OBJECT */
	add(unary("text", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (text)." << std::endl; return {}; })); /* STRING, LOCATION */
	add(unary("items", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (items)." << std::endl; return {}; })); /* OBJECT */
	add(unary("showcinemaborder", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showcinemaborder)." << std::endl; return {}; })); /* BOOL */
	add(unary("ctrlautoscrolldelay", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlautoscrolldelay)." << std::endl; return {}; })); /* CONTROL */
	add(unary("onpreloadfinished", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onpreloadfinished)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("nearestlocation", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearestlocation)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getrepaircargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getrepaircargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("titlersc", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (titlersc)." << std::endl; return {}; })); /* ARRAY */
	add(unary("setcurrentchannel", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcurrentchannel)." << std::endl; return {}; })); /* SCALAR */
	add(unary("lbsetpictureright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpictureright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("menusize", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusize)." << std::endl; return {}; })); /* ARRAY */
	add(unary("pitch", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (pitch)." << std::endl; return {}; })); /* OBJECT */
	add(unary("onbriefingplan", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onbriefingplan)." << std::endl; return {}; })); /* STRING */
	add(unary("formattext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (formattext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("camerainterest", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camerainterest)." << std::endl; return {}; })); /* OBJECT */
	add(unary("removeheadgear", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeheadgear)." << std::endl; return {}; })); /* OBJECT */
	//add(unary("side", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (side)." << std::endl; return {}; })); /* LOCATION */
	add(unary("completedfsm", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (completedfsm)." << std::endl; return {}; })); /* SCALAR */
	add(unary("playableslotsnumber", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playableslotsnumber)." << std::endl; return {}; })); /* SIDE */
	add(unary("servercommandexecutable", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (servercommandexecutable)." << std::endl; return {}; })); /* STRING */
	add(unary("enablesaving", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablesaving)." << std::endl; return {}; })); /* BOOL,ARRAY */
	add(unary("remove3denconnection", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (remove3denconnection)." << std::endl; return {}; })); /* ARRAY */
	add(unary("queryitemspool", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (queryitemspool)." << std::endl; return {}; })); /* STRING */
	add(unary("isformationleader", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isformationleader)." << std::endl; return {}; })); /* OBJECT */
	add(unary("numberofenginesrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (numberofenginesrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setplayerrespawntime", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setplayerrespawntime)." << std::endl; return {}; })); /* SCALAR */
	add(unary("preloadsound", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (preloadsound)." << std::endl; return {}; })); /* STRING */
	add(unary("getallsoundcontrollers", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getallsoundcontrollers)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setviewdistance", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setviewdistance)." << std::endl; return {}; })); /* SCALAR */
	add(unary("magazinesdetail", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazinesdetail)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlshow", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlshow)." << std::endl; return {}; })); /* ARRAY */
	add(unary("waypointattachedobject", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointattachedobject)." << std::endl; return {}; })); /* ARRAY */
	add(unary("ishidden", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ishidden)." << std::endl; return {}; })); /* OBJECT */
	add(unary("preloadtitleobj", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (preloadtitleobj)." << std::endl; return {}; })); /* ARRAY */
	add(unary("tvcollapseall", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvcollapseall)." << std::endl; return {}; })); /* SCALAR, CONTROL */
	add(unary("ctrlparent", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlparent)." << std::endl; return {}; })); /* CONTROL */
	add(unary("diag_dynamicsimulationend", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (diag_dynamicsimulationend)." << std::endl; return {}; })); /* STRING */
	add(unary("registeredtasks", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (registeredtasks)." << std::endl; return {}; })); /* TEAM_MEMBER */
	add(unary("forceatpositionrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (forceatpositionrtd)." << std::endl; return {}; })); /* ARRAY */
	add(unary("actionkeysnamesarray", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (actionkeysnamesarray)." << std::endl; return {}; })); /* ARRAY,STRING */
	add(unary("titlecut", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (titlecut)." << std::endl; return {}; })); /* ARRAY */
	add(unary("configsourceaddonlist", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (configsourceaddonlist)." << std::endl; return {}; })); /* CONFIG */
	add(unary("menuaction", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuaction)." << std::endl; return {}; })); /* ARRAY */
	add(unary("disableuserinput", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (disableuserinput)." << std::endl; return {}; })); /* BOOL */
	add(unary("set3denlinesvisible", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3denlinesvisible)." << std::endl; return {}; })); /* ARRAY */
	add(unary("aimpos", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (aimpos)." << std::endl; return {}; })); /* OBJECT */
	add(unary("cancelsimpletaskdestination", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cancelsimpletaskdestination)." << std::endl; return {}; })); /* TASK */
	add(unary("clearweaponcargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clearweaponcargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("enableenvironment", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableenvironment)." << std::endl; return {}; })); /* BOOL */
	add(unary("createsimpleobject", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createsimpleobject)." << std::endl; return {}; })); /* ARRAY */
	add(unary("airportside", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (airportside)." << std::endl; return {}; })); /* SCALAR */
	add(unary("assignedvehiclerole", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignedvehiclerole)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lnbgetcolumnsposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbgetcolumnsposition)." << std::endl; return {}; })); /* CONTROL, SCALAR */
	add(unary("enableteamswitch", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableteamswitch)." << std::endl; return {}; })); /* BOOL */
	add(unary("waypointforcebehaviour", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointforcebehaviour)." << std::endl; return {}; })); /* ARRAY */
	add(unary("precision", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (precision)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ropes", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropes)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbcolor", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbcolor)." << std::endl; return {}; })); /* ARRAY */
	add(unary("settrafficdensity", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settrafficdensity)." << std::endl; return {}; })); /* ARRAY */
	add(unary("createsoundsource", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createsoundsource)." << std::endl; return {}; })); /* ARRAY */
	add(unary("backpackcontainer", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (backpackcontainer)." << std::endl; return {}; })); /* OBJECT */
	add(unary("vectorupvisual", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vectorupvisual)." << std::endl; return {}; })); /* OBJECT */
	add(unary("leaderboardrequestrowsglobalarounduser", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (leaderboardrequestrowsglobalarounduser)." << std::endl; return {}; })); /* ARRAY */
	add(unary("vectordir", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vectordir)." << std::endl; return {}; })); /* OBJECT */
	add(unary("clearbackpackcargoglobal", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clearbackpackcargoglobal)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getdlcusagetime", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getdlcusagetime)." << std::endl; return {}; })); /* SCALAR */
	add(unary("members", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (members)." << std::endl; return {}; })); /* TEAM_MEMBER */
	add(unary("worldtoscreen", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (worldtoscreen)." << std::endl; return {}; })); /* ARRAY */
	add(unary("teamtype", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (teamtype)." << std::endl; return {}; })); /* TEAM_MEMBER */
	add(unary("removeallhandgunitems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallhandgunitems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("curatoreditingareatype", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (curatoreditingareatype)." << std::endl; return {}; })); /* OBJECT */
	add(unary("magazinesdetailbackpack", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazinesdetailbackpack)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlmapscale", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmapscale)." << std::endl; return {}; })); /* CONTROL */
	add(unary("createdialog", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createdialog)." << std::endl; return {}; })); /* STRING */
	add(unary("currentwaypoint", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentwaypoint)." << std::endl; return {}; })); /* GROUP */
	add(unary("magazinesammofull", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazinesammofull)." << std::endl; return {}; })); /* OBJECT */
	add(unary("putweaponpool", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (putweaponpool)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getobjectdlc", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getobjectdlc)." << std::endl; return {}; })); /* OBJECT */
	add(unary("enabledebriefingstats", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enabledebriefingstats)." << std::endl; return {}; })); /* ARRAY */
	add(unary("goto", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (goto)." << std::endl; return {}; })); /* STRING */
	add(unary("deletewaypoint", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deletewaypoint)." << std::endl; return {}; })); /* ARRAY */
	add(unary("setshadowdistance", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setshadowdistance)." << std::endl; return {}; })); /* SCALAR */
	add(unary("tvsortbyvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsortbyvalue)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lockidentity", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lockidentity)." << std::endl; return {}; })); /* OBJECT */
	add(unary("score", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (score)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbsetpicturecolor", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpicturecolor)." << std::endl; return {}; })); /* ARRAY */
	add(unary("nextmenuitemindex", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nextmenuitemindex)." << std::endl; return {}; })); /* CONTROL */
	add(unary("lasertarget", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lasertarget)." << std::endl; return {}; })); /* OBJECT */
	add(unary("unitready", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unitready)." << std::endl; return {}; })); /* ARRAY,OBJECT */
	add(unary("showmap", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showmap)." << std::endl; return {}; })); /* BOOL */
	add(unary("isautostartupenabledrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isautostartupenabledrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("allmissionobjects", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (allmissionobjects)." << std::endl; return {}; })); /* STRING */
	add(unary("getcenterofmass", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getcenterofmass)." << std::endl; return {}; })); /* OBJECT */
	add(unary("stance", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (stance)." << std::endl; return {}; })); /* OBJECT */
	add(unary("curatorpoints", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (curatorpoints)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbtextright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbtextright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getterrainheightasl", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getterrainheightasl)." << std::endl; return {}; })); /* ARRAY */
	add(unary("triggerattachedvehicle", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggerattachedvehicle)." << std::endl; return {}; })); /* OBJECT */
	add(unary("rank", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (rank)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getrotorbrakertd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getrotorbrakertd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("itemswithmagazines", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (itemswithmagazines)." << std::endl; return {}; })); /* OBJECT */
	add(unary("isbleeding", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isbleeding)." << std::endl; return {}; })); /* OBJECT */
	add(unary("isrealtime", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isrealtime)." << std::endl; return {}; })); /* CONTROL */
	add(unary("ctrlactivate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlactivate)." << std::endl; return {}; })); /* CONTROL */
	add(unary("processdiarylink", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (processdiarylink)." << std::endl; return {}; })); /* STRING */
	add(unary("lbcolorright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbcolorright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("menupicture", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menupicture)." << std::endl; return {}; })); /* ARRAY */
	add(unary("namesound", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (namesound)." << std::endl; return {}; })); /* OBJECT */
	add(unary("locked", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (locked)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlenable", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlenable)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getdlcassetsusagebyname", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getdlcassetsusagebyname)." << std::endl; return {}; })); /* STRING */
	add(unary("clearbackpackcargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clearbackpackcargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("cuttext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cuttext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("formationdirection", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (formationdirection)." << std::endl; return {}; })); /* OBJECT */
	add(unary("preloadtitlersc", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (preloadtitlersc)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getweaponcargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getweaponcargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("isabletobreathe", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isabletobreathe)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getassignedcuratorunit", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getassignedcuratorunit)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlcommitted", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlcommitted)." << std::endl; return {}; })); /* CONTROL */
	add(unary("get3denentity", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (get3denentity)." << std::endl; return {}; })); /* SCALAR */
	add(unary("ctrlmapanimdone", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmapanimdone)." << std::endl; return {}; })); /* CONTROL */
	add(unary("setgroupiconsvisible", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setgroupiconsvisible)." << std::endl; return {}; })); /* ARRAY */
	add(unary("gearslotammocount", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (gearslotammocount)." << std::endl; return {}; })); /* CONTROL */
	add(unary("enginespowerrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enginespowerrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("sethorizonparallaxcoef", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sethorizonparallaxcoef)." << std::endl; return {}; })); /* SCALAR */
	add(unary("echo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (echo)." << std::endl; return {}; })); /* STRING */
	add(unary("dynamicsimulationenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dynamicsimulationenabled)." << std::endl; return {}; })); /* OBJECT, GROUP */
	add(unary("hcallgroups", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hcallgroups)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setcamshakedefparams", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcamshakedefparams)." << std::endl; return {}; })); /* ARRAY */
	add(unary("screentoworld", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (screentoworld)." << std::endl; return {}; })); /* ARRAY */
	add(unary("menuenable", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuenable)." << std::endl; return {}; })); /* ARRAY */
	add(unary("currentcommand", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentcommand)." << std::endl; return {}; })); /* OBJECT */
	add(unary("sliderposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sliderposition)." << std::endl; return {}; })); /* CONTROL, SCALAR */
	add(unary("unitpos", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unitpos)." << std::endl; return {}; })); /* OBJECT */
	add(unary("finddisplay", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (finddisplay)." << std::endl; return {}; })); /* SCALAR */
	add(unary("itemcargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (itemcargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("secondaryweaponitems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (secondaryweaponitems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("menuvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuvalue)." << std::endl; return {}; })); /* ARRAY */
	add(unary("deleteidentity", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deleteidentity)." << std::endl; return {}; })); /* STRING */
	add(unary("getaimingcoef", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getaimingcoef)." << std::endl; return {}; })); /* OBJECT */
	add(unary("breakout", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (breakout)." << std::endl; return {}; })); /* STRING */
	add(unary("ropeattachedobjects", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropeattachedobjects)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbsetselectcolor", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetselectcolor)." << std::endl; return {}; })); /* ARRAY */
	add(unary("mineactive", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (mineactive)." << std::endl; return {}; })); /* OBJECT */
	add(unary("enablestressdamage", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablestressdamage)." << std::endl; return {}; })); /* BOOL */
	add(unary("mapcenteroncamera", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (mapcenteroncamera)." << std::endl; return {}; })); /* CONTROL */
	add(unary("lbsetpicture", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpicture)." << std::endl; return {}; })); /* ARRAY */
	add(unary("handgunweapon", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (handgunweapon)." << std::endl; return {}; })); /* OBJECT */
	add(unary("activateaddons", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (activateaddons)." << std::endl; return {}; })); /* ARRAY */
	add(unary("addmagazinepool", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmagazinepool)." << std::endl; return {}; })); /* ARRAY */
	add(unary("synchronizedwaypoints", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (synchronizedwaypoints)." << std::endl; return {}; })); /* OBJECT, ARRAY */
	add(unary("vehiclecargoenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vehiclecargoenabled)." << std::endl; return {}; })); /* OBJECT */
	add(unary("save3deninventory", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (save3deninventory)." << std::endl; return {}; })); /* ARRAY */
	add(unary("compilefinal", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (compilefinal)." << std::endl; return {}; })); /* STRING */
	add(unary("taskhint", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (taskhint)." << std::endl; return {}; })); /* ARRAY */
	add(unary("moveout", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (moveout)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setstatvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setstatvalue)." << std::endl; return {}; })); /* ARRAY */
	add(unary("deleteteam", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deleteteam)." << std::endl; return {}; })); /* TEAM_MEMBER */
	add(unary("get3denactionstate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (get3denactionstate)." << std::endl; return {}; })); /* STRING */
	add(unary("createagent", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createagent)." << std::endl; return {}; })); /* ARRAY */
	add(unary("importallgroups", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (importallgroups)." << std::endl; return {}; })); /* CONTROL */
	add(unary("assignedvehicle", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignedvehicle)." << std::endl; return {}; })); /* OBJECT */
	add(unary("expecteddestination", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (expecteddestination)." << std::endl; return {}; })); /* OBJECT */
	add(unary("goggles", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (goggles)." << std::endl; return {}; })); /* OBJECT */
	add(unary("removeallcuratoraddons", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallcuratoraddons)." << std::endl; return {}; })); /* OBJECT */
	add(unary("objectfromnetid", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (objectfromnetid)." << std::endl; return {}; })); /* STRING */
	add(unary("ctrlchecked", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlchecked)." << std::endl; return {}; })); /* CONTROL */
	add(unary("tvsettooltip", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsettooltip)." << std::endl; return {}; })); /* ARRAY */
	add(unary("isburning", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isburning)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getobjectmaterials", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getobjectmaterials)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getplayeruid", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getplayeruid)." << std::endl; return {}; })); /* OBJECT */
	add(unary("binocular", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (binocular)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getweaponsway", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getweaponsway)." << std::endl; return {}; })); /* OBJECT */
	add(unary("collect3denhistory", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (collect3denhistory)." << std::endl; return {}; })); /* CODE */
	add(unary("handgunitems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (handgunitems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("removeallcuratoreditingareas", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallcuratoreditingareas)." << std::endl; return {}; })); /* OBJECT */
	add(unary("skiptime", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (skiptime)." << std::endl; return {}; })); /* SCALAR */
	add(unary("getpos", type::LOCATION, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getpos)." << std::endl; return {}; })); /* LOCATION */
	add(unary("curatorcameraarea", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (curatorcameraarea)." << std::endl; return {}; })); /* OBJECT */
	add(unary("enableaudiofeature", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enableaudiofeature)." << std::endl; return {}; })); /* ARRAY */
	add(unary("publicvariable", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (publicvariable)." << std::endl; return {}; })); /* STRING */
	add(unary("create3dencomposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (create3dencomposition)." << std::endl; return {}; })); /* ARRAY */
	add(unary("showscoretable", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showscoretable)." << std::endl; return {}; })); /* SCALAR */
	add(unary("backpackcargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (backpackcargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getobjecttextures", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getobjecttextures)." << std::endl; return {}; })); /* OBJECT */
	add(unary("vestmagazines", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vestmagazines)." << std::endl; return {}; })); /* OBJECT */
	add(unary("curatoraddons", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (curatoraddons)." << std::endl; return {}; })); /* OBJECT */
	add(unary("secondaryweapon", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (secondaryweapon)." << std::endl; return {}; })); /* OBJECT */
	add(unary("get3denlayerentities", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (get3denlayerentities)." << std::endl; return {}; })); /* SCALAR */
	add(unary("getposvisual", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getposvisual)." << std::endl; return {}; })); /* OBJECT */
	add(unary("deletecollection", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deletecollection)." << std::endl; return {}; })); /* OBJECT */
	add(unary("triggerstatements", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggerstatements)." << std::endl; return {}; })); /* OBJECT */
	add(unary("priority", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (priority)." << std::endl; return {}; })); /* TASK */
	add(unary("menusetpicture", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusetpicture)." << std::endl; return {}; })); /* ARRAY */
	add(unary("setlocalwindparams", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setlocalwindparams)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lbvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbvalue)." << std::endl; return {}; })); /* ARRAY */
	add(unary("createcenter", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createcenter)." << std::endl; return {}; })); /* SIDE */
	add(unary("menuexpand", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuexpand)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getposaslw", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getposaslw)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lineintersectssurfaces", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lineintersectssurfaces)." << std::endl; return {}; })); /* ARRAY */
	add(unary("ongroupiconoverleave", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ongroupiconoverleave)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("camdestroy", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camdestroy)." << std::endl; return {}; })); /* OBJECT */
	add(unary("curatorwaypointcost", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (curatorwaypointcost)." << std::endl; return {}; })); /* OBJECT */
	add(unary("slidersetposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (slidersetposition)." << std::endl; return {}; })); /* ARRAY */
	add(unary("ismarkedforcollection", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ismarkedforcollection)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getassignedcuratorlogic", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getassignedcuratorlogic)." << std::endl; return {}; })); /* OBJECT */
	add(unary("triggeractivation", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (triggeractivation)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrldelete", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrldelete)." << std::endl; return {}; })); /* CONTROL */
	add(unary("parsetext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (parsetext)." << std::endl; return {}; })); /* STRING */
	add(unary("teammember", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (teammember)." << std::endl; return {}; })); /* OBJECT */
	add(unary("actionkeys", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (actionkeys)." << std::endl; return {}; })); /* STRING */
	add(unary("waypointbehaviour", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointbehaviour)." << std::endl; return {}; })); /* ARRAY */
	add(unary("preloadcamera", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (preloadcamera)." << std::endl; return {}; })); /* ARRAY */
	add(unary("parsenumber", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (parsenumber)." << std::endl; return {}; })); /* STRING, BOOL */
	add(unary("deletecenter", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deletecenter)." << std::endl; return {}; })); /* SIDE */
	add(unary("remoteexeccall", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (remoteexeccall)." << std::endl; return {}; })); /* ARRAY */
	add(unary("setstaminascheme", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setstaminascheme)." << std::endl; return {}; })); /* STRING */
	add(unary("rectangular", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (rectangular)." << std::endl; return {}; })); /* LOCATION */
	add(unary("move3dencamera", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (move3dencamera)." << std::endl; return {}; })); /* ARRAY */
	add(unary("addmusiceventhandler", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmusiceventhandler)." << std::endl; return {}; })); /* ARRAY */
	add(unary("resetsubgroupdirection", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (resetsubgroupdirection)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lognetworkterminate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lognetworkterminate)." << std::endl; return {}; })); /* SCALAR */
	add(unary("channelenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (channelenabled)." << std::endl; return {}; })); /* SCALAR */
	add(unary("lnbsetpictureright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpictureright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("cleargroupicons", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (cleargroupicons)." << std::endl; return {}; })); /* GROUP */
	add(unary("taskdestination", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (taskdestination)." << std::endl; return {}; })); /* TASK */
	add(unary("taskalwaysvisible", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (taskalwaysvisible)." << std::endl; return {}; })); /* TASK */
	add(unary("vestitems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vestitems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("switchcamera", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (switchcamera)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ppeffectcommitted", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ppeffectcommitted)." << std::endl; return {}; })); /* STRING, SCALAR */
	add(unary("buttonsetaction", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (buttonsetaction)." << std::endl; return {}; })); /* ARRAY */
	add(unary("menudelete", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menudelete)." << std::endl; return {}; })); /* ARRAY */
	add(unary("fromeditor", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (fromeditor)." << std::endl; return {}; })); /* TEAM_MEMBER */
	add(unary("waypointsenableduav", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointsenableduav)." << std::endl; return {}; })); /* OBJECT */
	add(unary("tvcollapse", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvcollapse)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbcolorright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbcolorright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("airdensityrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (airdensityrtd)." << std::endl; return {}; })); /* SCALAR */
	add(unary("keyname", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (keyname)." << std::endl; return {}; })); /* SCALAR */
	add(unary("closedialog", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (closedialog)." << std::endl; return {}; })); /* SCALAR */
	add(unary("lnbsetcolumnspos", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetcolumnspos)." << std::endl; return {}; })); /* ARRAY */
	add(unary("commandstop", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commandstop)." << std::endl; return {}; })); /* ARRAY,OBJECT */
	add(unary("list", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (list)." << std::endl; return {}; })); /* OBJECT */
	add(unary("hintc", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hintc)." << std::endl; return {}; })); /* STRING */
	add(unary("detach", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (detach)." << std::endl; return {}; })); /* OBJECT */
	add(unary("needreload", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (needreload)." << std::endl; return {}; })); /* OBJECT */
	add(unary("waypointdescription", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointdescription)." << std::endl; return {}; })); /* ARRAY */
	add(unary("image", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (image)." << std::endl; return {}; })); /* STRING */
	add(unary("dynamicsimulationdistancemult", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dynamicsimulationdistancemult)." << std::endl; return {}; })); /* STRING */
	add(unary("tvsetcursel", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetcursel)." << std::endl; return {}; })); /* ARRAY */
	add(unary("tvclear", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvclear)." << std::endl; return {}; })); /* SCALAR, CONTROL */
	add(unary("isnull", type::SCRIPT, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isnull)." << std::endl; return {}; })); /* SCRIPT, CONTROL, DISPLAY, NetObject, TASK, LOCATION */
	add(unary("isnull", type::CONTROL, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isnull)." << std::endl; return {}; })); /* SCRIPT, CONTROL, DISPLAY, NetObject, TASK, LOCATION */
	add(unary("isnull", type::DISPLAY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isnull)." << std::endl; return {}; })); /* SCRIPT, CONTROL, DISPLAY, NetObject, TASK, LOCATION */
	add(unary("isnull", type::NetObject, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isnull)." << std::endl; return {}; })); /* SCRIPT, CONTROL, DISPLAY, NetObject, TASK, LOCATION */
	add(unary("isnull", type::TASK, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isnull)." << std::endl; return {}; })); /* SCRIPT, CONTROL, DISPLAY, NetObject, TASK, LOCATION */
	add(unary("isnull", type::LOCATION, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isnull)." << std::endl; return {}; })); /* SCRIPT, CONTROL, DISPLAY, NetObject, TASK, LOCATION */
	add(unary("setacctime", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setacctime)." << std::endl; return {}; })); /* SCALAR */
	add(unary("remove3denlayer", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (remove3denlayer)." << std::endl; return {}; })); /* SCALAR */
	add(unary("removebackpack", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removebackpack)." << std::endl; return {}; })); /* OBJECT */
	add(unary("hideobjectglobal", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hideobjectglobal)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lnbcolor", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbcolor)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbcurselrow", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbcurselrow)." << std::endl; return {}; })); /* CONTROL, SCALAR */
	add(unary("getbackpackcargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getbackpackcargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getshotparents", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getshotparents)." << std::endl; return {}; })); /* OBJECT */
	add(unary("curatorregisteredobjects", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (curatorregisteredobjects)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlenabled)." << std::endl; return {}; })); /* CONTROL, SCALAR */
	add(unary("removeallmusiceventhandlers", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallmusiceventhandlers)." << std::endl; return {}; })); /* STRING */
	add(unary("playersnumber", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playersnumber)." << std::endl; return {}; })); /* SIDE */
	add(unary("onplayerconnected", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onplayerconnected)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("menutext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menutext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbaddcolumn", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbaddcolumn)." << std::endl; return {}; })); /* ARRAY */
	add(unary("menushortcuttext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menushortcuttext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("mapgridposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (mapgridposition)." << std::endl; return {}; })); /* ARRAY,OBJECT */
	add(unary("ropeattachenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropeattachenabled)." << std::endl; return {}; })); /* OBJECT */
	add(unary("firstbackpack", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (firstbackpack)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lnbsetpicture", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpicture)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getitemcargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getitemcargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("tvsetvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetvalue)." << std::endl; return {}; })); /* ARRAY */
	add(unary("removemusiceventhandler", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removemusiceventhandler)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getburningvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getburningvalue)." << std::endl; return {}; })); /* OBJECT */
	add(unary("formation", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (formation)." << std::endl; return {}; })); /* OBJECT,GROUP,TEAM_MEMBER */
	add(unary("simulclouddensity", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (simulclouddensity)." << std::endl; return {}; })); /* ARRAY */
	add(unary("localize", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (localize)." << std::endl; return {}; })); /* STRING */
	add(unary("loadbackpack", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (loadbackpack)." << std::endl; return {}; })); /* OBJECT */
	add(unary("unassignteam", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unassignteam)." << std::endl; return {}; })); /* OBJECT */
	add(unary("removeallownedmines", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallownedmines)." << std::endl; return {}; })); /* OBJECT */
	add(unary("menudata", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menudata)." << std::endl; return {}; })); /* ARRAY */
	add(unary("ropelength", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropelength)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getanimspeedcoef", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getanimspeedcoef)." << std::endl; return {}; })); /* OBJECT */
	add(unary("fullcrew", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (fullcrew)." << std::endl; return {}; })); /* OBJECT,ARRAY */
	add(unary("formationmembers", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (formationmembers)." << std::endl; return {}; })); /* OBJECT */
	add(unary("addcamshake", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addcamshake)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getcontainermaxload", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getcontainermaxload)." << std::endl; return {}; })); /* STRING */
	add(unary("type", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (type)." << std::endl; return {}; })); /* TASK, LOCATION */
	add(unary("lnbsetpicturecolor", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpicturecolor)." << std::endl; return {}; })); /* ARRAY */
	add(unary("visibleposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (visibleposition)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getpilotcameradirection", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getpilotcameradirection)." << std::endl; return {}; })); /* OBJECT */
	add(unary("speedmode", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (speedmode)." << std::endl; return {}; })); /* OBJECT,GROUP */
	add(unary("vestcontainer", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (vestcontainer)." << std::endl; return {}; })); /* OBJECT */
	add(unary("currenttasks", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currenttasks)." << std::endl; return {}; })); /* TEAM_MEMBER */
	add(unary("showhud", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showhud)." << std::endl; return {}; })); /* BOOL, ARRAY */
	add(unary("getposasl", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getposasl)." << std::endl; return {}; })); /* OBJECT */
	add(unary("showcuratorcompass", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showcuratorcompass)." << std::endl; return {}; })); /* BOOL */
	add(unary("forcemap", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (forcemap)." << std::endl; return {}; })); /* BOOL */
	add(unary("inflamed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (inflamed)." << std::endl; return {}; })); /* OBJECT */
	add(unary("waypoints", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypoints)." << std::endl; return {}; })); /* OBJECT,GROUP */
	add(unary("menusetaction", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusetaction)." << std::endl; return {}; })); /* ARRAY */
	add(unary("weaponinertia", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weaponinertia)." << std::endl; return {}; })); /* OBJECT */
	add(unary("haspilotcamera", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (haspilotcamera)." << std::endl; return {}; })); /* OBJECT */
	add(unary("isforcedwalk", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isforcedwalk)." << std::endl; return {}; })); /* OBJECT */
	add(unary("currentvisionmode", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentvisionmode)." << std::endl; return {}; })); /* OBJECT */
	add(unary("enablecaustics", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablecaustics)." << std::endl; return {}; })); /* BOOL */
	add(unary("deactivatekey", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deactivatekey)." << std::endl; return {}; })); /* STRING */
	add(unary("setdate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdate)." << std::endl; return {}; })); /* ARRAY */
	add(unary("direction", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (direction)." << std::endl; return {}; })); /* OBJECT,LOCATION, */
	add(unary("dynamicsimulationdistance", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (dynamicsimulationdistance)." << std::endl; return {}; })); /* STRING */
	add(unary("ppeffectenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ppeffectenabled)." << std::endl; return {}; })); /* SCALAR */
	add(unary("getdlcs", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getdlcs)." << std::endl; return {}; })); /* SCALAR */
	add(unary("getsuppression", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getsuppression)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbsetpicturecolorselected", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetpicturecolorselected)." << std::endl; return {}; })); /* ARRAY */
	add(unary("settrafficgap", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (settrafficgap)." << std::endl; return {}; })); /* ARRAY */
	add(unary("camusenvg", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (camusenvg)." << std::endl; return {}; })); /* BOOL */
	add(unary("resources", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (resources)." << std::endl; return {}; })); /* TEAM_MEMBER */
	add(unary("hintsilent", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hintsilent)." << std::endl; return {}; })); /* STRING,TEXT */
	add(unary("assignedcommander", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignedcommander)." << std::endl; return {}; })); /* OBJECT */
	add(unary("taskchildren", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (taskchildren)." << std::endl; return {}; })); /* TASK */
	add(unary("updateobjecttree", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (updateobjecttree)." << std::endl; return {}; })); /* CONTROL */
	add(unary("showcompass", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showcompass)." << std::endl; return {}; })); /* BOOL */
	add(unary("lnbpicture", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbpicture)." << std::endl; return {}; })); /* ARRAY */
	add(unary("waypointname", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointname)." << std::endl; return {}; })); /* ARRAY */
	add(unary("deletesite", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deletesite)." << std::endl; return {}; })); /* OBJECT */
	add(unary("nearestbuilding", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearestbuilding)." << std::endl; return {}; })); /* OBJECT,ARRAY */
	add(unary("addforcegeneratorrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addforcegeneratorrtd)." << std::endl; return {}; })); /* ARRAY */
	add(unary("forcegeneratorrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (forcegeneratorrtd)." << std::endl; return {}; })); /* SCALAR */
	add(unary("getposatlvisual", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getposatlvisual)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getallenvsoundcontrollers", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getallenvsoundcontrollers)." << std::endl; return {}; })); /* ARRAY */
	add(unary("clearitemcargoglobal", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clearitemcargoglobal)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getmissionconfig", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getmissionconfig)." << std::endl; return {}; })); /* STRING */
	add(unary("ctrltextheight", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrltextheight)." << std::endl; return {}; })); /* CONTROL */
	add(unary("removegoggles", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removegoggles)." << std::endl; return {}; })); /* OBJECT */
	add(unary("showsubtitles", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showsubtitles)." << std::endl; return {}; })); /* BOOL */
	add(unary("getplayerchannel", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getplayerchannel)." << std::endl; return {}; })); /* OBJECT */
	add(unary("menuadd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuadd)." << std::endl; return {}; })); /* ARRAY */
	add(unary("clearitemcargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clearitemcargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("sendudpmessage", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sendudpmessage)." << std::endl; return {}; })); /* ARRAY */
	add(unary("checkaifeature", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (checkaifeature)." << std::endl; return {}; })); /* STRING */
	add(unary("uavcontrol", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (uavcontrol)." << std::endl; return {}; })); /* OBJECT */
	add(unary("iswalking", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (iswalking)." << std::endl; return {}; })); /* OBJECT */
	add(unary("tasktype", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tasktype)." << std::endl; return {}; })); /* TASK */
	add(unary("flag", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (flag)." << std::endl; return {}; })); /* OBJECT */
	add(unary("composetext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (composetext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("formleader", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (formleader)." << std::endl; return {}; })); /* OBJECT */
	add(unary("stopped", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (stopped)." << std::endl; return {}; })); /* OBJECT */
	add(unary("clearallitemsfrombackpack", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clearallitemsfrombackpack)." << std::endl; return {}; })); /* OBJECT */
	add(unary("leaderboardgetrows", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (leaderboardgetrows)." << std::endl; return {}; })); /* STRING */
	add(unary("nearestlocations", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearestlocations)." << std::endl; return {}; })); /* ARRAY */
	add(unary("addmissioneventhandler", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (addmissioneventhandler)." << std::endl; return {}; })); /* ARRAY */
	add(unary("ongroupiconclick", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ongroupiconclick)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("hmd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hmd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setobjectviewdistance", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setobjectviewdistance)." << std::endl; return {}; })); /* SCALAR,ARRAY */
	add(unary("remove3deneventhandler", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (remove3deneventhandler)." << std::endl; return {}; })); /* ARRAY */
	add(unary("waypointscript", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointscript)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbtextright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbtextright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("magazinesdetailuniform", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazinesdetailuniform)." << std::endl; return {}; })); /* OBJECT */
	add(unary("tvsetpicturerightcolor", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvsetpicturerightcolor)." << std::endl; return {}; })); /* ARRAY */
	add(unary("hintcadet", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (hintcadet)." << std::endl; return {}; })); /* STRING,TEXT */
	add(unary("set3dengrid", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3dengrid)." << std::endl; return {}; })); /* ARRAY */
	add(unary("roadsconnectedto", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (roadsconnectedto)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lbcursel", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbcursel)." << std::endl; return {}; })); /* CONTROL,SCALAR */
	add(unary("canfire", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (canfire)." << std::endl; return {}; })); /* OBJECT */
	add(unary("creategeardialog", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (creategeardialog)." << std::endl; return {}; })); /* ARRAY */
	add(unary("slidersetspeed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (slidersetspeed)." << std::endl; return {}; })); /* ARRAY */
	add(unary("sendaumessage", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sendaumessage)." << std::endl; return {}; })); /* ARRAY */
	add(unary("iskeyactive", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (iskeyactive)." << std::endl; return {}; })); /* STRING */
	add(unary("isobjecthidden", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isobjecthidden)." << std::endl; return {}; })); /* OBJECT */
	add(unary("configsourcemod", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (configsourcemod)." << std::endl; return {}; })); /* CONFIG */
	add(unary("lnbvalue", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbvalue)." << std::endl; return {}; })); /* ARRAY */
	add(unary("tvpicture", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvpicture)." << std::endl; return {}; })); /* ARRAY */
	add(unary("underwater", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (underwater)." << std::endl; return {}; })); /* OBJECT */
	add(unary("showwaypoints", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (showwaypoints)." << std::endl; return {}; })); /* BOOL */
	add(unary("sliderspeed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (sliderspeed)." << std::endl; return {}; })); /* CONTROL,SCALAR */
	add(unary("setmusiceventhandler", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setmusiceventhandler)." << std::endl; return {}; })); /* ARRAY */
	add(unary("unitaimposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unitaimposition)." << std::endl; return {}; })); /* OBJECT */
	add(unary("lnbsize", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsize)." << std::endl; return {}; })); /* CONTROL,SCALAR */
	add(unary("ropecreate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ropecreate)." << std::endl; return {}; })); /* ARRAY */
	add(unary("deletestatus", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (deletestatus)." << std::endl; return {}; })); /* STRING */
	add(unary("lnbsetpicturecolorselected", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpicturecolorselected)." << std::endl; return {}; })); /* ARRAY */
	add(unary("morale", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (morale)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlfade", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlfade)." << std::endl; return {}; })); /* CONTROL */
	add(unary("selectplayer", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selectplayer)." << std::endl; return {}; })); /* OBJECT */
	add(unary("menusetcheck", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusetcheck)." << std::endl; return {}; })); /* ARRAY */
	add(unary("createlocation", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createlocation)." << std::endl; return {}; })); /* ARRAY */
	add(unary("menuclear", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menuclear)." << std::endl; return {}; })); /* CONTROL,SCALAR */
	add(unary("failmission", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (failmission)." << std::endl; return {}; })); /* STRING */
	add(unary("lnbsetcolorright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetcolorright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("setcompassoscillation", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setcompassoscillation)." << std::endl; return {}; })); /* ARRAY */
	add(unary("iscollisionlighton", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (iscollisionlighton)." << std::endl; return {}; })); /* OBJECT */
	add(unary("removeallmissioneventhandlers", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallmissioneventhandlers)." << std::endl; return {}; })); /* STRING */
	add(unary("ctrlvisible", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlvisible)." << std::endl; return {}; })); /* SCALAR */
	add(unary("formationtask", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (formationtask)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ismanualfire", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ismanualfire)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getpilotcamerarotation", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getpilotcamerarotation)." << std::endl; return {}; })); /* OBJECT */
	add(unary("opendlcpage", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (opendlcpage)." << std::endl; return {}; })); /* SCALAR */
	add(unary("wingsforcesrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (wingsforcesrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("setdefaultcamera", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdefaultcamera)." << std::endl; return {}; })); /* ARRAY */
	add(unary("removefromremainscollector", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removefromremainscollector)." << std::endl; return {}; })); /* ARRAY */
	add(unary("playscriptedmission", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (playscriptedmission)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbsetcolor", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetcolor)." << std::endl; return {}; })); /* ARRAY */
	add(unary("onteamswitch", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onteamswitch)." << std::endl; return {}; })); /* STRING,CODE */
	add(unary("lineintersects", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lineintersects)." << std::endl; return {}; })); /* ARRAY */
	add(unary("isuavconnected", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isuavconnected)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlposition)." << std::endl; return {}; })); /* CONTROL */
	add(unary("lbsize", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsize)." << std::endl; return {}; })); /* CONTROL, SCALAR */
	add(unary("getunitloadout", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getunitloadout)." << std::endl; return {}; })); /* ARRAY,OBJECT,CONFIG,STRING */
	add(unary("roadat", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (roadat)." << std::endl; return {}; })); /* ARRAY,OBJECT */
	add(unary("createguardedpoint", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createguardedpoint)." << std::endl; return {}; })); /* ARRAY */
	add(unary("commitoverlay", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (commitoverlay)." << std::endl; return {}; })); /* CONTROL */
	add(unary("currentweapon", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentweapon)." << std::endl; return {}; })); /* OBJECT */
	add(unary("getdirvisual", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getdirvisual)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlmapmouseover", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmapmouseover)." << std::endl; return {}; })); /* CONTROL */
	add(unary("drawline3d", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (drawline3d)." << std::endl; return {}; })); /* ARRAY */
	add(unary("ctrlmapanimclear", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlmapanimclear)." << std::endl; return {}; })); /* CONTROL */
	add(unary("leaderboarddeinit", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (leaderboarddeinit)." << std::endl; return {}; })); /* STRING */
	add(unary("clearoverlay", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clearoverlay)." << std::endl; return {}; })); /* CONTROL */
	add(unary("enginesrpmrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enginesrpmrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("datetonumber", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (datetonumber)." << std::endl; return {}; })); /* ARRAY */
	add(unary("setsystemofunits", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setsystemofunits)." << std::endl; return {}; })); /* SCALAR */
	add(unary("breakto", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (breakto)." << std::endl; return {}; })); /* STRING */
	add(unary("difficultyenabled", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (difficultyenabled)." << std::endl; return {}; })); /* STRING */
	add(unary("clearmagazinecargoglobal", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (clearmagazinecargoglobal)." << std::endl; return {}; })); /* OBJECT */
	add(unary("synchronizedobjects", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (synchronizedobjects)." << std::endl; return {}; })); /* OBJECT */
	add(unary("tg", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tg)." << std::endl; return {}; })); /* SCALAR,NaN */
	add(unary("useaisteeringcomponent", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (useaisteeringcomponent)." << std::endl; return {}; })); /* BOOL */
	add(unary("lbsetcursel", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbsetcursel)." << std::endl; return {}; })); /* ARRAY */
	add(unary("setarmorypoints", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setarmorypoints)." << std::endl; return {}; })); /* SCALAR */
	add(unary("weightrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (weightrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("loadvest", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (loadvest)." << std::endl; return {}; })); /* OBJECT */
	add(unary("gearidcammocount", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (gearidcammocount)." << std::endl; return {}; })); /* SCALAR */
	add(unary("nearestlocationwithdubbing", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (nearestlocationwithdubbing)." << std::endl; return {}; })); /* ARRAY */
	add(unary("createmine", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createmine)." << std::endl; return {}; })); /* ARRAY */
	add(unary("lnbsetpicturecolorright", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lnbsetpicturecolorright)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getposaslvisual", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getposaslvisual)." << std::endl; return {}; })); /* OBJECT */
	add(unary("eyepos", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (eyepos)." << std::endl; return {}; })); /* OBJECT */
	add(unary("removeallitems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (removeallitems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("createvehiclecrew", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (createvehiclecrew)." << std::endl; return {}; })); /* OBJECT */
	add(unary("uniformmagazines", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (uniformmagazines)." << std::endl; return {}; })); /* OBJECT */
	add(unary("enablecamshake", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enablecamshake)." << std::endl; return {}; })); /* BOOL */
	add(unary("tvcursel", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (tvcursel)." << std::endl; return {}; })); /* SCALAR,CONTROL */
	add(unary("setdetailmapblendpars", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (setdetailmapblendpars)." << std::endl; return {}; })); /* ARRAY */
	add(unary("onbriefingnotes", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (onbriefingnotes)." << std::endl; return {}; })); /* STRING */
	add(unary("backpackitems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (backpackitems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("magazines", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazines)." << std::endl; return {}; })); /* OBJECT */
	add(unary("waypointcombatmode", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (waypointcombatmode)." << std::endl; return {}; })); /* ARRAY */
	add(unary("enginesisonrtd", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (enginesisonrtd)." << std::endl; return {}; })); /* OBJECT */
	add(unary("queryweaponpool", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (queryweaponpool)." << std::endl; return {}; })); /* STRING */
	add(unary("didjipowner", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (didjipowner)." << std::endl; return {}; })); /* OBJECT */
	add(unary("primaryweaponitems", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (primaryweaponitems)." << std::endl; return {}; })); /* OBJECT */
	add(unary("visiblepositionasl", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (visiblepositionasl)." << std::endl; return {}; })); /* OBJECT */
	add(unary("set3denselected", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (set3denselected)." << std::endl; return {}; })); /* ARRAY */
	add(unary("get3denselected", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (get3denselected)." << std::endl; return {}; })); /* STRING */
	add(unary("lightison", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lightison)." << std::endl; return {}; })); /* OBJECT */
	add(unary("assignedcargo", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (assignedcargo)." << std::endl; return {}; })); /* OBJECT */
	add(unary("currentmagazine", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (currentmagazine)." << std::endl; return {}; })); /* OBJECT */
	add(unary("taskstate", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (taskstate)." << std::endl; return {}; })); /* TASK */
	add(unary("magazinesdetailvest", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (magazinesdetailvest)." << std::endl; return {}; })); /* OBJECT */
	add(unary("not", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (not)." << std::endl; return {}; })); /* BOOL */
	add(unary("lbclear", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lbclear)." << std::endl; return {}; })); /* CONTROL,SCALAR */
	add(unary("getpilotcameraposition", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getpilotcameraposition)." << std::endl; return {}; })); /* OBJECT */
	add(unary("forcerespawn", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (forcerespawn)." << std::endl; return {}; })); /* OBJECT */
	add(unary("titletext", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (titletext)." << std::endl; return {}; })); /* ARRAY */
	add(unary("getconnecteduav", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getconnecteduav)." << std::endl; return {}; })); /* OBJECT */
	add(unary("geteditorcamera", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (geteditorcamera)." << std::endl; return {}; })); /* CONTROL */
	add(unary("menusort", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (menusort)." << std::endl; return {}; })); /* ARRAY */
	add(unary("get3denentityid", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (get3denentityid)." << std::endl; return {}; })); /* ANY */
	add(unary("lockeddriver", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (lockeddriver)." << std::endl; return {}; })); /* OBJECT */
	add(unary("ctrlsetfocus", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (ctrlsetfocus)." << std::endl; return {}; })); /* CONTROL */
	add(unary("unitaddons", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (unitaddons)." << std::endl; return {}; })); /* STRING */
	add(unary("parsesimplearray", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (parsesimplearray)." << std::endl; return {}; })); /* STRING */
	add(unary("getpylonmagazines", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (getpylonmagazines)." << std::endl; return {}; })); /* STRING */
	add(unary("selectrandomweighted", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (selectrandomweighted)." << std::endl; return {}; })); /* STRING */
	add(unary("isdamageallowed", type::ANY, "", [](virtualmachine* vm, value::cref r) -> value { vm->err() << "NOT IMPLEMENTED (isdamageallowed)." << std::endl; return {}; })); /* STRING */



	add(nular("safezonex", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (safezonex)." << std::endl; return {}; }));
	add(nular("clearmagazinepool", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (clearmagazinepool)." << std::endl; return {}; }));
	add(nular("slingloadassistantshown", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (slingloadassistantshown)." << std::endl; return {}; }));
	add(nular("windstr", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (windstr)." << std::endl; return {}; }));
	add(nular("isinstructorfigureenabled", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (isinstructorfigureenabled)." << std::endl; return {}; }));
	add(nular("safezoney", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (safezoney)." << std::endl; return {}; }));
	add(nular("getclientstatenumber", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getclientstatenumber)." << std::endl; return {}; }));
	add(nular("getclientstate", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getclientstate)." << std::endl; return {}; }));
	add(nular("date", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (date)." << std::endl; return {}; }));
	add(nular("getelevationoffset", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getelevationoffset)." << std::endl; return {}; }));
	add(nular("current3denoperation", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (current3denoperation)." << std::endl; return {}; }));
	add(nular("getterraingrid", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getterraingrid)." << std::endl; return {}; }));
	add(nular("exit", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (exit)." << std::endl; return {}; }));
	add(nular("mapanimclear", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (mapanimclear)." << std::endl; return {}; }));
	add(nular("missiondifficulty", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (missiondifficulty)." << std::endl; return {}; }));
	add(nular("nextweatherchange", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (nextweatherchange)." << std::endl; return {}; }));
	add(nular("allsites", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (allsites)." << std::endl; return {}; }));
	add(nular("teamswitchenabled", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (teamswitchenabled)." << std::endl; return {}; }));
	add(nular("visiblescoretable", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (visiblescoretable)." << std::endl; return {}; }));
	add(nular("is3denmultiplayer", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (is3denmultiplayer)." << std::endl; return {}; }));
	add(nular("worldname", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (worldname)." << std::endl; return {}; }));
	add(nular("ismultiplayersolo", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (ismultiplayersolo)." << std::endl; return {}; }));
	add(nular("isremoteexecuted", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (isremoteexecuted)." << std::endl; return {}; }));
	add(nular("curatorcamera", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (curatorcamera)." << std::endl; return {}; }));
	add(nular("displaynull", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (displaynull)." << std::endl; return {}; }));
	add(nular("diag_frameno", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (diag_frameno)." << std::endl; return {}; }));
	add(nular("currentnamespace", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (currentnamespace)." << std::endl; return {}; }));
	add(nular("armorypoints", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (armorypoints)." << std::endl; return {}; }));
	add(nular("controlnull", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (controlnull)." << std::endl; return {}; }));
	add(nular("markasfinishedonsteam", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (markasfinishedonsteam)." << std::endl; return {}; }));
	add(nular("visiblewatch", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (visiblewatch)." << std::endl; return {}; }));
	add(nular("estimatedendservertime", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (estimatedendservertime)." << std::endl; return {}; }));
	add(nular("diag_activemissionfsms", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (diag_activemissionfsms)." << std::endl; return {}; }));
	add(nular("hudmovementlevels", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (hudmovementlevels)." << std::endl; return {}; }));
	add(nular("rainbow", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (rainbow)." << std::endl; return {}; }));
	add(nular("servertime", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (servertime)." << std::endl; return {}; }));
	add(nular("teams", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (teams)." << std::endl; return {}; }));
	add(nular("all3denentities", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (all3denentities)." << std::endl; return {}; }));
	add(nular("librarycredits", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (librarycredits)." << std::endl; return {}; }));
	add(nular("shownwarrant", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (shownwarrant)." << std::endl; return {}; }));
	add(nular("allmines", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (allmines)." << std::endl; return {}; }));
	add(nular("getshadowdistance", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getshadowdistance)." << std::endl; return {}; }));
	add(nular("alldisplays", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (alldisplays)." << std::endl; return {}; }));
	add(nular("isautotest", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (isautotest)." << std::endl; return {}; }));
	add(nular("viewdistance", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (viewdistance)." << std::endl; return {}; }));
	add(nular("sideambientlife", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (sideambientlife)." << std::endl; return {}; }));
	add(nular("worldsize", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (worldsize)." << std::endl; return {}; }));
	add(nular("rain", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (rain)." << std::endl; return {}; }));
	add(nular("didjip", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (didjip)." << std::endl; return {}; }));
	add(nular("missionstart", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (missionstart)." << std::endl; return {}; }));
	add(nular("airdensitycurvertd", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (airdensitycurvertd)." << std::endl; return {}; }));
	add(nular("sunormoon", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (sunormoon)." << std::endl; return {}; }));
	add(nular("safezonewabs", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (safezonewabs)." << std::endl; return {}; }));
	add(nular("profilename", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (profilename)." << std::endl; return {}; }));
	add(nular("difficultyenabledrtd", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (difficultyenabledrtd)." << std::endl; return {}; }));
	add(nular("savegame", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (savegame)." << std::endl; return {}; }));
	add(nular("hasinterface", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (hasinterface)." << std::endl; return {}; }));
	add(nular("clearitempool", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (clearitempool)." << std::endl; return {}; }));
	add(nular("runinitscript", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (runinitscript)." << std::endl; return {}; }));
	add(nular("getartillerycomputersettings", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getartillerycomputersettings)." << std::endl; return {}; }));
	add(nular("visiblemap", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (visiblemap)." << std::endl; return {}; }));
	add(nular("forceweatherchange", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (forceweatherchange)." << std::endl; return {}; }));
	add(nular("shownchat", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (shownchat)." << std::endl; return {}; }));
	add(nular("mapanimdone", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (mapanimdone)." << std::endl; return {}; }));
	add(nular("resetcamshake", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (resetcamshake)." << std::endl; return {}; }));
	add(nular("showncompass", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (showncompass)." << std::endl; return {}; }));
	add(nular("winddir", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (winddir)." << std::endl; return {}; }));
	add(nular("currentchannel", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (currentchannel)." << std::endl; return {}; }));
	add(nular("gettotaldlcusagetime", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (gettotaldlcusagetime)." << std::endl; return {}; }));
	add(nular("get3denlinesvisible", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (get3denlinesvisible)." << std::endl; return {}; }));
	add(nular("endloadingscreen", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (endloadingscreen)." << std::endl; return {}; }));
	add(nular("profilenamesteam", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (profilenamesteam)." << std::endl; return {}; }));
	add(nular("playableunits", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (playableunits)." << std::endl; return {}; }));
	add(nular("cheatsenabled", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (cheatsenabled)." << std::endl; return {}; }));
	add(nular("opencuratorinterface", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (opencuratorinterface)." << std::endl; return {}; }));
	add(nular("gusts", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (gusts)." << std::endl; return {}; }));
	add(nular("overcastforecast", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (overcastforecast)." << std::endl; return {}; }));
	add(nular("commandingmenu", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (commandingmenu)." << std::endl; return {}; }));
	add(nular("distributionregion", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (distributionregion)." << std::endl; return {}; }));
	add(nular("windrtd", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (windrtd)." << std::endl; return {}; }));
	add(nular("allcurators", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (allcurators)." << std::endl; return {}; }));
	add(nular("benchmark", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (benchmark)." << std::endl; return {}; }));
	add(nular("getmissiondlcs", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getmissiondlcs)." << std::endl; return {}; }));
	add(nular("istuthintsenabled", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (istuthintsenabled)." << std::endl; return {}; }));
	add(nular("shownartillerycomputer", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (shownartillerycomputer)." << std::endl; return {}; }));
	add(nular("scriptnull", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (scriptnull)." << std::endl; return {}; }));
	add(nular("isfilepatchingenabled", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (isfilepatchingenabled)." << std::endl; return {}; }));
	add(nular("missionname", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (missionname)." << std::endl; return {}; }));
	add(nular("clearweaponpool", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (clearweaponpool)." << std::endl; return {}; }));
	add(nular("issteammission", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (issteammission)." << std::endl; return {}; }));
	add(nular("linebreak", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (linebreak)." << std::endl; return {}; }));
	add(nular("language", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (language)." << std::endl; return {}; }));
	add(nular("diag_fps", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (diag_fps)." << std::endl; return {}; }));
	add(nular("acctime", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (acctime)." << std::endl; return {}; }));
	add(nular("enableenddialog", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (enableenddialog)." << std::endl; return {}; }));
	add(nular("safezoneh", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (safezoneh)." << std::endl; return {}; }));
	add(nular("allunitsuav", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (allunitsuav)." << std::endl; return {}; }));
	add(nular("shownradio", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (shownradio)." << std::endl; return {}; }));
	add(nular("allplayers", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (allplayers)." << std::endl; return {}; }));
	add(nular("clearforcesrtd", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (clearforcesrtd)." << std::endl; return {}; }));
	add(nular("clearradio", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (clearradio)." << std::endl; return {}; }));
	add(nular("disabledebriefingstats", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (disabledebriefingstats)." << std::endl; return {}; }));
	add(nular("logentities", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (logentities)." << std::endl; return {}; }));
	add(nular("visiblecompass", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (visiblecompass)." << std::endl; return {}; }));
	add(nular("locationnull", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (locationnull)." << std::endl; return {}; }));
	add(nular("shownwatch", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (shownwatch)." << std::endl; return {}; }));
	add(nular("playerside", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (playerside)." << std::endl; return {}; }));
	add(nular("lightnings", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (lightnings)." << std::endl; return {}; }));
	add(nular("simulweathersync", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (simulweathersync)." << std::endl; return {}; }));
	add(nular("shownmap", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (shownmap)." << std::endl; return {}; }));
	add(nular("shownpad", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (shownpad)." << std::endl; return {}; }));
	add(nular("clientowner", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (clientowner)." << std::endl; return {}; }));
	add(nular("getobjectviewdistance", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getobjectviewdistance)." << std::endl; return {}; }));
	add(nular("overcast", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (overcast)." << std::endl; return {}; }));
	add(nular("safezonexabs", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (safezonexabs)." << std::endl; return {}; }));
	add(nular("forceend", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (forceend)." << std::endl; return {}; }));
	add(nular("diag_activesqfscripts", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (diag_activesqfscripts)." << std::endl; return {}; }));
	add(nular("selectnoplayer", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (selectnoplayer)." << std::endl; return {}; }));
	add(nular("getdlcassetsusage", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getdlcassetsusage)." << std::endl; return {}; }));
	add(nular("diag_activesqsscripts", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (diag_activesqsscripts)." << std::endl; return {}; }));
	add(nular("wind", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (wind)." << std::endl; return {}; }));
	add(nular("servername", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (servername)." << std::endl; return {}; }));
	add(nular("freelook", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (freelook)." << std::endl; return {}; }));
	add(nular("humidity", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (humidity)." << std::endl; return {}; }));
	add(nular("hcshownbar", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (hcshownbar)." << std::endl; return {}; }));
	add(nular("visiblegps", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (visiblegps)." << std::endl; return {}; }));
	add(nular("shownuavfeed", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (shownuavfeed)." << std::endl; return {}; }));
	add(nular("ismultiplayer", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (ismultiplayer)." << std::endl; return {}; }));
	add(nular("isdedicated", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (isdedicated)." << std::endl; return {}; }));
	add(nular("alldeadmen", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (alldeadmen)." << std::endl; return {}; }));
	add(nular("diag_fpsmin", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (diag_fpsmin)." << std::endl; return {}; }));
	add(nular("showncuratorcompass", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (showncuratorcompass)." << std::endl; return {}; }));
	add(nular("finishmissioninit", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (finishmissioninit)." << std::endl; return {}; }));
	add(nular("fog", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (fog)." << std::endl; return {}; }));
	add(nular("buldozer_isenabledroaddiag", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (buldozer_isenabledroaddiag)." << std::endl; return {}; }));
	add(nular("userinputdisabled", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (userinputdisabled)." << std::endl; return {}; }));
	add(nular("loadgame", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (loadgame)." << std::endl; return {}; }));
	add(nular("forcedmap", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (forcedmap)." << std::endl; return {}; }));
	add(nular("cameraview", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (cameraview)." << std::endl; return {}; }));
	add(nular("moonintensity", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (moonintensity)." << std::endl; return {}; }));
	add(nular("dynamicsimulationenabled", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (dynamicsimulationenabled)." << std::endl; return {}; }));
	add(nular("isserver", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (isserver)." << std::endl; return {}; }));
	add(nular("reversedmousey", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (reversedmousey)." << std::endl; return {}; }));
	add(nular("librarydisclaimers", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (librarydisclaimers)." << std::endl; return {}; }));
	add(nular("getremotesensorsdisabled", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getremotesensorsdisabled)." << std::endl; return {}; }));
	add(nular("tasknull", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (tasknull)." << std::endl; return {}; }));
	add(nular("get3dencamera", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (get3dencamera)." << std::endl; return {}; }));
	add(nular("cadetmode", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (cadetmode)." << std::endl; return {}; }));
	add(nular("cameraon", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (cameraon)." << std::endl; return {}; }));
	add(nular("allgroups", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (allgroups)." << std::endl; return {}; }));
	add(nular("agents", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (agents)." << std::endl; return {}; }));
	add(nular("briefingname", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (briefingname)." << std::endl; return {}; }));
	add(nular("shownscoretable", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (shownscoretable)." << std::endl; return {}; }));
	add(nular("copyfromclipboard", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (copyfromclipboard)." << std::endl; return {}; }));
	add(nular("cursorobject", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (cursorobject)." << std::endl; return {}; }));
	add(nular("teamswitch", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (teamswitch)." << std::endl; return {}; }));
	add(nular("diag_activescripts", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (diag_activescripts)." << std::endl; return {}; }));
	add(nular("isstressdamageenabled", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (isstressdamageenabled)." << std::endl; return {}; }));
	add(nular("fogparams", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (fogparams)." << std::endl; return {}; }));
	add(nular("shownhud", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (shownhud)." << std::endl; return {}; }));
	add(nular("switchableunits", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (switchableunits)." << std::endl; return {}; }));
	add(nular("playerrespawntime", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (playerrespawntime)." << std::endl; return {}; }));
	add(nular("initambientlife", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (initambientlife)." << std::endl; return {}; }));
	add(nular("allcutlayers", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (allcutlayers)." << std::endl; return {}; }));
	add(nular("curatormouseover", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (curatormouseover)." << std::endl; return {}; }));
	add(nular("get3denmouseover", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (get3denmouseover)." << std::endl; return {}; }));
	add(nular("missionversion", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (missionversion)." << std::endl; return {}; }));
	add(nular("activatedaddons", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (activatedaddons)." << std::endl; return {}; }));
	add(nular("showngps", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (showngps)." << std::endl; return {}; }));
	add(nular("cursortarget", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (cursortarget)." << std::endl; return {}; }));
	add(nular("difficulty", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (difficulty)." << std::endl; return {}; }));
	add(nular("fogforecast", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (fogforecast)." << std::endl; return {}; }));
	add(nular("savejoysticks", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (savejoysticks)." << std::endl; return {}; }));
	add(nular("buldozer_reloadopermap", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (buldozer_reloadopermap)." << std::endl; return {}; }));
	add(nular("getmissionlayers", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getmissionlayers)." << std::endl; return {}; }));
	add(nular("is3den", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (is3den)." << std::endl; return {}; }));
	add(nular("dialog", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (dialog)." << std::endl; return {}; }));
	add(nular("systemofunits", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (systemofunits)." << std::endl; return {}; }));
	add(nular("radiovolume", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (radiovolume)." << std::endl; return {}; }));
	add(nular("savingenabled", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (savingenabled)." << std::endl; return {}; }));
	add(nular("particlesquality", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (particlesquality)." << std::endl; return {}; }));
	add(nular("mapanimcommit", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (mapanimcommit)." << std::endl; return {}; }));
	add(nular("vehicles", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (vehicles)." << std::endl; return {}; }));
	add(nular("curatorselected", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (curatorselected)." << std::endl; return {}; }));
	add(nular("musicvolume", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (musicvolume)." << std::endl; return {}; }));
	add(nular("waves", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (waves)." << std::endl; return {}; }));
	add(nular("disableserialization", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (disableserialization)." << std::endl; return {}; }));
	add(nular("daytime", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (daytime)." << std::endl; return {}; }));
	add(nular("teammembernull", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (teammembernull)." << std::endl; return {}; }));
	add(nular("soundvolume", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (soundvolume)." << std::endl; return {}; }));
	add(nular("alldead", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (alldead)." << std::endl; return {}; }));
	add(nular("getresolution", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getresolution)." << std::endl; return {}; }));
	add(nular("timemultiplier", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (timemultiplier)." << std::endl; return {}; }));
	add(nular("safezonew", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (safezonew)." << std::endl; return {}; }));
	add(nular("getmouseposition", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (getmouseposition)." << std::endl; return {}; }));
	add(nular("get3deniconsvisible", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (get3deniconsvisible)." << std::endl; return {}; }));
	add(nular("isstreamfriendlyuienabled", "", [](virtualmachine* vm) -> value { vm->err() << "NOT IMPLEMENTED (isstreamfriendlyuienabled)." << std::endl; return {}; }));
}
