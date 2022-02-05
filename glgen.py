#!/usr/bin/env python3
import urllib.request
import xml.etree.ElementTree

class Type:
	def __init__(self, type):
		self.text = "".join(type.itertext())
		self.name = type.get("name") or type.find("name").text

	def gen_h(self):
		yield self.text
		yield "\n"

class Enum:
	def __init__(self, enum):
		self.name = enum.get("name")
		self.value = enum.get("value")
		self.group = enum.get("group")

	def gen_h(self):
		yield "#define "
		yield self.name
		yield " "
		yield self.value
		yield "\n"

class Variable:
	def __init__(self, element):
		self.type = element.text or ""
		for child in element:
			if child.tag == "name":
				self.name = "".join(child.itertext())
			else:
				self.type += "".join(child.itertext())
			if child.tail is not None:
				self.type += child.tail

class Command(Variable):
	def __init__(self, command):
		Variable.__init__(self, command.find("proto"))
		self.params = [Variable(param) for param in command.findall("param")]

	def gen_h(self):
		yield "\t"
		yield self.type
		yield "(*"
		if self.name.startswith("gl"):
			yield self.name[2:]
		else:
			yield self.name
		yield ")("
		for param in self.params:
			yield param.type
			yield param.name
			if self.params[-1] is not param:
				yield ","
		yield ");\n"

	def gen_c(self):
		yield "\tptr = li_ctx_get_proc_addr(\""
		yield self.name
		yield "\");\n"
		yield "\tgl->"
		if self.name.startswith("gl"):
			yield self.name[2:]
		else:
			yield self.name
		yield " = *("
		yield self.type
		yield "(**)("
		for param in self.params:
			yield param.type
			if self.params[-1] is not param:
				yield ","
		yield ")) &ptr;\n"

class Require:
	def __init__(self, element):
		self.types = [type.get("name") for type in element.findall("require/type")]
		self.enums = [enum.get("name") for enum in element.findall("require/enum")]
		self.commands = [command.get("name") for command in element.findall("require/command")]

class Feature(Require):
	def __init__(self, feature):
		Require.__init__(self, feature)
		self.api = feature.get("api")

class Extension(Require):
	def __init__(self, extension):
		Require.__init__(self, extension)
		self.name = extension.get("name")

class Registry:
	def __init__(self, registry):
		self.types = [Type(type) for type in registry.findall("types/type")]
		self.types = { type.name: type for type in self.types }
		self.enums = [Enum(enum) for enum in registry.findall("enums/enum")]
		self.enums = { enum.name: enum for enum in self.enums }
		self.commands = [Command(command) for command in registry.findall("commands/command")]
		self.commands = { command.name: command for command in self.commands }
		self.features = [Feature(feature) for feature in registry.findall("feature")]
		self.features = { feature.api: feature for feature in self.features }
		self.extensions = [Extension(extension) for extension in registry.findall("extensions/extension")]
		self.extensions = { extension.name: extension for extension in self.extensions }

	def gen_h(self):
		yield "#ifndef LI_GL_H\n"
		yield "#define LI_GL_H\n"
		for type in self.types.values():
			yield from type.gen_h()
		for enum in self.enums.values():
			yield from enum.gen_h()
		yield "typedef struct li_gl {\n"
		for command in self.commands.values():
			yield from command.gen_h()
		yield "} li_gl_t;\n"
		yield "void li_gl_init(li_gl_t *gl);\n"
		yield "#endif\n"

	def gen_c(self):
		yield "#include \"li/gl.h\"\n"
		yield "#include \"li/win.h\"\n"
		yield "void li_gl_init(li_gl_t *gl) {\n"
		yield "\tvoid *ptr;\n"
		for command in self.commands.values():
			yield from command.gen_c()
		yield "}\n"

with urllib.request.urlopen("https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/main/xml/gl.xml") as gl:
	spec = xml.etree.ElementTree.parse(gl)
registry = Registry(spec.getroot())
text = "".join(registry.gen_h())
with open("Lightning-Engine/include/li/gl.h", "w") as f:
	f.write(text)
text = "".join(registry.gen_c())
with open("Lightning-Engine/src/li_gl.c", "w") as f:
	f.write(text)