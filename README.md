# fxgl readme

## Linux Version

### Project Structure

Every java project has a corresponding native code project in `native/${project}.native`. Those projects are at the moment managed cdt projects. Their build output is a shared object which is copied into the native folder of the corresponding java project.

* at.bestsolution.fxgl
 This is the main bundle. It contains the javafx GLSurface Node.
* at.bestsolution.fxgl.glcommon
 Contains some native helpers for the sample
* at.bestsolution.fxgl.sample.renderer
 Basic sample renderer infrastructure
* at.bestsolution.fxgl.sample.renderer.objrenderer
 A simple OBJ Renderer implementation
* at.bestsolution.fxgl.sample.renderer.simple
 A very simple colored triangle renderer
* at.bestsolution.fxgl.sample.application
 The javafx examples
