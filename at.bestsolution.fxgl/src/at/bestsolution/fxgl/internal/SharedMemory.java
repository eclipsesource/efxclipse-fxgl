package at.bestsolution.fxgl.internal;

import java.lang.reflect.Field;
import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import at.bestsolution.fxgl.es2.GLSurface.Texture;
import sun.misc.Unsafe;

public class SharedMemory {

	private static int convert(int num) {
		return ((num>>24)&0xff) | // move byte 3 to byte 0
		       ((num<<8)&0xff0000) | // move byte 1 to byte 2
		       ((num>>8)&0xff00) | // move byte 2 to byte 1
		       ((num<<24)&0xff000000); // byte 0 to byte 3
	}
	public static Unsafe getUnsafe() {
		try {
		Field f = Unsafe.class.getDeclaredField("theUnsafe");
		f.setAccessible(true);
		return (Unsafe) f.get(null);
		}
		catch (Exception e) {
			e.printStackTrace();
			return null;
		}
	}

	static Unsafe unsafe = getUnsafe();
	
	static void putTexture(int texIndex, Texture tex) {
		data.put(3 * texIndex, convert((int) tex.textureId));
		data.put(3 * texIndex + 1, convert(tex.width));
		data.put(3 * texIndex + 2, convert(tex.height));
	}
	
	public static void setForegroundTexture(Texture tex) {
		putTexture(freeTextureIndex, tex);
		freeTextureIndex = unsafe.getAndSetInt(null, foregroundAddress, freeTextureIndex);
//		System.err.println("freeTextureIndex is now " + freeTextureIndex);
	}
	
	public static void setBackgroundTexture(Texture tex) {
		putTexture(freeTextureIndex, tex);
		freeTextureIndex = unsafe.getAndSetInt(null, backgroundAddress, freeTextureIndex);
	}
	
	public static void setNextTexture(Texture tex) {
		putTexture(freeTextureIndex, tex);
		freeTextureIndex = unsafe.getAndSetInt(null, nextTextureAddress, freeTextureIndex);
	}
	
	public static int getRenderTextureId() {
		return unsafe.getIntVolatile(null, renderTextureAddress);
	}
	
	static int freeTextureIndex = 0;
	static IntBuffer data;
	static long foregroundAddress = 0;
	static long backgroundAddress = 0;
	static long nextTextureAddress = 0;
	static long renderTextureAddress = 0;
	
	static native long nGetForegroundAddress();
	static native long nGetBackgroundAddress();
	static native long nGetNextTextureAddress();
	static native long nGetRenderTextureAddress();
	
	static {
		System.loadLibrary("FXGL");
		
		data = nGetSharedBuffer(0).asIntBuffer();
		foregroundAddress = nGetForegroundAddress();
		backgroundAddress = nGetBackgroundAddress();
		nextTextureAddress = nGetNextTextureAddress();
		renderTextureAddress = nGetRenderTextureAddress();
	}
	
	
	public static native ByteBuffer nGetSharedBuffer(long surfaceId);
	
	public static ByteBuffer getSharedBuffer(long surfaceId) {
		return nGetSharedBuffer(surfaceId);
	}
	
	public static native void nOutput();
	
	private static void jOutput(IntBuffer intBuf) {
		intBuf.rewind();
		System.err.println("Data (Java)");
		int h = intBuf.get();
		System.err.println(h + ", 0x" + Integer.toHexString(h));
		int i = intBuf.get();
		System.err.println(i + ", 0x" + Integer.toHexString(i));
	}
	
	public static void main(String[] args) {
		nOutput();
		
		Texture t1 = new Texture();
		t1.textureId = 1;
		t1.width = 100;
		t1.height = 101;
		setForegroundTexture(t1);
		
		nOutput();
		
		t1.width = 300;
		setForegroundTexture(t1);
		
		nOutput();
	}
	
}
