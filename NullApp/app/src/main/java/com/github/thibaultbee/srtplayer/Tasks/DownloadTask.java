package com.github.thibaultbee.srtplayer.Tasks;

import android.app.ProgressDialog;
import android.content.Context;
import android.os.AsyncTask;
import android.os.Environment;
import android.os.PowerManager;
import android.util.Log;
import android.widget.Toast;

import com.jcraft.jsch.ChannelSftp;


import com.github.thibaultbee.srtplayer.ftp.DirectoryElement;
import com.github.thibaultbee.srtplayer.ftp.Globals;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;


public class DownloadTask extends AsyncTask<DirectoryElement, Integer, String> {

    private Context context;
    private PowerManager.WakeLock mWakeLock;
    ProgressDialog mProgressDialog = Globals.mProgressDialogDownload;

    public DownloadTask(Context context) {
        this.context = context;
    }


    @Override
    protected String doInBackground(DirectoryElement... params) {
        BufferedInputStream bis = null;
        BufferedOutputStream bos = null;
        DirectoryElement toDownload = params[0];

        ChannelSftp channelSftp = (ChannelSftp) Globals.channel;
        long progress = 0;
        float percentage = 0;
        long size = 0;
        try {
            byte[] buffer = new byte[1024];
            bis = new BufferedInputStream(channelSftp.get(toDownload.getName()));
            File f = new File(Environment.getExternalStorageDirectory() + "/" + Globals.downloadFolder);
            if ( !f.exists() ) {
                f.mkdirs();
            }

            File newFile = new File(Environment.getExternalStorageDirectory() + "/" + Globals.downloadFolder + "/" + toDownload.getShortname());

            OutputStream os = new FileOutputStream(newFile);
            bos = new BufferedOutputStream(os);
            int readCount;
            size = toDownload.getSize();

            while ((readCount = bis.read(buffer)) > 0) {
                bos.write(buffer, 0, readCount);
                progress += buffer.length;
                percentage = progress * 100 / size;
                Log.d("DOWNLOAD", "size: " + size);
                Log.d("DOWNLOAD", "progress: " + progress);
                Log.d("DOWNLOAD", "Writing: " + percentage + "%");

                publishProgress((int) percentage);
            }

        } catch (Exception e) {
            return e.toString();
        } finally {
            try {
                if (bis != null)
                    bis.close();
                if (bos != null)
                    bos.close();
            } catch (IOException ignored) {
            }
        }
        return null;
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();

        // take CPU lock to prevent CPU from going off if the user
        // presses the power button during download
        PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
        try {
            mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, getClass().getName());
            mWakeLock.acquire(10*60);
        }catch (NullPointerException e){
            e.printStackTrace();
        }

        mProgressDialog.show();
    }

    @Override
    protected void onProgressUpdate(Integer... progress) {
        super.onProgressUpdate(progress);
        // if we get here, length is known, now set indeterminate to false
        mProgressDialog.setIndeterminate(false);
        mProgressDialog.setMax(100);
        mProgressDialog.setProgress(progress[0]);
    }

    @Override
    protected void onPostExecute(String result) {
        mWakeLock.release();
        mProgressDialog.dismiss();
        if (result != null)
            Toast.makeText(context, "Download error: " + result, Toast.LENGTH_LONG).show();
        else
            Toast.makeText(context, "File downloaded in: " + Environment.getExternalStorageDirectory() + "/" + Globals.downloadFolder, Toast.LENGTH_LONG).show();
    }
}