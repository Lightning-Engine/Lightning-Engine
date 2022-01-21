require("Vendor/compile-commands-generator/compile-commands-generator")

workspace "Lightning-Engine"
	architecture "x86_64"

	configurations {
		"Debug",
		"DebugDLL",
		"Release",
		"ReleaseDLL",
		"Distribution",
		"DistributionDLL"
	}

outputdir = "%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/"

include "Lightning-Engine"
include "Sandbox"