package at.bestsolution.fxgl.sample.application;

import at.bestsolution.fxgl.es2.GLSurface;
import at.bestsolution.fxgl.sample.renderer.objrenderer.ObjRenderer;
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
