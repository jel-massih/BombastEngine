-- Redefinition of require and print
do
	local resourceIdMap = {};  -- Map of Loaded Resources
	
	require = function(script)
		if (not resourceIdMap[script]) then
			if (LoadAndRunScriptResource(script)) then
				resourceIdMap[script] = true;
			end
		end
	end
end

do
    print = function(text)
		if (text ~= nil) then
			Log(text);
		else
			Log("<nil>");
		end
	end
end

function class(baseClass, body)
	local ret = body or {};

	-- if baseClass, attach this to it
	if(baseClass ~= nil) then
		setmetatable(ret, ret);
		ret.__index = baseClass;
		ret.base = baseClass;
	end

	ret.Create = function(self, constructionData, origionalSubClass)
		local obj;
		if(self.__index ~= nul) then
			if(origionalSubClass ~= nil) then
				obj = self.__index:Create(constructionData, origionalSubClass);
			else
				obj = self.__index:Create(constructionData, self);
			end
		else
			obj = constructionData or {};
		end

		setmetatable(obj, obj);
		obj.__index = self;

		if(self.__operators ~= nil) then
			for key, val in pairs(self.__operators) do
				obj[key] = val;
			end
		end

		return obj;
	end

	ret.IsInstance = function(self, otherClass)
		local cls = self.__index;
		while cls do
			if cls == otherClass then
				return true
			end
			cls = cls.base
		end
		return false
	end

	return ret;
end

Event = {};
EventType = {};