package com.github.thibaultbee.srtplayer.ftp;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.Switch;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;


import com.github.thibaultbee.srtplayer.R;
import com.github.thibaultbee.srtplayer.Tasks.ConnectTask;
import com.google.android.material.snackbar.Snackbar;
import com.jcraft.jsch.*;

import com.github.thibaultbee.srtplayer.Tasks.ConnectTask;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.concurrent.ExecutionException;

public class RegisterActivity extends AppCompatActivity {

    Switch sID, PEM_password_switch;
    EditText ePassword, PEM_password;
    Button privateKeyButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.register_connection);
        sID = findViewById(R.id.register_switch_id);
        ePassword = findViewById(R.id.register_password);
        privateKeyButton = findViewById(R.id.register_button_choose_file);
        privateKeyButton.setVisibility(View.INVISIBLE);

        PEM_password_switch = findViewById(R.id.register_switch_pem_password);
        PEM_password = findViewById(R.id.register_pem_password);
        PEM_password_switch.setVisibility(View.INVISIBLE);
        PEM_password.setVisibility(View.INVISIBLE);


        int permissionCheck = ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE);

        if (permissionCheck != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, 84);
        }


        sID.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    ePassword.setVisibility(View.INVISIBLE);
                    privateKeyButton.setVisibility(View.VISIBLE);
                    PEM_password_switch.setVisibility(View.VISIBLE);
                } else {
                    ePassword.setVisibility(View.VISIBLE);
                    privateKeyButton.setVisibility(View.INVISIBLE);
                    PEM_password_switch.setVisibility(View.INVISIBLE);
                    PEM_password.setVisibility(View.INVISIBLE);
                }
            }
        });


        PEM_password_switch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    PEM_password.setVisibility(View.VISIBLE);
                } else {
                    PEM_password.setVisibility(View.INVISIBLE);

                }
            }
        });
    }


    Uri uri;
    byte[] filecontent_bytes;
    ConnectTask connectTask;
    Session session;

    /**
     * Called when the user taps the Send button
     */
    public void saveConnection(View view) {
        // Do something in response to button
        EditText eIP = findViewById(R.id.register_ip);
        EditText ePort = findViewById(R.id.register_port);
        EditText eUser = findViewById(R.id.register_user);


        String user = eUser.getText().toString();
        String IP = eIP.getText().toString();
        String port = ePort.getText().toString();
        Globals.currentPath = "/home/" + user;

        if (!user.equals("") && !IP.equals("") && !port.equals("")) {
            connectTask = new ConnectTask(RegisterActivity.this);

            if (sID.isChecked()) {
                //use private key
                if (Globals.private_bytes != null) {

                    if(PEM_password_switch.isChecked()){

                        String PEM_pass = PEM_password.getText().toString();
                        connectTask.execute(user, IP, port, String.valueOf(sID.isChecked()), String.valueOf(PEM_password_switch.isChecked()), "", PEM_pass);
                    }else {
                        connectTask.execute(user, IP, port, String.valueOf(sID.isChecked()), String.valueOf(PEM_password_switch.isChecked()), "", "");
                    }
                } else {
                    Snackbar.make(view, "Check PEM key file", Snackbar.LENGTH_LONG).show();

                }
            } else {
                String password = ePassword.getText().toString();

                if (!password.equals("")) {
                    connectTask.execute(user, IP, port, String.valueOf(sID.isChecked()), String.valueOf(PEM_password_switch.isChecked()), password, "");
                } else {
                    Snackbar.make(view, "Please input a password", Snackbar.LENGTH_LONG).show();
                    return;
                }
            }

            try {
                session = connectTask.get();

                if (session == null) {
                    Snackbar.make(view, "Could not connect", Snackbar.LENGTH_LONG).show();
                }

            } catch (ExecutionException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }


            if (session != null) {
                Intent FolderIntent = new Intent(RegisterActivity.this,FolderActivity.class);

                Globals.session = session;
                connectTask.cancel(true);
                Log.d("REGISTER ACTIVITY", "launching Folder activity");
                RegisterActivity.this.startActivity(FolderIntent);
            }
        } else {
            Snackbar.make(view, "Check input fields", Snackbar.LENGTH_LONG).show();

        }


    }

    @Override
    public void onResume() {
        super.onResume();
        if (Globals.mProgressDialogConnect != null && Globals.mProgressDialogConnect.isShowing())
            Globals.mProgressDialogConnect.dismiss();
    }

    private static final int READ_REQUEST_CODE_PRIVATEKEY = 42;

    /**
     * Fires an intent to spin up the "file chooser" UI and select an image.
     */
    public void performFileSearchPrivate(View view) {

        // ACTION_OPEN_DOCUMENT is the intent to choose a file via the system's file
        // browser.
        Intent intent = new Intent(Intent.ACTION_OPEN_DOCUMENT);

        // Filter to only show results that can be "opened", such as a
        // file (as opposed to a list of contacts or timezones)
        intent.addCategory(Intent.CATEGORY_OPENABLE);

        // Filter to show only images, using the image MIME data type.
        // If one wanted to search for ogg vorbis files, the type would be "audio/ogg".
        // To search for all documents available via installed storage providers,
        // it would be "*/*".
        intent.setType("*/*");

        startActivityForResult(intent, READ_REQUEST_CODE_PRIVATEKEY);
    }


    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent resultData) {

        // The ACTION_OPEN_DOCUMENT intent was sent with the request code
        // READ_REQUEST_CODE. If the request code seen here doesn't match, it's the
        // response to some other intent, and the code below shouldn't run at all.

        super.onActivityResult(requestCode, resultCode, resultData);
        if (requestCode == READ_REQUEST_CODE_PRIVATEKEY && resultCode == Activity.RESULT_OK) {
            // The document selected by the user won't be returned in the intent.
            // Instead, a URI to that document will be contained in the return intent
            // provided to this method as a parameter.
            // Pull that URI using resultData.getData().
            //Uri uri = null;
            if (resultData != null) {
                uri = resultData.getData();
                try {
                    filecontent_bytes = readBytes(getContentResolver().openInputStream(uri));
                    if (checkKey(filecontent_bytes) || PEM_password_switch.isChecked()) {
                        Globals.private_bytes = filecontent_bytes;
                    } else {
                        Globals.private_bytes = null;
                        Toast.makeText(getApplicationContext(), "Selected is not in PEM format", Toast.LENGTH_LONG).show();
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }

            }
        }
    }


    public boolean checkKey(byte[] keyin) {
        String validKey = "-----BEGIN RSA PRIVATE KEY-----";
        String keyContent = new String(keyin);

        return keyContent.split("\n")[0].equals(validKey);
    }

    public byte[] readBytes(InputStream inputStream) throws IOException {
        // this dynamically extends to take the bytes you read
        ByteArrayOutputStream byteBuffer = new ByteArrayOutputStream();

        // this is storage overwritten on each iteration with bytes
        int bufferSize = 1024;
        byte[] buffer = new byte[bufferSize];

        // we need to know how may bytes were read to write them to the byteBuffer
        int len = 0;
        while ((len = inputStream.read(buffer)) != -1) {
            byteBuffer.write(buffer, 0, len);
        }

        // and then we can return your byte array.
        return byteBuffer.toByteArray();
    }

}
