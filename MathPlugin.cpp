//#define NO_EVENT_HANDLER
#include "MathPlugin.h"

#include <math.h>
#include <string>

//#include "source/CScripting.h"

#include <sys/stat.h>
#include <sys/types.h>

std::list<AMX *> p_Amx;
void **ppPluginData;
extern void *pAMXFunctions;

// track activity
int lastplayerinput[max_players];
int lastplayerupdate[max_players];

WaterDataTriQuad watertris[6] = {
	{ 0.0, { -1610.0, 108.0 }, { -1550.0, 168.0 }, { -1610.0, 168.0 }, { -1550.0, 168.0 }, { -1550.0, 108.0 } },
	{ 0.0, { -1728.0, -222.0 }, { -1568.0, -62.0 }, { -1728.0, -62.0 }, { -1568.0, -62.0 }, { -1728.0, -222.0 } },
	{ 0.0, { -1724.0, 58.0 }, { -1612.0, 170.0 }, { -1724.0, 170.0 }, { -1612.0, 170.0 }, { -1724.0, 58.0 } },
	{ 0.0, { -1550.0, -20.0 }, { -1362.0, 168.0 }, { -1550.0, 168.0 }, { -1362.0, 168.0 }, { -1550.0, -20.0 } },
	{ 0.0, { -1722.0, -62.0 }, { -1574.0, 86.0 }, { -1722.0, -62.0 }, { -1574.0, -62.0 }, { -1574.0, 86.0 } },
	{ 0.0, { -912.0, 20.0 }, { -724.0, 208.0 }, { -912.0, 208.0 }, { -724.0, 208.0 }, { -912.0, 20.0 } }
};

WaterDataQuad waterrects[301] = {
		{ 1083.0, { 1270.0, -780.0 }, { 1290.0, -768.0 } },
		{ 112.0, { 1084.0, -684.0 }, { 1104.0, -660.0 } },
		{ 87.0, { 1270.0, -812.0 }, { 1290.0, -800.0 } },
		{ 78.0, { 502.0, -1114.0 }, { 522.0, -1098.0 } },
		{ 77.0, { 178.0, -1244.0 }, { 206.0, -1216.0 } },
		{ 74.0, { 218.0, -1180.0 }, { 238.0, -1172.0 } },
		{ 74.0, { 214.0, -1208.0 }, { 246.0, -1180.0 } },
		{ 41.0, { -1248.0, 2536.0 }, { -1088.0, 2824.0 } },
		{ 41.0, { -1400.0, 2074.0 }, { -1328.0, 2150.0 } },
		{ 41.0, { -550.0, 2004.0 }, { -494.0, 2036.0 } },
		{ 41.0, { -1088.0, 2416.0 }, { -832.0, 2544.0 } },
		{ 41.0, { -1040.0, 2304.0 }, { -864.0, 2416.0 } },
		{ 41.0, { -1024.0, 2144.0 }, { -864.0, 2304.0 } },
		{ 41.0, { -604.0, 2036.0 }, { -484.0, 2060.0 } },
		{ 41.0, { -1200.0, 2114.0 }, { -1072.0, 2242.0 } },
		{ 41.0, { -976.0, 2016.0 }, { -848.0, 2144.0 } },
		{ 41.0, { -1072.0, 2152.0 }, { -1024.0, 2264.0 } },
		{ 41.0, { -700.0, 2272.0 }, { -484.0, 2320.0 } },
		{ 41.0, { -608.0, 2320.0 }, { -528.0, 2352.0 } },
		{ 41.0, { -816.0, 2060.0 }, { -496.0, 2144.0 } },
		{ 41.0, { -1328.0, 2082.0 }, { -1200.0, 2210.0 } },
		{ 41.0, { -1088.0, 2544.0 }, { -1040.0, 2800.0 } },
		{ 41.0, { -1040.0, 2544.0 }, { -832.0, 2760.0 } },
		{ 41.0, { -848.0, 2044.0 }, { -816.0, 2144.0 } },
		{ 41.0, { -864.0, 2144.0 }, { -448.0, 2272.0 } },
		{ 35.0, { -2522.0, -310.0 }, { -2382.0, -234.0 } },
		{ 30.0, { -2048.0, -962.0 }, { -2004.0, -758.0 } },
		{ 23.0, { 2248.0, -1182.0 }, { 2260.0, -1170.0 } },
		{ 22.0, { 890.0, -1106.0 }, { 902.0, -1098.0 } },
		{ 22.0, { 2292.0, -1432.0 }, { 2328.0, -1400.0 } },
		{ 18.0, { 1928.0, -1222.0 }, { 2012.0, -1178.0 } },
		{ 16.0, { 2564.0, 2370.0 }, { 2604.0, 2398.0 } },
		{ 10.0, { 2058.0, 1868.0 }, { 2110.0, 1964.0 } },
		{ 10.0, { 2090.0, 1670.0 }, { 2146.0, 1694.0 } },
		{ 9.0, { 2506.0, 1546.0 }, { 2554.0, 1586.0 } },
		{ 9.0, { 1202.0, -2414.0 }, { 1278.0, -2334.0 } },
		{ 9.0, { 1888.0, 1468.0 }, { 2036.0, 1700.0 } },
		{ 9.0, { 1836.0, 1468.0 }, { 1888.0, 1568.0 } },
		{ 8.0, { 1744.0, 2780.0 }, { 1792.0, 2868.0 } },
		{ 8.0, { 2110.0, 1234.0 }, { 2178.0, 1330.0 } },
		{ 8.0, { 2108.0, 1084.0 }, { 2180.0, 1172.0 } },
		{ 5.0, { -664.0, -1924.0 }, { -464.0, -1864.0 } },
		{ 5.0, { -848.0, -2082.0 }, { -664.0, -1866.0 } },
		{ 3.0, { -2778.0, -522.0 }, { -2662.0, -414.0 } },
		{ 0.0, { -2000.0, -1536.0 }, { -1856.0, -1280.0 } },
		{ 0.0, { -1856.0, -1648.0 }, { -1728.0, -1440.0 } },
		{ 0.0, { -1728.0, -1728.0 }, { -1584.0, -1520.0 } },
		{ 0.0, { -1360.0, -2052.0 }, { -1216.0, -1696.0 } },
		{ 0.0, { -1440.0, -2110.0 }, { -1360.0, -1950.0 } },
		{ 0.0, { -1484.0, -2180.0 }, { -1440.0, -2036.0 } },
		{ 0.0, { -1572.0, -2352.0 }, { -1484.0, -2096.0 } },
		{ 0.0, { -1216.0, -2208.0 }, { -1104.0, -1864.0 } },
		{ 0.0, { -1232.0, -2304.0 }, { -1120.0, -2208.0 } },
		{ 0.0, { -1270.0, -2480.0 }, { -1178.0, -2304.0 } },
		{ 0.0, { -1260.0, -2560.0 }, { -1188.0, -2480.0 } },
		{ 0.0, { -1262.0, -2640.0 }, { -1146.0, -2560.0 } },
		{ 0.0, { -1216.0, -2752.0 }, { -1080.0, -2640.0 } },
		{ 0.0, { -1200.0, -2896.0 }, { -928.0, -2752.0 } },
		{ 0.0, { -2016.0, -3000.0 }, { -1520.0, -2704.0 } },
		{ 0.0, { -1520.0, -3000.0 }, { -1376.0, -2894.0 } },
		{ 0.0, { -2256.0, -3000.0 }, { -2016.0, -2772.0 } },
		{ 0.0, { -2448.0, -3000.0 }, { -2256.0, -2704.0 } },
		{ 0.0, { -3000.0, -3000.0 }, { -2448.0, -2704.0 } },
		{ 0.0, { -3000.0, -2704.0 }, { -2516.0, -2576.0 } },
		{ 0.0, { -3000.0, -2576.0 }, { -2600.0, -2448.0 } },
		{ 0.0, { -3000.0, -2448.0 }, { -2628.0, -2144.0 } },
		{ 0.0, { -3000.0, -2144.0 }, { -2670.0, -2032.0 } },
		{ 0.0, { -3000.0, -2032.0 }, { -2802.0, -1904.0 } },
		{ 0.0, { -3000.0, -1904.0 }, { -2920.0, -1376.0 } },
		{ 0.0, { -3000.0, -1376.0 }, { -2936.0, -1186.0 } },
		{ 0.0, { -768.0, 208.0 }, { -720.0, 672.0 } },
		{ 0.0, { -720.0, 256.0 }, { -656.0, 672.0 } },
		{ 0.0, { -656.0, 276.0 }, { -496.0, 576.0 } },
		{ 0.0, { -496.0, 298.0 }, { -384.0, 566.0 } },
		{ 0.0, { -384.0, 254.0 }, { -224.0, 530.0 } },
		{ 0.0, { -224.0, 212.0 }, { -64.0, 528.0 } },
		{ 0.0, { -64.0, 140.0 }, { 64.0, 544.0 } },
		{ 0.0, { 64.0, 140.0 }, { 304.0, 544.0 } },
		{ 0.0, { 120.0, 544.0 }, { 304.0, 648.0 } },
		{ 0.0, { 304.0, 164.0 }, { 384.0, 608.0 } },
		{ 0.0, { 384.0, 222.0 }, { 464.0, 630.0 } },
		{ 0.0, { 464.0, 304.0 }, { 544.0, 656.0 } },
		{ 0.0, { 544.0, 362.0 }, { 800.0, 646.0 } },
		{ 0.0, { 800.0, 432.0 }, { 944.0, 704.0 } },
		{ 0.0, { 944.0, 480.0 }, { 976.0, 720.0 } },
		{ 0.0, { 976.0, 528.0 }, { 1040.0, 704.0 } },
		{ 0.0, { 1040.0, 560.0 }, { 1280.0, 672.0 } },
		{ 0.0, { 1280.0, 480.0 }, { 1472.0, 640.0 } },
		{ 0.0, { 1472.0, 432.0 }, { 1616.0, 640.0 } },
		{ 0.0, { 1616.0, 416.0 }, { 1824.0, 608.0 } },
		{ 0.0, { 1824.0, 400.0 }, { 2160.0, 576.0 } },
		{ 0.0, { 2160.0, 400.0 }, { 2432.0, 512.0 } },
		{ 0.0, { 2432.0, 368.0 }, { 2560.0, 544.0 } },
		{ 0.0, { 2560.0, 336.0 }, { 2720.0, 576.0 } },
		{ 0.0, { 2720.0, 196.0 }, { 2816.0, 560.0 } },
		{ 0.0, { 2816.0, 160.0 }, { 3000.0, 576.0 } },
		{ 0.0, { 2860.0, -80.0 }, { 3000.0, 160.0 } },
		{ 0.0, { -1376.0, -3000.0 }, { -544.0, -2896.0 } },
		{ 0.0, { -928.0, -2896.0 }, { -544.0, -2800.0 } },
		{ 0.0, { -544.0, -3000.0 }, { -320.0, -2824.0 } },
		{ 0.0, { -320.0, -3000.0 }, { -192.0, -2876.0 } },
		{ 0.0, { -192.0, -3000.0 }, { 160.0, -2920.0 } },
		{ 0.0, { -128.0, -2920.0 }, { 160.0, -2872.0 } },
		{ 0.0, { -60.0, -2872.0 }, { 160.0, -2816.0 } },
		{ 0.0, { -4.0, -2816.0 }, { 160.0, -2672.0 } },
		{ 0.0, { 40.0, -2672.0 }, { 160.0, -2256.0 } },
		{ 0.0, { 16.0, -2560.0 }, { 40.0, -2256.0 } },
		{ 0.0, { -32.0, -2440.0 }, { 16.0, -2256.0 } },
		{ 0.0, { -32.0, -2488.0 }, { 16.0, -2440.0 } },
		{ 0.0, { -96.0, -2440.0 }, { -32.0, -2256.0 } },
		{ 0.0, { -168.0, -2384.0 }, { -96.0, -2256.0 } },
		{ 0.0, { -224.0, -2256.0 }, { 160.0, -2080.0 } },
		{ 0.0, { -248.0, -2080.0 }, { 160.0, -1968.0 } },
		{ 0.0, { -280.0, -1968.0 }, { -128.0, -1824.0 } },
		{ 0.0, { -264.0, -2016.0 }, { -248.0, -1968.0 } },
		{ 0.0, { -264.0, -1824.0 }, { -128.0, -1640.0 } },
		{ 0.0, { -128.0, -1768.0 }, { 124.0, -1648.0 } },
		{ 0.0, { -128.0, -1792.0 }, { 140.0, -1768.0 } },
		{ 0.0, { -128.0, -1968.0 }, { 148.0, -1792.0 } },
		{ 0.0, { 160.0, -2128.0 }, { 592.0, -1976.0 } },
		{ 0.0, { 480.0, -1976.0 }, { 592.0, -1896.0 } },
		{ 0.0, { 352.0, -1976.0 }, { 480.0, -1896.0 } },
		{ 0.0, { 232.0, -1976.0 }, { 352.0, -1880.0 } },
		{ 0.0, { 160.0, -1976.0 }, { 232.0, -1872.0 } },
		{ 0.0, { 160.0, -2784.0 }, { 592.0, -2128.0 } },
		{ 0.0, { 160.0, -3000.0 }, { 592.0, -2784.0 } },
		{ 0.0, { 352.0, -1896.0 }, { 544.0, -1864.0 } },
		{ 0.0, { 592.0, -2112.0 }, { 976.0, -1896.0 } },
		{ 0.0, { 736.0, -1896.0 }, { 904.0, -1864.0 } },
		{ 0.0, { 704.0, -1896.0 }, { 736.0, -1728.0 } },
		{ 0.0, { 736.0, -1864.0 }, { 752.0, -1728.0 } },
		{ 0.0, { 688.0, -1728.0 }, { 752.0, -1480.0 } },
		{ 0.0, { 592.0, -2192.0 }, { 976.0, -2112.0 } },
		{ 0.0, { 592.0, -2328.0 }, { 1008.0, -2192.0 } },
		{ 0.0, { 592.0, -3000.0 }, { 1008.0, -2328.0 } },
		{ 0.0, { 1008.0, -3000.0 }, { 1072.0, -2368.0 } },
		{ 0.0, { 1008.0, -2368.0 }, { 1064.0, -2320.0 } },
		{ 0.0, { 1072.0, -2672.0 }, { 1288.0, -2412.0 } },
		{ 0.0, { 1072.0, -2768.0 }, { 1288.0, -2672.0 } },
		{ 0.0, { 1072.0, -3000.0 }, { 1288.0, -2768.0 } },
		{ 0.0, { 1288.0, -3000.0 }, { 1448.0, -2760.0 } },
		{ 0.0, { 1288.0, -2760.0 }, { 1392.0, -2688.0 } },
		{ 0.0, { 1448.0, -3000.0 }, { 1720.0, -2754.0 } },
		{ 0.0, { 1720.0, -3000.0 }, { 2064.0, -2740.0 } },
		{ 0.0, { 2064.0, -3000.0 }, { 2144.0, -2742.0 } },
		{ 0.0, { 2144.0, -3000.0 }, { 2208.0, -2700.0 } },
		{ 0.0, { 2208.0, -3000.0 }, { 2272.0, -2684.0 } },
		{ 0.0, { 2272.0, -3000.0 }, { 2376.0, -2312.0 } },
		{ 0.0, { 2376.0, -2480.0 }, { 2472.0, -2240.0 } },
		{ 0.0, { 2472.0, -2376.0 }, { 2776.0, -2240.0 } },
		{ 0.0, { 2776.0, -2336.0 }, { 2856.0, -2192.0 } },
		{ 0.0, { -2880.0, -1168.0 }, { -2768.0, -896.0 } },
		{ 0.0, { 2856.0, -2336.0 }, { 3000.0, -2136.0 } },
		{ 0.0, { 2888.0, -2136.0 }, { 3000.0, -1840.0 } },
		{ 0.0, { 2872.0, -1880.0 }, { 2888.0, -1840.0 } },
		{ 0.0, { 2864.0, -1840.0 }, { 3000.0, -1720.0 } },
		{ 0.0, { 2888.0, -1720.0 }, { 3000.0, -1664.0 } },
		{ 0.0, { 2896.0, -1664.0 }, { 3000.0, -1592.0 } },
		{ 0.0, { 2920.0, -1592.0 }, { 3000.0, -1504.0 } },
		{ 0.0, { 2940.0, -1504.0 }, { 3000.0, -1344.0 } },
		{ 0.0, { 2908.0, -1344.0 }, { 3000.0, -1096.0 } },
		{ 0.0, { 2912.0, -1096.0 }, { 3000.0, -800.0 } },
		{ 0.0, { 2918.0, -800.0 }, { 3000.0, -472.0 } },
		{ 0.0, { 2872.0, -472.0 }, { 3000.0, -376.0 } },
		{ 0.0, { 2912.0, -376.0 }, { 3000.0, -80.0 } },
		{ 0.0, { 2864.0, -376.0 }, { 2912.0, -80.0 } },
		{ 0.0, { 2560.0, -2560.0 }, { 2680.0, -2456.0 } },
		{ 0.0, { -992.0, -422.0 }, { -848.0, -238.0 } },
		{ 0.0, { -848.0, -384.0 }, { -512.0, -256.0 } },
		{ 0.0, { -512.0, -400.0 }, { -320.0, -272.0 } },
		{ 0.0, { -320.0, -400.0 }, { -208.0, -304.0 } },
		{ 0.0, { -384.0, -528.0 }, { -100.0, -460.0 } },
		{ 0.0, { -384.0, -704.0 }, { -64.0, -528.0 } },
		{ 0.0, { -336.0, -816.0 }, { -80.0, -704.0 } },
		{ 0.0, { -208.0, -936.0 }, { -48.0, -816.0 } },
		{ 0.0, { -48.0, -936.0 }, { 144.0, -874.0 } },
		{ 0.0, { 32.0, -1024.0 }, { 128.0, -936.0 } },
		{ 0.0, { -16.0, -1104.0 }, { 96.0, -1024.0 } },
		{ 0.0, { 0.0, -1200.0 }, { 144.0, -1104.0 } },
		{ 0.0, { -16.0, -1296.0 }, { 128.0, -1200.0 } },
		{ 0.0, { -16.0, -1440.0 }, { 112.0, -1296.0 } },
		{ 0.0, { 0.0, -1552.0 }, { 96.0, -1440.0 } },
		{ 0.0, { -128.0, -1648.0 }, { 96.0, -1552.0 } },
		{ 0.0, { -64.0, -672.0 }, { 32.0, -576.0 } },
		{ 0.0, { -64.0, -576.0 }, { 96.0, -496.0 } },
		{ 0.0, { 16.0, -496.0 }, { 144.0, -392.0 } },
		{ 0.0, { 144.0, -448.0 }, { 240.0, -384.0 } },
		{ 0.0, { 240.0, -432.0 }, { 304.0, -320.0 } },
		{ 0.0, { 304.0, -384.0 }, { 352.0, -288.0 } },
		{ 0.0, { 352.0, -332.0 }, { 400.0, -252.0 } },
		{ 0.0, { 400.0, -298.0 }, { 464.0, -234.0 } },
		{ 0.0, { 464.0, -288.0 }, { 576.0, -208.0 } },
		{ 0.0, { 576.0, -272.0 }, { 688.0, -192.0 } },
		{ 0.0, { 688.0, -256.0 }, { 768.0, -144.0 } },
		{ 0.0, { 768.0, -212.0 }, { 800.0, -124.0 } },
		{ 0.0, { 800.0, -180.0 }, { 976.0, -92.0 } },
		{ 0.0, { 976.0, -160.0 }, { 1200.0, -64.0 } },
		{ 0.0, { 1200.0, -244.0 }, { 1264.0, -108.0 } },
		{ 0.0, { 1264.0, -330.0 }, { 1344.0, -158.0 } },
		{ 0.0, { 1344.0, -320.0 }, { 1456.0, -208.0 } },
		{ 0.0, { 1456.0, -282.0 }, { 1520.0, -198.0 } },
		{ 0.0, { 1520.0, -208.0 }, { 1648.0, -80.0 } },
		{ 0.0, { 1568.0, -80.0 }, { 1648.0, 16.0 } },
		{ 0.0, { 1648.0, -64.0 }, { 1792.0, 16.0 } },
		{ 0.0, { 1792.0, -128.0 }, { 1888.0, 0.0 } },
		{ 0.0, { 1888.0, -268.0 }, { 2016.0, -20.0 } },
		{ 0.0, { 2016.0, -256.0 }, { 2144.0, -16.0 } },
		{ 0.0, { 2144.0, -272.0 }, { 2224.0, -96.0 } },
		{ 0.0, { 2224.0, -272.0 }, { 2288.0, -144.0 } },
		{ 0.0, { 2048.0, -16.0 }, { 2144.0, 112.0 } },
		{ 0.0, { 2096.0, 112.0 }, { 2224.0, 240.0 } },
		{ 0.0, { 2098.0, 240.0 }, { 2242.0, 400.0 } },
		{ 0.0, { 2160.0, 512.0 }, { 2432.0, 576.0 } },
		{ 0.0, { 2432.0, 544.0 }, { 2560.0, 592.0 } },
		{ 0.0, { 2560.0, 576.0 }, { 2720.0, 608.0 } },
		{ 0.0, { 2720.0, 560.0 }, { 2816.0, 608.0 } },
		{ 0.0, { 2816.0, 576.0 }, { 3000.0, 752.0 } },
		{ 0.0, { -656.0, 576.0 }, { -496.0, 672.0 } },
		{ 0.0, { -740.0, 672.0 }, { -484.0, 784.0 } },
		{ 0.0, { -720.0, 784.0 }, { -384.0, 1008.0 } },
		{ 0.0, { -640.0, 1008.0 }, { -400.0, 1216.0 } },
		{ 0.0, { -880.0, 1296.0 }, { -688.0, 1408.0 } },
		{ 0.0, { -688.0, 1216.0 }, { -400.0, 1424.0 } },
		{ 0.0, { -672.0, 1424.0 }, { -448.0, 1616.0 } },
		{ 0.0, { -832.0, 1616.0 }, { -512.0, 1728.0 } },
		{ 0.0, { -984.0, 1632.0 }, { -832.0, 1712.0 } },
		{ 0.0, { -832.0, 1728.0 }, { -576.0, 2032.0 } },
		{ 0.0, { -1584.0, -1826.0 }, { -1360.0, -1642.0 } },
		{ 0.0, { -3000.0, 354.0 }, { -2832.0, 2942.0 } },
		{ 0.0, { -2832.0, 1296.0 }, { -2704.0, 2192.0 } },
		{ 0.0, { -2704.0, 1360.0 }, { -2240.0, 2224.0 } },
		{ 0.0, { -2240.0, 1432.0 }, { -2000.0, 2224.0 } },
		{ 0.0, { -2064.0, 1312.0 }, { -2000.0, 1432.0 } },
		{ 0.0, { -2000.0, 1392.0 }, { -1712.0, 1792.0 } },
		{ 0.0, { -2000.0, 1792.0 }, { -1724.0, 2016.0 } },
		{ 0.0, { -2000.0, 2016.0 }, { -1836.0, 2176.0 } },
		{ 0.0, { -2000.0, 2176.0 }, { -1920.0, 2224.0 } },
		{ 0.0, { -2208.0, 2224.0 }, { -2000.0, 2432.0 } },
		{ 0.0, { -2208.0, 2432.0 }, { -2000.0, 2576.0 } },
		{ 0.0, { -2352.0, 2448.0 }, { -2208.0, 2576.0 } },
		{ 0.0, { -2312.0, 2344.0 }, { -2208.0, 2448.0 } },
		{ 0.0, { -1712.0, 1360.0 }, { -1600.0, 1792.0 } },
		{ 0.0, { 2376.0, -3000.0 }, { 3000.0, -2688.0 } },
		{ 0.0, { 2520.0, -2688.0 }, { 3000.0, -2560.0 } },
		{ 0.0, { -1664.0, 1280.0 }, { -1600.0, 1360.0 } },
		{ 0.0, { -1600.0, 1280.0 }, { -1440.0, 1696.0 } },
		{ 0.0, { -1248.0, -144.0 }, { -992.0, 208.0 } },
		{ 0.0, { -1176.0, -432.0 }, { -992.0, -144.0 } },
		{ 0.0, { -1792.0, -592.0 }, { -1728.0, -144.0 } },
		{ 0.0, { -1792.0, 170.0 }, { -1600.0, 274.0 } },
		{ 0.0, { -1600.0, 168.0 }, { -1256.0, 208.0 } },
		{ 0.0, { -1574.0, -44.0 }, { -1550.0, 108.0 } },
		{ 0.0, { -1600.0, 1696.0 }, { -1488.0, 1744.0 } },
		{ 0.0, { -464.0, -1908.0 }, { -280.0, -1832.0 } },
		{ 0.0, { -1440.0, 1440.0 }, { -1232.0, 1696.0 } },
		{ 0.0, { -1232.0, 1440.0 }, { -1136.0, 1616.0 } },
		{ 0.0, { -1440.0, 1280.0 }, { -1136.0, 1440.0 } },
		{ 0.0, { -1136.0, 1248.0 }, { -1104.0, 1424.0 } },
		{ 0.0, { -1520.0, 1104.0 }, { -1104.0, 1248.0 } },
		{ 0.0, { -1520.0, 1248.0 }, { -1136.0, 1280.0 } },
		{ 0.0, { -1600.0, 1200.0 }, { -1520.0, 1280.0 } },
		{ 0.0, { -1104.0, 944.0 }, { -932.0, 1136.0 } },
		{ 0.0, { -1424.0, 944.0 }, { -1104.0, 1104.0 } },
		{ 0.0, { -1520.0, 1008.0 }, { -1424.0, 1104.0 } },
		{ 0.0, { 2808.0, -2560.0 }, { 3000.0, -2336.0 } },
		{ 0.0, { -1424.0, 784.0 }, { -896.0, 944.0 } },
		{ 0.0, { -1488.0, 560.0 }, { -896.0, 784.0 } },
		{ 0.0, { -1536.0, 560.0 }, { -1488.0, 672.0 } },
		{ 0.0, { -2832.0, 2888.0 }, { 3000.0, 3000.0 } },
		{ 0.0, { -896.0, 208.0 }, { -768.0, 732.0 } },
		{ 0.0, { 1520.0, -252.0 }, { 1572.0, -208.0 } },
		{ 0.0, { 2922.0, 752.0 }, { 3000.0, 2888.0 } },
		{ 0.0, { -3000.0, -446.0 }, { -2910.0, 354.0 } },
		{ 0.0, { -2434.0, 2224.0 }, { -2294.0, 2340.0 } },
		{ 0.0, { -2294.0, 2224.0 }, { -2208.0, 2312.0 } },
		{ 0.0, { -1600.0, 208.0 }, { -896.0, 560.0 } },
		{ 0.0, { -3000.0, 2942.0 }, { -2832.0, 3000.0 } },
		{ 0.0, { -992.0, -144.0 }, { -912.0, 208.0 } },
		{ 0.0, { -896.0, 842.0 }, { -776.0, 954.0 } },
		{ 0.0, { -2240.0, 1336.0 }, { -2088.0, 1432.0 } },
		{ 0.0, { -3000.0, -822.0 }, { -2930.0, -446.0 } },
		{ 0.0, { -2660.0, 2224.0 }, { -2520.0, 2264.0 } },
		{ 0.0, { -378.0, -460.0 }, { -138.0, -400.0 } },
		{ 0.0, { -1748.0, -816.0 }, { -1180.0, -592.0 } },
		{ 0.0, { -1458.0, -592.0 }, { -1054.0, -432.0 } },
		{ 0.0, { -3000.0, -1186.0 }, { -2880.0, -822.0 } },
		{ 0.0, { 1072.0, -2412.0 }, { 1128.0, -2372.0 } },
		{ 0.0, { -2768.0, -1106.0 }, { -2656.0, -830.0 } },
		{ 0.0, { -2656.0, -1024.0 }, { -2512.0, -816.0 } },
		{ 0.0, { -1484.0, 784.0 }, { -1424.0, 840.0 } },
		{ 0.0, { -496.0, 566.0 }, { -432.0, 642.0 } },
		{ 0.0, { 250.0, 2808.0 }, { 818.0, 2888.0 } },
		{ 0.0, { 2502.0, -2240.0 }, { 2670.0, -2120.0 } },
		{ 0.0, { -2512.0, -976.0 }, { -2400.0, -816.0 } },
		{ 0.0, { 88.0, 544.0 }, { 120.0, 572.0 } },
		{ 0.0, { 1856.0, -202.0 }, { 1888.0, -158.0 } },
		{ 0.0, { -2400.0, -1056.0 }, { -2256.0, -864.0 } },
		{ 0.0, { -2256.0, -1198.0 }, { -2144.0, -950.0 } },
		{ 0.0, { -2144.0, -1408.0 }, { -2000.0, -1072.0 } },
		{ 0.0, { 2872.0, -2136.0 }, { 2888.0, -2120.0 } },
		{ 0.0, { 2760.0, -2240.0 }, { 2776.0, -2232 } }
};

using namespace sampgdk;

	// And an array containing the native function-names and the functions
	// specified with them.
	AMX_NATIVE_INFO natives[ ] =
	{
		{ "MPGetVehicleDriver",				nat_MPGetVehicleDriver },
		{ "MPGetVehicleDriverCount",		nat_MPGetVehicleDriverCount },
		{ "MPGetPlayerVehicleOtherDriver",	nat_MPGetPlayerVehicleOtherDriver },
		{ "MPGetVehicleOccupantCnt",		nat_MPGetVehicleOccupantCnt },
		{ "MPGetVehicleSurfersCnt", 		nat_MPGetVehicleSurfersCnt},
		{ "MPCrossProduct",					nat_MPCrossProduct },
		{ "MPDotProduct",					nat_MPDotProduct },
		{ "MPDistanceCameraToLocation",		nat_MPDistanceCameraTargetToLocation },
		{ "MPProjectPointOnVehicle",		nat_MPProjectPointOnVehicle},
		{ "MPProjectPointOnPlayer",			nat_MPProjectPointOnPlayer},
		{ "MPProjectPointXYZA",				nat_MPProjectPointXYZA},
		{ "MPGetAimTarget",					nat_MPGetAimTarget },
		{ "MPDistancePointLine",			nat_MPDistancePointLine },
		{ "MPInterpolatePoint",				nat_MPInterpolatePoint},
		{ "MPDistance",						nat_MPDistance },
		{ "MPFDistance",					nat_MPFDistance },
		{ "MPFSQRT",						nat_MPFSQRT },
		{ "MPFloatIsFinite",				nat_MPFloatIsFinite },

		{ "MPReturnInf",				nat_returninf },
		{ "MPReturnNan",				nat_returnnan },
		{ "MPFiniteFloat",				nat_normalizefloat },

		{ "MPClamp360",					nat_MPClamp360 },
		{ "MPFNormalize",				nat_MPFNormalize },
		{ "MPGetVehicleUpsideDown",		nat_MPGetVehicleUpsideDown },
		{ "MPGetTrailerTowingVehicle",	nat_MPGetTrailerTowingVehicle },
		{ "MPVecLength",				nat_MPVecLength },
		{ "MPFVecLength",				nat_MPFVecLength },

		{ "MPInRange",					nat_MPWithinRange },
		{ "MPPtInRect2D",				nat_MPPtInRect2D },
		{ "MPPtInRect3D",				nat_MPPtInRect3D },
		
		{ "IsBitSet", nat_IsBitSet },
		{ "BitToOn", nat_BitToOn },
		{ "BitToOff", nat_BitToOff },

		{ "UtilGetWaterZlevel", nat_UtilGetWaterZlevel },

		{ "UtilGetPlayerIdleTime", nat_GetPlayerIdleTime },
		{ "UtilGetPlayerAbsentTime", nat_GetPlayerTimeSinceUpdate },
		

		// ticks related
		{ "GetTimeDistanceMs",			nat_GetTimeDistance },
		{ "GetTimeMs",					nat_GetTimeMs },

		{ 0,					    	0 }
	};

	#ifndef NO_EVENT_HANDLER

	static samputils theGameMode;

	samputils::samputils() {
		// Register our gamemode in order to catch events - if we don't do this
		// somewhere none of the HelloWorld callbacks will be ever called.
		this->Register();
	}

	samputils::~samputils() {}
	/*
	int samputils::getidletime(int playerid) {

		return samputils::lastplayerinput[playerid];

	}
	*/

	#endif

// i got this from blueg's plugin, no idea who wrote this originally
std::string GetString(AMX* amx,cell param)
{
	cell *pString;
	char *szDest;
	int nLen;
	amx_GetAddr(amx,param,&pString);
	amx_StrLen(pString, &nLen);
	szDest = new char[nLen+1];
	amx_GetString(szDest, pString, 0, UNLIMITED);
	szDest[nLen] = '\0';
	std::string szReturn(szDest);
	delete szDest;
	return szReturn;
}

float refcell2float(AMX* amx, cell param) {

	cell* padd;
	amx_GetAddr(amx, param, &padd);
	cell value = *padd;
	return amx_ctof(value);
}


// google 0x5f3759df
float FastSqrt(float number) {
    long i;
    float x, y;
    const float f = 1.5F;

    x = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;
    i  = 0x5f3759df - ( i >> 1 );
    y  = * ( float * ) &i;
    y  = y * ( f - ( x * y * y ) );
    y  = y * ( f - ( x * y * y ) );
    return number * y;
}

void Normalize(float &vx, float &vy, float &vz) {

	float Length;

	Length = 1 /FastSqrt(vx * vx + vy * vy + vz * vz);

	vx = vx * Length;
	vy = vy * Length;
	vz = vz * Length;
}

int GetPlayerVehicleOtherDriver(int playerid) {

	if (playerid == INVALID_PLAYER_ID)
		return INVALID_PLAYER_ID;

	int vehicleid = GetPlayerVehicleID(playerid);

	if ((!IsValidVehicle(vehicleid)) || (vehicleid == 0) || (vehicleid == INVALID_VEHICLE_ID) || (vehicleid > MAX_VEHICLES))
		return INVALID_PLAYER_ID; // not possible to find any vehicle, so there is no "other" driver.

	int i;

	for (i = 0; i < MAX_PLAYERS; i++) {

		if ((IsPlayerConnected(i) == false) || (playerid == i)) // no connected player in slot or this is same player?
			continue;

		if (GetPlayerState(i) != PLAYER_STATE_DRIVER) continue; // not a driver of any vehicle?

		int thisplayervehid = GetPlayerVehicleID(i);

		if (!IsVehicleStreamedIn(thisplayervehid, i)) // so players who are in wrong virtual world (like a pause sysstem would do) are not returned as drivers anymore.
			continue;

		// todo: maybe use IsPlayerInVehicle instead.

		if (thisplayervehid == vehicleid) // found a connected driver in this car.
			return i;

	}

	return INVALID_PLAYER_ID;

}

int GetTrailerTowingVehicle(int trailerid) {

    if ((trailerid == 0) || (trailerid == INVALID_VEHICLE_ID) || (trailerid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;

    for (i = 0; i < MAX_VEHICLES; i++) {

		if (GetVehicleModel(i) == 0) // invalid vehicle.
			continue;

        if (GetVehicleTrailer(i) == trailerid)
			return i;

    }

    return INVALID_VEHICLE_ID; // found no driver.

}

int GetVehicleDriver(int vehicleid) {

    if ((vehicleid == 0) || (vehicleid == INVALID_VEHICLE_ID) || (vehicleid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;

    for (i = 0; i < MAX_PLAYERS; i++) {

        if (IsPlayerConnected(i) == false) // no connected player in slot?
            continue;

            if (GetPlayerState(i) != PLAYER_STATE_DRIVER) continue; // not a driver of any vehicle?

            int thisplayervehid = GetPlayerVehicleID(i);

			if (!IsVehicleStreamedIn(thisplayervehid, i)) // so players who are in wrong virtual world (like a pause sysstem would do) are not returned as drivers anymore.
				continue;

            // todo: maybe use IsPlayerInVehicle instead.

            if (thisplayervehid == vehicleid) // found a connected driver in this car.
                return i;

    }

    return INVALID_PLAYER_ID; // found no driver.

}

SCRIPT_NATIVE nat_MPGetVehicleDriver(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(nat_MPGetVehicleDriver, 1);

	vehicleid = (int)params[1];

	return GetVehicleDriver(vehicleid);
}

SCRIPT_NATIVE nat_MPGetTrailerTowingVehicle(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(nat_MPGetTrailerTowingVehicle, 1);

	vehicleid = (int)params[1];

	return GetTrailerTowingVehicle(vehicleid);
}

SCRIPT_NATIVE nat_MPGetPlayerVehicleOtherDriver(AMX* amx, cell* params) {
	int playerid;

	CHECK_PARAM_COUNT(nat_MPGetPlayerVehicleOtherDriver, 1);

	playerid = (int)params[1];

	return GetPlayerVehicleOtherDriver(playerid);
}

int GetVehicleDriverCount(int vehicleid) {

    if ((vehicleid == 0) || (vehicleid == INVALID_VEHICLE_ID) || (vehicleid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;
    int totaldrivers = 0;

    for (i = 0; i < MAX_PLAYERS; i++) {

        if (IsPlayerConnected(i) == false) // no connected player in slot?
            continue;

            if (GetPlayerState(i) != PLAYER_STATE_DRIVER) continue; // not a driver of any vehicle?

            int thisplayervehid = GetPlayerVehicleID(i);

			if (!IsVehicleStreamedIn(thisplayervehid, i)) // so players who are in wrong virtual world (like a pause sysstem would do) are not returned as drivers anymore.
				continue;

            if (thisplayervehid == vehicleid) // found a connected driver in this car.
                totaldrivers++;

    }

    return totaldrivers; // return how many there are.

}

int GetVehicleOccupantCnt(int vehicleid) {

    if ((vehicleid == 0) || (vehicleid == INVALID_VEHICLE_ID) || (vehicleid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;
    int totalocc = 0;

    for (i = 0; i < MAX_PLAYERS; i++) {

        if (IsPlayerConnected(i) == false) // no connected player in slot?
            continue;

			if (GetPlayerVirtualWorld(i) != GetVehicleVirtualWorld(vehicleid)) // solves specific issues if you put paused players into separate virtual world.
				continue;

            int thisplayervehid = GetPlayerVehicleID(i);

			if (!IsVehicleStreamedIn(thisplayervehid, i)) // so players who are in wrong virtual world (like a pause sysstem would do) are not returned as drivers anymore.
				continue;

            if (thisplayervehid == vehicleid) // found a connected player in this car.
                totalocc++;

    }

    return totalocc; // return how many there are.

}

void ProjectPointXYZA(
				float origin_x, float origin_y, float origin_z, float playerheading,
				float incoords_x, float incoords_y, float incoords_z, float &outx, float &outy, float &outz) {

	float originalangle;
	originalangle = atan2(incoords_x, incoords_y) * 180.0 / PI;

	float originaldistance;
	originaldistance = FastSqrt(incoords_x * incoords_x + incoords_y * incoords_y);

	originalangle -= playerheading;

	// *PI/180 = degtorad
	outx = origin_x + sin(originalangle * PI/180.0) * originaldistance;
	outy = origin_y + cos(originalangle * PI/180.0) * originaldistance;
	outz = incoords_z + origin_z; // nothing magical on Z!

}

void ProjectPointOnPlayer(int PlayerID, float incoords_x, float incoords_y, float incoords_z, float &outx, float &outy, float &outz) {

	float playerpos[3];
	float playerheading;
	GetPlayerPos(PlayerID, &playerpos[0], &playerpos[1], &playerpos[2]);
	GetPlayerFacingAngle(PlayerID, &playerheading);

	float originalangle;
	originalangle = atan2(incoords_x, incoords_y) * 180 / PI;

	float originaldistance;
	originaldistance = FastSqrt(incoords_x * incoords_x + incoords_y * incoords_y);

	originalangle -= playerheading;

	// *PI/180 = degtorad
	outx = playerpos[0] + sin(originalangle * PI/180) * originaldistance;
	outy = playerpos[1] + cos(originalangle * PI/180) * originaldistance;
	outz = incoords_z + playerpos[2]; // nothing magical on Z!

}

int GetVehicleSurfersCnt(int vehicleid) {

    if ((vehicleid == 0) || (vehicleid == INVALID_VEHICLE_ID) || (vehicleid > MAX_VEHICLES))
        return INVALID_PLAYER_ID;

    int i;
    int totalocc = 0;

    for (i = 0; i < MAX_PLAYERS; i++) {

        if (IsPlayerConnected(i) == false) // no connected player in slot?
            continue;

			if (GetPlayerVirtualWorld(i) != GetVehicleVirtualWorld(vehicleid)) // solves specific issues if you put paused players into separate virtual world.
				continue;

            int thisplayervehid;
            thisplayervehid = GetPlayerSurfingVehicleID(i);

            if (thisplayervehid == vehicleid) // found a connected player in this car.
                totalocc++;

    }

    return totalocc; // return how many there are.

}

SCRIPT_NATIVE nat_MPGetVehicleDriverCount(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(nat_MPGetVehicleDriverCount, 1);

	vehicleid = (int)params[1];

	return GetVehicleDriverCount(vehicleid);
}

SCRIPT_NATIVE nat_MPGetVehicleOccupantCnt(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(nat_MPGetVehicleOccupantCnt, 1);

	vehicleid = (int)params[1];

	return GetVehicleOccupantCnt(vehicleid);
}

SCRIPT_NATIVE nat_MPGetVehicleSurfersCnt(AMX* amx, cell* params) {
	int vehicleid;

    CHECK_PARAM_COUNT(nat_MPGetVehicleSurfersCnt, 1);

	vehicleid = (int)params[1];

	return GetVehicleSurfersCnt(vehicleid);
}

void MPCrossProduct(float v1[3], float v2[3], float &resx, float &resy, float &resz) {

	resx = (v1[1]*v2[2]) - (v1[2]*v2[1]);
	resy = (v1[2]*v2[0]) - (v1[0]*v2[2]);
	resz = (v1[0]*v2[1]) - (v1[1]*v2[0]);

}

SCRIPT_NATIVE nat_MPCrossProduct(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPCrossProduct, 9);

    float v1[3];
    float v2[3];
    float resx, resy, resz;

    v1[0] = amx_ctof(params[1]);
    v1[1] = amx_ctof(params[2]);
    v1[2] = amx_ctof(params[3]);

    v2[0] = amx_ctof(params[4]);
    v2[1] = amx_ctof(params[5]);
    v2[2] = amx_ctof(params[6]);

    MPCrossProduct(v1, v2, resx, resy, resz);;

	cell* padd;

	amx_GetAddr(amx, params[7], &padd);
	*padd = amx_ftoc(resx);
	amx_GetAddr(amx, params[8], &padd);
	*padd = amx_ftoc(resy);
	amx_GetAddr(amx, params[9], &padd);
	*padd = amx_ftoc(resz);

	return 1;

}

float DotProduct(float AX, float AY, float AZ, float BX, float BY, float BZ) {
	return AX * BX + AY * BY + AZ * BZ;
}

SCRIPT_NATIVE nat_MPDotProduct(AMX* amx, cell* params) {

	CHECK_PARAM_COUNT(nat_MPDotProduct, 6);

	float ret = DotProduct(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));

	return amx_ftoc(ret);
}

float DistanceCameraTargetToLocation(float CamX, float CamY, float CamZ,   float ObjX, float ObjY, float ObjZ,   float FrX, float FrY, float FrZ) {

	float TGTDistance;

	// get distance from camera to target
	TGTDistance = sqrt((CamX - ObjX) * (CamX - ObjX) + (CamY - ObjY) * (CamY - ObjY) + (CamZ - ObjZ) * (CamZ - ObjZ));

	float tmpX, tmpY, tmpZ;

	tmpX = FrX * TGTDistance + CamX;
	tmpY = FrY * TGTDistance + CamY;
	tmpZ = FrZ * TGTDistance + CamZ;

	return sqrt((tmpX - ObjX) * (tmpX - ObjX) + (tmpY - ObjY) * (tmpY - ObjY) + (tmpZ - ObjZ) * (tmpZ - ObjZ));
}


SCRIPT_NATIVE nat_MPDistanceCameraTargetToLocation(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPDistanceCameraTargetToLocation, 9);

	float resdist = DistanceCameraTargetToLocation(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),   amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]),   amx_ctof(params[7]), amx_ctof(params[8]), amx_ctof(params[9]));

    return amx_ftoc(resdist);

}

// transforms a vector using a 4x4 matrix (translation and scaling is ignored, only 3x3 part is used, we only need rotation!)
void MatrixTransformVector(float vector[3], float m[4][4], float &resx, float &resy, float &resz) {
	resz = vector[2] * m[0][0] + vector[1] * m[0][1] + vector[0] * m[0][2] + m[0][3];
	resy = vector[2] * m[1][0] + vector[1] * m[1][1] + vector[0] * m[1][2] + m[1][3];
	resx = -(vector[2] * m[2][0] + vector[1] * m[2][1] + vector[0] * m[2][2] + m[2][3]); // don't ask why -x was needed, i don't know either.
}


void MPProjectPointOnVehicle(int vehid, float Invector[3], float &resx, float &resy, float &resz, int worldspace) {

	float Quaternion[4];
	float transformationmatrix[4][4];

	// to native!
	GetVehicleRotationQuat(vehid, &Quaternion[0], &Quaternion[1], &Quaternion[2], &Quaternion[3]);

	// build a transformation matrix out of the quaternion
	float xx = Quaternion[0] * Quaternion[0];
	float xy = Quaternion[0] * Quaternion[1];
	float xz = Quaternion[0] * Quaternion[2];
	float xw = Quaternion[0] * Quaternion[3];
	float yy = Quaternion[1] * Quaternion[1];
	float yz = Quaternion[1] * Quaternion[2];
	float yw = Quaternion[1] * Quaternion[3];
	float zz = Quaternion[2] * Quaternion[2];
	float zw = Quaternion[2] * Quaternion[3];

	transformationmatrix[0][0] = 1 - 2 * ( yy + zz );
	transformationmatrix[0][1] =     2 * ( xy - zw );
	transformationmatrix[0][2] =     2 * ( xz + yw );
	transformationmatrix[0][3] = 0.0;

	transformationmatrix[1][0] =     2 * ( xy + zw );
	transformationmatrix[1][1] = 1 - 2 * ( xx + zz );
	transformationmatrix[1][2] =     2 * ( yz - xw );
	transformationmatrix[1][3] = 0.0;

	transformationmatrix[2][0] =     2 * ( xz - yw );
	transformationmatrix[2][1] =     2 * ( yz + xw );
	transformationmatrix[2][2] = 1 - 2 * ( xx + yy );
	transformationmatrix[2][3] = 0;

	transformationmatrix[3][0] = 0;
	transformationmatrix[3][1] = 0;
	transformationmatrix[3][2] = 0;
	transformationmatrix[3][3] = 1;

	// transform the point thru car's rotation
	MatrixTransformVector(Invector, transformationmatrix, resx, resy, resz);

	// if worldspace is set it'll return the coords in global space - useful to check tire coords against tire spike proximity directly, etc..

	if (worldspace == 1) {
		float fX, fY, fZ;

		GetVehiclePos(vehid, &fX, &fY, &fZ);

		resx += fX;
		resy += fY;
		resz += fZ;
	}
}

float Distance(float fPos1x, float fPos1y, float fPos1z, float fPos2x, float fPos2y, float fPos2z) {
	return sqrt((fPos1x - fPos2x) * (fPos1x - fPos2x) + (fPos1y - fPos2y) * (fPos1y - fPos2y) + (fPos1z - fPos2z) * (fPos1z - fPos2z));
}

float FDistance(float fPos1x, float fPos1y, float fPos1z, float fPos2x, float fPos2y, float fPos2z) {
	return FastSqrt((fPos1x - fPos2x) * (fPos1x - fPos2x) + (fPos1y - fPos2y) * (fPos1y - fPos2y) + (fPos1z - fPos2z) * (fPos1z - fPos2z));
}


SCRIPT_NATIVE nat_MPDistance(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPDistance, 6);

    float tmpret;
	tmpret = Distance(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),    amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));

	return amx_ftoc(tmpret);

}

SCRIPT_NATIVE nat_MPFloatIsFinite(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPFloatIsFinite, 1);

    float tmpret;
	tmpret = amx_ctof(params[1]);

	return isfinite(tmpret);

}


SCRIPT_NATIVE nat_returninf(AMX* amx, cell* params) {
	
	float tmpret;
	
	if (params[1] == 1)
		tmpret = INFINITY;
	else
		tmpret = -INFINITY;
	
	return amx_ftoc(tmpret);
	
}

SCRIPT_NATIVE nat_returnnan(AMX* amx, cell* params) {
	
	float tmpret;
	
	if (params[1] == 1)
		tmpret = NAN;
	else
		tmpret = -NAN;
	
	return amx_ftoc(tmpret);
	
}

SCRIPT_NATIVE nat_normalizefloat(AMX* amx, cell* params) {
	
	CHECK_PARAM_COUNT(nat_normalizefloat, 2);
	
	float tmpret;
	
	tmpret = amx_ctof(params[1]);
	
	if (!isfinite(tmpret))
		tmpret = amx_ctof(params[2]);
	
	return amx_ftoc(tmpret);
	
}


SCRIPT_NATIVE nat_MPFDistance(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPFDistance, 6);

    float tmpret;
	tmpret = FDistance(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),    amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));

	return amx_ftoc(tmpret);

}

float VecLength(float fsx, float fsy, float fsz) {

	return sqrt(fsx * fsx + fsy * fsy + fsz * fsz);

}

SCRIPT_NATIVE nat_MPVecLength(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPVecLength, 3);

    float tmpret;
	tmpret = VecLength(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));

	return amx_ftoc(tmpret);

}

float FVecLength(float fsx, float fsy, float fsz) {

	return FastSqrt(fsx * fsx + fsy * fsy + fsz * fsz);

}

SCRIPT_NATIVE nat_MPFVecLength(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPFVecLength, 3);

    float tmpret;
	tmpret = FVecLength(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]));

	return amx_ftoc(tmpret);

}

SCRIPT_NATIVE nat_FMPDistance(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_FMPDistance, 6);

    float tmpret;
	tmpret = FDistance(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),    amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]));

	return amx_ftoc(tmpret);

}


SCRIPT_NATIVE nat_MPProjectPointOnVehicle(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPProjectPointOnVehicle, 8);

    float v1[3];
    float resx, resy, resz;

    int vehid = params[1];

    v1[0] = amx_ctof(params[2]);
    v1[1] = amx_ctof(params[3]);
    v1[2] = amx_ctof(params[4]);

    MPProjectPointOnVehicle(vehid, v1, resx, resy, resz, params[8]);

	//logprintf("%0.5f %0.5f %0.5f", resx, resy, resz);

	cell* padd = NULL;

	amx_GetAddr(amx, params[5], &padd);
	*padd = amx_ftoc(resx);

	amx_GetAddr(amx, params[6], &padd);
	*padd = amx_ftoc(resy);

	amx_GetAddr(amx, params[7], &padd);
	*padd = amx_ftoc(resz);

	return 1;
}

SCRIPT_NATIVE nat_MPProjectPointXYZA(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPProjectPointXYZA, 10);

    float resx, resy, resz;

	ProjectPointXYZA(
					amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]),
					amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]),
					resx, resy, resz);

	//logprintf("%0.5f %0.5f %0.5f", resx, resy, resz);

	cell* padd = NULL;

	amx_GetAddr(amx, params[8], &padd);
	*padd = amx_ftoc(resx);

	amx_GetAddr(amx, params[9], &padd);
	*padd = amx_ftoc(resy);

	amx_GetAddr(amx, params[10], &padd);
	*padd = amx_ftoc(resz);

	return 1;

}

SCRIPT_NATIVE nat_MPProjectPointOnPlayer(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPProjectPointOnVehicle, 7);

    float resx, resy, resz;

    int playerid = params[1];

	ProjectPointOnPlayer(playerid, amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), resx, resy, resz);

	//logprintf("%0.5f %0.5f %0.5f", resx, resy, resz);

	cell* padd = NULL;

	amx_GetAddr(amx, params[5], &padd);
	*padd = amx_ftoc(resx);

	amx_GetAddr(amx, params[6], &padd);
	*padd = amx_ftoc(resy);

	amx_GetAddr(amx, params[7], &padd);
	*padd = amx_ftoc(resz);

	return 1;
}

int MPGetAimTarget(int playerid, float searchradius) {

	//logprintf("MPGetAimTarget called for player %d with radius %0.3f", playerid, searchradius);

	if ((IsPlayerConnected(playerid) == false) || (playerid > MAX_PLAYERS)) {
		logprintf("MPGetAimTarget called for invalid player.");
		return INVALID_PLAYER_ID;
	}

	float ccpos[3];
	float ccfront[3];

	GetPlayerCameraFrontVector(playerid, &ccfront[0], &ccfront[1], &ccfront[2]);

	GetPlayerCameraPos(playerid, &ccpos[0], &ccpos[1], &ccpos[2]);

	//logprintf("camera location for %d = (%0.5f %0.5f %0.5f) front (%0.5f %0.5f %0.5f)", playerid, ccpos[0], ccpos[1], ccpos[2], ccfront[0], ccfront[1], ccfront[2]);

	int closestplayer = INVALID_PLAYER_ID;
	float distmax = 90000.0;
	int i;

    for (i = 0; i < MAX_PLAYERS; i++) {

    	if (i == playerid) // smartass
			continue;

		if (IsPlayerConnected(i) == false) // no connected player in slot?
			continue;

		float thiscoords[3];

		GetPlayerPos(i, &thiscoords[0], &thiscoords[1], &thiscoords[2]);

		float dist;

		dist = Distance(ccpos[0], ccpos[1], ccpos[2], thiscoords[0], thiscoords[1], thiscoords[2]);

		//logprintf("(%0.5f %0.5f %0.5f) (%0.5f %0.5f %0.5f) distance %0.5f", ccpos[0], ccpos[1], ccpos[2], thiscoords[0], thiscoords[1], thiscoords[2], dist);

		if (dist < searchradius) {

			// distance between camera and this player has to be
			float camdist = DistanceCameraTargetToLocation(ccpos[0], ccpos[1], ccpos[2],   thiscoords[0], thiscoords[1], thiscoords[2],   ccfront[0], ccfront[1], ccfront[2] );

			//logprintf("camdist = %0.5f", camdist);

			if ((camdist < distmax) && (camdist < camera_actor_range_threshhold)) {
				closestplayer = i;
				distmax = dist;
			}
		}

	}

	return closestplayer;

}

SCRIPT_NATIVE nat_MPGetAimTarget(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPGetAimTarget, 2);

    return MPGetAimTarget(params[1], amx_ctof(params[2]));

}


//========================================================================================
//
// Portion of DistancePointLine Unit Test
// Copyright (c) 2002, All rights reserved
//
// Damian Coventry
// Tuesday, 16 July 2002
//
// Implementation of theory by Paul Bourke
// http://paulbourke.net/geometry/pointline/
//
//========================================================================================

typedef struct tagXYZ
{
    float X, Y, Z;
}
XYZ;

float Magnitude( XYZ *Point1, XYZ *Point2 )
{
    XYZ Vector;

    Vector.X = Point2->X - Point1->X;
    Vector.Y = Point2->Y - Point1->Y;
    Vector.Z = Point2->Z - Point1->Z;

    return (float)sqrt( Vector.X * Vector.X + Vector.Y * Vector.Y + Vector.Z * Vector.Z );
}

int DistancePointLine( XYZ *Point, XYZ *LineStart, XYZ *LineEnd, float *Distance )
{
    float LineMag;
    float U;
    XYZ Intersection;

//	logprintf("(%0.5f %0.5f %0.5f ) (%0.5f %0.5f %0.5f ) (%0.5f %0.5f %0.5f ) ", Point->X, Point->Y, Point->Z, LineStart->X, LineStart->Y, LineStart->Z, LineEnd->X, LineEnd->Y, LineEnd->Z);

    LineMag = Magnitude( LineEnd, LineStart );

    U = ( ( ( Point->X - LineStart->X ) * ( LineEnd->X - LineStart->X ) ) +
        ( ( Point->Y - LineStart->Y ) * ( LineEnd->Y - LineStart->Y ) ) +
        ( ( Point->Z - LineStart->Z ) * ( LineEnd->Z - LineStart->Z ) ) ) /
        ( LineMag * LineMag );

    if( U < 0.0f || U > 1.0f )
        return 0;   // closest point does not fall within the line segment

    Intersection.X = LineStart->X + U * ( LineEnd->X - LineStart->X );
    Intersection.Y = LineStart->Y + U * ( LineEnd->Y - LineStart->Y );
    Intersection.Z = LineStart->Z + U * ( LineEnd->Z - LineStart->Z );

    *Distance = Magnitude( Point, &Intersection );

    return 1;
}

void interpolatebetweenvectors(float ax, float ay, float az, float bx, float by, float bz, float &rx, float &ry, float &rz, float distance) {
	rx = ax + distance * (bx - ax);
	ry = ay + distance * (by - ay);
	rz = az + distance * (bz - az);
}

SCRIPT_NATIVE nat_MPInterpolatePoint(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPInterpolatePoint, 10);

    float resx, resy, resz;

    interpolatebetweenvectors(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3]),  amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]), resx, resy, resz, amx_ctof(params[10]));

	cell* padd = NULL;

	amx_GetAddr(amx, params[7], &padd);
	*padd = amx_ftoc(resx);

	amx_GetAddr(amx, params[8], &padd);
	*padd = amx_ftoc(resy);

	amx_GetAddr(amx, params[9], &padd);
	*padd = amx_ftoc(resz);

	return 1;
}

float Clamp360(float angle) {

	int roundedone;

	roundedone = trunc(angle);
	roundedone = roundedone / 360;

	if (angle >= 360.0)
		return angle - (roundedone * 360.0);

	if (angle < 0.0)
		return 360.0 + angle - (roundedone * 360.0);

	return angle;

}

SCRIPT_NATIVE nat_MPClamp360(AMX* amx, cell* params) {

	CHECK_PARAM_COUNT(nat_MPClamp360, 1);

	float ret = Clamp360(amx_ctof(params[1]));

	return amx_ftoc(ret);
}

SCRIPT_NATIVE nat_MPFNormalize(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPFNormalize, 3);

	float resx, resy, resz;

	resx = amx_ctof(params[1]);
	resz = amx_ctof(params[2]);
	resx = amx_ctof(params[3]);

    Normalize(resx, resy, resz);

	cell* padd = NULL;

	amx_GetAddr(amx, params[1], &padd);
	*padd = amx_ftoc(resx);

	amx_GetAddr(amx, params[2], &padd);
	*padd = amx_ftoc(resy);

	amx_GetAddr(amx, params[3], &padd);
	*padd = amx_ftoc(resz);

	return 1;

}

SCRIPT_NATIVE nat_MPFSQRT(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPFSQRT, 1);

	float ret = FastSqrt(amx_ctof(params[1]));

	return amx_ftoc(ret);

}

SCRIPT_NATIVE nat_MPDistancePointLine(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPDistancePointLine, 9);

    XYZ LineStart, LineEnd, Point;

    Point.X     =  amx_ctof(params[1]); Point.Y =   	amx_ctof(params[2]); Point.Z =  	amx_ctof(params[3]);
    LineStart.X =  amx_ctof(params[4]); LineStart.Y =   amx_ctof(params[5]); LineStart.Z =  amx_ctof(params[6]);
    LineEnd.X   =  amx_ctof(params[7]); LineEnd.Y =   	amx_ctof(params[8]); LineEnd.Z =  	amx_ctof(params[9]);

    float linedist;

// DistancePointLine( XYZ *Point, XYZ *LineStart, XYZ *LineEnd, float *Distance )

    int isok = DistancePointLine( &Point, &LineStart, &LineEnd, &linedist );

	if (isok == 0) // out of range
		linedist = 10000000.0;

    return amx_ftoc(linedist);
}

SCRIPT_NATIVE nat_MPGetVehicleUpsideDown(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPGetVehicleUpsideDown, 1);

    float v1[3];
    float resx, resy, resz;

    int vehid = params[1];

    if ((vehid == 0) || (vehid == INVALID_VEHICLE_ID) || (vehid > MAX_VEHICLES)) {
    	resz = -5.0;
    	return amx_ftoc(resz);
	}

    v1[0] = 0.0;
    v1[1] = 0.0;
    v1[2] = 1.0;

	MPProjectPointOnVehicle(vehid, v1, resx, resy, resz, 0);

	return amx_ftoc(resz);
}

bool IsFltWithin(float a, float b, float value) {

	float minv = a;
	float maxv = b;

	if (b < a) {
		minv = b;
		maxv = a;
	}

	if ((value >= minv) && (value <= maxv))
		return true;

//	logprintf("from %0.5f to %0.5f value %0.5f is %d", minv, maxv, value, res);

	return false;
}

int Distance1Dint(int fPos1, int fPos2) {

	if (fPos1 > fPos2)
		return abs(fPos1 - fPos2);

	return abs(fPos2 - fPos1);

}

// This function manages to properly calculate time even if the timers wrap around and underflow.
int GetTimeDistance(int a, int b) {

	// a pawn cell is signed 32-bit integer.
	// -2147483648 to 2147483647

	// handle invalid time. If any time is invalid (zero) then it returns infinite time (2147483647)

	if ((a == 0) || (b == 0)) {
		return 2147483647;
	}

	if ((a < 0) && (b > 0)) {

		int dist = Distance1Dint(a, b);
		if (dist > 2147483647)
			return Distance1Dint(a - 2147483647, b - 2147483647);
		else
			return dist;

	} else {

		return Distance1Dint(a, b);

	}

}

bool intpoint_inside_trigon(float s[2], float a[2], float b[2], float c[2]) {

	int as_x = s[0] - a[0];
	int as_y = s[1] - a[1];

	bool s_ab = (b[0] - a[0])*as_y - (b[1] - a[1])*as_x > 0;

	if ((c[0] - a[0])*as_y - (c[1] - a[1])*as_x > 0 == s_ab) return false;

	if ((c[0] - b[0])*(s[1] - b[1]) - (c[1] - b[1])*(s[0] - b[0]) > 0 != s_ab) return false;

	return true;
}

SCRIPT_NATIVE nat_UtilGetWaterZlevel(AMX* amx, cell* params) {

	CHECK_PARAM_COUNT(nat_UtilGetWaterZlevel, 2);

	float cxy[2];

	cxy[0] = amx_ctof(params[1]);
	cxy[1] = amx_ctof(params[2]);

	float retval;

	// out of world area has constant water level.
	if ((cxy[0] < -3000) || (cxy[0] > 3000) || (cxy[1] < -3000) || (cxy[1] > 3000)) {
		retval = 0.0;
		return amx_ftoc(retval);
	}

	// quad waters:
	for (unsigned int i = 0; i < sizeof(waterrects) / sizeof(WaterDataQuad); i = i + 1) {

		if (cxy[0] < waterrects[i].pointmin[0])
			continue;

		if (cxy[0] > waterrects[i].pointmax[0])
			continue;

		if (cxy[1] < waterrects[i].pointmin[1])
			continue;

		if (cxy[1] > waterrects[i].pointmax[1])
			continue;

		return amx_ftoc(waterrects[i].zlevel);

	}

	// triangle waters:
	for (unsigned int i = 0; i < sizeof(watertris) / sizeof(WaterDataTriQuad); i = i + 1) {

		if (cxy[0] < watertris[i].pointmin[0])
			continue;

		if (cxy[0] > watertris[i].pointmax[0])
			continue;

		if (cxy[1] < watertris[i].pointmin[1])
			continue;

		if (cxy[1] > watertris[i].pointmax[1])
			continue;

		if (intpoint_inside_trigon(cxy, watertris[i].pointa, watertris[i].pointb, watertris[i].pointc))
			return amx_ftoc(watertris[i].zlevel);

	}

	retval = -1.0; // no water.
	return amx_ftoc(retval);

}

SCRIPT_NATIVE nat_GetPlayerIdleTime(AMX* amx, cell* params) {

	CHECK_PARAM_COUNT(nat_GetPlayerIdleTime, 1);
	
	return GetTimeDistance(TickCount(), lastplayerinput[params[1]]);

}

SCRIPT_NATIVE nat_GetPlayerTimeSinceUpdate(AMX* amx, cell* params) {

	CHECK_PARAM_COUNT(nat_GetPlayerTimeSinceUpdate, 1);

	return GetTimeDistance(TickCount(), lastplayerupdate[params[1]]);

}

SCRIPT_NATIVE nat_GetTimeMs(AMX* amx, cell* params) {

	int timex = GetTickCount(); // uptime of server in miliseconds

	if (timex == 0)
		return 1; // never return 0, 0 for us means not initialized / no data.

	return timex;

}


SCRIPT_NATIVE nat_GetTimeDistance(AMX* amx, cell* params) {

	CHECK_PARAM_COUNT(nat_GetTimeDistance, 2);

	return GetTimeDistance(params[1], params[2]);

}


/*
int FltInRange(float a, float b, float value, float ExtraBorder) {

	float minv = a;
	float maxv = b;

	if (b < a) {
		minv = b;
		maxv = a;
	}

	if ((value + ExtraBorder > minv) && (value - ExtraBorder < maxv))
		return 1;

	return 0;
}
*/

SCRIPT_NATIVE nat_MPWithinRange(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPWithinRange, 3);

	if (IsFltWithin(amx_ctof(params[1]), amx_ctof(params[2]), amx_ctof(params[3])))
		return 1;
	
	return 0;
	
}

SCRIPT_NATIVE nat_MPPtInRect2D(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPPtInRect2D, 6);

	if (IsFltWithin(amx_ctof(params[1]), amx_ctof(params[3]), amx_ctof(params[5])) != 1)
		return 0;

	if (IsFltWithin(amx_ctof(params[2]), amx_ctof(params[4]), amx_ctof(params[6])) != 1)
		return 0;

	return 1;
}

SCRIPT_NATIVE nat_MPPtInRect3D(AMX* amx, cell* params) {

    CHECK_PARAM_COUNT(nat_MPPtInRect3D, 9);

	if (IsFltWithin(amx_ctof(params[1]), amx_ctof(params[4]), amx_ctof(params[7])) != 1)
		return 0;

	if (IsFltWithin(amx_ctof(params[2]), amx_ctof(params[5]), amx_ctof(params[8])) != 1)
		return 0;

	if (IsFltWithin(amx_ctof(params[3]), amx_ctof(params[6]), amx_ctof(params[9])) != 1)
		return 0;

	return 1;
}


SCRIPT_NATIVE nat_IsBitSet(AMX* amx, cell* params) {

	//	returns True if a bit is ON (1)
	//	Nth can have any bit order value in [0..31]

	return (params[1] & (1 << params[2])) != 0;

}

// sets a bit in number to on and returns new number

SCRIPT_NATIVE nat_BitToOn(AMX* amx, cell* params) {

	return params[1] | (1 << params[2]);

}

// sets a bit in number to off and returns new number
SCRIPT_NATIVE nat_BitToOff(AMX* amx, cell* params) {
	
	return params[1] & ((1 << params[2]) ^ 0xFFFFFFFF);

}


PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()  {
    return sampgdk::Supports() | SUPPORTS_PROCESS_TICK | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) {
    if (!sampgdk::Load(ppData)) return false;

    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];

    logprintf("Math Plugin Loaded.");

	/*
	// todo: maybe load water data from file.
	for (unsigned int i = 0; i < sizeof(waterrects) / sizeof(WaterDataQuad); i = i + 1) {

		logprintf("level %0.1f bounds %0.1f %0.1f %0.1f %0.1f", waterrects[i].zlevel, waterrects[i].pointmin[0], waterrects[i].pointmin[1], waterrects[i].pointmax[0], waterrects[i].pointmax[1]);

	}
	*/

    return true;
}

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) {
	p_Amx.push_back(amx);
	return amx_Register( amx, natives, -1 );
}

PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) {
	return AMX_ERR_NONE;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload() {
	p_Amx.clear();
    sampgdk::Unload();
}

PLUGIN_EXPORT void PLUGIN_CALL ProcessTick() {
    // todo: code that checks if server is laggy / tick not going at a sufficient rate

    sampgdk::ProcessTick();
}

#ifndef NO_EVENT_HANDLER

void samputils::OnGameModeInit() {
	
	return;

}

bool samputils::OnPlayerConnect(int playerid) {

	samputils::oldanim[playerid] = 0;

	samputils::LastAnimSetTime[playerid] = invalid_time;
	samputils::oldvehiclechangetime[playerid] = invalid_time;
	lastplayerinput[playerid] = invalid_time;
	lastplayerupdate[playerid] = invalid_time;

	return true;

}

bool samputils::OnPlayerKeyStateChange(int playerid, int newkeys, int oldkeys) {

	lastplayerinput[playerid] = TickCount();
	lastplayerupdate[playerid] = TickCount();

	return true;

}

bool samputils::OnPlayerUpdate(int playerid) {

	lastplayerupdate[playerid] = TickCount();

	// todo: OnPlayerVirtualWorldChange
	// todo: onplayerteamidchange
	// can i do these via hooking natives?

	int ckeys, cud, clr;

	GetPlayerKeys(playerid, &ckeys, &cud, &clr);

	// todo: detect camera upvector change and take it as input change.

	if ((samputils::oldkeys[playerid] != ckeys) || (samputils::oldupdown[max_players] = cud) || (samputils::oldleftright[max_players] = clr)) {
		
		lastplayerinput[playerid] = TickCount();

		cell amx_Ret;
		int amx_Idx;

		for (std::list<AMX *>::iterator amxe = p_Amx.begin(); amxe != p_Amx.end(); amxe++) {
			if (amx_FindPublic(*amxe, "OnPlayerAnalogInput", &amx_Idx) == AMX_ERR_NONE) {

				amx_Push(*amxe, playerid);
				amx_Push(*amxe, samputils::oldkeys[playerid]);
				amx_Push(*amxe, samputils::oldupdown[playerid]);
				amx_Push(*amxe, samputils::oldleftright[playerid]);

				amx_Push(*amxe, ckeys);
				amx_Push(*amxe, cud);
				amx_Push(*amxe, clr);

				amx_Exec(*amxe, &amx_Ret, amx_Idx);

			}
		}

	}

	samputils::oldkeys[playerid] = ckeys;
	samputils::oldupdown[max_players] = cud;
	samputils::oldleftright[max_players] = clr;

	int currentanim = GetPlayerAnimationIndex(playerid);

	// animations tracker
	if (samputils::oldanim[playerid] != currentanim) {

		cell amx_Ret;
		int amx_Idx;

		for (std::list<AMX *>::iterator amxe = p_Amx.begin(); amxe != p_Amx.end(); amxe++) {
			if (amx_FindPublic(*amxe, "OnPlayerAnimationChanged", &amx_Idx) == AMX_ERR_NONE) {

				amx_Push(*amxe, playerid);
				amx_Push(*amxe, samputils::oldanim[playerid]);
				amx_Push(*amxe, currentanim);
				amx_Push(*amxe, samputils::oldanim[playerid] == 0 ? 0 : GetTimeDistance(TickCount(), samputils::LastAnimSetTime[playerid]));
				amx_Exec(*amxe, &amx_Ret, amx_Idx);
				
			}
		}

		samputils::oldanim[playerid] = currentanim;
		samputils::LastAnimSetTime[playerid] = TickCount();

	}

	int cvehid = GetPlayerVehicleID(playerid);
	int cvehseat = GetPlayerVehicleSeat(playerid);

	if ((cvehid != samputils::oldvehicleid[playerid]) || (cvehseat != samputils::oldseatid[playerid])) {

		int changecode = 0;

		if ((cvehid == INVALID_VEHICLE_ID) && (samputils::oldvehicleid[playerid] != INVALID_VEHICLE_ID))
			changecode = -1; // exited vehicle

		if ((cvehid != INVALID_VEHICLE_ID) && (samputils::oldvehicleid[playerid] == INVALID_VEHICLE_ID))
			changecode = 1; // entered vehicle

		if ((cvehid != INVALID_VEHICLE_ID) && (samputils::oldvehicleid[playerid] != INVALID_VEHICLE_ID)) {
			if (cvehid != samputils::oldvehicleid[playerid])
				changecode = 2; // changed vehicle-to-vehicle
			else
				changecode = 3; // changed vehicle seat
		}

		cell amx_Ret;
		int amx_Idx;

		for (std::list<AMX *>::iterator amxe = p_Amx.begin(); amxe != p_Amx.end(); amxe++) {
			if (amx_FindPublic(*amxe, "OnPlayerVehicleInfoChanged", &amx_Idx) == AMX_ERR_NONE) {

				amx_Push(*amxe, playerid);
				amx_Push(*amxe, oldvehicleid[playerid]);
				amx_Push(*amxe, oldseatid[playerid]);
				amx_Push(*amxe, cvehid);
				amx_Push(*amxe, cvehseat);
				amx_Push(*amxe, GetTimeDistance(TickCount(), samputils::oldvehiclechangetime[playerid]));
				amx_Push(*amxe, changecode);
				amx_Exec(*amxe, &amx_Ret, amx_Idx);

			}
		}

		samputils::oldvehiclechangetime[playerid] = TickCount();

	}

	samputils::oldvehicleid[playerid] = cvehid;
	samputils::oldseatid[playerid] = cvehseat;

	return true;

}

#endif

/*
_re_OnPlayerWeaponDataChanged
_re_OnPlayerVirtualWorldChange

can ide add vehicle mod
can cab attach as traIler(ide.a ide.b)
time when vehicle was entered(time of last vehicle - id change)

used for vehicle id change AND seat id change
_re_OnPlayerVehicleIDChanged / OnPlayerVehicleInfoChanged(entered / exited / switch - id / changed_seat, oldvehicleid newvehicleid, oldseatid, seatid, newvehicledrivers)

_re_OnPlayerVehicleDamaged
_re_OnPlayerChangedTeam
_re_OnClientPlayerMoneyChanged
_re_OnVehicleDamageStatusUpdate ? ? ?
_re_OnVehicleLightsChanged
_re_OnVehicleHealthIncrease
_re_OnPlayerSActionChange
_re_OnPlayerMoneyCheckNotify
_re_OnPlayerWeaponChanged
_re_OnPlayerStuntMoney ? ?
*/
