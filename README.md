# Compiler | mrcceppc
mrcceppc is a reimplementation project for the Metrowerks `mwcceppc` compiler.

# Compiling
Run `generate_{version}.bat` for which version of the GameCube CodeWarrior SDK you want to use or specify the `--{version}` argument when running premake.
### Available Versions
#### Version | SDK Version | Argument
| Version 2.3.3 b144 | GC CW 1.0 | `--v10` |
|--------------------|-----------|-----|
| Version 2.4.7 b108 | GC CW 2.7 | `--v27` |

By default, the premake script is setup to copy the build of the version you are working on to the `ref/{version}/` directory for quick testing.
For extensive testing, we recommend using one of the projects over at https://github.com/doldecomp.

# Codebase Information
### Comment Legend
- `//[/!\]\\` : wrong or garunteed to fail code; important to rework
