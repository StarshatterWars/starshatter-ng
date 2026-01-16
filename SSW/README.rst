Starshatter: The Open Source Project
====================================
Starshatter is a military space combat simulator set in a distant future.


Where to start
^^^^^^^^^^^^^^
Starshatter is a game. Start by enjoying it for a bit. Prebuilt game is available on itch.io_. Download the full game
package, unpack it, and run *Starshatter.exe*. There is a tutorial available along with storyline dynamic campaigns, and
one-off missions.


Contributing
^^^^^^^^^^^^
You may contribute in various ways: writing and reviewing documents and lore, suggesting game balance changes, creating
new assets, coming up with ship names, and finally writing code. Whatever you want to do, start by saying hi on
discord_.


Building
^^^^^^^^
Build systems are configured with CMake. Distributed prebuilt packages are built with MinGW and Makefiles, so you can
expect them to work. To run the game, install it after building with ``--install`` option. Running in build tree is not
supported at the moment.


MinGW
-----
You will need 32-bit MinGW compiler. Run CMake with ``--toolchain`` option or ``CMAKE_TOOLCHAIN_FILE`` variable to point
at included *cmake/toolchains/i686-w64-mingw32.cmake* or another appropriate toolchain definition. After initial
generation you can follow regular workflow.


MSVC
----
You will need Windows SDK and old stand-alone DirectX SDK. Configure ``WINDOWSSDK_LIBPATH`` and ``WINDOWSSDK_PATH``
CMake variables and ``DXSDK_DIR`` environment variable to point at their respective targets.


License
^^^^^^^
See *COPYING* and *NOTICE* files for licensing details.


.. _itch.io: https://nthirtyone.itch.io/starshatter-open
.. _discord: https://discord.com/invite/6GFacfuyBP
