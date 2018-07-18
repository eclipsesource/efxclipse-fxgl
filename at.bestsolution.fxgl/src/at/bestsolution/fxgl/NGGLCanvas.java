package at.bestsolution.fxgl;


import com.sun.javafx.sg.prism.NGNode;
import com.sun.javafx.tk.ScreenConfigurationAccessor;
import com.sun.javafx.tk.Toolkit;
import com.sun.prism.Graphics;
import com.sun.prism.PixelFormat;
import com.sun.prism.ResourceFactory;
import com.sun.prism.Texture;
import com.sun.prism.paint.Paint;


@SuppressWarnings("restriction")
public class NGGLCanvas extends NGNode {

	GLCanvas node;

	
	Texture currenttexture;
	long lastHandleVersion;
	private int tw;
	private int th;
	private boolean isDirty = true;

	static String luid;

	static boolean UBER_SUPPORTED = false;

//	static {
//		String OS_NAME = System.getProperty("os.name");
//		if ( OS_NAME != null && OS_NAME.startsWith("Windows"))
//		{
//			UBER_SUPPORTED = true;
//			try {
//				if ("64".equals(System.getProperty("sun.arch.data.model")))
//					System.loadLibrary("uberfx_x64");
//				else
//					System.loadLibrary("uberfx_x86");
//
//				luid = Long.toString(nGetLUID());
//			} catch (UnsatisfiedLinkError e) {
//				e.printStackTrace();
//			}
//		}
//	}

	public NGGLCanvas(GLCanvas node) {
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
//	@Override
//	public void visualsChanged() {
//		// TODO Auto-generated method stub
//		super.visualsChanged();
//	}

	@Override
	protected void renderContent(Graphics g) {
		if (!UBER_SUPPORTED) {
			renderDummy(g);
			return;
		}
		if (isDirty)
		{
			if (currenttexture != null)
			{
				currenttexture.dispose();
				currenttexture = null;
			}
			isDirty = false;
		}

//		if (view == null)
//		{
//			renderDummy(g);
//			return;
//		}

		// now we have the most recent Camera and Projection Matrix
//		Three3DContent content = (Three3DContent)view.getData();
//		if (content == null)
//		{
//			renderDummy(g);
//			return;
//		}

//		if (lastHandleVersion != content.handleVersion)
//		{
			// dispose old texture, create a new one
			if (currenttexture != null)
				currenttexture.dispose();

			ResourceFactory factory = g.getResourceFactory();
			Texture texture = factory.createTexture(PixelFormat.INT_ARGB_PRE, Texture.Usage.DYNAMIC, Texture.WrapMode.CLAMP_TO_ZERO, 800, 600);
			texture.makePermanent();

			//nRegisterTexture(texture, content.sharedHandle, content.texFormat);
			
			int result = nRegisterTexture(texture, 0x000002b4b5baca60l, 87);
			System.err.println("native: " + result);
//			lastHandleVersion = content.handleVersion;

			
			
			currenttexture = texture;
//		}

		if (currenttexture != null)
			g.drawTexture(currenttexture, 0, 0, this.tw, this.th, 0, 0, 800, 600);

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

	public static native int blub(Texture texture);

	private static native long nGetLUID();



}
