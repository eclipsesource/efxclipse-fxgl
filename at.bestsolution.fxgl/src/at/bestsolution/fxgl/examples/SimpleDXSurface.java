package at.bestsolution.fxgl.examples;

import java.awt.print.Book;
import java.util.Timer;
import java.util.TimerTask;

import com.sun.javafx.logging.PulseLogger;

import at.bestsolution.fxgl.DXSurface;
import at.bestsolution.fxgl.DXUtils;
import at.bestsolution.fxgl.GLCanvas;
import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.RotateTransition;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.BorderPane;
import javafx.scene.text.Text;
import javafx.scene.transform.Rotate;
import javafx.scene.transform.Scale;
import javafx.scene.transform.Transform;
import javafx.stage.Stage;
import javafx.util.Duration;

public class SimpleDXSurface extends Application {

	@Override
	public void start(Stage primaryStage) throws Exception {
		try {
		System.err.println(System.getProperty("java.library.path"));
		System.err.println("adapter luid: " + DXUtils.getAdapterLUID());
		System.err.println("noparam: " + DXUtils.noparam());
		System.err.println("texParam: " + DXUtils.texparam(null));
		}
		catch (Exception e) {
			e.printStackTrace();
			System.exit(0);
		}
		
		
		BorderPane root = new BorderPane();
		Scene scen = new Scene(root);
		root.setPrefSize(900, 700);
		root.setStyle("-fx-background-color: lightblue");
		primaryStage.setScene(scen);
		primaryStage.setTitle("JavaFX Stage");
		primaryStage.show();
		
		TextField text = new TextField();
		text.setEditable(false);
		Button update = new Button("update");
		DXSurface canvas = new DXSurface(800, 600);
	
		update.setOnAction(e -> text.setText("Handle: " + canvas.getNativeHandle()));
		BorderPane top = new BorderPane();
		top.setCenter(text);
		top.setRight(update);
		
		
		
		root.setTop(top);
		root.setCenter(canvas);
		
		
		Group overlay = new Group();

		root.getChildren().add(overlay);
		
		
		
		
		ImageView image = new ImageView(new Image(SimpleDXSurface.class.getResourceAsStream("beso_665_180.png")));
		image.getTransforms().setAll(Transform.translate(650, 600), Transform.scale(0.3, 0.3));
		overlay.getChildren().add(image);
		
		Text tx0 = new Text("JavaFX Text");
		tx0.setStyle("-fx-font-size: 90px; -fx-font-weight: bold; -fx-fill: teal; -fx-stroke: white; -fx-stroke-width: 2px;"); 
		tx0.setLayoutX(50);
		tx0.setLayoutY(300);
		
		RotateTransition r = new RotateTransition(Duration.millis(600), tx0);
		r.setFromAngle(-30);
		r.setToAngle(30);
		r.setAutoReverse(true);
		r.setCycleCount(Timeline.INDEFINITE);
		r.play();
		
		
//		Rotate rotate = Transform.rotate(0, 200, 0);
//		
//		Timeline t0 = new Timeline(
//		
//		new KeyFrame(Duration.millis(300), new KeyValue(rotate.angleProperty(), 90)),
//		new KeyFrame(Duration.millis(300), new KeyValue(rotate.angleProperty(), -90))
//		
//		);
//		t0.setCycleCount(Timeline.INDEFINITE);
//		t0.setAutoReverse(true);
//		t0.play();
		
		//tx0.getTransforms().setAll(Transform.translate(300, 300));
		
		overlay.getChildren().add(tx0);
		
	}
	
	public static void main(String[] args) {
		// force load library
		System.loadLibrary("uberfx_x64");
		//System.setProperty("java.library.path", "C:\\projects\\fxGL\\at.bestsolution.fxgl\\os\\win32\\x86_64");
		
		launch(args);
	}

}
