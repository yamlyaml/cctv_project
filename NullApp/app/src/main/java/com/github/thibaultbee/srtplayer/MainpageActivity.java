package com.github.thibaultbee.srtplayer;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout;


import com.github.thibaultbee.srtplayer.ftp.RegisterActivity;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;

public class MainpageActivity extends AppCompatActivity {
    ImageButton button_realtime;
    ImageButton button_opendir;
    ImageButton button_buzzer;

    ImageView imageView_Illegal;
    ImageView imageView_Illegal2;
    ImageView imageView_Illegal3;
    ImageView imageView_Motion;
    ImageView imageView_Motion2;
    ImageView imageView_Motion3;

    private static final String TCP_IP = "59.13.236.209";
    private static final int PORT_WRITE = 33156;
    private static final int PORT_READ = 33157;
    private static final String SRTurl = "59.13.236.209:33125";
    boolean ButtonState = false;
    boolean Viewstate = false;

    LinearLayout container;
    ConstraintLayout rootLayout;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_mainpage);

        button_realtime = findViewById(R.id.imageButton_Realtime);
        button_opendir = findViewById(R.id.imageButton_OpenDir);
        button_buzzer = findViewById(R.id.imageButton_Buzzer);

        imageView_Illegal = findViewById(R.id.imageView_Alarm_Illegal);
        imageView_Illegal2 = findViewById(R.id.imageView_Alarm_Illegal2);
        imageView_Illegal3 = findViewById(R.id.imageView_Alarm_Illegal3);
        imageView_Motion = findViewById(R.id.imageView_Alarm_Motion);
        imageView_Motion2 = findViewById(R.id.imageView_Alarm_Motion2);
        imageView_Motion3 = findViewById(R.id.imageView_Alarm_Motion3);

        container = findViewById(R.id.container_buzzer_selector);

        LayoutInflater inflater = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View subView = inflater.inflate(R.layout.selector_buzzer, container, true);

        Button button_area1 = container.findViewById(R.id.button_Buzzer_Area1);
        Button button_area2 = container.findViewById(R.id.button_Buzzer_Area2);
        Button button_area3 = container.findViewById(R.id.button_Buzzer_Area3);


        //스레드 위치

        imageView_Motion.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                imageView_Motion.setVisibility(View.INVISIBLE);
            }
        });

        imageView_Motion2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                imageView_Motion2.setVisibility(View.INVISIBLE);
            }
        });

        imageView_Motion3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                imageView_Motion3.setVisibility(View.INVISIBLE);
            }
        });

        button_realtime.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
//                LayoutInflater inflater = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
//                View Srt_layout = (View)inflater.inflate(R.layout.activity_srtplayer,null);
//                addContentView(Srt_layout,new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT));
//                inflater.inflate(R.layout.activity_srtplayer,container,true);
                Intent intent = new Intent(getApplicationContext(),SRTplayerActivity.class);
                startActivity(intent);
            }
            });

        button_opendir.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(getApplicationContext(), RegisterActivity.class);
                startActivity(intent);
            }
        });

        button_buzzer.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                    LinearLayout subViewLayout = subView.findViewById(R.id.layout_buzzer_selector);
                    if(!Viewstate) {
                        subViewLayout.setVisibility(View.VISIBLE);
                        Viewstate = true;
                    }else{
                        subViewLayout.setVisibility(View.GONE);
                        Viewstate = false;
                    }
                }
            });
        button_area1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {

                        if (!ButtonState) {
                            SendSignal(TCP_IP, PORT_WRITE, 1);
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    button_buzzer.setImageDrawable(getDrawable(R.drawable.speaker_yellow));

                                }
                            });
                            ButtonState = true;


                        }else{
                            SendSignal(TCP_IP, PORT_WRITE, 0);
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    button_buzzer.setImageDrawable(getDrawable(R.drawable.speaker_white));

                                }
                            });
                            ButtonState = false;

                        }
                    }
                }).start();
            }
        });


        button_area2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {

                        if (!ButtonState) {
                            SendSignal(TCP_IP, PORT_WRITE, 2);
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    button_buzzer.setImageDrawable(getDrawable(R.drawable.speaker_yellow));

                                }
                            });
                            ButtonState = true;

                        }else{
                            SendSignal(TCP_IP, PORT_WRITE, 0);
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    button_buzzer.setImageDrawable(getDrawable(R.drawable.speaker_white));

                                }
                            });
                            ButtonState = false;

                        }
                    }
                }).start();
            }
        });


        button_area3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {

                        if (!ButtonState) {
                            SendSignal(TCP_IP, PORT_WRITE, 3);
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    button_buzzer.setImageDrawable(getDrawable(R.drawable.speaker_yellow));

                                }
                            });
                            ButtonState = true;

                        }else{
                            SendSignal(TCP_IP, PORT_WRITE, 0);
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    button_buzzer.setImageDrawable(getDrawable(R.drawable.speaker_white));

                                }
                            });
                            ButtonState = false;

                        }
                    }
                }).start();
            }
        });
    }



    @Override
    protected void onResume() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                int Recv_data = 0;
                int divide_16 =0, divide_4 =0;
                int Area1_data=0, Area2_data=0, Area3_data=0;
//                int motion_data1=0, motion_data2=0, motion_data3=0;
                while (true) {
                    Recv_data = ReadSignal(TCP_IP, PORT_READ);
                    Area1_data = Recv_data / 16;
                    divide_16 = Recv_data % 16;
                    Area2_data = divide_16 / 4;
                    divide_4 = Recv_data % 4;
                    Area3_data = divide_4;
//                    Area1_data = Recv_data / 100;
//                    divide_100 = Recv_data % 100;
//                    Area2_data = divide_100 / 10;
//                    divide_10 = Recv_data % 10;
//                    Area3_data = divide_10;
//                    if(divide_10 == 0){
//                        temp_data = 0;
//                    }else if(divide_10 == 1){
//                        temp_data = divide_10 - 1;
//                    }else {
//                        temp_data = divide_10 - 2;
//                    }
//                    motion_data1 = temp_data & 1;
//                    motion_data2 = temp_data & 2;
//                    motion_data3 = temp_data & 4;

                    //Area1
                    switch (Area1_data) {
                        case 0:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Illegal.setVisibility(View.INVISIBLE);
                                    imageView_Motion.setVisibility(View.INVISIBLE);
                                }
                            });
                            break;
                        case 1:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Illegal.setVisibility(View.VISIBLE);
                                    imageView_Illegal.setImageDrawable(getDrawable(R.drawable.illegarcar_red));
                                }
                            });
                            break;
                        case 2:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Illegal.setVisibility(View.VISIBLE);
                                    imageView_Illegal.setImageDrawable(getDrawable(R.drawable.illegarcar_white));
                                }
                            });
//                            if (motion_data1 == 1) {
//                                runOnUiThread(new Runnable() {
//                                    @Override
//                                    public void run() {
//                                        imageView_Motion.setVisibility(View.VISIBLE);
//                                    }
//                                });
//
//                            }
//                            else {
//                                runOnUiThread(new Runnable() {
//                                    @Override
//                                    public void run() {
//                                        imageView_Motion.setVisibility(View.INVISIBLE);
//                                    }
//                                });
//
//                            }
                            break;
                        case 3:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Motion.setVisibility(View.VISIBLE);
                                }
                            });
                            break;
                        default:
                            break;
                    }

                    //Area2
                    switch (Area2_data) {
                        case 0:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Illegal2.setVisibility(View.INVISIBLE);
                                    imageView_Motion2.setVisibility(View.INVISIBLE);
                                }
                            });
                            break;
                        case 1:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Illegal2.setVisibility(View.VISIBLE);
                                    imageView_Illegal2.setImageDrawable(getDrawable(R.drawable.illegarcar_red));
                                }
                            });
                            break;
                        case 2:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Illegal2.setVisibility(View.VISIBLE);
                                    imageView_Illegal2.setImageDrawable(getDrawable(R.drawable.illegarcar_white));
                                }
                            });
//                            if (motion_data2 == 2) {
//                                runOnUiThread(new Runnable() {
//                                    @Override
//                                    public void run() {
//                                        imageView_Motion2.setVisibility(View.VISIBLE);
//                                    }
//                                });
//
//                            }
//                            else {
//                                runOnUiThread(new Runnable() {
//                                    @Override
//                                    public void run() {
//                                        imageView_Motion2.setVisibility(View.INVISIBLE);
//                                    }
//                                });
//
//                            }
                            break;
                        case 3:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Motion2.setVisibility(View.VISIBLE);
                                }
                            });
                            break;
                        default:
                            break;
                    }

                    //Area3
                    switch (Area3_data) {
                        case 0:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Illegal3.setVisibility(View.INVISIBLE);
                                    imageView_Motion3.setVisibility(View.INVISIBLE);
                                }
                            });
                            break;
                        case 1:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Illegal3.setVisibility(View.VISIBLE);
                                    imageView_Illegal3.setImageDrawable(getDrawable(R.drawable.illegarcar_red));
                                }
                            });
                            break;
                        case 2:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Illegal3.setVisibility(View.VISIBLE);
                                    imageView_Illegal3.setImageDrawable(getDrawable(R.drawable.illegarcar_white));
                                }
                            });
//                            if (motion_data3 == 4) {
//                                runOnUiThread(new Runnable() {
//                                    @Override
//                                    public void run() {
//                                        imageView_Motion3.setVisibility(View.VISIBLE);
//                                    }
//                                });
//                            }
//                            else {
//                                runOnUiThread(new Runnable() {
//                                    @Override
//                                    public void run() {
//                                        imageView_Motion3.setVisibility(View.INVISIBLE);
//                                    }
//                                });
//                            }
                            break;
                        case 3:
                            runOnUiThread(new Runnable() {
                                @Override
                                public void run() {
                                    imageView_Motion3.setVisibility(View.VISIBLE);
                                }
                            });
                            break;
                        default:
                            break;
                    }

                }
            }
        }).start();

        super.onResume();


    }

    public void SendSignal(String IP, int portNumber, int data){
        try {
            Socket socket = new Socket(IP,portNumber);
            if(socket.isConnected()){
                Log.d("TCP_Connect","TCP connected");
            }
            DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());
            dataOutputStream.writeByte(data);
            Log.d("TCP_wrtie","Write data:"+data);
            dataOutputStream.flush();

            socket.close();
            if(socket.isClosed()){
                Log.d("TCP_Close","TCP closed");
            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    public int ReadSignal(String IP, int portNumber){
        try {
            Socket socket = new Socket(IP,portNumber);
            if(socket.isConnected()){
                Log.d("TCP_Connect","TCP connected");
            }
            DataInputStream dataInputStream = new DataInputStream(socket.getInputStream());
            int data = dataInputStream.readUnsignedByte();
            Log.d("TCP_Received","Received data:"+data);
            socket.close();
            if(socket.isClosed()){
                Log.d("TCP_Close","TCP closed");
            }
            return data;
        }catch (Exception e){
            e.printStackTrace();
        }
        return 0;
    }

}