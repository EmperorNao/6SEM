package com.example.animeapp

import android.graphics.Bitmap


class Anime(_id: Int, name: String) {

    var name: String = ""
    var image : Bitmap? = null
    var id: Int = 0

    init {
        id = _id
        this.name = name
    }

    fun get_name(): String {
        return name
    }

    fun set_image(_image: Bitmap) {
        image = _image
    }

    fun get_image(): Bitmap {
        return image!!
    }

    fun get_id(): Int {
        return id
    }

}