package at.bestsolution.fxgl.es2;

import com.sun.javafx.geom.BaseBounds;
import com.sun.javafx.geom.RectBounds;
import com.sun.javafx.geom.transform.BaseTransform;
import com.sun.javafx.jmx.MXNodeAlgorithm;
import com.sun.javafx.jmx.MXNodeAlgorithmContext;
import com.sun.javafx.scene.DirtyBits;
import com.sun.javafx.sg.prism.NGNode;

import at.bestsolution.fxgl.DXSurface;
import at.bestsolution.fxgl.NGDXSurface;
import at.bestsolution.fxgl.NGGLCanvas;
import javafx.animation.AnimationTimer;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.DoublePropertyBase;
import javafx.scene.Node;
import javafx.scene.Scene;

public class GLSurface extends Node {

	public GLSurface() {
		this(0, 0);
	}
	
	public GLSurface(double width, double height) {
		setWidth(width);
		setHeight(height);
		setMouseTransparent(false);
		
		Scene.impl_setAllowPGAccess(true);
//		((NGGLCanvas) impl_getPeer()).setThreeDView(view);
		Scene.impl_setAllowPGAccess(false);
		updateRender();
		impl_markDirty(DirtyBits.NODE_GEOMETRY);
		impl_geomChanged();
		
		// AUTO REDRAW (TODO hanging thread!)
//		Timer tx = new Timer();
//		tx.scheduleAtFixedRate(new TimerTask() {
//			@Override
//			public void run() {
//				Platform.runLater(() -> impl_markDirty(DirtyBits.NODE_GEOMETRY));
//			}
//		}, 0, 20);
//		
		AnimationTimer at = new AnimationTimer() {
			long last = 0;
			@Override
			public void handle(long now) {
				long delta = now - last;
				if (delta > 2_000_000) {
					((NGGLSurface)impl_getPeer()).visualsChanged();
					//impl_markDirty(DirtyBits.NODE_GEOMETRY);
					//System.err.println("pulse " + String.format("%5.3fms", delta / 1000_000d));
					last = now;
				}
			}
		};
		at.start();
		
	}
	

	private void updateRender() {
		impl_markDirty(DirtyBits.NODE_GEOMETRY);
	}

	public long getContextHandle() {
		return ((NGGLSurface) impl_getPeer()).getContextHandle();
	}
	
	public long getTextureId() {
		return ((NGGLSurface) impl_getPeer()).getTextureId();
	}
	
	public void forceUpdate() {
		updateRender();
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

	@Deprecated
	@Override
	public void impl_updatePeer() {
		super.impl_updatePeer();
		if (impl_isDirty(DirtyBits.NODE_GEOMETRY)) {
			NGGLSurface peer = impl_getPeer();
			peer.updateBounds((float) getWidth(), (float) getHeight());
		}
	}

	@Override
	public Object impl_processMXNode(MXNodeAlgorithm alg, MXNodeAlgorithmContext ctx) {
		throw new UnsupportedOperationException("Not supported yet.");
	}

	public boolean isResizable() {
		return true;
	}

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

	/**
	 * Defines the width of the canvas.
	 *
	 * @profile common
	 * @defaultvalue 0.0
	 */
	private DoubleProperty width;

	public final void setWidth(double value) {
		widthProperty().set(value);
	}

	public final double getWidth() {
		return width == null ? 0.0 : width.get();
	}

	public final DoubleProperty widthProperty() {
		if (width == null) {
			width = new DoublePropertyBase() {

				@Override
				public void invalidated() {
					updateRender();
					impl_geomChanged();
				}

				@Override
				public Object getBean() {
					return GLSurface.this;
				}

				@Override
				public String getName() {
					return "width";
				}
			};
		}
		return width;
	}

	/**
	 * Defines the height of the canvas.
	 *
	 * @profile common
	 * @defaultvalue 0.0
	 */
	private DoubleProperty height;

	public final void setHeight(double value) {
		heightProperty().set(value);
	}

	public final double getHeight() {
		return height == null ? 0.0 : height.get();
	}

	public final DoubleProperty heightProperty() {
		if (height == null) {
			height = new DoublePropertyBase() {

				@Override
				public void invalidated() {
					updateRender();
					impl_geomChanged();
				}

				@Override
				public Object getBean() {
					return GLSurface.this;
				}

				@Override
				public String getName() {
					return "height";
				}
			};
		}
		return height;
	}
}
