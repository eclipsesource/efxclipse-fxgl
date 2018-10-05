# fxgl readme

## Linux Version

### Project Structure

Every java project has a corresponding native code project in `native/${project}.native`. Those projects are at the moment managed cdt projects. Their build output is a shared object which is copied into the native folder of the corresponding java project.

* **at.bestsolution.fxgl**  
  This is the main bundle. It contains the javafx GLSurface Node.
* **at.bestsolution.fxgl.glcommon**  
 Contains some native helpers for the sample
* **at.bestsolution.fxgl.sample.renderer**  
 Basic sample renderer infrastructure
* **at.bestsolution.fxgl.sample.renderer.objrenderer**  
 A simple OBJ Renderer implementation
* **at.bestsolution.fxgl.sample.renderer.simple**  
 A very simple colored triangle renderer
* **at.bestsolution.fxgl.sample.application**  
 The javafx examples

### Requirements

Oracle Java 8 JDK

### Basic Usage

#### Java Side

Create a JavaFX Application and add the **at.bestsolution.fxgl** bridge jar to the project.
Then you can add a `GLSurface` node to your scene graph.

Also you need to create an JNI entry point to start your own native part.
Here you need to pass the surfaceId of your surface to the native world.

You can obtain it by calling `GLSurface#getSurfaceId()`.

for example a simple class like
```java
package somewhere;

public class NativeLauncher {
    public static native startNativeSuff(long surfaceId);
}
```


#### Native Side

here you need the corresponding JNI function, where you can obtain the native `GLSurface` API object.

```c++
extern "C" JNIEXPORT void JNICALL Java_somewhere_NativeLauncher_startNativeSuff(JNIEnv *env, jclass cls, jlong surfaceId)
{
    GLSurface* mySurface = GLSurface::GetSurface((long) surfaceId);
}
```

With this you are ready to go:


```c++
    GLSurface* mySurface = GLSurface::GetSurface((long) surfaceId);
    
    // launch your own render infrastructure or just a simple render thread
    
    // First obtain the sharedContextHandle
    long openGLContextHandle = mySurface->GetContextHandle();
    
    // create your opengl context with the shared handle however you prefer
    
    // now for rendering you can obtain a surface texture by calling
    Texture nextTexture = mySurface->GetNextTexture();
    
    // set nextTexture.textureId as color attachment of your rendertarget
    // if you need a depth buffer ensure that it has the right size by
    //  checking nextTexture.width and nextTexture.height; maybe recreate it
    
    // render your frame
    
    // call SwapTexture
    mySurface->SwapTexture();
    
    
    // before you render thread terminates you should detach it from the jvm
    // (most of the GLSurface methods autmatically attach your thread to the
    //  jvm, but you can also manually do this by calling AttachThread())
    GLSurface::ReleaseThread();
    

```


#### Notes on the linker

At the moment the bridge jar (`at.bestsolution.fxgl.jar`) extracts its native code (`libfxgl.so`) to the current working directory. Maybe you need to put it also next to your shared objects so they can link against it.

