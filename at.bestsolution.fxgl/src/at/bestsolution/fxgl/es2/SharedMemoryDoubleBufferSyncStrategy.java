package at.bestsolution.fxgl.es2;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicReference;
import java.util.function.BiFunction;

import at.bestsolution.fxgl.es2.GLSurface.FrameReport;
import at.bestsolution.fxgl.es2.NGGLSurface.Tex;
import at.bestsolution.fxgl.internal.SharedMemory;

public class SharedMemoryDoubleBufferSyncStrategy extends AQuantumSyncStrategy {

	Map<Integer, Tex> textures = new HashMap<>(); 
	
	private class DoubleBuffer {
		Tex foreground;
		Tex background;
	}
	
	private AtomicReference<DoubleBuffer> buffer = new AtomicReference<>();
	
	private GLSurface.Texture toTexture(Tex tex) {
		GLSurface.Texture texture = new GLSurface.Texture();
		texture.textureId = tex.textureId;
		texture.width = tex.w;
		texture.height = tex.h;
		return texture;
	}
	
	@Override
	public Tex GetNextTex(int w, int h, BiFunction<Integer, Integer, Tex> createTex, FrameReport report) {
		return buffer.get().background;
	}

	@Override
	public void SwapTex(FrameReport report) {
		buffer.updateAndGet(buf -> {
			DoubleBuffer newBuf = new DoubleBuffer();
			newBuf.foreground = buf.background;
			newBuf.background = buf.foreground;
			return newBuf;
		});
		DoubleBuffer b = buffer.get();
//		SharedMemory.setForegroundTexture(toTexture(b.foreground));
//		SharedMemory.setBackgroundTexture(toTexture(b.background));
		
		SharedMemory.setNextTexture(toTexture(b.background));
		//SharedMemory.nOutput();
	}
	
	private Tex doCreateTex(int w, int h, BiFunction<Integer, Integer, Tex> createTex) {
		Tex tex = createTex.apply(w, h);
		textures.put(tex.textureId, tex);
		return tex;
	}

	@Override
	public void beforeRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex) {
		// init
		buffer.updateAndGet(buf -> {
			if (buf == null) {
				DoubleBuffer newBuf = new DoubleBuffer();
				newBuf.foreground = doCreateTex(w, h, createTex);
				newBuf.background = doCreateTex(w, h, createTex);
				return newBuf;
			}
			return buf;
		});
	}
	@Override
	public void afterRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex) {
		
		buffer.updateAndGet(buf -> {
			DoubleBuffer newBuf = new DoubleBuffer();
			newBuf.foreground = buf.foreground;
			
			if (checkSize(w, h, buf.background)) {
				newBuf.background = buf.background;
			}
			else {
				System.err.println("recreating background tex");
				newBuf.background = createTex.apply(w, h);
			}
			
			newBuf.background = checkSize(w, h, buf.background) ? buf.background : doCreateTex(w, h, createTex);
			return newBuf;
		});
		
		DoubleBuffer b = buffer.get();
		//SharedMemory.setForegroundTexture(toTexture(b.foreground));
		//SharedMemory.setBackgroundTexture(toTexture(b.background));
		
		SharedMemory.setNextTexture(toTexture(b.background));
		//SharedMemory.nOutput();
	}

	@Override
	public Tex getRenderTex() {
		return textures.get(SharedMemory.getRenderTextureId());
		
		//return buffer.get().foreground;
	}

}
