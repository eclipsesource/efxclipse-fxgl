package at.bestsolution.fxgl;


import org.lwjgl.glfw.GLFW;

import com.sun.javafx.geom.BaseBounds;
import com.sun.javafx.geom.RectBounds;
import com.sun.javafx.geom.transform.BaseTransform;
import com.sun.javafx.jmx.MXNodeAlgorithm;
import com.sun.javafx.jmx.MXNodeAlgorithmContext;
import com.sun.javafx.scene.DirtyBits;
import com.sun.javafx.sg.prism.NGNode;

import javafx.beans.property.DoubleProperty;
import javafx.beans.property.DoublePropertyBase;
import javafx.scene.Node;
import javafx.scene.Scene;

@SuppressWarnings("restriction")
public class GLCanvas extends Node {

	private long glfwWindow;
	
	public GLCanvas() {
		this(0, 0);
	}
	
	
	public void fxglMakeContextCurrent() {
		GLFW.glfwMakeContextCurrent(glfwWindow);
	}
	
	public void fxglSwapBuffers() {
		updateRender();
	}
	

	public GLCanvas(double width, double height) {
		setWidth(width);
		setHeight(height);
		setMouseTransparent(false);
		
		Scene.impl_setAllowPGAccess(true);
//		((NGGLCanvas) impl_getPeer()).setThreeDView(view);
		Scene.impl_setAllowPGAccess(false);
		updateRender();
		impl_markDirty(DirtyBits.NODE_GEOMETRY);
		impl_geomChanged();
	}

	private void updateRender() {
		impl_markDirty(DirtyBits.NODE_GEOMETRY);
	}

	

	@Override
	public NGNode impl_createPeer() {
		return new NGGLCanvas(this);
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
			NGGLCanvas peer = impl_getPeer();
			peer.updateBounds((float) getWidth(), (float) getHeight());
		}
	}

	@Override
	public Object impl_processMXNode(MXNodeAlgorithm alg, MXNodeAlgorithmContext ctx) {
		throw new UnsupportedOperationException("Not supported yet.");
	}

	public static String getLUID() {
		return NGGLCanvas.luid;
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
					return GLCanvas.this;
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
					return GLCanvas.this;
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
