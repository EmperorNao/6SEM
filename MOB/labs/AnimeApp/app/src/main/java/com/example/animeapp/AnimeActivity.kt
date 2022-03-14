package com.example.animeapp

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.recyclerview.widget.GridLayoutManager
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView


fun map_from_pairs(pairs :Array<String>): Map<String, String> {

    var map: MutableMap<String, String> = mutableMapOf<String, String>()

    for (tag in pairs) {
        val pair = tag.split("|").toTypedArray()
        val key = pair[0]
        val value = pair[1]
        map[key] = value
    }
    return map

}


class AnimeActivity : AppCompatActivity() {

    var anime: Array<Anime> = emptyArray()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_anime)

//        val anime_pairs = resources.getStringArray(R.array.anime_names)
//        val anime_images = map_from_pairs(anime_pairs)
//        val anime_names = anime_images.keys

        var anime_images: Map<String, Int> = mapOf(
            "Sword Art Online" to R.drawable.sao,
            "Kuroko no Basuke" to R.drawable.knb,
            "Death Note" to R.drawable.note,
            "Berserk" to R.drawable.berserk,
            "Code Geass" to R.drawable.geass,
            "Hajime no Ippo" to R.drawable.ippo,
            "Initial D" to R.drawable.d,
            "JoJo Bizzare Adventure" to R.drawable.jjba,
            "Atack on Titan" to R.drawable.aot,
        )


        anime = arrayOf(
            Anime(0, "Sword Art Online", R.drawable.sao),
            Anime(1, "Kuroko no Basuke", R.drawable.knb),
            Anime(2, "Death Note", R.drawable.note),
            Anime(3, "Berserk", R.drawable.berserk),
            Anime(4, "Code Geass", R.drawable.geass),
            Anime(5, "Hajime no Ippo", R.drawable.ippo),
            Anime(6, "Initial D", R.drawable.d),
            Anime(7, "JoJo Bizzare Adventure", R.drawable.jjba),
            Anime(8, "Atack on Titan", R.drawable.aot)
        )


        val adapter = AnimeAdapter(anime, {position -> onListItemClick(position) })
        val recyclerView = findViewById<RecyclerView>(R.id.main_recycler)
        recyclerView.layoutManager = GridLayoutManager(this, resources.getInteger(R.integer.nrows))
        //recyclerView.layoutManager = LinearLayoutManager(this)
        recyclerView.adapter = adapter

    }

    private fun onListItemClick(position: Int) {

        val intent = Intent(this, FactsActivity::class.java)
        intent.putExtra("anime_id", anime[position].get_id() );
        startActivity(intent)

    }


}