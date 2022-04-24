package com.example.mmoapp

import android.graphics.BitmapFactory
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import okhttp3.*
import java.io.IOException




class GameViewHolder(itemView: View, private val onItemClicked: (position: Int) -> Unit)
    : RecyclerView.ViewHolder(itemView), View.OnClickListener {

    var gameTextView: TextView
    var gameImageView: ImageView

    init {
        itemView.setOnClickListener(this)
        gameTextView = itemView.findViewById(R.id.text)
        gameImageView = itemView.findViewById(R.id.image)
    }

    override fun onClick(v: View) {
        val position = adapterPosition
        onItemClicked(position)
    }

}


class GameAdapter(private var client: OkHttpClient, private val onItemClicked: (position: Int) -> Unit): RecyclerView.Adapter<GameViewHolder>() {

    private var games: MutableList<Game> = mutableListOf()

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): GameViewHolder {
        val layoutInflater =
            LayoutInflater.from(parent.context)

        return GameViewHolder(
            layoutInflater.inflate(
                R.layout.game_item,
                parent,
                false
            ),
            onItemClicked
        )

    }

    override fun onBindViewHolder(holder: GameViewHolder, position: Int) {

        downloadImage(holder, games[position].get_url())
        holder.gameTextView.text = games[position].get_name()

    }

    fun setData(games: MutableList<Game>) {
        this.games = games
    }

    fun getData(): List<Game> {
        return games
    }

    override fun getItemCount(): Int {
        return games.size
    }

    private fun downloadImage(holder: GameViewHolder, url: String) {

        val request = Request.Builder()
            .url(url)
            .build()

        client.newCall(request).enqueue(object : Callback {
            override fun onFailure(call: Call, e: IOException) {
                e.printStackTrace()
            }

            override fun onResponse(call: Call, response: Response) {
                response.use {

                    if (!response.isSuccessful) throw IOException("Unexpected code $response")
                    val bitmap = BitmapFactory.decodeStream(response.body!!.byteStream())
                    holder.gameImageView.post {
                        holder.gameImageView.setImageBitmap(bitmap)
                    }

                }

            }

        })

    }


}