package com.example.animeapp

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView

class FactsActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_facts)

        getSupportActionBar()?.setDisplayHomeAsUpEnabled(true);

        val anime_name = intent.getIntExtra("anime_id", 0)


        var anime_facts: Map<Int, Int> = mapOf(
            0 to R.array.sao_facts,
            1 to R.array.knb_facts
        )

        val facts = anime_facts.get(anime_name)?.let { resources.getStringArray(it) }

        val adapter = facts?.let { FactsAdapter(it) }

        val recyclerView = findViewById<RecyclerView>(R.id.facts_recycler)
        recyclerView.layoutManager = LinearLayoutManager(this)
        recyclerView.adapter = adapter


    }
}