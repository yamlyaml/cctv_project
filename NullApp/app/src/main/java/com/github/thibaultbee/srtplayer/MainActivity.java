package com.github.thibaultbee.srtplayer;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    EditText edit_id;
    EditText edit_password;
    Button button_login;

    private static final String adminID = "admin";
    private static final String admionPW = "1234";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_loginpage);



        edit_id = findViewById(R.id.edit_ID);
        edit_password = findViewById(R.id.edit_Password);
        button_login = findViewById(R.id.button_Login);

        button_login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String ID = edit_id.getText().toString();
                String Password = edit_password.getText().toString();
                boolean checkID = ID.equals(adminID);
                boolean checkPassword = Password.equals(admionPW);

                if(checkID && checkPassword){
                    Intent intent = new Intent(getApplicationContext(), MainpageActivity.class);
                    startActivity(intent);
                    finish();
                }
                else if(!checkID){
                    makeToast("ID를 확인해주세요");
                }
                else{
                    makeToast("Password를 확인해주세요");
                }

            }
        });

    }

    public void makeToast(String msg){
        Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_SHORT).show();
        return;
    }
}