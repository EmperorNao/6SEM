package com.example.animeapp

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView


class FactsViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {

    var factTextView: TextView? = null

    init {
        factTextView = itemView.findViewById(R.id.text_fact)
    }

}


class FactsAdapter(facts: Array<String>): RecyclerView.Adapter<FactsViewHolder>() {

    var anime_facts: Array<String> = emptyArray()

        init {
            anime_facts = facts
        }

    override fun getItemCount(): Int {
        return anime_facts.size
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): FactsViewHolder {
        val layoutInflater =
            LayoutInflater.from(parent.getContext())

        return FactsViewHolder(
            layoutInflater.inflate(
                R.layout.fact_item,
                parent,
                false
            )
        )

    }

    override fun onBindViewHolder(holder: FactsViewHolder, position: Int) {

        val fact: String = anime_facts.get(position)
        holder.factTextView?.setText((position + 1).toString() + ") " + fact)

    }

}