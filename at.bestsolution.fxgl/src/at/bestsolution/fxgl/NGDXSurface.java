package at.bestsolution.fxgl;


import java.util.Timer;
import java.util.TimerTask;

import com.sun.javafx.sg.prism.NGNode;
import com.sun.javafx.tk.ScreenConfigurationAccessor;
import com.sun.javafx.tk.Toolkit;
import com.sun.prism.Graphics;
import com.sun.prism.PixelFormat;
import com.sun.prism.ResourceFactory;
import com.sun.prism.Texture;
import com.sun.prism.paint.Paint;

import javafx.application.Platform;


@SuppressWarnings("restriction")
public class NGDXSurface extends NGNode {

	DXSurface node;

	
	Texture currenttexture;
	long lastHandleVersion;
	private int tw;
	private int th;
	private boolean isDirty = true;

	public NGDXSurface(DXSurface node) {
		this.node = node;
		
		
	}

	private void renderDummy(Graphics g)
	{
		Paint white = (Paint)Toolkit.getPaintAccessor().getPlatformPaint(javafx.scene.paint.Color.WHITE);
		g.setPaint(white);
		g.fillQuad(0, 0, this.tw, this.th);

		Paint red = (Paint)Toolkit.getPaintAccessor().getPlatformPaint(javafx.scene.paint.Color.RED);
		g.setPaint(red);
		g.drawRect(0, 0, this.tw - 1, this.th - 1 );
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

	
	@Override
	protected void renderContent(Graphics g) {
//		System.err.println("ACTUAL FX RENDER!");
		if (currenttexture == null) {
			ResourceFactory factory = g.getResourceFactory();
			Texture texture = factory.createTexture(PixelFormat.INT_ARGB_PRE, Texture.Usage.DYNAMIC, Texture.WrapMode.CLAMP_TO_ZERO, 800, 600);
			texture.makePermanent();
//			nativeHandle = NGGLCanvas.blub(texture);
			nativeHandle = DXUtils.recreateTexture(texture);
			currenttexture = texture;
		}
		

		if (currenttexture != null)
			
			g.drawTexture(currenttexture, 0, 0, 800, 600);
	}

	@Override
	protected boolean hasOverlappingContents() {
		return false;
	}


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

	public static native int nRegisterTexture(Texture texture, long handle, int format);

	//private static native long nGetLUID();

	private long nativeHandle;
	public long getNativeHandle() {
		return nativeHandle;
	}



}
