# ciri
> ciri is a perpetually developed collection of toolsets unified into a single coherent framework. ciri attempts to abstract complexity where possible, leaving a simplified, yet sufficient interface.  ciri started out as a graphics abstraction layer for DirectX 11 and OpenGL 3.3+; features are added and removed as seen fit. Feel free to browse the source code to see what ciri is currently capable of.
ciri is an aspiring framework in perpetual development intended for use in personal projects.  It is being created with the intention of replacing one-time frameworks for my own projects with an all-around generic one instead.

## Should I use it?
Probably not. For learning, sure, but it’s far from a golden standard; it’s for personal use.

## Status
ciri is no longer being developed.

## Examples
### Dynamic Vertex Buffers
Dynamic vertex buffers were tested with a simple CPU cloth simulation based on [OpenCloth's](https://github.com/mmmovania/opencloth) verlet implementation. This sample tests dynamic vertex buffers, contains multiple shaders, multiple meshes, a Maya-like camera system, and so on. The texture for the cloth is procedurally generated using an adaption of Marco Fratarcangeli’s algorithm found in [Game Engine Gems 2](http://www.gameenginegems.net/geg2.php). The source code to this sample can be found in the code repository as dynvb.

![Dynamic Vertex Buffers](https://raw.githubusercontent.com/KasumiL5x/ciri/master/screenshots/ciri-dynvb.png)

### Terrain and Water Rendering
This demo shows a water plane dynamically reflecting and refracting its surrounding smoothed heightmap terrain and projected cubemap. The intention was to test rendering to textures and cubemaps for skyboxes, reflections, and so on. The water and terrain are based loosely on [Riemer's](http://riemers.net/) XNA tutorials. The cubemapped skybox is actually a single quad using projected coordinates, as is explained clearly by user msell on [StackExchange](http://gamedev.stackexchange.com/questions/60313/implementing-a-skybox-with-glsl-version-330/60377#60377).

![Terrain and Water Rendering](https://raw.githubusercontent.com/KasumiL5x/ciri/master/screenshots/ciri-terrain.jpg)

### Sprite Batching
Batching of sprites using an [XNA-like model](https://msdn.microsoft.com/en-us/library/Microsoft.Xna.Framework.Graphics.SpriteBatch.aspx). A miniature game somewhat representing a simplified [Geometry Wars](https://en.wikipedia.org/wiki/Geometry_Wars) with smooth player movement, firing towards the cursor, multiple kinds of enemy behavior, and best of all, a fully interactive and dynamic grid simulation that reacts to game objects.

![Sprite Batching](https://raw.githubusercontent.com/KasumiL5x/ciri/master/screenshots/ciri-sprites.png)

### Reflection, Refraction, and Chromatic Aberration
An interesting effect is reflection and refraction of a cubemap. Objects fit much better into a scene when they appear to incorporate the surrounding scene into their composition. It is possible to also refract each channel – red, green, and blue – using different indices of refraction to given the effect that each wavelength is being scattered around differently, which results in a nice overlapping rainbow effect.

![Reflection, Refraction, and Chromatic Aberration](https://raw.githubusercontent.com/KasumiL5x/ciri/master/screenshots/ciri-refract.jpg)

### Steep Parallax Occlusion Mapping and Shadowing
Normal maps are good for displaying micro-facets within a surface. Convincing as they can be, viewing angles often spoil their illusion. A different effect is parallax occlusion mapping. This technique is still faking detail, as are normal maps, but appears drastically more three dimensional than normal mapping alone. Viewing angles are seldom an issue with parallax techniques. Below shows a simple scene with steep parallax mapping and shadowing implemented. The two red triangles show the real geometry. Everything you see is a visual trick; no actual geometry beyond a plane is used.

![Steep Parallax Occlusion Mapping and Shadowing](https://raw.githubusercontent.com/KasumiL5x/ciri/master/screenshots/ciri-parallax.jpg)

