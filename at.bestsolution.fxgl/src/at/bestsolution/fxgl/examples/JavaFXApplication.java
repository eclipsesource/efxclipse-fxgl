package at.bestsolution.fxgl.examples;

import java.awt.print.Book;
import java.util.Timer;
import java.util.TimerTask;

import com.sun.javafx.logging.PulseLogger;

import at.bestsolution.fxgl.GLCanvas;
import javafx.animation.Animation;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.scene.transform.Scale;
import javafx.stage.Stage;

public class JavaFXApplication extends Application {

	@Override
	public void start(Stage primaryStage) throws Exception {
		BorderPane root = new BorderPane();
		Scene scen = new Scene(root);
		root.setPrefSize(900, 700);
		root.setStyle("-fx-background-color: lightblue");
		primaryStage.setScene(scen);
		primaryStage.setTitle("JavaFX Stage");
		primaryStage.show();
		
		GLCanvas canvas = new GLCanvas(800, 600);
	
		root.setCenter(canvas);
		
		Timer t0 = new Timer();
		t0.scheduleAtFixedRate(new TimerTask() {
			
			@Override
			public void run() {
				Platform.runLater(() -> {
					canvas.fxglSwapBuffers();
				});
			}
		}, 0, 40);

//		Thread t = new Thread(new SimpleLWJGL()::run);
//		t.start();
		
		
	}
	
	public static void main(String[] args) {
		System.setProperty("java.library.path", "C:\\projects\\fxGL\\at.bestsolution.fxgl\\os\\win32\\x86_64");
		launch(args);
	}

}
