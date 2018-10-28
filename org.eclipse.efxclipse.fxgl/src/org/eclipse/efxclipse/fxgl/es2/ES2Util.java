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
package org.eclipse.efxclipse.fxgl.es2;

import java.lang.reflect.Array;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

import com.sun.prism.ResourceFactory;
import com.sun.prism.Texture;

@SuppressWarnings({"restriction"})
public class ES2Util {

	public static ResourceFactory getResourceFactory() throws Exception {
		Class<?> es2PipelineClass = Class.forName("com.sun.prism.es2.ES2Pipeline");
//		Class<?> es2ResourceFactoryClass = Class.forName("com.sun.prism.es2.ES2ResourceFactory");
		
		Field factoriesField = es2PipelineClass.getDeclaredField("factories");
		factoriesField.setAccessible(true);
		Object factories = factoriesField.get(null);
		
		int length = Array.getLength(factories);
		for (int i = 0; i < length; i++) {
			Object factory = Array.get(factories, i);
			System.err.println(" * Factory " + factory);
		}

		// TODO check if we can always get the first factory
		
		if (length > 0) {
			return (ResourceFactory) Array.get(factories, 0);
		}
		
		return null;
	}
	
	
	public static long getContextHandle(ResourceFactory resourceFactory) throws Exception {
		Class<?> es2ResourceFactoryClass = Class.forName("com.sun.prism.es2.ES2ResourceFactory");
		Field es2ContextField = es2ResourceFactoryClass.getDeclaredField("context");
		es2ContextField.setAccessible(true);
		Object es2Context = es2ContextField.get(resourceFactory);
		
		Method glContextGetter = es2Context.getClass().getDeclaredMethod("getGLContext");
		glContextGetter.setAccessible(true);
		Object glContext = glContextGetter.invoke(es2Context);
		
		Method nativeHandleGetter = glContext.getClass().getDeclaredMethod("getNativeHandle");
		//Method nativeHandleGetter = glContext.getClass().getDeclaredMethod("getNativeCtxInfo");
		nativeHandleGetter.setAccessible(true);
		long nativeHandle = (long) nativeHandleGetter.invoke(glContext);
		
		return nativeHandle;
	}
	
	public static long getContextHandle(Texture texture) throws Exception {
//		Class<?> es2TextureClass = Class.forName("com.sun.prism.es2.ES2Texture");
		Field es2ContextField = texture.getClass().getDeclaredField("context");
		es2ContextField.setAccessible(true);
		Object es2Context = es2ContextField.get(texture);
		
		Method glContextGetter = es2Context.getClass().getDeclaredMethod("getGLContext");
		glContextGetter.setAccessible(true);
		Object glContext = glContextGetter.invoke(es2Context);
		
		Method nativeHandleGetter = glContext.getClass().getDeclaredMethod("getNativeHandle");
		nativeHandleGetter.setAccessible(true);
		long nativeHandle = (long) nativeHandleGetter.invoke(glContext);
		
		return nativeHandle;
	}
	
	public static int getTextureId(Texture texture) throws Exception {
		Method nativeSourceHandleGetter = texture.getClass().getMethod("getNativeSourceHandle");
		nativeSourceHandleGetter.setAccessible(true);
		int texId  = (int) nativeSourceHandleGetter.invoke(texture);
		return texId;
		
	}
}
