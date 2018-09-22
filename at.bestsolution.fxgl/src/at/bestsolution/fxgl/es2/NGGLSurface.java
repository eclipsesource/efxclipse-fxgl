package at.bestsolution.fxgl.es2;

import java.lang.reflect.Method;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.function.Supplier;

import com.sun.javafx.font.FontStrike;
import com.sun.javafx.scene.text.GlyphList;
import com.sun.javafx.sg.prism.NGGroup;
import com.sun.javafx.sg.prism.NGNode;
import com.sun.javafx.text.TextRun;
import com.sun.prism.Graphics;
import com.sun.prism.PixelFormat;
import com.sun.prism.RTTexture;
import com.sun.prism.ResourceFactory;
import com.sun.prism.Texture;
import com.sun.prism.paint.Color;

import at.bestsolution.fxgl.es2.GLSurface.FrameReport;
import at.bestsolution.fxgl.internal.GLSurfaceAPI;
import at.bestsolution.fxgl.internal.SharedMemory;

public class NGGLSurface extends NGNode {

//	private ISyncStrategy syncStrategy = new QuantumRendererSyncStrategy();
	private ISyncStrategy syncStrategy = new SimpleDoubleBufferSyncStrategy();
//	private ISyncStrategy syncStrategy = new SharedMemoryDoubleBufferSyncStrategy();
	
	private ResourceFactory resourceFactory;
	
	class Tex {
		Texture fxTex;
		int textureId;
		int w;
		int h;
	}
	
	private Tex renderTex;
	private Tex nextTex;
	
	
	private GLSurface node;
	
	private long contextHandle;
	
	public NGGLSurface(GLSurface node) {
		this.node = node;
		try {
			resourceFactory = ES2Util.getResourceFactory();
			contextHandle = ES2Util.getContextHandle(resourceFactory);
		}
		catch (Exception e) {
			System.err.println("Error accessing es2 pipeline!");
			e.printStackTrace();
		}
		
		
	}
	
	
	private Tex createTex(int width, int height) {
//		System.err.println("Java createTex(" + width + ", " + height + ") @ " + Thread.currentThread().getName());
		Tex tex = new Tex();
		tex.w = width;
		tex.h = height;
		tex.fxTex = resourceFactory.createTexture(PixelFormat.BYTE_RGB, Texture.Usage.DYNAMIC, Texture.WrapMode.CLAMP_NOT_NEEDED, width, height);
		if (tex.fxTex == null) {
			System.err.println("WE FAILED BIG TIME!!");
		}
		tex.fxTex.makePermanent();
		try {
			tex.textureId = ES2Util.getTextureId(tex.fxTex);
//			System.err.println("Java  -> " + tex.textureId);
		}
		catch (Exception e) {
			System.err.println("Failed to get texture id!");
			e.printStackTrace();
		}
		return tex;
	}
	
	private int getTexWidth() {
		return Math.max(1, (int) Math.ceil(tw));
	}
	private int getTexHeight() {
		return Math.max(1, (int) Math.ceil(th));
	}

	public Tex GetNextTex(FrameReport frameReport) {
		return syncStrategy.GetNextTex(getTexWidth(), getTexHeight(), this::createTex, frameReport);
	}
	
	public void SwapTex(FrameReport frameReport) {
		syncStrategy.SwapTex(frameReport);
	}
	
	@Override
	protected void renderContent(Graphics g) {
		int w = Math.max(1, (int) Math.ceil(tw));
		int h = Math.max(1, (int) Math.ceil(th));
		syncStrategy.beforeRenderContent(w, h, this::createTex);
		
//		g.setPaint(Color.GREEN);
//		g.drawRect(1, 1, w-2, h-2);
		
		Tex renderTex = syncStrategy.getRenderTex();
		if (renderTex != null) {
			//System.err.println(g.getRenderTarget());
			//System.err.println("RENDERING " + renderTex.textureId);
			//g.blit(srcTex, dstTex, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1);
			
			g.drawTexture(renderTex.fxTex, 0, 0, renderTex.fxTex.getContentWidth(), renderTex.fxTex.getContentHeight());
		}
		
		syncStrategy.afterRenderContent(w, h, this::createTex);
	}
	
	private float tw, th;
	
	public void setSize(float width, float height) {
		if (tw != width || th != height) {
			this.tw = width;
			this.th = height;
			
			GLSurfaceAPI.fireSizeChanged(node.getSurfaceId());
		}
	}
	
	public void setWidth(float width) {
		if (tw != width) {
			tw = width;
		}
	}
	public void setHeight(float height) {
		if (th != height) {
			th = height;
		}
	}
	
	@Override
	protected boolean hasOverlappingContents() {
		return false;
	}

	public long getContextHandle() {
		return contextHandle;
	}
	
}
