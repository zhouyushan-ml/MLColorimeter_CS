using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xunit;
using MLColorimeterCS.Interface;
using MLColorimeterCS.MLCommon;
using MLColorimeterCS.NotifyCameraCallback;
using MLColorimeterCS.NotifyMotionCallback;
using MLColorimeterCS.NotifyFilterWheelCallback;

namespace MLColorimeter_CSUnitTest
{
    public class MLColorimeterTest
    {
        [Fact]
        public void TestToSave()
        {
            MLColorimeterWrapper test = new MLColorimeterWrapper();
            MLColorimeterModuleWrapper mlcolorimeter = test.GetMLColorimeterInstance();
            MLBinoBusinessModuleWrapper businessManage = mlcolorimeter.GetBusinessManageModule();

            string path = "D:/GitProject/Miami/src/app/config/measureCamera/EYE1";
            MLResult ret = businessManage.ML_AddModule(path);
            Console.WriteLine($"Add Module result :{ret}");
            ret = businessManage.ML_ConnectModules();
            Console.WriteLine($"Connect modules result :{ret}");

        }
    }
}
