package com.example.mmoapp

import android.graphics.Bitmap
import androidx.room.*


@Entity
class Anime(id: Int, title: String, url: String) {

    @PrimaryKey var id: Int
    @ColumnInfo(name="title") var title: String
    @ColumnInfo(name="url") var url: String
    @ColumnInfo(name="genre") var genre: String? = null
    @ColumnInfo(name="status") var status: String? = null
    @ColumnInfo(name="platform") var platform: String? = null
    @ColumnInfo(name="developer") var developer: String? = null
    @ColumnInfo(name="release") var release: String? = null
    @ColumnInfo(name="description") var description: String? = null

    init {
        this.id = id
        this.title = title
        this.url = url
    }


    fun get_name(): String {
        return title
    }

    fun get_url() :String {
        return url
    }

    fun get_id(): Int {
        return id
    }

    fun get_genre(): String? {
        return genre
    }

    fun get_status() :String? {
        return status
    }

    fun get_platform(): String? {
        return platform
    }

    fun get_developer(): String? {
        return developer
    }

    fun get_release() :String? {
        return release
    }

    fun get_description(): String? {
        return description
    }

    // setters

    fun set_name(value_: String) {
        title = value_
    }

    fun set_url(value_: String) {
        url = value_
    }

    fun set_id(value_: Int) {
        id = value_
    }

    fun set_genre(value_: String) {
        genre = value_
    }

    fun set_status(value_: String) {
        status = value_
    }

    fun set_platform(value_: String) {
        platform = value_
    }

    fun set_developer(value_: String) {
        developer = value_
    }

    fun set_release(value_: String) {
        release = value_
    }

    fun set_description(value_: String) {
        description = value_
    }

}



@Dao
interface AnimeDao {
    @Query("SELECT * FROM Anime")
    fun getAll(): MutableList<Anime>

    @Query("SELECT * FROM Anime WHERE id IN (:AnimeIds)")
    fun loadAllByIds(AnimeIds: IntArray): List<Anime>

    @Insert
    fun insertAll(animes: MutableList<Anime>)

    @Delete
    fun delete(Anime: Anime)
}



@Database(entities = [Anime::class], version = 1)
abstract class AnimeDatabase : RoomDatabase() {
    abstract fun animeDao(): AnimeDao
}