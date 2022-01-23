#!/usr/bin/env python3
import asyncio
import os.path
import sys
import shutil

async def run(program, *args):
	print(program, *args)
	proc = await asyncio.create_subprocess_exec(program, *args)
	returncode = await proc.wait()
	if returncode != 0:
		sys.exit(1)

class PosixFmt:
	def obj(path):
		head, tail = os.path.split(path)
		return os.path.join(head, f"{tail}.o")

	def shared(path):
		head, tail = os.path.split(path)
		return os.path.join(head, f"lib{tail}.so")

	def program(path):
		head, tail = os.path.split(path)
		return os.path.join(head, f"{tail}")

class Win32Fmt:
	def obj(path):
		head, tail = os.path.split(path)
		return os.path.join(head, f"{tail}.o")

	def shared(path):
		head, tail = os.path.split(path)
		return os.path.join(head, f"{tail}.dll")

	def program(path):
		head, tail = os.path.split(path)
		return os.path.join(head, f"{tail}.exe")

class BaseConfig:
	@classmethod
	async def comp_c(cls, tgt, src, incs, defs):
		incs = [f"-I{inc}" for inc in incs]
		defs = [f"-D{define}" for define in defs]
		await run(cls.cc, *cls.cflags, "-c", "-o", tgt, src, *incs, *defs, "-fPIC")
	
	@classmethod
	async def comp_cxx(cls, tgt, src, incs, defs):
		incs = [f"-I{inc}" for inc in incs]
		defs = [f"-D{define}" for define in defs]
		await run(cls.cxx, *cls.cxxflags, "-c", "-o", tgt, src, *incs, *defs, "-fPIC")

	@classmethod
	async def shared(cls, tgt, objs, libs):
		libs = [f"-l{lib}" for lib in libs]
		await run(cls.cc, *cls.ldflags, "-shared", "-o", tgt, *objs, *libs)

	@classmethod
	async def program(cls, tgt, objs, libs):
		libs = [f"-l{lib}" for lib in libs]
		await run(cls.cc, *cls.ldflags, "-o", tgt, *objs, *libs)

class GnuConfig(BaseConfig):
	cc = "gcc"
	cxx = "g++"
	fmt = PosixFmt
	cflags = ["-std=c11", "-Wall", "-Wextra", "-pedantic", "-fsanitize=address", "-Og", "-g"]
	cxxflags = ["-std=c11", "-Wall", "-Wextra", "-pedantic", "-fsanitize=address", "-Og", "-g"]
	ldflags = ["-std=c11", "-Wall", "-Wextra", "-pedantic", "-fsanitize=address"]

class ClangConfig(BaseConfig):
	cc = "clang"
	cxx = "clang++"
	fmt = PosixFmt
	cflags = ["-std=c11", "-Wall", "-Wextra", "-pedantic", "-fsanitize=address", "-Og", "-g"]
	cxxflags = ["-std=c11", "-Wall", "-Wextra", "-pedantic", "-fsanitize=address", "-Og", "-g"]
	ldflags = ["-std=c11", "-Wall", "-Wextra", "-pedantic", "-fsanitize=address"]

class MingwConfig(BaseConfig):
	cc = "x86_64-w64-mingw32-gcc"
	cxx = "x86_64-w64-mingw32-g++"
	fmt = Win32Fmt
	cflags = ["-municode", "-std=c11", "-Wall", "-Wextra", "-pedantic", "-Og", "-g"]
	cxxflags = ["-municode", "-std=c11", "-Wall", "-Wextra", "-pedantic", "-Og", "-g"]
	ldflags = ["-std=c11", "-Wall", "-Wextra", "-pedantic", ]

class MacosConfig(BaseConfig):
	cc = "clang"
	cxx = "clang++"
	fmt = PosixFmt
	cflags = ["-std=c11", "-Wall", "-Wextra", "-pedantic", "-fsanitize=address", "-Og", "-g"]
	cxxflags = ["-std=c11", "-Wall", "-Wextra", "-pedantic", "-fsanitize=address", "-Og", "-g"]
	ldflags = ["-std=c11", "-Wall", "-Wextra", "-pedantic", "-fsanitize=address"]

	@classmethod
	async def shared(cls, tgt, objs, libs):
		libs = sum([["-framework", lib] for lib in libs], [])
		await run(cls.cc, *cls.ldflags, "-shared", "-o", tgt, *objs, *libs)

	@classmethod
	async def program(cls, tgt, objs, libs):
		libs = sum([["-framework", lib] for lib in libs], [])
		await run(cls.cc, *cls.ldflags, "-o", tgt, *objs, *libs)

def shared_path(tgt, config):
	return os.path.join("bin", config.fmt.shared(tgt.name))

def program_path(tgt, config):
	return os.path.join("bin", config.fmt.program(tgt.name))

def src_path(tgt, src):
	return os.path.join(tgt.path, "src", src)

def obj_path(tgt, src, config):
	return os.path.join("obj", tgt.path, config.fmt.obj(os.path.splitext(src)[0]))

class Target:
	def __init__(self, mode, name, path):
		self.mode = mode
		self.name = name
		self.path = path

	async def build(self, config):
		objs = []
		for dep in self.deps:
			objs.append(shared_path(dep, config))
		for src in self.srcs:
			obj = obj_path(self, src, config)
			objs.append(obj)
			src = src_path(self, src)
			os.makedirs(os.path.dirname(obj), exist_ok=True)
			if src.endswith(".c"):
				await config.comp_c(obj, src, self.incs, self.defs)
			elif src.endswith(".cc"):
				await config.comp_cxx(obj, src, self.incs, self.defs)
		if self.mode == "shared":
			await config.shared(shared_path(self, config), objs, self.libs)
		else:
			await config.program(program_path(self, config), objs, self.libs)

arg = sys.argv[1]
if arg == "gnu":
	config = GnuConfig
	platforms = ["posix", "xlib"]
if arg == "clang":
	config = ClangConfig
	platforms = ["posix", "xlib"]
if arg == "mingw":
	config = MingwConfig
	platforms = ["win32"]
if arg == "macos":
	config = MacosConfig
	platforms = ["posix", "macos"]

liengine = Target("shared", "liengine", "Lightning-Engine")
liengine.libs = []
liengine.srcs = ["li_assert.c", "li_win.c"]
liengine.deps = []
liengine.incs = ["Lightning-Engine/include"]
liengine.defs = []

if "posix" in platforms:
	liengine.libs.extend(["dl"])
	liengine.srcs.extend(["posix_dl.c"])
	liengine.defs.extend([])
if "win32" in platforms:
	liengine.libs.extend([])
	liengine.srcs.extend(["win32_dl.c"])
	liengine.defs.extend([])

liengine_win_xlib = Target("shared", "liengine_win_xlib", "Lightning-Engine")
liengine_win_xlib.libs = ["X11", "GL"]
liengine_win_xlib.srcs = ["win/xlib_win.c", "win/xlib_keymap.c"]
liengine_win_xlib.deps = [liengine]
liengine_win_xlib.incs = ["Lightning-Engine/include"]
liengine_win_xlib.defs = []

liengine_win_win32 = Target("shared", "liengine_win_win32", "Lightning-Engine")
liengine_win_win32.libs = ["gdi32", "opengl32"]
liengine_win_win32.srcs = ["win/win32_win.c", "win/win32_keymap.c"]
liengine_win_win32.deps = [liengine]
liengine_win_win32.incs = ["Lightning-Engine/include"]
liengine_win_win32.defs = []

liengine_win_macos = Target("shared", "liengine_win_macos", "Lightning-Engine")
liengine_win_macos.libs = ["Cocoa"]
liengine_win_macos.srcs = ["win/macos_win.c", "win/macos_keymap.c"]
liengine_win_macos.deps = [liengine]
liengine_win_macos.incs = ["Lightning-Engine/include"]
liengine_win_macos.defs = []

liengine_libs = []
if "xlib" in platforms:
	liengine_libs.append(liengine_win_xlib)
if "win32" in platforms:
	liengine_libs.append(liengine_win_win32)
if "macos" in platforms:
	liengine_libs.append(liengine_win_macos)

sandbox = Target("program", "sandbox", "Sandbox")
sandbox.libs = []
sandbox.srcs = ["main.c"]
sandbox.deps = [liengine]
sandbox.incs = ["Lightning-Engine/include"]
sandbox.defs = []

if "posix" in platforms:
	sandbox.libs.extend(["GL"])
if "win32" in platforms:
	sandbox.libs.extend(["opengl32"])

async def main():
	shutil.rmtree("bin")
	shutil.rmtree("obj")
	os.makedirs("bin", exist_ok=True)
	await liengine.build(config)
	for lib in liengine_libs:
		await lib.build(config)
	await sandbox.build(config)

if __name__ == "__main__":
	asyncio.run(main())