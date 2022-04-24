package com.example.mmoapp

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity


class FactsActivity : AppCompatActivity(), InfoUser {

    var id: Int = -1 // to cause errors

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_facts)

            id = intent.getIntExtra("id", 0)


    }

    override fun currentId(): Int {
        return id
    }

}