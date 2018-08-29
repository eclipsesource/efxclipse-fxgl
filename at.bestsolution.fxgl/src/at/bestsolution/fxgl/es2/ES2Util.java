package at.bestsolution.fxgl.es2;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

import com.sun.prism.Texture;

public class ES2Util {

	
	public static long getContextHandle(Texture texture) throws Exception {
		Class<?> es2TextureClass = Class.forName("com.sun.prism.es2.ES2Texture");
		Field es2ContextField = texture.getClass().getDeclaredField("context");
		es2ContextField.setAccessible(true);
		Object es2Context = es2ContextField.get(texture);
		
		Method glContextGetter = es2Context.getClass().getDeclaredMethod("getGLContext");
		glContextGetter.setAccessible(true);
		Object glContext = glContextGetter.invoke(es2Context);
		
		Method nativeHandleGetter = glContext.getClass().getDeclaredMethod("getNativeHandle");
		nativeHandleGetter.setAccessible(true);
		long nativeHandle = (long) nativeHandleGetter.invoke(glContext);
		
		return nativeHandle;
	}
	
	public static long getTextureId(Texture texture) throws Exception {
		Method nativeSourceHandleGetter = texture.getClass().getMethod("getNativeSourceHandle");
		nativeSourceHandleGetter.setAccessible(true);
		long texId  = (int) nativeSourceHandleGetter.invoke(texture);
		return texId;
		
	}
}
