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
package org.eclipse.efxclipse.fxgl.sample.application;

import org.eclipse.efxclipse.fxgl.es2.GLSurface;
import org.eclipse.efxclipse.fxgl.sample.renderer.objrenderer.ObjRenderer;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ToolBar;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.stage.Stage;

public class SimpleObjRenderer extends Application {

	@Override
	public void start(Stage primaryStage) throws Exception {
		GLSurface surface = new GLSurface();
		
		ObjRenderer renderer = new ObjRenderer();
		renderer.setRenderTarget(surface);
		
		BorderPane layout = new BorderPane();
		
		ToolBar tb = new ToolBar();
		{
			Button b = new Button("Start");
			b.setOnAction(e -> renderer.start());
			tb.getItems().add(b);
		}
		{
			Button b = new Button("Stop");
			b.setOnAction(e -> renderer.stop());
			tb.getItems().add(b);
		}
		
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
