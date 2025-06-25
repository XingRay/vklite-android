package io.github.xingray.vulkandemo

import android.Manifest
import android.content.Context
import android.content.Intent
import android.content.pm.PackageManager
import android.graphics.Color
import android.net.Uri
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.provider.Settings
import android.view.Gravity
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.FrameLayout
import androidx.activity.result.ActivityResultLauncher
import androidx.activity.result.contract.ActivityResultContracts
import androidx.core.content.ContextCompat
import androidx.core.view.ViewCompat
import com.google.androidgamesdk.GameActivity


class MainActivity : GameActivity() {

    companion object {
        init {
            System.loadLibrary("vulkan_demo")
        }

        @JvmStatic
        private val TAG = MainActivity::class.java.simpleName

        private const val REQUEST_CODE_STORAGE_PERMISSION: Int = 100

    }

    private var mPermissionRequest = SinglePermissionRequest(android.Manifest.permission.WRITE_EXTERNAL_STORAGE)
//    private var mPermissionRequest = SinglePermissionRequest(android.Manifest.permission.CAMERA)

    private lateinit var manageAllFilesPermissionLauncher: ActivityResultLauncher<Intent>

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            if (hasManageExternalStoragePermission(baseContext)) {
//                execTasks()
            } else {
                requestExternalStoragePermission(baseContext)
            }
        } else {
            mPermissionRequest.register(this)
            mPermissionRequest.perform(this, null) {
//                execTasks()
            }
        }
    }

    override fun onCreateSurfaceView() {
        this.mSurfaceView = InputEnabledSurfaceView(this)
        this.mSurfaceView.layoutParams = FrameLayout.LayoutParams(9 * 116, 16 * 116);
        val frameLayout = FrameLayout(this)
        frameLayout.layoutParams = ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
        frameLayout.setPadding(20, 100, 20, 100)
        frameLayout.setBackgroundColor(Color.GRAY)
        this.contentViewId = View.generateViewId()
        frameLayout.id = contentViewId

        // 添加 SurfaceView
        frameLayout.addView(this.mSurfaceView)

        // 添加按钮
        val button = Button(this)
        button.text = "Change Color"
        button.layoutParams = FrameLayout.LayoutParams(
            FrameLayout.LayoutParams.WRAP_CONTENT,
            FrameLayout.LayoutParams.WRAP_CONTENT
        ).apply {
            gravity = Gravity.BOTTOM or Gravity.CENTER_HORIZONTAL
            bottomMargin = 50
        }
        frameLayout.addView(button)

        // 设置点击事件
        button.setOnClickListener {
            changeTriangleColor(gameActivityNativeHandle, randomColor())
        }

        this.setContentView(frameLayout)
        frameLayout.requestFocus()
        mSurfaceView.holder.addCallback(this)
        ViewCompat.setOnApplyWindowInsetsListener(this.mSurfaceView, this)
    }

    fun randomColor(): Int {
        // 定义可选的颜色数组
        val colors = arrayOf(
            Color.BLACK,
            Color.DKGRAY,
            Color.GRAY,
            Color.LTGRAY,
            Color.WHITE,
            Color.RED,
            Color.GREEN,
            Color.BLUE,
            Color.YELLOW,
            Color.CYAN,
            Color.MAGENTA
        )
        // 从数组中随机选择一个颜色并返回
        return colors.random()
    }


    private fun requestExternalStoragePermission(context: Context) {
        manageAllFilesPermissionLauncher = registerForActivityResult(
            ActivityResultContracts.StartActivityForResult()
        ) {
            // 在这里检查权限状态
            if (hasManageExternalStoragePermission(this)) {
                // 权限已授予
                //execTasks()
            } else {
                // 权限未授予
                showMessage("程序运行所需要的权限没有被授予， 2秒后退出")
                runOnUiThreadDelayed(2000) {
                    finish()
                }
            }
        }

        val intent = Intent(Settings.ACTION_MANAGE_APP_ALL_FILES_ACCESS_PERMISSION)
        intent.data = Uri.parse("package:" + context.packageName)
        manageAllFilesPermissionLauncher.launch(intent)
    }

    fun hasManageExternalStoragePermission(context: Context): Boolean {
        return if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            Environment.isExternalStorageManager()
        } else {
            ContextCompat.checkSelfPermission(
                context,
                Manifest.permission.MANAGE_EXTERNAL_STORAGE
            ) == PackageManager.PERMISSION_GRANTED
        }
    }

    private external fun changeTriangleColor(nativeHandle: Long, color: Int)
}