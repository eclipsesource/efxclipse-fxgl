package at.bestsolution.fxgl.es2;

import com.sun.javafx.sg.prism.NGNode;
import com.sun.prism.Graphics;
import com.sun.prism.PixelFormat;
import com.sun.prism.ResourceFactory;
import com.sun.prism.Texture;

import at.bestsolution.fxgl.DXUtils;

public class NGGLSurface extends NGNode {

	GLSurface node;
	Texture currenttexture;
	
	long contextHandle;
	long textureId;
	
	public NGGLSurface(GLSurface node) {
		this.node = node;
	}
	
	
	@Override
	protected void renderContent(Graphics g) {
		if (currenttexture == null) {
			ResourceFactory factory = g.getResourceFactory();
			Texture texture = factory.createTexture(PixelFormat.INT_ARGB_PRE, Texture.Usage.DYNAMIC, Texture.WrapMode.CLAMP_TO_ZERO, 800, 600);
			texture.makePermanent();
			currenttexture = texture;
			try {
				contextHandle = ES2Util.getContextHandle(texture);
			}
			catch (Exception e) {
				System.err.println("Failed to get context handle!");
				e.printStackTrace();
			}
			try {
				textureId = ES2Util.getTextureId(texture);
			}
			catch (Exception e) {
				System.err.println("Failed to get texture id!");
				e.printStackTrace();
			}
		}

		if (currenttexture != null)
			g.drawTexture(currenttexture, 0, 0, 800, 600);
	}
	
	private float tw, th;
	
	public void updateBounds(float width, float height) {
		int newWidth = (int)width;
		int newHeight = (int) height;
		if (newWidth == this.tw && newHeight == this.th)
			return;

        this.tw = newWidth;
        this.th = newHeight;

//        if (view != null) {
//        	view.setSize(tw, th);
//        }
	}

	@Override
	protected boolean hasOverlappingContents() {
		return false;
	}

	public long getContextHandle() {
		return contextHandle;
	}
	
	public long getTextureId() {
		return textureId;
	}
	
	/**
	 * 2018/01/25: Call with Christoph
	 * An overlying node does prevent updating the dirty flag of an underlying node.
	 * Therefore we make this method public and call it from UberSceneView.accept().
	 */
	@Override
	public void visualsChanged() {
		// TODO Auto-generated method stub
		super.visualsChanged();
	}
	
}
