premake.modules.lua = {}
local m = premake.modules.lua

local p = premake
local project = p.project

function getIncludeDirs(cfg)
	local flags = {}
	print("cfg name " .. cfg.name)
	for _,dir in ipairs(cfg.includedirs) do
		table.insert(flags, '-I' .. '\'' .. dir .. '\'')
	end
	for _,dir in ipairs(cfg.sysincludedirs) do
		table.insert(flags, '-isystem' .. '\'' .. dir .. '\'')
	end
	return test
end

newaction {
	trigger = "compile_commands",
	description = "internal compile_commands.json generator",

	onWorkspace = function(wks)
		print("Generating compile_commands.json for workspace " .. wks.name)
	end,

	onProject = function(prj)
		printf("Generating compile_commands for project " .. prj.name)
		for cfg in project.eachconfig(prj) do   
			getIncludeDirs(cfg)
		end
	end,

	execute = Execute,
}

return m