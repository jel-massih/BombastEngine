require("scripts\\ActorManager.lua");

g_actorMgr = ActorManager:Create();


function AddActor(scriptObject)
	g_actorMgr:AddActor(scriptObject);
end

function RemoveActor(scriptObject)
	g_actorMgr:RemoveActor(scriptObject);
end