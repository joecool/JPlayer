package cc.co.androidzoo.jplayer;

import java.util.Random;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.os.Handler;
import android.os.SystemClock;
import android.os.Message;
import android.util.AttributeSet;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;

public class MovieSurface extends SurfaceView {
    private TextView mTextView = null;
    protected SurfaceHolder mSurfaceHolder;

    final private int SHOW_FPS = 0;
    
    private Integer mFps = 0;
    public Integer mFpsShow = 0;
    
    private int mRunType = 0;
    
    Handler mHandler = new Handler(){
        @Override
            public void handleMessage(Message msg) {
            switch (msg.what) {
            case SHOW_FPS:
                mHandler.sendEmptyMessageDelayed(SHOW_FPS, 1000);
                Log.v("FlickerSurface", "Receive 1s tick.");
                synchronized(mFps){
                    mFpsShow = mFps;
                    mFps = 0;
                }
                if(null != mTextView) mTextView.setText(mFpsShow.toString());
            }
        }
        };
    
    public void setTextView(TextView textview){
        mTextView = textview;
    }
    
    public MovieSurface(Context context) {
        super(context);
        initView();
    }
    
    public MovieSurface(Context context, AttributeSet attrs){
        super(context, attrs);
        initView();
    }

    private void initView() {
        getHolder().addCallback(mSHCallback);
        setFocusable(true);
        setFocusableInTouchMode(true);
        requestFocus();
    }
    
    
    public void showFps(boolean isShow){
        if(isShow){
            if(null != mTextView) mTextView.setVisibility(View.VISIBLE);
            mHandler.sendEmptyMessageDelayed(SHOW_FPS, 1000);
        }else{
            if(null != mTextView) mTextView.setVisibility(View.GONE);
            mHandler.removeMessages(SHOW_FPS);
        }
    }
    
    SurfaceHolder.Callback mSHCallback = new SurfaceHolder.Callback()
        {
            public void surfaceChanged(SurfaceHolder holder, int format,
                                       int w, int h)
            {

            }

            public void surfaceCreated(SurfaceHolder holder)
            {
                mSurfaceHolder = holder;
                new Thread(new UpdateThread()).start();  
            }

            public void surfaceDestroyed(SurfaceHolder holder)
            {
                // after we return from this we can't use the surface any more
                mSurfaceHolder = null;
            }
        
            class UpdateThread implements Runnable{  

            
                //            @Override  
                public void run() {  
                
                Canvas canvas = mSurfaceHolder.lockCanvas(null);
                Paint mPaint = new Paint();  
                int h = canvas.getHeight();
                int w = canvas.getWidth();  
                mSurfaceHolder.unlockCanvasAndPost(canvas);
                mHandler.sendEmptyMessageDelayed(SHOW_FPS, 1000);
                
                Log.v("FlickerSurface", "Entering run");
                while(true){
                    if(null != mSurfaceHolder)
                        canvas = mSurfaceHolder.lockCanvas(null);
                    
                    switch(mRunType){
                    case 0:
                        runRandomText(mPaint, canvas, w, h);
                        break;
                        
                    case 1:
                        break;
                        
                    default:
                    }
                    synchronized(mFps){
                        mFps++;
                    }
                        
                    if(null != mSurfaceHolder && null != canvas)
                        mSurfaceHolder.unlockCanvasAndPost(canvas);
                    try {
                        Thread.sleep(30);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }  
              
                private void runRandomText(Paint paint, Canvas canvas, int weight, int height){
                    Random rd = new Random();
                    rd.setSeed(SystemClock.currentThreadTimeMillis());
                    for(int i = 0; i < weight; i+=weight/10)
                        {
                            for(int j = 0; j < height; j+=height/15){
                                if(rd.nextBoolean()){
                                    paint.setColor(Color.WHITE);
                                }else{
                                    paint.setColor(Color.BLACK);
                                }
                                canvas.drawRect(new RectF(i,j,i+weight/10,j+height/10), paint);
                            }
                        }
                }
            
            }  
          
        };

}
