ActorManager = class(nil,
{
	_actors = {},
});

function ActorManager:AddActor(scriptObject)
	print("Adding Actor");
	
	-- add actor to list of actors
	local actorId = scriptObject:GetActorId();
	if (self._actors[actorId] ~= nil) then
		print("Overwriting Actor with id: " .. actorId);
	end
	self._actors[actorId] = scriptObject;
end

function ActorManager:RemoveActor(scriptObject)
	local actorId = scriptObject:GetActorId();
	self._actors[actorId] = nil;
end

function ActorManager:GetActor(actorId)
	return self._actors[actorId];
end