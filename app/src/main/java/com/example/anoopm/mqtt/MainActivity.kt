package com.example.anoopm.mqtt


import android.os.Bundle
import android.view.View
import android.widget.CheckBox
import android.widget.EditText
import android.widget.ToggleButton
import androidx.appcompat.app.AppCompatActivity
import com.example.anoopm.mqtt.manager.MQTTConnectionParams
import com.example.anoopm.mqtt.manager.MQTTmanager
import com.example.anoopm.mqtt.protocols.UIUpdaterInterface
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity(), UIUpdaterInterface {

    var mqttManager:MQTTmanager? = null

    // Interface methods
    override fun resetUIWithConnection(status: Boolean) {

        ipAddressField.isEnabled  = !status
        topicField.isEnabled      = !status
        messageField.isEnabled    = status
        connectBtn.isEnabled      = !status
        sendBtn.isEnabled         = status
        togBtn.isEnabled          = status



        // Update the status label.
        if (status){
            updateStatusViewWith("Conectado")
        }else{
            updateStatusViewWith("Desconectado")
        }
    }

    override fun updateStatusViewWith(status: String) {
        statusLabl.text = status
    }

    override fun update(message: String) {
                if ((message == "D") || (message == "L"))
                    {}
                else {
                    var text = messageHistoryView.text.toString()
                    var newText = """
            $text
            $message
            """
                    //var newText = text.toString() + "\n" + message +  "\n"
                    messageHistoryView.setText(newText)
                    messageHistoryView.setSelection(messageHistoryView.text.length)
                }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // Enable send button and message textfield only after connection
        resetUIWithConnection(false)



        val toggle = findViewById<View>(R.id.togBtn) as ToggleButton
        toggle.setOnCheckedChangeListener { buttonView, isChecked ->
            if (isChecked) {
                mqttManager?.publish("L")
            } else {
                mqttManager?.publish("D")
            }
        }

        uSuario.isEnabled         = false
        passWord.isEnabled        = false


    }

    fun connect(view: View){

        if (!(ipAddressField.text.isNullOrEmpty() && topicField.text.isNullOrEmpty())) {

            var usuario = findViewById(R.id.uSuario) as EditText
            var uSuario1 =  usuario.getText().toString()

            var password = findViewById(R.id.passWord) as EditText
            var passWord1 =  password.getText().toString()

                    if (!uSuario.isEnabled)
                            {uSuario1 = ""
                             passWord1 = ""   }

            var host = "tcp://" + ipAddressField.text.toString() + ":1883"
            var topic = topicField.text.toString()
            var connectionParams = MQTTConnectionParams(
                "MQTTSample",
                host,
                topic,
                uSuario1,
                passWord1
            )
            mqttManager = MQTTmanager(connectionParams, applicationContext, this)
            mqttManager?.connect()
        }else{
            updateStatusViewWith("Por favor preencha todos os campos.")
        }

    }

    fun sendMessage(view: View){

        mqttManager?.publish(messageField.text.toString())

        messageField.setText("")
    }

    fun quandoClicar(view: View) {

        if (view is CheckBox) {
            val checked: Boolean = view.isChecked
            if (checked) {
                 uSuario.isEnabled         = true
                 passWord.isEnabled        = true

            } else {
                uSuario.isEnabled         = false
                passWord.isEnabled        = false
            }
        }
    }



}
