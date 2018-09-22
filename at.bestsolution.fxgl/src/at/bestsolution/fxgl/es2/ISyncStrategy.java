package at.bestsolution.fxgl.es2;

import java.util.function.BiFunction;

import at.bestsolution.fxgl.es2.GLSurface.FrameReport;
import at.bestsolution.fxgl.es2.NGGLSurface.Tex;

public interface ISyncStrategy {
	Tex GetNextTex(int w, int h, BiFunction<Integer, Integer, Tex> createTex, FrameReport report);
	void SwapTex(FrameReport report);
	void beforeRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex);
	void afterRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex);
	
	Tex getRenderTex();
	
	
	default boolean checkSize(int w, int h, Tex tex) {
		return tex.w == w && tex.h == h;
	}
}
