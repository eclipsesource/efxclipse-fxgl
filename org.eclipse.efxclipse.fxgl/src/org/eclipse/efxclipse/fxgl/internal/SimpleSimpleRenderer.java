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
package org.eclipse.efxclipse.fxgl.internal;

import org.eclipse.efxclipse.fxgl.es2.GLSurface;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ToolBar;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;

public class SimpleSimpleRenderer extends Application {

	@Override
	public void start(Stage primaryStage) throws Exception {
		GLSurface surface = new GLSurface();
		
		System.err.println("contextHandle = " + surface.getContextHandle());
		
		BorderPane layout = new BorderPane();
		
		ToolBar tb = new ToolBar();
		
		
		layout.setTop(tb);
		
		layout.setCenter(surface);
		layout.setPrefSize(500, 300);
		
		Scene scen = new Scene(layout);
		
		primaryStage.setScene(scen);
		primaryStage.setTitle("SimpleObjRenderer");
		primaryStage.show();
		
		
	}
	
	public static void main(String[] args) {
		launch(args);
	}

}
