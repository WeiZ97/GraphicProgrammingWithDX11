# 準備工作


* [Direct3D](#Driect3D)
* [COM組件](#COM)
* [紋理與數據資源格式](#Texture-data-format)
* [交換鏈與頁面翻轉](#swap-chain-and-page-flipping)
* [深度緩衝區](#depth-buffer)
* [Texture資源視圖](#resource-view)

<h2 id="Driect3D">Direct3D</h2>

位於繪圖硬體以及軟體之間的API

<h2 id="COM">COM組件</h2>

微軟提出的開發框架，以純虛函式類別實現物件導向

每個類別都繼承IUnknow介面

    // 只能有虛函式成員
    #define struct interface
    interface IUnknow{...}

1. 透過使用特定的函數而非C++的new關鍵字來建立物件

2. 當不使用時使用Release()釋放(組件所在模組自行管理物件的生命週期)，而不使用delte關鍵字

<h2 id="Texture-data-format">紋理與數據資源格式</h2>

1. 2DTexture是一種數據元素矩陣，用途之一是儲存2D圖像數據

2. 在紋理中的每個元素中儲存一個像素顏色。但不是Texture唯一用途，例如在法線貼圖(normal texture)，儲存的是3D向量。

3. 1DTexture是一個一維陣列，3DTexture是三維陣列。

4. GPU可以在Texture上做特殊運算，例如fliter(過濾器)、multisampling(多重採樣，鋸齒用)

5. 常用格式為，RGBA代表Red、Green、Blue、Alpha(透明度):

    * Texture只支持特定格式的資料儲存，這些格式由DXGI_FORMAT列舉值決定

    * D XGI_FORMAT_R32G32B32_FLOAT：每個元素包含3個32位浮點分量。

    * DXGI_FORMAT_R16G16B16A16_UNORM：每個元素包含4個16位分量，分量的取值範圍在[0,1]區間內。

    * DXGI_FORMAT_R32G32_UINT：每個元素包含兩個32位無符號整數分量。
    * DXGI_FORMAT_R8G8B8A8_UNORM：每個元素包含4個8位無符號分量，分量的取值範圍在[0,1]區間內。

    * DXGI_FORMAT_R8G8B8A8_SNORM：每個元素包含4個8位有符號分量，分量的取值範圍在[−1,1] 區間內。

    * DXGI_FORMAT_R8G8B8A8_SINT：每個元素包含4個8位有符號整數分量，分量的取值範圍在[−128, 127] 區間內。

    * DXGI_FORMAT_R8G8B8A8_UINT：每個元素包含4個8位無符號整數分量，分量的取值範圍在[0, 255]區間內。

6. 如上述，Texture不一定是顏色訊息，格式DXGI_FORMAT_R32G32B32_FLOAT可以用來表示3d float向量

7. 有一種弱類型格式，可以預先分配記憶體空間，然後在Texture綁定時在決定如何解釋數據內容:例如
    * DXGI_FORMAT_R8G8B8A8_TYPELESS
預先預留4個八位芬輛，不指定為整數、浮點數等

8. Texture元素稱為**texel**，但一般情況下習慣說pixel

<h2 id="swap-chain-and-page-flipping">交換鏈與頁面翻轉</h2>

電腦渲染畫面有時在顯示器更新前就能完成，這樣的話會導致螢幕顯示未渲染完成的畫面

解決方案便是使用兩個Texture做為顏色緩存，分為**前台緩衝(front buffer)**與**後台緩衝(back buffer)**。

前台緩衝區紀錄目前這一幀顏色訊息，下一幀則在後台緩稱區執行渲染，當選渲染完成後，兩個緩衝區翻轉，前台變後台，後台變前台...

翻轉僅是將兩邊的指標做交換，是很快的動作稱為**呈現(presenting)**。

![image](../pic/ch4/4.1.jpg)

* 這樣前後台緩衝區便形成的**交換鏈(swap chain)**

* 使用兩個緩衝稱為**雙緩衝(double buffering)**


<h2 id="depth-buffer">深度緩衝區</h2>

 * **深度緩衝區(depth buffer)** 是一個紋理對象。

* 深度訊息可以被認為是一種特殊的像素，常見的深度值範圍在[0.0,1.0]之間，0.0表示最近物體，1.0表示最遠

* Driect3D中，深度緩衝區稱為**深度緩存(depth buffering)**或**Z緩存(z-buffering)**

* 為了處理彼此遮擋的物體

![image](../pic/ch4/4.3.jpg)

* 如圖有三個像素p1、p2、p3(深度d1 d2 d3)被投影到同一個像素點p上，並且假設渲染順序為圓柱->球體->圓錐
    1. 清空back buffer
    2. 清空depth buffer為默認值，通常為1.0(最遠) d=1.0
    3. 繪製圓柱體 :d3<=1.0，深度測試通過，更新緩衝區，p=p3 d=d3
    4. 繪製球體 :d1<=d3，深度測試通過，更新緩衝區，p=p1 d=d1
    5. 繪製圓錐 :d2>d=d1，不通過，不更新緩衝區
     
    最終結果，能被繪製的有最小深度值的點，成為寫入後台緩衝區的最終像素

* 深度緩衝區也是Texture，創建的格式為:

    * DXGI_FORMAT_D32_FLOAT_S8X24_UINT：32位浮點深度緩衝區。為模板緩衝區預留8位（無符號整數），每個模板值的取值範圍為[0,255]。其餘24位閒置。

    * DXGI_FORMAT_D32_FLOAT：32位浮點深度緩衝區。

    * DXGI_FORMAT_D24_UNORM_S8_UINT：無符號24位深度緩衝區，每個深度值的取值範圍為[0,1]。為模板緩衝區預留8位（無符號整數），每個模板值的取值範圍為[0,255]。

    * DXGI_FORMAT_D16_UNORM：無符號16位深度緩衝區，每個深度值的取值範圍為[0,1]。

<h2 id="resource-view">資源視圖</h2>

* Texture可以綁定在渲染管線中的不同階段(stage)；例如將Texxture做為渲染目標(D3D渲染的結果)，或著色器資源(Texture會在著色器被採樣)。使用這兩個綁定值

    D3D11_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE

* 一個資源不能直接被綁定在一個渲染管線階段，為Texture在初始化創建**資源視圖(resource view)**，減少綁定時的類型檢查
    * 例:一個Texture同時綁定為渲染目標及著色器資源時，為其創造
        
        ID3D11RenderTargetView

        ID3D11ShaderResourceView

        這兩種資源視圖

* 有一種**弱形別(typeless)** 資