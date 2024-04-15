# **Sphynx Framework**
This is the Core of the Sphynx Engine, It exposes interfaces and implements all the core fonctionality for the Game Engine (Graphics, Input, Scripting with mono, and in the future Audio, Asset Pipeline)
## **Graphics**
## **Scripting**
The Framework currently uses mono to support scripting.

The ScriptAssembly project is the interface between Managed and Core. It's exposes functionality as Internal Calls (Check [Embedded Mono](https://www.mono-project.com/docs/advanced/embedding/) for details)

Right now, on the core side it's kind of ugly and may be refactored but the Managed side will stay same.
There is also plans to interop with the component system of Core after i finalize the design.