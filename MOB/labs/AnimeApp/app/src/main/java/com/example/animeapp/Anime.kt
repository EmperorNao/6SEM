package com.example.animeapp

import android.graphics.Bitmap


class Anime(_id: Int, name: String, url: String) {

    var name: String = ""
    var image : Bitmap? = null
    var id: Int = 0
    var url: String = ""

    init {
        id = _id
        this.name = name
        this.url = url
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

    fun get_url() :String {
        return url
    }

    fun get_id(): Int {
        return id
    }

}