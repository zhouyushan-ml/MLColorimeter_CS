using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
//using System.Drawing;

using OpenCvSharp;
using MLColorimeterCS.Interface;
using MLColorimeterCS.MLCommon;
using MLColorimeterCS.NotifyCameraCallback;
using MLColorimeterCS.NotifyMotionCallback;
using MLColorimeterCS.NotifyFilterWheelCallback;
using System.IO;
using System.Runtime.InteropServices;

namespace MLColorimeter_CSTest
{
    class Program
    {
        static void Main(string[] args)
        {
            //----------------------------------------SDK函数测试----------------------------------------
            MLColorimeterWrapper test = new MLColorimeterWrapper();
            MLColorimeterModuleWrapper mlcolorimeter = test.GetMLColorimeterInstance();
            MLBinoBusinessModuleWrapper businessManage = mlcolorimeter.GetBusinessManageModule();

            string path = "D:/GitProject/Miami/src/app/config/measureCamera/EYE1";
            MLResult ret = businessManage.ML_AddModule(path);
            ret = businessManage.ML_ConnectModules();
            string keyName = "ColorFilter";

            DateTime startTime = DateTime.Now; // 当前时间
            byte[] byteData = businessManage.GetImageByte();
            DateTime endTime = DateTime.Now; // 当前时间
            double differenceInSeconds = (endTime - startTime).TotalSeconds;
            Console.WriteLine($"Mat to byte（秒）：{differenceInSeconds}");

            //startTime = DateTime.Now; // 当前时间
            ImageConversion.SaveImageOpencv(byteData);
            //ImageConversion.SaveImage(byteData);

            DateTime endTime1 = DateTime.Now; // 当前时间
            differenceInSeconds = (endTime1 - startTime).TotalSeconds;
            Console.WriteLine($"save ：{differenceInSeconds}");
            //ImageConversion.OpenCvType type; int rows; int cols; int channels; Array pixelData;
            //ImageConversion.DeserializeMat(byteData, out type, out rows, out cols, out channels, out pixelData);
            //float[] floatData = (float[])pixelData;
            //string savepath = "output_32f.tiff";
            //ImageConversion.Save32FToTiff(floatData, cols, rows, savepath);
            //DateTime endTime2 = DateTime.Now; // 当前时间
            ////TimeSpan timeDifference = endTime - startTime;
            //differenceInSeconds = (endTime2 - endTime).TotalSeconds;
            //// 格式化输出
            ////Console.WriteLine($"时间差格式化：{timeDifference.Days}天 {timeDifference.Hours}小时 {timeDifference.Minutes}分钟 {timeDifference.Seconds}秒");
            //Console.WriteLine($"save image ：{differenceInSeconds}");
            Console.ReadLine(); // 防止程序退出

            //MLFilterEnum filter = MLFilterEnum.X;
            //OperationMode mode = OperationMode.Parallel;
            ////ret = businessManage.ML_MoveModulesND_XYZFilterSync(keyName,filter);
            //ret = businessManage.ML_MoveModulesND_XYZFilterSync(keyName, filter, mode);
            //businessManage.ML_WaitForMovingStop();
            //businessManage.ML_MoveModulesRXFilterSync(keyName, 1);

            //var vid = new Dictionary<int, double> { { 1, 10.5 }, { 2, 20.3 } };
            //var position = new Dictionary<int, double> { { 3, 30.7 }, { 4, 40.1 } };
            //var vid = new Dictionary<int, double>();
            //var position = new Dictionary<int, double>();
            //var focusConfig = new ThroughFocusConfig
            //{
            //    FocusMax = 200.0,
            //    ROIs = new List<MLColorimeterCS.MLCommon.Rect> { new MLColorimeterCS.MLCommon.Rect(0, 0, 100, 100) }
            //};
            //var param = new ThroughFocusParams("Key1", vid, position)
            //{
            //    FocusConfig = focusConfig,
            //    //Mode = OperationMode.Serial
            //};
            //var result = businessManage.ML_ThroughFocus(param);

            //string ndKey = "";
            //string xyzKey = "";
            //CalibrationConfig config = new CalibrationConfig();
            ////config.RX = new RXCombination(2, -1, 30);
            //config.RX.Axis = 2;

            //ExposureSetting exposure = new ExposureSetting(ExposureMode.Auto, 1000.0);
            //bool isColorCamera = false;
            //ret = mlcolorimeter.ML_Measurement(ndKey, xyzKey, config, exposure, isColorCamera, mode);

            ////----------------------------------------回调函数测试----------------------------------------
            ////String imagePath = "D:/Image/img1.tif";
            ////Mat mat = Cv2.ImRead(imagePath, ImreadModes.Unchanged);
            ////if (mat.Empty())
            ////{
            ////    Console.WriteLine("Failed to load image.");
            ////    return;
            ////}
            ////// 获取 Mat 对象的指针
            ////IntPtr matPtr = mat.CvPtr;
            ////MLPixelFormat format = MLPixelFormat.MLMono16;
            ////camera.ChangeImage(matPtr, format);

            //// 创建回调实例
            //NotifyCameraStateChangedDelegate stateChanged = OnCameraStateChanged;
            //NotifyCameraFrameReceivedDelegate frameReceived = OnCameraFrameReceived;
            //NotifyCameraGrayLevelDelegate grayLevel = OnCameraGrayLevel;

            //// 注册回调到 C++/CLI 桥接代码
            //// 假设有一个方法 RegisterCallback，它接受这些委托作为参数
            //ManagedCameraCallback cameracall = new ManagedCameraCallback(stateChanged, frameReceived, grayLevel);

            ////cameracall.changeCameraGrayLevel();

            //cameracall.changeCameraImage();

            ////Console.WriteLine("Callbacks registered. Waiting for events...");
            ////Console.ReadLine(); // 防止程序退出
        }








        // 回调方法：相机状态改变
        static void OnCameraStateChanged(MLCameraState oldState, MLCameraState newState)
        {
            Console.WriteLine($"Camera state changed from {oldState} to {newState}");
        }

        // 回调方法：接收相机帧
        static void OnCameraFrameReceived(IntPtr frame, int width, int height, int channels, MLPixelFormat format)
        {
            Console.WriteLine($"Received frame: {width}x{height}, {channels} channels, format {format}");
            Mat mat = new Mat(frame);
            string filePath = "output.tif";
            mat.SaveImage(filePath);
        }

        // 回调方法：灰度级别改变
        static void OnCameraGrayLevel(int grayLevel)
        {
            Console.WriteLine($"Camera gray level: {grayLevel}");
        }
    }
}