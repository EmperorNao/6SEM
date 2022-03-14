package com.example.animeapp

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView


class AnimeViewHolder(itemView: View, private val onItemClicked: (position: Int) -> Unit) : RecyclerView.ViewHolder(itemView), View.OnClickListener {

    var animeTextView: TextView? = null
    var animeImageView: ImageView? = null

    override fun onClick(v: View) {
        val position = adapterPosition
        onItemClicked(position)
    }

    init {
        itemView.setOnClickListener(this)
        animeTextView = itemView.findViewById(R.id.text)
        animeImageView = itemView.findViewById(R.id.image)
    }

}


class AnimeAdapter(_anime: Array<Anime>, private val onItemClicked: (position: Int) -> Unit): RecyclerView.Adapter<AnimeViewHolder>() {

    var anime: Array<Anime> = emptyArray()

    init {
        anime = _anime
    }

    override fun getItemCount(): Int {
        return anime.size
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): AnimeViewHolder {
        val layoutInflater =
            LayoutInflater.from(parent.getContext())
                
        return AnimeViewHolder(
            layoutInflater.inflate(
                R.layout.anime_item,
                parent,
                false
            ),
            onItemClicked
        )

    }

    override fun onBindViewHolder(holder: AnimeViewHolder, position: Int) {

        val t: Anime = anime.get(position)
        val name: String = t.get_name()
        val id = t.get_image_id()

        //val draw = Drawable.createFromRes
        if (id != null) {
            holder.animeImageView?.setImageResource(id)
        }
        holder.animeTextView?.setText(name)
        //holder.animeImageView?.setImageBitmap(bmImg)

    }


}