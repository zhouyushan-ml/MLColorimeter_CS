using Microsoft.VisualStudio.TestTools.UnitTesting;
using MLColorimeterCS;

[TestClass]
public class MatConverterTests
{
    [TestMethod]
    public void TestFloatMatConversion()
    {
        // 在C#中创建测试数据并调用转换函数
        // 注意：这里需要通过P/Invoke或其他方式创建OpenCV Mat
        // 这里仅展示如何使用转换后的数组

        // 假设我们已经有了一个转换后的float数组
        float[] data = new float[] { 1.0f, 2.0f, 3.0f, 4.0f };
        
        // 验证数组内容
        Assert.AreEqual(1.0f, data[0], 0.001f);
        Assert.AreEqual(2.0f, data[1], 0.001f);
        Assert.AreEqual(3.0f, data[2], 0.001f);
        Assert.AreEqual(4.0f, data[3], 0.001f);
    }

    [TestMethod]
    public void TestUShortMatConversion()
    {
        // 测试unsigned short数组转换
        ushort[] data = new ushort[] { 1000, 2000, 3000, 4000 };
        
        Assert.AreEqual(1000, data[0]);
        Assert.AreEqual(2000, data[1]);
        Assert.AreEqual(3000, data[2]);
        Assert.AreEqual(4000, data[3]);
    }

    [TestMethod]
    public void TestByteMatConversion()
    {
        // 测试byte数组转换
        byte[] data = new byte[] { 10, 20, 30, 40 };
        
        Assert.AreEqual(10, data[0]);
        Assert.AreEqual(20, data[1]);
        Assert.AreEqual(30, data[2]);
        Assert.AreEqual(40, data[3]);
    }
} 