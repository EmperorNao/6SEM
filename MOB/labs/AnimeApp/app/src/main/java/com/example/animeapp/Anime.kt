package com.example.animeapp


class Anime(_id: Int, name: String, image_id: Int) {

    var anime_name: String = ""
    var anime_image_id: Int = 0
    var id: Int = 0

    init {
        id = _id
        anime_name = name
        anime_image_id = image_id
    }

    fun get_name(): String {
        return anime_name
    }

    fun get_image_id(): Int {
        return anime_image_id
    }

    fun get_id(): Int {
        return id
    }

}