package com.example.heart.qrcodeex;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.view.Menu;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import  android.hardware.Camera;
import android.widget.TextView;

import static com.example.heart.qrcodeex.R.drawable.cat;


public class MainActivity extends AppCompatActivity {

    private static final int CAMERA_REQUEST = 1888;
    ImageView imageView;
    Camera cam;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        imageView = (ImageView) this.findViewById(R.id.imageView);
        Button photoButton = (Button) this.findViewById(R.id.TakePhoto);

        photoButton.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                Intent cameraIntent = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);
                startActivityForResult(cameraIntent, CAMERA_REQUEST);
            }
        });
    }

    private String getPersonAndSetImg(long id) {
        if (id == 170) {
            imageView.setImageResource(R.drawable.cat);
            return "Cat.";
        }
        else if (id == 8208) {
            imageView.setImageResource(R.drawable.dog);
            return "Dog.";
        }
        else if (id == 264721) {
            imageView.setImageResource(R.drawable.cthulhu);
            return "Cthulhu.";
        }
        else {
            imageView.setImageResource(R.drawable.what);
            return "Undefined person.";
        }
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == CAMERA_REQUEST) {
            Bitmap photo = (Bitmap) data.getExtras().get("data");

            //photo.getPixels();
            //imageView.setImageMatrix();
            //imageView.setImageBitmap(photo);
            int []pixels = new int[photo.getWidth()*photo.getHeight()];
            photo.getPixels(pixels, 0, photo.getWidth(), 0, 0, photo.getWidth(), photo.getHeight());
            long guid = getGUIDUser(pixels, photo.getWidth(), photo.getHeight());

            TextView tv = (TextView) this.findViewById(R.id.textView);
            tv.setText(Long.toString(guid) + " " + getPersonAndSetImg(guid));
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native long getGUIDUser(int []pixels, int width, int height);
}
