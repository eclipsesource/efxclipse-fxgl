package at.bestsolution.fxgl.es2;

import java.lang.reflect.Method;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.function.BiFunction;
import java.util.function.Supplier;

import at.bestsolution.fxgl.es2.GLSurface.FrameReport;
import at.bestsolution.fxgl.es2.NGGLSurface.Tex;

public class QuantumRendererSyncStrategy extends AQuantumSyncStrategy implements ISyncStrategy {

	private Tex renderTex;
	private Tex nextTex;
	
	@Override
	public Tex GetNextTex(int w, int h, BiFunction<Integer, Integer, Tex> createTex, FrameReport frameReport) {
		return supplyQuantumRenderer(() -> {
			frameReport.getNextTextureQuantum = - System.nanoTime();
			if (nextTex != null) {
				if (nextTex.fxTex.getContentWidth() != w || nextTex.fxTex.getContentHeight() != h) {
					nextTex.fxTex.dispose();
					nextTex = null;
				}
			}
				
			if (nextTex == null) {
				nextTex = createTex.apply(w, h);
			}
			frameReport.getNextTextureQuantum += System.nanoTime();
			return nextTex;
			
		});
	}

	@Override
	public void SwapTex(FrameReport frameReport) {
		runQuantumRenderer(() -> {
			frameReport.swapTextureQuantum = - System.nanoTime();
			Tex oldTex = renderTex;
			renderTex = nextTex;
			nextTex = oldTex;
			frameReport.swapTextureQuantum += System.nanoTime();
		});
	}

	@Override
	public void beforeRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex) {
		// init
		if (renderTex == null) {
			renderTex = createTex.apply(w, h);
		}
		if (nextTex == null) {
			nextTex = createTex.apply(w, h);
		}
	}
	@Override
	public void afterRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex) {
	
	}
	@Override
	public Tex getRenderTex() {
		return renderTex;
	}
}
