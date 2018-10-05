/*
 * Copyright (c) 2018 BestSolution and Others. All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0, which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception, which is available at
 * https://www.gnu.org/software/classpath/license.html.
 *
 * SPDX-License-Identifier: EPL-2.0 OR GPL-2.0 WITH Classpath-exception-2.0
 */
package org.eclipse.efxclipse.fxgl.sample.renderer.simple;


import org.eclipse.efxclipse.fxgl.sample.renderer.Renderer;

public class SimpleRenderer extends Renderer {

	static {
		Library.require();
	}
	
	private long rendererId;
	
	public SimpleRenderer() {
		rendererId = nCreate();
	}
	
	@Override
	public long getRendererId() {
		return rendererId;
	}
	
	public void dispose() {
		nDestroy(rendererId);
	}
	
	public void LoadObj(String name) {
		nLoadObj(rendererId, name);
	}

	private static native long nCreate();
	private static native void nDestroy(long rendererId);
	private static native void nLoadObj(long rendererId, String name);
	
}
