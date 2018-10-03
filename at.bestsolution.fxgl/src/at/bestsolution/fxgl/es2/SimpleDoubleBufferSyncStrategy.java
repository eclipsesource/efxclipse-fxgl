package at.bestsolution.fxgl.es2;

import java.util.concurrent.atomic.AtomicReference;
import java.util.function.BiFunction;

import at.bestsolution.fxgl.es2.NGGLSurface.Tex;

public class SimpleDoubleBufferSyncStrategy implements ISyncStrategy {

	private class DoubleBuffer {
		Tex foreground;
		Tex background;
	}
	
	private AtomicReference<DoubleBuffer> buffer = new AtomicReference<>();
	
	@Override
	public Tex GetNextTex(int w, int h, BiFunction<Integer, Integer, Tex> createTex) {
		return buffer.get().background;
	}

	@Override
	public void SwapTex() {
		buffer.updateAndGet(buf -> {
			DoubleBuffer newBuf = new DoubleBuffer();
			newBuf.foreground = buf.background;
			newBuf.background = buf.foreground;
			return newBuf;
		});
	}

	@Override
	public void beforeRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex) {
		// init
		buffer.updateAndGet(buf -> {
			if (buf == null) {
				DoubleBuffer newBuf = new DoubleBuffer();
				newBuf.foreground = createTex.apply(w, h);
				newBuf.background = createTex.apply(w, h);
				return newBuf;
			}
			return buf;
		});
	}
	@Override
	public void afterRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex) {
		buffer.updateAndGet(buf -> {
			DoubleBuffer newBuf = new DoubleBuffer();
			// keep foreground
			newBuf.foreground = buf.foreground;
			
			// prepare background
			newBuf.background = checkSize(w, h, buf.background) ? buf.background : createTex.apply(w, h);
			return newBuf;
		});
	}

	@Override
	public Tex getRenderTex() {
		return buffer.get().foreground;
	}

}
