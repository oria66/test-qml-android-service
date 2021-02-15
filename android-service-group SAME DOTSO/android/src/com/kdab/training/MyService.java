// java file android/src/com/kdab/training/MyService.java
package com.kdab.training;
import android.util.Log;
import android.content.Context;
import android.content.Intent;
import org.qtproject.qt5.android.bindings.QtService;

public class MyService extends QtService
{
    public static void startMyService(Context ctx) {
        ctx.startService(new Intent(ctx, MyService.class));

        Log.d("Service starts", "here");

    }

    public static void stopMyService(Context ctx) {
        ctx.stopService(new Intent(ctx, MyService.class));

        Log.d("Service stops", "here");

    }

//    private static final String TAG = "MyService";

//    @Override
//    public void onCreate() {
//        Log.i(TAG, "Creating Service");
//        super.onCreate();
//    }

//    @Override
//    public void onDestroy() {
//        Log.i(TAG, "Destroying Service");
//        super.onDestroy();
//    }
}
