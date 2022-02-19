package com.example.start

import android.content.Context
import android.graphics.*
import android.util.AttributeSet
import android.view.View
import android.graphics.Typeface
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.drawable.Drawable
import android.content.res.Resources

import android.graphics.Picture
import android.view.accessibility.AccessibilityNodeInfo
import androidx.core.content.withStyledAttributes
import androidx.core.view.AccessibilityDelegateCompat
import androidx.core.view.ViewCompat
import androidx.core.view.accessibility.AccessibilityNodeInfoCompat
// import android.view.accessibility.AccessibilityNodeInfo
import kotlin.math.cos
import kotlin.math.min
import kotlin.math.sin
// import androidx.core.view.AccessibilityDelegateCompat
// import androidx.core.view.ViewCompat
// import androidx.core.view.accessibility.AccessibilityNodeInfoCompat
import android.util.Log

class DialView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : View(context, attrs, defStyleAttr) {

    private val paint = Paint(Paint.ANTI_ALIAS_FLAG).apply {
        // Paint styles used for rendering are initialized here. This
        // is a performance optimization, since onDraw() is called
        // for every screen refresh.
        style = Paint.Style.FILL
        textAlign = Paint.Align.CENTER
        textSize = 55.0f
        typeface = Typeface.create("", Typeface.BOLD)
    }

    private val pointPosition: PointF = PointF(0.0f, 0.0f)

    init {



    }



    override fun onDraw(canvas: Canvas) {

        canvas.drawText("123 РУБ", (width).toFloat(), (height).toFloat(), paint)

        var d: Drawable = getResources().getDrawable(R.drawable.test, null);
        d.setBounds(pointPosition.x.toInt(), pointPosition.y.toInt(), pointPosition.x.toInt() + 400, pointPosition.y.toInt() + 400);
        d.draw(canvas);
        Log.e("A", width.toString() + " " +  height.toString())
        Log.e("A", pointPosition.x.toString() + " " + pointPosition.y.toString())

    }

}