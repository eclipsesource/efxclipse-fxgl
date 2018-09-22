package at.bestsolution.fxgl.es2;

import com.sun.javafx.geom.BaseBounds;
import com.sun.javafx.geom.RectBounds;
import com.sun.javafx.geom.transform.BaseTransform;
import com.sun.javafx.jmx.MXNodeAlgorithm;
import com.sun.javafx.jmx.MXNodeAlgorithmContext;
import com.sun.javafx.scene.DirtyBits;
import com.sun.javafx.sg.prism.NGNode;

import at.bestsolution.fxgl.es2.NGGLSurface.Tex;
import at.bestsolution.fxgl.internal.GLSurfaceAPI;
import javafx.application.Platform;
import javafx.beans.property.ReadOnlyDoubleProperty;
import javafx.beans.property.ReadOnlyDoubleWrapper;
import javafx.scene.Node;
import javafx.scene.input.MouseEvent;
import javafx.scene.input.ScrollEvent;

public class GLSurface extends Node {

	private long surfaceId;
	
	public GLSurface() {
		setMouseTransparent(false);
		
		surfaceId = GLSurfaceAPI.createGLSurface(this);
		
		addEventHandler(MouseEvent.MOUSE_CLICKED, e -> {
			System.err.println("Java mouse click event");
		});
		
		addEventHandler(ScrollEvent.SCROLL, e -> {
			System.err.println("Java scroll event " + e);
			GLSurfaceAPI.ScrollEvent event = new GLSurfaceAPI.ScrollEvent();
			event.deltaX = (float) e.getDeltaX();
			event.deltaY = (float) e.getDeltaY();
			
			GLSurfaceAPI.fireScrollEvent(surfaceId, event);
		});
		
	}
  
	public long getSurfaceId() {
		return surfaceId;
	}

	public long getContextHandle() {
		return ((NGGLSurface) impl_getPeer()).getContextHandle();
	}
	
	@Override
	public NGNode impl_createPeer() {
		return new NGGLSurface(this);
	}

	@Override
	public BaseBounds impl_computeGeomBounds(BaseBounds bounds, BaseTransform tx) {
		bounds = new RectBounds(0f, 0f, (float) getWidth(), (float) getHeight());
		bounds = tx.transform(bounds, bounds);
		return bounds;
	}

	@Override
	protected boolean impl_computeContains(double localX, double localY) {
		double w = getWidth();
		double h = getHeight();
		return (w > 0 && h > 0 && localX >= 0 && localY >= 0 && localX < w && localY < h);
	}

	@Override
	public Object impl_processMXNode(MXNodeAlgorithm alg, MXNodeAlgorithmContext ctx) {
		throw new UnsupportedOperationException("Not supported yet.");
	}

	@Override
	public boolean isResizable() {
		return true;
	}

	@Override
	public void resize(double width, double height) {
		setWidth(width);
		setHeight(height);
		impl_markDirty(DirtyBits.NODE_GEOMETRY);
	}

	@Override
	public double minHeight(double width) {
		return 0;
	}

	@Override
	public double minWidth(double height) {
		return 0;
	}
	
	@Override
	public double prefWidth(double height) {
		return 100;
	}
	
	@Override
	public double prefHeight(double width) {
		return 100;
	}
	
	@Override
	public double maxWidth(double height) {
		return Double.MAX_VALUE;
	}
	
	@Override
	public double maxHeight(double width) {
		return Double.MAX_VALUE;
	}

	
	// NOTE width code from Region
	private void widthChanged(double value) {
        if (value != _width) {
            _width = value;
//            cornersValid = false;
//            boundingBox = null;
            impl_layoutBoundsChanged();
            impl_geomChanged();
            impl_markDirty(DirtyBits.NODE_GEOMETRY);
//            setNeedsLayout(true);
//            requestParentLayout();
            
            
            NGGLSurface peer = impl_getPeer();
            peer.setWidth((float) value);
        }
	}
	private double _width;
	private ReadOnlyDoubleWrapper width;
	public final double getWidth() { return width == null ? _width : width.get(); }
	public final ReadOnlyDoubleProperty widthProperty() {
        if (width == null) {
            width = new ReadOnlyDoubleWrapper(_width) {
                @Override protected void invalidated() { widthChanged(get()); }
                @Override public Object getBean() { return GLSurface.this; }
                @Override public String getName() { return "width"; }
            };
        }
        return width.getReadOnlyProperty();
    }
	protected void setWidth(double value) {
        if(width == null) {
            widthChanged(value);
        } else {
            width.set(value);
        }
    }
	
	// NOTE height code from Region
	   private void heightChanged(double value) {
	        if (_height != value) {
	            _height = value;
//	            cornersValid = false;
	            // It is possible that somebody sets the height of the region to a value which
	            // it previously held. If this is the case, we want to avoid excessive layouts.
	            // Note that I have biased this for layout over binding, because the heightProperty
	            // is now going to recompute the height eagerly. The cost of excessive and
	            // unnecessary bounds changes, however, is relatively high.
//	            boundingBox = null;
	            // Note: although impl_geomChanged will usually also invalidate the
	            // layout bounds, that is not the case for Regions, and both must
	            // be called separately.
	            impl_geomChanged();
	            impl_layoutBoundsChanged();
	            // We use "NODE_GEOMETRY" to mean that the bounds have changed and
	            // need to be sync'd with the render tree
	            impl_markDirty(DirtyBits.NODE_GEOMETRY);
	            // Change of the height (or width) won't change the preferred size.
	            // So we don't need to flush the cache. We should however mark this node
	            // as needs layout to be internally layouted.
//	            setNeedsLayout(true);
	            // This call is only needed when this was not called from the parent during the layout.
	            // Otherwise it would flush the cache of the parent, which is not necessary
//	            requestParentLayout();
	            
	            NGGLSurface peer = impl_getPeer();
	            peer.setHeight((float) value);
	        }
	    }
	private double _height;
	private ReadOnlyDoubleWrapper height;
	public final double getHeight() { return height == null ? _height : height.get(); }
	public final ReadOnlyDoubleProperty heightProperty() {
        if (height == null) {
            height = new ReadOnlyDoubleWrapper(_height) {
                @Override protected void invalidated() { heightChanged(get()); }
                @Override public Object getBean() { return GLSurface.this; }
                @Override public String getName() { return "height"; }
            };
        }
        return height.getReadOnlyProperty();
    }
    protected void setHeight(double value) {
        if (height == null) {
            heightChanged(value);
        } else {
            height.set(value);
        }
    }

	public static class Texture {
		public long textureId;
		public int width;
		public int height;
	}
	
	private long frameNumber = 0;
	private FrameReport frameReport;
	
	public Texture GetNextTexture() {
//		System.err.println("java GetNextTexture()");
		frameReport = new FrameReport();
		frameReport.number = frameNumber++;
		frameReport.getNextTexture -= System.nanoTime();
		try {
			NGGLSurface peer = impl_getPeer();
			
			Tex next = peer.GetNextTex(frameReport);
			
			Texture t = new Texture();
			t.textureId = next.textureId;
			t.width = next.fxTex.getContentWidth();
			t.height = next.fxTex.getContentHeight();
			
			return t;
		}
		finally {
			frameReport.getNextTexture += System.nanoTime();
			
			frameReport.nativeTime = -System.nanoTime();
		}
	}
	
	public static class FrameReport {
		public long number;
		public long getNextTexture;
		public long swapTexture;
		public long nativeTime;
		public long swapTextureQuantum;
		public long getNextTextureQuantum;
		
		@Override
		public String toString() {
			return String.format("Frame %d nextTex: %5.3fms(q: %5.3fms) native: %5.3fms swap: %5.3fms(q: %5.3fms)", number, getNextTexture/1_000_000d, getNextTextureQuantum/1_000_000d, nativeTime/1_000_000d, swapTexture/1_000_000d, swapTextureQuantum/1_000_000d);
		}
	}
	
	public void SwapTexture() {
		
		if (frameReport != null) {
			frameReport.nativeTime += System.nanoTime();
			frameReport.swapTexture = -System.nanoTime();
		}
//		System.err.println("java SwapTexture()");
		
		NGGLSurface peer = impl_getPeer();
		peer.SwapTex(frameReport);
		
		Platform.runLater(()-> {
			impl_markDirty(DirtyBits.NODE_CONTENTS);
			impl_markDirty(DirtyBits.NODE_GEOMETRY);
		});
		
		
		if (frameReport != null) {
			frameReport.swapTexture += System.nanoTime();
		}
		float curFps = GLSurfaceAPI.getFPS(surfaceId);
		Platform.runLater(() -> fps.set(curFps));
		if (frameReport != null) {
			if (frameReport.number % 1000 == 0) System.err.println(frameReport);
		}
	}
	
	@Deprecated
	@Override
	public void impl_updatePeer() {
		super.impl_updatePeer();
		NGGLSurface peer = impl_getPeer();
		
		if (impl_isDirty(DirtyBits.NODE_GEOMETRY)) {
			peer.setSize((float) getWidth(), (float) getHeight());
			peer.markDirty();
		}
		
		if (impl_isDirty(DirtyBits.NODE_CONTENTS)) {
			//peer.SwapTex(null);
			peer.markDirty();
		}
		
	}

	
	
	private ReadOnlyDoubleWrapper fps = new ReadOnlyDoubleWrapper(this, "fps", 0);
	public ReadOnlyDoubleProperty fpsProperty() {
		return fps.getReadOnlyProperty();
	}
	
	public double getFps() {
		return fps.get();
	}
	
}
