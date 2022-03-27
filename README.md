# Lightning Engine

The Lightning Engine or "Lightning" as we call it is a work in progress, portable game engine
that uses no external libraries other than the graphics and window libraries of the systems it's
build for

---

## Getting Started
<ins>**1) Get the source code**</ins>

Clone the repository: `git clone https://github.com/Lightning-Engine/Lightning-Engine.git`

<ins>**2) Compile the engine**</ins>

If you're just interested in using the engine run: `make platform={linux,mingw,macos}`

If you're interested in contributing to the engine or just making your own changes to it, you can build it with the following options

`make platform={linux,mingw,macos} [config={debug,release,distr}] [san={none,basic,thread}]`

the **san** option only works in the debug config