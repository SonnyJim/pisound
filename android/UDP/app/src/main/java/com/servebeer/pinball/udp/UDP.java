package com.servebeer.pinball.udp;

import android.app.Activity;
import android.os.AsyncTask;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.NumberPicker;
import android.widget.Spinner;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;


public class UDP extends ActionBarActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_udp);
        addSceneSpinner();
        SetNumberPickers();
    }

    public void SetNumberPickers () {
        NumberPicker SndPicker = (NumberPicker) findViewById(R.id.SndPicker);

        SndPicker.setMinValue(0);
        SndPicker.setMaxValue(255);
        SndPicker.setValue(0);
        SndPicker.setWrapSelectorWheel(false);

        NumberPicker MusPicker = (NumberPicker) findViewById(R.id.MusPicker);

        MusPicker.setMinValue(0);
        MusPicker.setMaxValue(255);
        MusPicker.setValue(0);
        MusPicker.setWrapSelectorWheel(false);
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_ud, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    public void SendPing(View view) {

        new UDPSendTask().execute("fe");
    }

    public void SendShutdown(View view) {
        new UDPSendTask().execute("fc");
    }

    public void SendCustomMsg(View view) {
        //Get message from CustomText
        EditText editText = (EditText) findViewById(R.id.CustomText);
        String message = editText.getText().toString();

        new UDPSendTask().execute(message);
    }

    public void SendSnd(View view) {

        NumberPicker SndPicker = (NumberPicker) findViewById(R.id.SndPicker);
        String message = "00" + SndPicker.getValue();
        new UDPSendTask().execute(message);
    }

    public void SendMus(View view) {
        NumberPicker MusPicker = (NumberPicker) findViewById(R.id.MusPicker);
        String message = "01" + MusPicker.getValue();
        new UDPSendTask().execute(message);
    }

    private class UDPSendTask extends AsyncTask<String, Void, Integer> {
        protected Integer doInBackground(String... params) {
            try {

                String messageStr = params[0];
                int server_port = 8008;
                DatagramSocket s = new DatagramSocket();
                InetAddress local = InetAddress.getByName("192.168.1.80");
                int msg_length = messageStr.length();
                byte[] message = messageStr.getBytes();
                DatagramPacket p = new DatagramPacket(message, msg_length, local,
                        server_port);
                s.send(p);
            } catch (Exception e) {
                Log.e("UDP", "exception", e);
            }
            return 1;
        }
    }

    public void addSceneSpinner() {
        //Populate scene Spinner
        Spinner spinner = (Spinner) findViewById(R.id.spinner);
        // Create an ArrayAdapter using the string array and a default spinner layout
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
                R.array.scenes_array, android.R.layout.simple_spinner_item);
        // Specify the layout to use when the list of choices appears
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // Apply the adapter to the spinner
        spinner.setAdapter(adapter);
        //Setup the callback
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            public void onItemSelected(AdapterView<?> parent, View view, int pos, long id) {
                //Build scene change message
                String message = "fb" + "0" + pos;
                new UDPSendTask().execute(message);
            }

            public void onNothingSelected(AdapterView<?> parent) {
                // Another interface callback
            }
        });
     }

}
