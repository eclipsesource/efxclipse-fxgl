# fxgl readme

## Linux Version

### Project Structure

Every java project has a corresponding native code project in `native/${project}.native`. Those projects are at the moment managed cdt projects. Their build output is a shared object which is copied into the native folder of the corresponding java project.

* **org.eclipse.efxclipse.fxgl**  
  This is the main bundle. It contains the javafx GLSurface Node.
* **org.eclipse.efxclipse.fxgl.sample.glcommon**  
 Contains some native helpers for the sample
* **org.eclipse.efxclipse.fxgl.sample.renderer**  
 Basic sample renderer infrastructure
* **org.eclipse.efxclipse.fxgl.sample.renderer.objrenderer**  
 A simple OBJ Renderer implementation
* **org.eclipse.efxclipse.fxgl.sample.renderer.simple**  
 A very simple colored triangle renderer
* **org.eclipse.efxclipse.fxgl.sample.application**  
 The javafx examples

### General Requirements

Oracle Java 8 JDK (this is a strict requirement, only Java 8 will work!)


### Sample Requirements

 * libGLX.so.0
 * libX11.so.6
 * libGLEW.so.2.0
 * libGL.so.1

### Java Native Library behavior

For now the java code extracts the native libs it bundles into the current working directory to allow them to link together.  
The samples also extract their resources (shaders, images, obj..) there.  

### Known Limitations / issues

 * The checked in `libfxgl_sample_glcommon.so` is linked against `libGLEW.so.2.0`  
 On some system only libGLEW.so.2.2 is available which causes the application to fail  
 Workaround: add a symlink in your workingdir to `libGLEW.so.2.2` and name it `libGLEW.so.2.0`

 * The renderer sample does not work with Intel Cards  
 When starting a renderer on a intel card the application dies with a message similar to `i965: Failed to submit batchbuffer: No such file or directory`


### Basic Usage

#### Java Side

Create a JavaFX Application and add the **org.eclipse.efxclipse.fxgl** bridge jar to the project.
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


