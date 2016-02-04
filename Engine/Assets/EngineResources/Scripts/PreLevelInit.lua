require("EngineResources.Scripts.ActorManager.lua");

g_actorMgr = ActorManager:Create();

function AddEntity(scriptObject)
	g_actorMgr:AddEntity(scriptObject);
end

function RemoveEntity(scriptObject)
	g_actorMgr:RemoveEntity(scriptObject);
end