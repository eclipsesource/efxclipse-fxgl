package at.bestsolution.fxgl;

import java.lang.reflect.Method;

import com.sun.prism.Texture;

public class DXUtils {

	public static native long noparam();
	public static native long texparam(Texture texture);
	
	private static native long nGetAdapterLUID();

	private static native long nRecreateTexture(Texture texture);

	public static long recreateTexture(Texture texture) {
		return nRecreateTexture(texture);
	}
	
	public static long getAdapterLUID() {
		return nGetAdapterLUID();
	}
	
	public static long getTextureHandle(Texture texture) {
		try {
			Class<?> cls = Class.forName("com.sun.prism.d3d.D3DTexture");
			Method method = cls.getMethod("getNativeSourceHandle");
			method.setAccessible(true);
			Object result = method.invoke(texture);
			return (long) result;
		}
		catch (Exception e) {
			e.printStackTrace();
			return 0;
		}
	}
}
