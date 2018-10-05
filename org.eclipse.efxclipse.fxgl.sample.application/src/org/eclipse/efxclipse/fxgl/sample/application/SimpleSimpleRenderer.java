package org.eclipse.efxclipse.fxgl.sample.application;

import org.eclipse.efxclipse.fxgl.es2.GLSurface;
import org.eclipse.efxclipse.fxgl.sample.renderer.simple.SimpleRenderer;

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
		
		SimpleRenderer renderer = new SimpleRenderer();
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
