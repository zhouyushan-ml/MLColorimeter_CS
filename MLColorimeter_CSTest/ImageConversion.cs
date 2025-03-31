using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using BitMiracle.LibTiff.Classic;
using OpenCvSharp;

namespace MLColorimeter_CSTest
{
    class ImageConversion
    {
        public enum OpenCvType
        {
            CV_8U = 0,   // 8位无符号整型
            CV_8S = 1,   // 8位有符号整型
            CV_16U = 2,  // 16位无符号整型
            CV_16S = 3,  // 16位有符号整型
            CV_32S = 4,  // 32位整型
            CV_32F = 5,  // 32位浮点型
            CV_64F = 6   // 64位浮点型
        }

        // 辅助方法：获取元素字节大小
        private static int GetElementSize(OpenCvType type)
        {
            switch (type)
            {
                case OpenCvType.CV_8U:
                case OpenCvType.CV_8S: return 1;
                case OpenCvType.CV_16U:
                case OpenCvType.CV_16S: return 2;
                case OpenCvType.CV_32S:
                case OpenCvType.CV_32F: return 4;
                case OpenCvType.CV_64F: return 8;
                default: throw new NotSupportedException();
            }
        }

        public static void DeserializeMat(byte[] byteData, out OpenCvType type, out int rows, out int cols, out int channels, out Array pixelData)
        {
            if (byteData.Length < 16)
            {
                throw new ArgumentException("Invalid byte array: missing metadata!");
            }

            // 解析元数据
            using (MemoryStream ms = new MemoryStream(byteData))
            using (BinaryReader reader = new BinaryReader(ms))
            {
                type = (OpenCvType)reader.ReadInt32();
                rows = reader.ReadInt32();
                cols = reader.ReadInt32();
                channels = reader.ReadInt32();
            }

            // 计算像素数据部分
            int headerSize = 16;
            byte[] pixelBytes = new byte[byteData.Length - headerSize];
            Array.Copy(byteData, headerSize, pixelBytes, 0, pixelBytes.Length);

            // 根据类型转换数据
            int elementSize = GetElementSize(type); // 辅助方法获取元素字节数
            int totalElements = rows * cols * channels;
            switch (type)
            {
                case OpenCvType.CV_8U:
                case OpenCvType.CV_8S:
                    pixelData = pixelBytes;
                    break;
                case OpenCvType.CV_16U:
                case OpenCvType.CV_16S:
                    pixelData = new short[totalElements];
                    Buffer.BlockCopy(pixelBytes, 0, pixelData, 0, pixelBytes.Length);
                    break;
                case OpenCvType.CV_32S:
                    pixelData = new int[totalElements];
                    Buffer.BlockCopy(pixelBytes, 0, pixelData, 0, pixelBytes.Length);
                    break;
                case OpenCvType.CV_32F:
                    pixelData = new float[totalElements];
                    Buffer.BlockCopy(pixelBytes, 0, pixelData, 0, pixelBytes.Length);
                    break;
                case OpenCvType.CV_64F:
                    pixelData = new double[totalElements];
                    Buffer.BlockCopy(pixelBytes, 0, pixelData, 0, pixelBytes.Length);
                    break;
                default:
                    throw new NotSupportedException($"Unsupported type: {type}");
            }
        }

        public static void Save32FToTiff(float[] pixelData, int width, int height, string path)
        {
            using (Tiff tiff = Tiff.Open(path, "w"))
            {
                tiff.SetField(TiffTag.IMAGEWIDTH, width);
                tiff.SetField(TiffTag.IMAGELENGTH, height);
                tiff.SetField(TiffTag.SAMPLESPERPIXEL, 1);
                tiff.SetField(TiffTag.BITSPERSAMPLE, 32);
                tiff.SetField(TiffTag.SAMPLEFORMAT, SampleFormat.IEEEFP);
                tiff.SetField(TiffTag.PHOTOMETRIC, Photometric.MINISBLACK);
                tiff.SetField(TiffTag.ROWSPERSTRIP, height);

                byte[] buffer = new byte[width * sizeof(float)];
                for (int y = 0; y < height; y++)
                {
                    Buffer.BlockCopy(pixelData, y * width * sizeof(float), buffer, 0, buffer.Length);
                    tiff.WriteScanline(buffer, y);
                }
            }
        }

        public static void Save16BitTiff(ushort[] byteData, int width, int height, string outputPath)
        {
            using (Tiff tiff = Tiff.Open(outputPath, "w"))
            {
                // 设置TIFF元数据
                tiff.SetField(TiffTag.IMAGEWIDTH, width);
                tiff.SetField(TiffTag.IMAGELENGTH, height);
                tiff.SetField(TiffTag.SAMPLESPERPIXEL, 1); // 单通道
                tiff.SetField(TiffTag.BITSPERSAMPLE, 16);  // 16位/通道
                tiff.SetField(TiffTag.SAMPLEFORMAT, SampleFormat.UINT); // 无符号整型
                tiff.SetField(TiffTag.PHOTOMETRIC, Photometric.MINISBLACK); // 灰度
                tiff.SetField(TiffTag.ROWSPERSTRIP, height); // 单条带存储

                // 写入像素数据
                byte[] buffer = new byte[width * 2]; // 每行字节数 = 宽度 × 2字节
                for (int y = 0; y < height; y++)
                {
                    int offset = y * width * 2;
                    Buffer.BlockCopy(byteData, offset, buffer, 0, buffer.Length);
                    tiff.WriteScanline(buffer, y);
                }
            }
        }

        public static Bitmap Convert8UToBitmap(byte[] pixelData, int width, int height, int channels)
        {
            PixelFormat format;
            switch (channels)
            {
                case 1:
                    format = PixelFormat.Format8bppIndexed;
                    break;
                case 3:
                    format = PixelFormat.Format24bppRgb;
                    break;
                case 4:
                    format = PixelFormat.Format32bppArgb;
                    break;
                default:
                    throw new NotSupportedException($"Unsupported channels: {channels}");
            }

            Bitmap bmp = new Bitmap(width, height, format);
            BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, width, height), ImageLockMode.WriteOnly, format);
            Marshal.Copy(pixelData, 0, bmpData.Scan0, pixelData.Length);
            bmp.UnlockBits(bmpData);

            // 灰度图像需设置调色板
            if (channels == 1)
            {
                ColorPalette palette = bmp.Palette;
                for (int i = 0; i < 256; i++)
                {
                    palette.Entries[i] = Color.FromArgb(i, i, i);
                }
                bmp.Palette = palette;
            }
            return bmp;
        }

        // 根据类型保存图像
        public static void SaveImage(byte[] byteData)
        {
            OpenCvType type; int rows; int cols; int channels; Array pixelData;
            DeserializeMat(byteData, out type, out rows, out cols, out channels, out pixelData);
            switch (type)
            {
                case OpenCvType.CV_8U:
                    byte[] ushart8bitData = (byte[])pixelData;
                    Bitmap bmp = Convert8UToBitmap(ushart8bitData, cols, rows, channels);
                    bmp.Save("output_8u.tif", ImageFormat.Png);
                    break;

                case OpenCvType.CV_32F:
                    float[] floatData = (float[])pixelData;
                    Save32FToTiff(floatData, cols, rows, "output_32f.tif");
                    break;

                case OpenCvType.CV_16U:
                    ushort[] ushortData = (ushort[])pixelData;
                    Save16BitTiff(ushortData, cols, rows, "output_16u.tif");
                    break;

                default:
                    throw new NotSupportedException($"Unsupported type: {type}");
            }
        }

        public static void SaveImageOpencv(byte[] byteData)
        {
            OpenCvType type; int rows; int cols; int channels; Array pixelData;
            // 解析元数据
            using (MemoryStream ms = new MemoryStream(byteData))
            using (BinaryReader reader = new BinaryReader(ms))
            {
                type = (OpenCvType)reader.ReadInt32();
                rows = reader.ReadInt32();
                cols = reader.ReadInt32();
                channels = reader.ReadInt32();
            }
            Mat matGray = new Mat(rows, cols, MatType.CV_32FC1); // 单通道
            Marshal.Copy(byteData, 0, matGray.Data, byteData.Length);
            Cv2.ImWrite("output_32f_opencv.tif", matGray);
        }
    }
}
